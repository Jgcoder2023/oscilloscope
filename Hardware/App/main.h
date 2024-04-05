#ifndef MAIN_H
#define MAIN_H


#include "gd32e23x.h"
#include "usart.h"
#include <stdbool.h>
#include <string.h>
#include "systick.h"
#include <stdio.h>
#include "led.h"
#include "key.h"
#include "ec11.h"
#include "timer.h"
#include "timer15.h"
#include "timerPwm.h"
#include "timerDac.h"
#include "signalRelay.h"
#include "tft_init.h"
#include "tft.h"
#include "show.h"
#include "adc.h"
#include "freq.h"
#include <stdint.h>


typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;



// ����ʱ��
typedef struct {
    uint16_t Timer;//��ʱ��
    uint16_t SpaceTime;//��ʱʱ��
    bool RunFalg;//���б�־
    void (*pTaskHook)(void); // ���е�������
} TaskInfo;




enum MenuState{
   PWM = 1,   //PWM״̬
   DAC,      //DAC״̬
   SetInput,    //���ý�ֱ����˥��

   SetPwmOut, //����PWM���״̬
   SetPwmOutAck,//ȷ�����ã���˸��

   SetPwmFreq,//����PwmƵ��
   SetPwmFreqAck,  //ȷ�����ã���˸��

   SetPwmDutyCycle,//����PWMռ�ձ�
   SetPwmDutyCycleAck,//ȷ�����ã���˸��

   SetDacOut, //ѡ������DAC���״̬
   SetDacOutAck,//ȷ�����ã���˸��
   SetDacDutyCycle, //ѡ��DACռ�ձ�����
   SetDacDutyCycleAck,//ȷ������DACռ�ձȣ���˸��

   SetDcAc,  //����DCAC���
   SetDcAcAck,// ����DCAC���(��˸)
   SetDecay, //����˥��
   SetDecayAck,//����˥������˸��
};


// ADC��������
#define ADC_VALUE_NUM 300U
// ʾ������ʾ����
typedef struct {
   uint8_t showbit:1;         //������±�־λ
   uint8_t IsPwmOut:1;      //PWM�����־λ
   uint8_t IsDACOut:1;      //DAC�����־λ
   uint8_t IsDACDC:1;      //���0����  1ֱ��
   uint8_t IsDecay:1;    //�����Ƿ�˥��1��˥��  0 1/50˥��
	 uint8_t IsComSend:1;    //�����Ƿ�������ݷ���

   uint32_t sampletime;     //��ѹ�ɼ�ʱ��
   uint32_t timerPeriod;    //��ʱ����������
   float gatherFreq;        //ʾ�����ɼ�Ƶ��
   float vpp;               //���ֵ
   float voltageValue[300]; //ADC�ɼ���ѹֵ

   // PWM���
   uint16_t PwmOutFreq;     //PWM�������Ƶ�ʣ�չʾ��
   uint16_t PwmSetFreqValue; //����Ƶ�ʵ�ֵ����װ��ֵ��
   uint16_t pwmDutyCycle;   //PWM���������PWMռ�ձ�

   // DAC
   float DacVoltage; //DAC����ĵ�ѹ
   uint16_t DacDutyCycle;   //Dac���������PWMռ�ձ�

   enum MenuState ThisMenu;   //��ǰ��ʾ�Ĳ˵�

   int KnobValue;  // ��������ťֵ

} Oscilloscope;

extern volatile Oscilloscope oscilloscope;


// ������Ҫ�ϴ��˵����ݸ�ʽ
typedef union{
    struct SUD{
        uint8_t Head;

        uint8_t IsPwmOut:1;      //PWM�����־λ
        uint8_t IsDACOut:1;      //DAC�����־λ
        uint8_t IsACDC:1;      //���0����  1ֱ��
        uint8_t IsDecay:1;    //�����Ƿ�˥��1��˥��  0 1/50˥��

        uint16_t Vpp;
        uint16_t GatherFreq;        //ʾ�����ɼ�Ƶ��
        uint16_t PwmFreq;       //PWM�������Ƶ��
        uint16_t DacVoltage;  //DAC����ĵ�ѹ
        uint16_t AdcV[300];
        uint8_t  PwmDutyCycle;   //PWM���������PWMռ�ձ�
        uint8_t  Tail;
    }sud;
    uint8_t cud[306*2];
}UpData;

void heartbeat(void);


#endif /* MAIN_H */
