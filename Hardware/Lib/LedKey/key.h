#ifndef __KEY_H__
#define __KEY_H__

#include "main.h"


#define MaxButtonNumber              1  //按键数量,如需32,更改u16Bit_Key_struct位域为32位

// #define Read_PROBE1_KEY              gpio_input_bit_get(GPIOB,GPIO_PIN_13)

// #define Read_PROBE2_KEY              gpio_input_bit_get(GPIOB,GPIO_PIN_14)

// #define Read_PROBE3_KEY              gpio_input_bit_get(GPIOB,GPIO_PIN_15)

#define Read_encoder_KEY             gpio_input_bit_get(GPIOB,GPIO_PIN_9)



//**********************************************************
struct uint32_t_Bit_Key_struct{
	uint32_t Bit0 : 1;
	uint32_t Bit1 : 1;
	uint32_t Bit2 : 1;
	uint32_t Bit3 : 1;
	uint32_t Bit4 : 1;
	uint32_t Bit5 : 1;
	uint32_t Bit6 : 1;
	uint32_t Bit7 : 1;
	uint32_t Bit8 : 1;
	uint32_t Bit9 : 1;
	uint32_t Bit10: 1;
	uint32_t Bit11: 1;
	uint32_t Bit12: 1;
	uint32_t Bit13: 1;
	uint32_t Bit14: 1;
	uint32_t Bit15: 1;
	uint32_t Bit16: 1;
	uint32_t Bit17: 1;
	uint32_t Bit18: 1;
	uint32_t Bit19: 1;
	uint32_t Bit20: 1;
	uint32_t Bit21: 1;
	uint32_t Bit22: 1;
	uint32_t Bit23: 1;
	uint32_t Bit24: 1;
	uint32_t Bit25: 1;
	uint32_t Bit26: 1;
	uint32_t Bit27: 1;
	uint32_t Bit28: 1;
	uint32_t Bit29: 1;
	uint32_t Bit30: 1;
	uint32_t Bit31: 1;
};
//*************
union uint32_t_Bit_Key_union{
	struct uint32_t_Bit_Key_struct uint32_t_Bit;
	uint32_t All;
};

#define  PROBE1_KEY      0//按键1
#define  PROBE2_KEY      1//按键2
#define  PROBE3_KEY      2//按键3


void Init_Key_GPIO(void);
void KeyInputScan(void);     //开关量扫描输入
void KeyValueCalculate(void);//键值计算
uint8_t KeyState(uint8_t KeyNumber);// 返回键值的当前状态
uint8_t R_KeyState(uint8_t KeyNumber);//返回键值的上升沿状态
uint8_t F_KeyState(uint8_t KeyNumber);//返回键值的下降沿状态
uint8_t L_KeyState(uint8_t KeyNumber);//返回键值的长按状态
uint8_t D_KeyState(uint8_t KeyNumber);//返回键值的断续状态
uint8_t DC_KeyState(uint8_t KeyNumber);//双击之后触发一次
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




