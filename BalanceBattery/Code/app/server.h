#ifndef		__SERVER_H
#define		__SERVER_H

#include "app.h"


//adc的值
typedef struct{
	float v_sun;   //总电压
	float a_sun;   //总电流
	float v_bat;   //充电电压
	float a_bat;   //充电电流
	float v_bat1;  //电池1电压
	float v_bat2;  //电池2电压
	float v_bat3;  //电池3电压
	float v_bat4;  //电池4电压
	
	u8 CHRG:1;   //充电指示，低电平
	u8 DONE:1;   //充满指示 低电平
	u8 PG:1;     //电压诱导状态
	u8 bat1_state:1;//电池1的状态 是否充满
	u8 bat2_state:1;//电池2的状态
	u8 bat3_state:1;//电池3的状态
	u8 bat4_state:1;//电池4的状态
}StateValue;

void QueryAllStateValue(StateValue *sv);
void UpdatePwmDuty(u16 PWM7_Duty);
void SetVoltage(u8 key);
void SetBatNum(char num);





#endif