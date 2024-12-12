#ifndef __KEY_H__
#define __KEY_H__

#include "app.h"
#include ".\library\Type_def.h"


#define MaxButtonNumber    3   //按键数量,如需32,更改u16Bit_Key_struct位域为32位

#define Read_Lift_KEY P27
#define Read_Set_KEY P26
#define Read_Right_KEY P25

// #define Read_encoder_KEY             gpio_input_bit_get(GPIOB,GPIO_PIN_9)



//**********************************************************
struct u32_Bit_Key_struct{
	u8 Bit0 : 1;
	u8 Bit1 : 1;
	u8 Bit2 : 1;
	u8 Bit3 : 1;
	u8 Bit4 : 1;
	u8 Bit5 : 1;
	u8 Bit6 : 1;
	u8 Bit7 : 1;
};
//*************
union u32_Bit_Key_union{
	struct u32_Bit_Key_struct u32_Bit;
	u8 All;
};


void Init_Key_GPIO(void);
void KeyInputScan(void);     //开关量扫描输入
void KeyValueCalculate(void);//键值计算
u8 KeyState(u8 KeyNumber);// 返回键值的当前状态
u8 R_KeyState(u8 KeyNumber);//返回键值的上升沿状态
u8 F_KeyState(u8 KeyNumber);//返回键值的下降沿状态
u8 L_KeyState(u8 KeyNumber);//返回键值的长按状态
u8 D_KeyState(u8 KeyNumber);//返回键值的断续状态
u8 DC_KeyState(u8 KeyNumber);//双击之后触发一次


#endif


/**
 * 
 * 使用指南
 * 1. 先调用 KeyIO_Init 进行io口初始化
 * 2. 每10ms 调用 KeyInputScan 进行按键扫描
 * 3. 每10ms 调用 KeyValueCalculate 进行按键状态处理
 * 4. 调用对应函数 返回对应按键对应的状态
 * 
*/