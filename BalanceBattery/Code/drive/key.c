#include "key.h"

u8 L_KeyDeal_T[MaxButtonNumber]; // ���������º��ʱ
u8 D_KeyDeal_T[MaxButtonNumber]; // ��������

union u32_Bit_Key_union KeyValue;     // ������
union u32_Bit_Key_union LongPressKey; // ����������
union u32_Bit_Key_union DoubleValueKey; // ˫ֵ����,���¼�0.5s��,ÿ0.2s����һ����Ч��
union u32_Bit_Key_union RiseKeyValue; // ��ֵ�����ر仯���,����Ϊ�ڰ��¼������һ����Ч
union u32_Bit_Key_union FallKeyValue; // ��ֵ�½��ر仯���,����Ϊ�ڰ��¼������һ����Ч
union u32_Bit_Key_union DCKeyValue;
//-------------------------------------------



// ��ʼ����������
void Init_Key_GPIO(void){
    // //ʹ��ʱ��
    // rcu_periph_clock_enable(RCU_GPIOB);
    // //�������ģʽ������
    // gpio_mode_set(GPIOB, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, GPIO_PIN_9);
    // // |GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15

	P2_MODE_IN_HIZ(GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);
	P2_PULL_UP_ENABLE(GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);//�ڲ���������ʹ��

}


//=======================================================================================================
// ��������: KeyInputScan
// ������������������ѭ��ɨ��,�Ѿ���1��ɨ�����ڵ��˲�
// �䡡��:  ��
// �䡡��:  ��
// ȫ�ֱ���:
// ����ģ��: ��
// �ա���:  2012��01��09��
// ��  ע:
// ����10ms����һ��,����ʹ������ʱ��,L_KeyDeal_T,D_KeyDeal_T�Ķ�ʱ������ֵע���޸�
//-------------------------------------------------------------------------------------------------------
//=======================================================================================================
// u16 BitKeyValue;
static u8 _KeyCounter[MaxButtonNumber];
void KeyInputScan(void) { // ������ɨ������
    static u32 Last_KeyValue = 0;
    union u32_Bit_Key_union New_KeyValue;
    u8 n;

    New_KeyValue.u32_Bit.Bit0 = Read_Lift_KEY; // lift
    New_KeyValue.u32_Bit.Bit1 = Read_Set_KEY; // set
    New_KeyValue.u32_Bit.Bit2 = Read_Right_KEY; // right
    // New_KeyValue.u32_Bit.Bit3 = Read_PROBE3_KEY; // ����3

    New_KeyValue.All = ~New_KeyValue.All; // ������ЧʱΪ0��ƽ,ȡ��,ʹ�����߼���ϵ

    KeyValue.All = Last_KeyValue & New_KeyValue.All; // ����ɨ��ļ�ֵ=����ɨ�� & �ϴ�ɨ��ֵ
    Last_KeyValue = New_KeyValue.All; // ���汾��ɨ��ֵ

    for (n = 0; n < MaxButtonNumber; n++) // ��������������������ʱ����
    {
        if (L_KeyDeal_T[n])   // ��������ʱ��
            L_KeyDeal_T[n]--; // ��������ʱ��

        if (D_KeyDeal_T[n])   // ����������ʱ��
            D_KeyDeal_T[n]--; // ����������ʱ��

        if (_KeyCounter[n])   // ˫������ʱ��
            _KeyCounter[n]--; // ˫��������ʱ��
    }

	
	KeyValueCalculate();
}

