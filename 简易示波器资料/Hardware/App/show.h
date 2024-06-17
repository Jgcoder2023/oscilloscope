#ifndef SHOW_H
#define SHOW_H
#include "main.h"



void TFT_ShowUI(void);
void PWM_StaticUI(void);
void DAC_StaticUI(void);
void SET_StaticUI(void);

void DisplayFreq(uint16_t fc,uint16_t bc);

void SetPwmOutFc(void);
void SetPwmOutAckFc(void);
void SetPwmFreqFc(void);
void SetPwmFreqAckFc(void);
void SetPwmDutyCycleFc(void);
void SetPwmDutyCycleAckFc(void);

void SetDacOutFc(void);
void SetDacOutAckFc(void);
void SetDacDutyCycleFc(void);
void SetDacDutyCycleAckFc(void);


void SetDcAcFc(void);
void SetDcAcAckFc(void);
void SetDecayFc(void);
void SetDecayAckFc(void);

void SetPwmAfterRefresh(bool ResetColor);


#endif 

