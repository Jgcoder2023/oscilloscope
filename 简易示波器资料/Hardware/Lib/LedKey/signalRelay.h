#ifndef __SIGNALRELAY_H    
#define __SIGNALRELAY_H

#include "main.h"

void InitSignalGPIO(void);
void SetSignalDecay(uint8_t value);
void SetSignalAcOrDc(uint8_t value);

#endif   //signalRelay