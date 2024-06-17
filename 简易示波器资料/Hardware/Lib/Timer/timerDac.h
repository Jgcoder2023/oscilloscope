#ifndef __TIMER_DAC_H
#define __TIMER_DAC_H

#include "gd32e23x.h"
#include "systick.h"
#include <stdio.h>
#include "main.h"

void Init_Dac_Output(uint32_t period,uint32_t pulse);
void SetDacComparex(uint16_t value);
void Set_Output2_Freq(uint32_t value);
void SetDacState(uint8_t value);


#endif