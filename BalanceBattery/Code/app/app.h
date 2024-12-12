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


// ����ʱ��
typedef struct {
    u16 Timer;//��ʱ��
    u16 SpaceTime;//��ʱʱ��
    u8 RunFalg;//���б�־
    void (*pTaskHook)(void); // ���е�������
} TaskInfo;


// ҳ��ö��
enum PageState{
    Index,  //��ҳ
    Result1, //�����ҳ
    Result2, //�����ҳ
    Result3, //�����ҳ
    Result4, //�����ҳ
    Charge, //���ҳ
};


// �����ˢ��״̬
typedef struct{
   enum PageState Page;
   u8 IsRefresh;//�Ƿ���Ҫˢ��һ��
   u8 BatteryNum; //��ĵ�ؽ���
   u8 x;
   u8 y;
} RefreshState;




void heartbeat(void);

#endif