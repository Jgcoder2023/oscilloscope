#include "main.h"

volatile Oscilloscope oscilloscope = {
    .showbit    = 1,//清除显示标志位
    .sampletime =ADC_SAMPLETIME_239POINT5,  //adc采样周期
    .IsPwmOut = 1,//默认打开PWM输出
    .IsDACOut = 1,//默认打开DAC输出
    .IsDecay = 0,  //1/50不衰减
    .IsDACDC = 0,  //AC
		.IsComSend = 1,
    .gatherFreq =1000,//采集频率
    .PwmOutFreq =1000,//输出频率
    .PwmSetFreqValue = 1000,  //重装载值
    .pwmDutyCycle=500,//PWM引脚输出的PWM占空比
    .timerPeriod=1000,//PWM输出定时器周期
    .vpp        =0.0f,//峰峰值

    .DacDutyCycle = 500, //占空比  500/1000

    .KnobValue = 0,
    .ThisMenu = PWM,   //默认状态为PWM
};

void Init(){
  //	初始化滴答定时器
	systick_config();	
	
  //初始化LED引脚
	Init_LED_GPIO();

  //初始化按键引脚
  Init_Key_GPIO();
  // 初始化EC11
  Init_EC11_EXITGPIO();

  // 初始化信号继电器
  InitSignalGPIO();
	
	//LCD初始化
	TFT_Init();
	//填充白色
  TFT_Fill(0,0,160,128,BLACK);
	
  // 画示波器UI
	PWM_StaticUI();

  //初始化频率测量定时器2
  //Init_FreqTimer();

   //ADC DMA初始化
  ADC_DMA_Init();
  

  //初始化串口
  Init_USART(115200);
  USART0_SendData("Hello World\r\n");

  // 初始化编码器专用去抖定时器
  Init_GeneralTimer15();
	
	
	//初始化PWM输出，输出频率=（72000000/((71+1)*(period+1))）
  //占空比是周期与比较值的判断，同时还与极性与模式有关
  //比较值*周期则是占空比，500/1000=50%占空比，周期是1K
  Init_PWM_Output(oscilloscope.PwmSetFreqValue,oscilloscope.pwmDutyCycle);
  Init_Dac_Output(1000,oscilloscope.DacDutyCycle);

  //初始化基本定时器5 (也就是心跳)
  Init_GeneralTimer5();
	
	SetSignalAcOrDc(0);
	SetSignalDecay(0);
}


//uint8_t TestValue;
void taskLed(void){
	gpio_bit_toggle(GPIOC,GPIO_PIN_13);    //LED Run灯闪烁
}



// 按键扫描
void task_10ms(void){
  uint8_t num=0;
  USART0_RecHandle();
  KeyInputScan();
  KeyValueCalculate();
  switch (oscilloscope.ThisMenu){
    case PWM:
      PWM_StaticUI();
      break;

    // 设置频率开关系列
    case SetPwmOut:
      SetPwmOutFc();
    break;
    case SetPwmOutAck:
      SetPwmOutAckFc();
    break;

    // 设置频率系列
    case SetPwmFreq:
      SetPwmFreqFc();
    break;
    case SetPwmFreqAck:
      SetPwmFreqAckFc();
    break;

    // 设置占空比系列
    case SetPwmDutyCycle:
      SetPwmDutyCycleFc();
    break;
    case SetPwmDutyCycleAck:
      SetPwmDutyCycleAckFc();
    break;

    case DAC:
      DAC_StaticUI();
    break;
    case SetDacOut:
      SetDacOutFc();
    break;
    case SetDacOutAck:
      SetDacOutAckFc();
    break;
    case SetDacDutyCycle:
      SetDacDutyCycleFc();
    break;
    case SetDacDutyCycleAck:
      SetDacDutyCycleAckFc();
    break;

    case SetInput:
      SET_StaticUI();
    case SetDcAc:
      SetDcAcFc();
    break;
    case SetDcAcAck:
      SetDcAcAckFc();
    break;
    case SetDecay:
      SetDecayFc();
    break;
    case SetDecayAck:
      SetDecayAckFc();
    break;
    break;
    default:
    break;
  }

}

void task_100ms(void){
    uint16_t i=0;
    
    //中间值
    float median=0;
    
    //峰峰值
    float voltage=0;
    
    //触发电压值
    float max_data=1.0f;
    
    //波形放大倍数
    float gainFactor=0;
		float adcValue = 0;
	    //触发沿标记
    uint16_t Trigger_number=0;
	

	//如果获取电压值完成，开始刷屏
        if(oscilloscope.showbit==1){           
            oscilloscope.showbit=0;
            oscilloscope.vpp=0;
            
            //转换电压值
            for(i=0;i<ADC_VALUE_NUM;i++){
								adcValue = (Get_ADC_Value(i)*3.3f)/4096.0f;
								oscilloscope.voltageValue[i] = (5 - (2.0f*adcValue));
                if((oscilloscope.vpp) < oscilloscope.voltageValue[i]){
                    oscilloscope.vpp = oscilloscope.voltageValue[i];
                }
//                if(oscilloscope.vpp <= 0.16){
//                    oscilloscope.gatherFreq=0;
//                }
            }


            //刷屏的同时获取电压值
            dma_transfer_number_config(DMA_CH0, ADC_VALUE_NUM);
            dma_channel_enable(DMA_CH0);
            
            //找到起始显示波形值
            for(i=0;i<ADC_VALUE_NUM-100;i++){
                if(oscilloscope.voltageValue[i] < max_data){
                  for(;i<ADC_VALUE_NUM-100;i++){
                    if(oscilloscope.voltageValue[i] > max_data){
                        Trigger_number=i;
                        break;
                      }
                  }
                  break;
               }
            }
            
            //如果幅值过小，会出现放大倍数过大导致波形显示异常的问题
            if(oscilloscope.vpp > 0.3){
								//获取中间值
								median = oscilloscope.vpp / 2.0f;
                //放大倍数，需要确定放大之后的区间，我将波形固定显示在（18.75~41.25中），(41.25-18.75)/2=11.25f
                gainFactor = 11.25f/median;
            }
            
            //依次显示后续100个数据，这样可以防止波形滚动
            for(i=Trigger_number;i<Trigger_number+100;i++){
                voltage = oscilloscope.voltageValue[i];
                if(voltage >= median){
                    voltage = 30 + (voltage - median)*gainFactor;
                }else{
                    voltage = 30 - (median - voltage)*gainFactor;
                }
                drawCurve(80,voltage);
          }  
        }        
        //参数显示UI
        TFT_ShowUI(); 
				SendAllData();
}




//任务树
#define TaskMaxNum 3
static TaskInfo task[TaskMaxNum] = {
  {10,10,0,task_10ms},
  {100,100,0,task_100ms},
  {1000,1000,0,taskLed},
};

int main(void){
  Init();
	while(1){
    for (uint8_t i = 0; i < TaskMaxNum; i++){
      if(task[i].RunFalg == true){
        task[i].pTaskHook();
        task[i].RunFalg = false;
      }
    }
	}
}

// 心跳
void heartbeat(void){
  for (uint8_t i = 0; i < TaskMaxNum; i++){
    if(task[i].Timer > 0 && task[i].RunFalg == false){
      task[i].Timer--;
      if(task[i].Timer == 0){
        task[i].RunFalg = true;
        task[i].Timer = task[i].SpaceTime;
      }
    }
  }
}



