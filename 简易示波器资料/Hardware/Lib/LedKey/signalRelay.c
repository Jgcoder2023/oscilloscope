#include "signalRelay.h"

/*
*   �������ݣ���ʼ��LED����
*   ������������
*   ����ֵ��  ��
*/
void InitSignalGPIO(void){
    //ʹ��ʱ��
    rcu_periph_clock_enable(RCU_GPIOB);
    //�������ģʽ������
    gpio_mode_set(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLDOWN, GPIO_PIN_0|GPIO_PIN_1);
    //����������ͣ����������50Mhz
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,  GPIO_PIN_0|GPIO_PIN_1);
    //Ĭ�ϲ�����
    gpio_bit_reset(GPIOB,GPIO_PIN_0);
    gpio_bit_reset(GPIOB,GPIO_PIN_1);
}



// �����źŽ����ǽ�����ϻ���ֱ�����
void SetSignalAcOrDc(uint8_t value){
		if(value > 0){
			gpio_bit_write(GPIOB,GPIO_PIN_0,SET);
			oscilloscope.IsDACDC = 1;
		}else{
			gpio_bit_write(GPIOB,GPIO_PIN_0,RESET);
			oscilloscope.IsDACDC = 0;
		}
    
}

// ����˥������
void SetSignalDecay(uint8_t value){
	if(value > 0){
		gpio_bit_write(GPIOB,GPIO_PIN_1,SET);
		oscilloscope.IsDecay = 1;
	}else{
		gpio_bit_write(GPIOB,GPIO_PIN_1,RESET);
		oscilloscope.IsDecay = 0;
	}
    
}