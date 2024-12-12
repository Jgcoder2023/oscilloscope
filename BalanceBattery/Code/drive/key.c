#include "key.h"

u8 L_KeyDeal_T[MaxButtonNumber]; // 长按键按下后计时
u8 D_KeyDeal_T[MaxButtonNumber]; // 连发按键

union u32_Bit_Key_union KeyValue;     // 键输入
union u32_Bit_Key_union LongPressKey; // 长按键输入
union u32_Bit_Key_union DoubleValueKey; // 双值按键,按下键0.5s后,每0.2s产生一个有效键
union u32_Bit_Key_union RiseKeyValue; // 键值上升沿变化检测,意义为在按下键后产生一次有效
union u32_Bit_Key_union FallKeyValue; // 键值下降沿变化检测,意义为在按下键后产生一次有效
union u32_Bit_Key_union DCKeyValue;
//-------------------------------------------



// 初始化按键引脚
void Init_Key_GPIO(void){
    // //使能时钟
    // rcu_periph_clock_enable(RCU_GPIOB);
    // //设置输出模式，上拉
    // gpio_mode_set(GPIOB, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, GPIO_PIN_9);
    // // |GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15

	P2_MODE_IN_HIZ(GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);
	P2_PULL_UP_ENABLE(GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);//内部上拉电阻使能

}


//=======================================================================================================
// 函数名称: KeyInputScan
// 功能描述：按键输入循环扫描,已经过1个扫描周期的滤波
// 输　入:  无
// 输　出:  无
// 全局变量:
// 调用模块: 无
// 日　期:  2012年01月09日
// 备  注:
// 建议10ms调用一次,如需使用其它时间,L_KeyDeal_T,D_KeyDeal_T的定时计数初值注意修改
//-------------------------------------------------------------------------------------------------------
//=======================================================================================================
// u16 BitKeyValue;
static u8 _KeyCounter[MaxButtonNumber];
void KeyInputScan(void) { // 开关量扫描输入
    static u32 Last_KeyValue = 0;
    union u32_Bit_Key_union New_KeyValue;
    u8 n;

    New_KeyValue.u32_Bit.Bit0 = Read_Lift_KEY; // lift
    New_KeyValue.u32_Bit.Bit1 = Read_Set_KEY; // set
    New_KeyValue.u32_Bit.Bit2 = Read_Right_KEY; // right
    // New_KeyValue.u32_Bit.Bit3 = Read_PROBE3_KEY; // 按键3

    New_KeyValue.All = ~New_KeyValue.All; // 输入有效时为0电平,取反,使用正逻辑体系

    KeyValue.All = Last_KeyValue & New_KeyValue.All; // 更新扫描的键值=本次扫描 & 上次扫描值
    Last_KeyValue = New_KeyValue.All; // 保存本次扫描值

    for (n = 0; n < MaxButtonNumber; n++) // 长按键和连发按键的延时计数
    {
        if (L_KeyDeal_T[n])   // 长按键计时器
            L_KeyDeal_T[n]--; // 长按键计时减

        if (D_KeyDeal_T[n])   // 连发按键计时器
            D_KeyDeal_T[n]--; // 连发按键计时减

        if (_KeyCounter[n])   // 双击键计时器
            _KeyCounter[n]--; // 双击按键计时减
    }

	
	KeyValueCalculate();
}

