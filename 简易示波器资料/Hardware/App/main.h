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



// 任务定时器
typedef struct {
    uint16_t Timer;//计时器
    uint16_t SpaceTime;//定时时间
    bool RunFalg;//运行标志
    void (*pTaskHook)(void); // 运行的任务函数
} TaskInfo;




enum MenuState{
   PWM = 1,   //PWM状态
   DAC,      //DAC状态
   SetInput,    //设置交直流和衰减

   SetPwmOut, //设置PWM输出状态
   SetPwmOutAck,//确认设置（闪烁）

   SetPwmFreq,//设置Pwm频率
   SetPwmFreqAck,  //确认设置（闪烁）

   SetPwmDutyCycle,//设置PWM占空比
   SetPwmDutyCycleAck,//确认设置（闪烁）

   SetDacOut, //选择设置DAC输出状态
   SetDacOutAck,//确认设置（闪烁）
   SetDacDutyCycle, //选择DAC占空比设置
   SetDacDutyCycleAck,//确认设置DAC占空比（闪烁）

   SetDcAc,  //设置DCAC耦合
   SetDcAcAck,// 设置DCAC耦合(闪烁)
   SetDecay, //设置衰减
   SetDecayAck,//设置衰减（闪烁）
};


// ADC采样点数
#define ADC_VALUE_NUM 300U
// 示波器显示数据
typedef struct {
   uint8_t showbit:1;         //画面更新标志位
   uint8_t IsPwmOut:1;      //PWM输出标志位
   uint8_t IsDACOut:1;      //DAC输出标志位
   uint8_t IsDACDC:1;      //耦合0交流  1直流
   uint8_t IsDecay:1;    //输入是否衰减1无衰减  0 1/50衰减
	 uint8_t IsComSend:1;    //串口是否进行数据发送

   uint32_t sampletime;     //电压采集时间
   uint32_t timerPeriod;    //定时器周期设置
   float gatherFreq;        //示波器采集频率
   float vpp;               //峰峰值
   float voltageValue[300]; //ADC采集电压值

   // PWM情况
   uint16_t PwmOutFreq;     //PWM波形输出频率（展示）
   uint16_t PwmSetFreqValue; //设置频率的值（重装载值）
   uint16_t pwmDutyCycle;   //PWM引脚输出的PWM占空比

   // DAC
   float DacVoltage; //DAC输出的电压
   uint16_t DacDutyCycle;   //Dac引脚输出的PWM占空比

   enum MenuState ThisMenu;   //当前显示的菜单

   int KnobValue;  // 编码器旋钮值

} Oscilloscope;

extern volatile Oscilloscope oscilloscope;


// 定义需要上传了的数据格式
typedef union{
    struct SUD{
        uint8_t Head;

        uint8_t IsPwmOut:1;      //PWM输出标志位
        uint8_t IsDACOut:1;      //DAC输出标志位
        uint8_t IsACDC:1;      //耦合0交流  1直流
        uint8_t IsDecay:1;    //输入是否衰减1无衰减  0 1/50衰减

        uint16_t Vpp;
        uint16_t GatherFreq;        //示波器采集频率
        uint16_t PwmFreq;       //PWM波形输出频率
        uint16_t DacVoltage;  //DAC输出的电压
        uint16_t AdcV[300];
        uint8_t  PwmDutyCycle;   //PWM引脚输出的PWM占空比
        uint8_t  Tail;
    }sud;
    uint8_t cud[306*2];
}UpData;

void heartbeat(void);


#endif /* MAIN_H */
