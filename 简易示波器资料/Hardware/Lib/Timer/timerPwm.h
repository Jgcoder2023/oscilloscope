#ifndef __TIMER_PWM_H
#define __TIMER_PWM_H

#include "gd32e23x.h"
#include "systick.h"
#include <stdio.h>
#include "main.h"

void Init_PWM_Output(uint32_t period,uint32_t pulse);
void Set_Output_PWMComparex(uint16_t value);
void Set_Output_Freq(uint32_t value);
void SetPwmState(uint8_t value);

#endif