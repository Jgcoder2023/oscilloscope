
#ifndef		__INIT_H
#define		__INIT_H

#include "app.h"

#define RunLed P22
#define MainOnOff  P53   //总开关
#define Bat1OnOff  P34   //bat1
#define Bat2OnOff  P33   //bat2
#define Bat3OnOff  P43   //bat3
#define Bat4OnOff  P47   //bat4


void RunLedConfig(void);
void TimerConfig(void);
void UartConfig(void);
void AdcConfig(void);
void PwmConfig(void);
//void UpdatePwm(void);




#endif