//=======================================================================================================
// 函数名称: KeyValueCalculate
// 功能描述：按键键值计算
// 输　入:  无
// 输　出:  无
// 全局变量:
// 调用模块: 无
// 日　期:  2012年01月09日
// 备  注:  用户函数循环调用即可
//=======================================================================================================
void KeyValueCalculate(void) {
    static u32 FristOutFlag = 0; // 多少按键对应多少按键数量
    static u32 Last_KeyValue = 0;
    static u32 L_Key_EnOutFlag;
    static u8 step[MaxButtonNumber];
    u8 n;
    u32 CurrentKeyValue;
    u32 TempKey;

    CurrentKeyValue = KeyValue.All;
    //---------------------------------------------------
    RiseKeyValue.All = ~Last_KeyValue & CurrentKeyValue; // 按键上升检测
    FallKeyValue.All = Last_KeyValue & ~CurrentKeyValue; // 按键下降检测
    //---------------------------------------------------
    TempKey = 0;
    for (n = 0; n < MaxButtonNumber; n++) {
        if (CurrentKeyValue & (1 << n)) {
            if ((FristOutFlag & (1 << n)) == 0) // 按键按下后,第一次键值输出是否已输出,如果没有
            {
                FristOutFlag = FristOutFlag | (1 << n); // 置位第一次键值输出标示
                TempKey = TempKey | (1 << n); // 产生连发按键的第一个键值,按下即有效
                D_KeyDeal_T[n] = 80; // 第一次键值有效到第二次有效延时时间,80*10ms=0.8s产生第二连发键
            } else if (D_KeyDeal_T[n] == 0) {
                TempKey = TempKey | (1 << n); // 双值按键有效
                D_KeyDeal_T[n] = 10; // 重新赋延时计数,10*10ms=100ms可产生一个连发键
            }
        } else {
            TempKey = TempKey & ~(1 << n); // 连发按键清零
            FristOutFlag = FristOutFlag & ~(1 << n); // 用于产生连发按键的第一个键值,按下即有效
        }
    }
    DoubleValueKey.All = TempKey; // 双值按键
    //---------------------------------------------------
    TempKey = 0;
    for (n = 0; n < MaxButtonNumber; n++) {
        if (CurrentKeyValue & (1 << n)) {
            if ((L_KeyDeal_T[n] == 0) && (L_Key_EnOutFlag & (1 << n))) // 延时时间到
            {
                TempKey = TempKey | (1 << n); // 长按键有效
                L_Key_EnOutFlag &= (~1 << n);
            }
        } else {
            TempKey = TempKey & ~(1 << n);
            L_KeyDeal_T[n] = 120; // 延时计数时间
            L_Key_EnOutFlag |= (1 << n);
        }
    }
    LongPressKey.All = TempKey; // 长按键
    //---------------------------------------------------
    for (n = 0; n < MaxButtonNumber; n++) {
        switch (step[n]) // 判断双击事件
        {
        case 0:
            if (RiseKeyValue.All & (1 << n)) // 按键被按下瞬间
            {
                step[n] = 1;
                _KeyCounter[n] = 50;
            }
            break;
        case 1:
            if (FallKeyValue.All & (1 << n)) // 按键被松开瞬间
            {
                step[n] = 2;
                _KeyCounter[n] = 50;
            }
            if (_KeyCounter[n] == 0)
                step[n] = 0;
            break;
        case 2:
            if (RiseKeyValue.All & (1 << n)) // 按键被按下瞬间
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
 * 返回对应按键对应的状态
 *
 */
u8 KeyState(u8 KeyNumber) // 返回键值的当前状态
{
    return (KeyValue.All >> KeyNumber) & 1;
}

u8 R_KeyState(u8 KeyNumber) // 返回键值的上升沿状态
{
    return (RiseKeyValue.All >> KeyNumber) & 1;
}

u8 F_KeyState(u8 KeyNumber) // 返回键值的下降沿状态
{
    return (FallKeyValue.All >> KeyNumber) & 1;
}

u8 L_KeyState(u8 KeyNumber) // 返回键值的长按状态
{
    return (LongPressKey.All >> KeyNumber) & 1;
}

u8 D_KeyState(u8 KeyNumber) // 返回键值的断续状态
{
    return (DoubleValueKey.All >> KeyNumber) & 1;
}

u8 DC_KeyState(u8 KeyNumber) // 双击之后触发一次
{
    u8 _DCKeyValue;
    _DCKeyValue = (DCKeyValue.All >> KeyNumber) & 1;
    DCKeyValue.All &= (~1 << KeyNumber);
    return _DCKeyValue;
}