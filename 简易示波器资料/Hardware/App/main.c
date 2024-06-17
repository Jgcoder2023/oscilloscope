#include "main.h"

volatile Oscilloscope oscilloscope = {
    .showbit    = 1,//�����ʾ��־λ
    .sampletime =ADC_SAMPLETIME_239POINT5,  //adc��������
    .IsPwmOut = 1,//Ĭ�ϴ�PWM���
    .IsDACOut = 1,//Ĭ�ϴ�DAC���
    .IsDecay = 0,  //1/50��˥��
    .IsDACDC = 0,  //AC
		.IsComSend = 1,
    .gatherFreq =1000,//�ɼ�Ƶ��
    .PwmOutFreq =1000,//���Ƶ��
    .PwmSetFreqValue = 1000,  //��װ��ֵ
    .pwmDutyCycle=500,//PWM���������PWMռ�ձ�
    .timerPeriod=1000,//PWM�����ʱ������
    .vpp        =0.0f,//���ֵ

    .DacDutyCycle = 500, //ռ�ձ�  500/1000

    .KnobValue = 0,
    .ThisMenu = PWM,   //Ĭ��״̬ΪPWM
};

void Init(){
  //	��ʼ���δ�ʱ��
	systick_config();	
	
  //��ʼ��LED����
	Init_LED_GPIO();

  //��ʼ����������
  Init_Key_GPIO();
  // ��ʼ��EC11
  Init_EC11_EXITGPIO();

  // ��ʼ���źż̵���
  InitSignalGPIO();
	
	//LCD��ʼ��
	TFT_Init();
	//����ɫ
  TFT_Fill(0,0,160,128,BLACK);
	
  // ��ʾ����UI
	PWM_StaticUI();

  //��ʼ��Ƶ�ʲ�����ʱ��2
  //Init_FreqTimer();

   //ADC DMA��ʼ��
  ADC_DMA_Init();
  

  //��ʼ������
  Init_USART(115200);
  USART0_SendData("Hello World\r\n");

  // ��ʼ��������ר��ȥ����ʱ��
  Init_GeneralTimer15();
	
	
	//��ʼ��PWM��������Ƶ��=��72000000/((71+1)*(period+1))��
  //ռ�ձ���������Ƚ�ֵ���жϣ�ͬʱ���뼫����ģʽ�й�
  //�Ƚ�ֵ*��������ռ�ձȣ�500/1000=50%ռ�ձȣ�������1K
  Init_PWM_Output(oscilloscope.PwmSetFreqValue,oscilloscope.pwmDutyCycle);
  Init_Dac_Output(1000,oscilloscope.DacDutyCycle);

  //��ʼ��������ʱ��5 (Ҳ��������)
  Init_GeneralTimer5();
	
	SetSignalAcOrDc(0);
	SetSignalDecay(0);
}


//uint8_t TestValue;
void taskLed(void){
	gpio_bit_toggle(GPIOC,GPIO_PIN_13);    //LED Run����˸
}



// ����ɨ��
void task_10ms(void){
  uint8_t num=0;
  USART0_RecHandle();
  KeyInputScan();
  KeyValueCalculate();
  switch (oscilloscope.ThisMenu){
    case PWM:
      PWM_StaticUI();
      break;

    // ����Ƶ�ʿ���ϵ��
    case SetPwmOut:
      SetPwmOutFc();
    break;
    case SetPwmOutAck:
      SetPwmOutAckFc();
    break;

    // ����Ƶ��ϵ��
    case SetPwmFreq:
      SetPwmFreqFc();
    break;
    case SetPwmFreqAck:
      SetPwmFreqAckFc();
    break;

    // ����ռ�ձ�ϵ��
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
    
    //�м�ֵ
    float median=0;
    
    //���ֵ
    float voltage=0;
    
    //������ѹֵ
    float max_data=1.0f;
    
    //���ηŴ���
    float gainFactor=0;
		float adcValue = 0;
	    //�����ر��
    uint16_t Trigger_number=0;
	

	//�����ȡ��ѹֵ��ɣ���ʼˢ��
        if(oscilloscope.showbit==1){           
            oscilloscope.showbit=0;
            oscilloscope.vpp=0;
            
            //ת����ѹֵ
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


            //ˢ����ͬʱ��ȡ��ѹֵ
            dma_transfer_number_config(DMA_CH0, ADC_VALUE_NUM);
            dma_channel_enable(DMA_CH0);
            
            //�ҵ���ʼ��ʾ����ֵ
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
            
            //�����ֵ��С������ַŴ��������²�����ʾ�쳣������
            if(oscilloscope.vpp > 0.3){
								//��ȡ�м�ֵ
								median = oscilloscope.vpp / 2.0f;
                //�Ŵ�������Ҫȷ���Ŵ�֮������䣬�ҽ����ι̶���ʾ�ڣ�18.75~41.25�У���(41.25-18.75)/2=11.25f
                gainFactor = 11.25f/median;
            }
            
            //������ʾ����100�����ݣ��������Է�ֹ���ι���
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
        //������ʾUI
        TFT_ShowUI(); 
				SendAllData();
}




//������
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

// ����
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



