#ifndef		__APP_H
#define		__APP_H


#include <stdio.h>
// #include <string.h>
#include	".\library\config.h"
#include	".\library\STC32G_GPIO.h"
#include	".\library\STC32G_Delay.h"
#include	".\library\STC32G_Timer.h"
#include	".\library\STC32G_NVIC.h"
#include	".\library\STC32G_UART.h"
#include	".\library\STC32G_Switch.h"
#include	".\library\STC32G_Soft_I2C.h"
#include	".\library\STC32G_ADC.h"
#include	".\library\STC32G_DMA.h"
#include	".\library\STC32G_PWM.h"

#include	".\drive\oled.h"
#include	".\drive\oledfont.h"
#include  ".\drive\key.h"


#include "init.h"
#include "server.h"


// 任务定时器
typedef struct {
    u16 Timer;//计时器
    u16 SpaceTime;//定时时间
    u8 RunFalg;//运行标志
    void (*pTaskHook)(void); // 运行的任务函数
} TaskInfo;


// 页面枚举
enum PageState{
    Index,  //主页
    Result1, //检测结果页
    Result2, //检测结果页
    Result3, //检测结果页
    Result4, //检测结果页
    Charge, //充电页
};


// 总体的刷新状态
typedef struct{
   enum PageState Page;
   u8 IsRefresh;//是否需要刷新一次
   u8 BatteryNum; //充的电池节数
   u8 x;
   u8 y;
} RefreshState;




void heartbeat(void);

#endif