//=======================================================================================================
// ��������: KeyValueCalculate
// ����������������ֵ����
// �䡡��:  ��
// �䡡��:  ��
// ȫ�ֱ���:
// ����ģ��: ��
// �ա���:  2012��01��09��
// ��  ע:  �û�����ѭ�����ü���
//=======================================================================================================
void KeyValueCalculate(void) {
    static u32 FristOutFlag = 0; // ���ٰ�����Ӧ���ٰ�������
    static u32 Last_KeyValue = 0;
    static u32 L_Key_EnOutFlag;
    static u8 step[MaxButtonNumber];
    u8 n;
    u32 CurrentKeyValue;
    u32 TempKey;

    CurrentKeyValue = KeyValue.All;
    //---------------------------------------------------
    RiseKeyValue.All = ~Last_KeyValue & CurrentKeyValue; // �����������
    FallKeyValue.All = Last_KeyValue & ~CurrentKeyValue; // �����½����
    //---------------------------------------------------
    TempKey = 0;
    for (n = 0; n < MaxButtonNumber; n++) {
        if (CurrentKeyValue & (1 << n)) {
            if ((FristOutFlag & (1 << n)) == 0) // �������º�,��һ�μ�ֵ����Ƿ������,���û��
            {
                FristOutFlag = FristOutFlag | (1 << n); // ��λ��һ�μ�ֵ�����ʾ
                TempKey = TempKey | (1 << n); // �������������ĵ�һ����ֵ,���¼���Ч
                D_KeyDeal_T[n] = 80; // ��һ�μ�ֵ��Ч���ڶ�����Ч��ʱʱ��,80*10ms=0.8s�����ڶ�������
            } else if (D_KeyDeal_T[n] == 0) {
                TempKey = TempKey | (1 << n); // ˫ֵ������Ч
                D_KeyDeal_T[n] = 10; // ���¸���ʱ����,10*10ms=100ms�ɲ���һ��������
            }
        } else {
            TempKey = TempKey & ~(1 << n); // ������������
            FristOutFlag = FristOutFlag & ~(1 << n); // ���ڲ������������ĵ�һ����ֵ,���¼���Ч
        }
    }
    DoubleValueKey.All = TempKey; // ˫ֵ����
    //---------------------------------------------------
    TempKey = 0;
    for (n = 0; n < MaxButtonNumber; n++) {
        if (CurrentKeyValue & (1 << n)) {
            if ((L_KeyDeal_T[n] == 0) && (L_Key_EnOutFlag & (1 << n))) // ��ʱʱ�䵽
            {
                TempKey = TempKey | (1 << n); // ��������Ч
                L_Key_EnOutFlag &= (~1 << n);
            }
        } else {
            TempKey = TempKey & ~(1 << n);
            L_KeyDeal_T[n] = 120; // ��ʱ����ʱ��
            L_Key_EnOutFlag |= (1 << n);
        }
    }
    LongPressKey.All = TempKey; // ������
    //---------------------------------------------------
    for (n = 0; n < MaxButtonNumber; n++) {
        switch (step[n]) // �ж�˫���¼�
        {
        case 0:
            if (RiseKeyValue.All & (1 << n)) // ����������˲��
            {
                step[n] = 1;
                _KeyCounter[n] = 50;
            }
            break;
        case 1:
            if (FallKeyValue.All & (1 << n)) // �������ɿ�˲��
            {
                step[n] = 2;
                _KeyCounter[n] = 50;
            }
            if (_KeyCounter[n] == 0)
                step[n] = 0;
            break;
        case 2:
            if (RiseKeyValue.All & (1 << n)) // ����������˲��
            {
                step[n] = 3;
            }
            if (_KeyCounter[n] == 0)
                step[n] = 0;
            break;
        default:
            DCKeyValue.All |= (1 << n);
            step[n] = 0;
            break;
        }
    }
    //---------------------------------------------------
    Last_KeyValue = CurrentKeyValue;
}

/**
 * ���ض�Ӧ������Ӧ��״̬
 *
 */
u8 KeyState(u8 KeyNumber) // ���ؼ�ֵ�ĵ�ǰ״̬
{
    return (KeyValue.All >> KeyNumber) & 1;
}

u8 R_KeyState(u8 KeyNumber) // ���ؼ�ֵ��������״̬
{
    return (RiseKeyValue.All >> KeyNumber) & 1;
}

u8 F_KeyState(u8 KeyNumber) // ���ؼ�ֵ���½���״̬
{
    return (FallKeyValue.All >> KeyNumber) & 1;
}

u8 L_KeyState(u8 KeyNumber) // ���ؼ�ֵ�ĳ���״̬
{
    return (LongPressKey.All >> KeyNumber) & 1;
}

u8 D_KeyState(u8 KeyNumber) // ���ؼ�ֵ�Ķ���״̬
{
    return (DoubleValueKey.All >> KeyNumber) & 1;
}

u8 DC_KeyState(u8 KeyNumber) // ˫��֮�󴥷�һ��
{
    u8 _DCKeyValue;
    _DCKeyValue = (DCKeyValue.All >> KeyNumber) & 1;
    DCKeyValue.All &= (~1 << KeyNumber);
    return _DCKeyValue;
}