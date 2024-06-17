#include "signalRelay.h"

/*
*   函数内容：初始化LED引脚
*   函数参数：无
*   返回值：  无
*/
void InitSignalGPIO(void){
    //使能时钟
    rcu_periph_clock_enable(RCU_GPIOB);
    //设置输出模式，下拉
    gpio_mode_set(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLDOWN, GPIO_PIN_0|GPIO_PIN_1);
    //设置输出类型，推挽输出，50Mhz
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,  GPIO_PIN_0|GPIO_PIN_1);
    //默认不开灯
    gpio_bit_reset(GPIOB,GPIO_PIN_0);
    gpio_bit_reset(GPIOB,GPIO_PIN_1);
}



// 设置信号进来是交流耦合还是直流耦合
void SetSignalAcOrDc(uint8_t value){
		if(value > 0){
			gpio_bit_write(GPIOB,GPIO_PIN_0,SET);
			oscilloscope.IsDACDC = 1;
		}else{
			gpio_bit_write(GPIOB,GPIO_PIN_0,RESET);
			oscilloscope.IsDACDC = 0;
		}
    
}

// 设置衰减倍数
void SetSignalDecay(uint8_t value){
	if(value > 0){
		gpio_bit_write(GPIOB,GPIO_PIN_1,SET);
		oscilloscope.IsDecay = 1;
	}else{
		gpio_bit_write(GPIOB,GPIO_PIN_1,RESET);
		oscilloscope.IsDecay = 0;
	}
    
}