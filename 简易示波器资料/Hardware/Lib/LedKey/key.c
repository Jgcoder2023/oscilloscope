#include "key.h"

uint8_t L_KeyDeal_T[MaxButtonNumber]; // ���������º��ʱ
uint8_t D_KeyDeal_T[MaxButtonNumber]; // ��������

union uint32_t_Bit_Key_union KeyValue;     // ������
union uint32_t_Bit_Key_union LongPressKey; // ����������
union uint32_t_Bit_Key_union
    DoubleValueKey; // ˫ֵ����,���¼�0.5s��,ÿ0.2s����һ����Ч��
union uint32_t_Bit_Key_union
    RiseKeyValue; // ��ֵ�����ر仯���,����Ϊ�ڰ��¼������һ����Ч
union uint32_t_Bit_Key_union
    FallKeyValue; // ��ֵ�½��ر仯���,����Ϊ�ڰ��¼������һ����Ч
union uint32_t_Bit_Key_union DCKeyValue;
//-------------------------------------------



// ��ʼ����������
void Init_Key_GPIO(void){
    //ʹ��ʱ��
    rcu_periph_clock_enable(RCU_GPIOB);
    //�������ģʽ������
    gpio_mode_set(GPIOB, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, GPIO_PIN_9);
    // |GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15

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
static uint8_t _KeyCounter[MaxButtonNumber];
void KeyInputScan(void) // ������ɨ������
{
    static uint32_t Last_KeyValue = 0;
    union uint32_t_Bit_Key_union New_KeyValue;
    uint8_t n;

    New_KeyValue.uint32_t_Bit.Bit0 = Read_encoder_KEY; // ����������
    // New_KeyValue.uint32_t_Bit.Bit1 = Read_PROBE1_KEY; // ����1
    // New_KeyValue.uint32_t_Bit.Bit2 = Read_PROBE2_KEY; // ����2
    // New_KeyValue.uint32_t_Bit.Bit3 = Read_PROBE3_KEY; // ����3

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
    static uint32_t FristOutFlag = 0; // ���ٰ�����Ӧ���ٰ�������
    static uint32_t Last_KeyValue = 0;
    static uint32_t L_Key_EnOutFlag;
    static uint8_t step[MaxButtonNumber];
    uint8_t n;
    uint32_t CurrentKeyValue;
    uint32_t TempKey;

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
uint8_t KeyState(uint8_t KeyNumber) // ���ؼ�ֵ�ĵ�ǰ״̬
{
    return (KeyValue.All >> KeyNumber) & 1;
}

uint8_t R_KeyState(uint8_t KeyNumber) // ���ؼ�ֵ��������״̬
{
    return (RiseKeyValue.All >> KeyNumber) & 1;
}

uint8_t F_KeyState(uint8_t KeyNumber) // ���ؼ�ֵ���½���״̬
{
    return (FallKeyValue.All >> KeyNumber) & 1;
}

uint8_t L_KeyState(uint8_t KeyNumber) // ���ؼ�ֵ�ĳ���״̬
{
    return (LongPressKey.All >> KeyNumber) & 1;
}

uint8_t D_KeyState(uint8_t KeyNumber) // ���ؼ�ֵ�Ķ���״̬
{
    return (DoubleValueKey.All >> KeyNumber) & 1;
}

uint8_t DC_KeyState(uint8_t KeyNumber) // ˫��֮�󴥷�һ��
{
    uint8_t _DCKeyValue;
    _DCKeyValue = (DCKeyValue.All >> KeyNumber) & 1;
    DCKeyValue.All &= (~1 << KeyNumber);
    return _DCKeyValue;
}
