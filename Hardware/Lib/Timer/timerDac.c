#include "timerDac.h"

/*
*	函数内容：初始化定时器，输出PWM信号
*	函数参数：无
*	返回值：	无
13 4通道
*/
void Init_Dac_Output(uint32_t period,uint32_t pulse)
{
	//定时器输出参数结构体
	timer_oc_parameter_struct timer_ocinitpara;
	
	//定时器初始化参数结构体
    timer_parameter_struct timer_initpara;
	
	//使能时钟
	rcu_periph_clock_enable(RCU_GPIOA);
	
	//使能定时器13时钟
	rcu_periph_clock_enable(RCU_TIMER13);
	
	//GPIO复用模式设置--PA4-TIMER13_CH0
	gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_4);
	
	//输出类型设置
	gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_4);
	
	//复用模式4
	gpio_af_set(GPIOA, GPIO_AF_4, GPIO_PIN_4);
	
	//复位定时器13
	timer_deinit(TIMER13);
	
	//初始化定时器结构体参数
	timer_struct_para_init(&timer_initpara);
	
	timer_initpara.prescaler         = 71;									//预分频器参数
	timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;	                //边沿对齐
	timer_initpara.counterdirection  = TIMER_COUNTER_UP;		            //向上计数
	timer_initpara.period            = period - 1;								//周期
	timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;		            //时钟分频
	timer_initpara.repetitioncounter = 0;								    //重装载值
	timer_init(TIMER13, &timer_initpara);
	
	//初始化定时器通道输出参数结构体
	timer_channel_output_struct_para_init(&timer_ocinitpara);
	
	timer_ocinitpara.outputstate  = TIMER_CCX_ENABLE;			//输出状态，主输出通道开启
	timer_ocinitpara.outputnstate = TIMER_CCXN_DISABLE;			//互补输出状态关闭
	timer_ocinitpara.ocpolarity   = TIMER_OC_POLARITY_HIGH;		//输出极性为高
	timer_ocinitpara.ocnpolarity  = TIMER_OCN_POLARITY_HIGH;	//互补输出极性为高
	timer_ocinitpara.ocidlestate  = TIMER_OC_IDLE_STATE_LOW;	//空闲状态通道输出低
	timer_ocinitpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;	//空闲状态互补通道输出低
	
	//通道输出配置
	timer_channel_output_config(TIMER13, TIMER_CH_0, &timer_ocinitpara);
	
	//输出比较值
	timer_channel_output_pulse_value_config(TIMER13, TIMER_CH_0, pulse);
	
	//输出模式0，当计时器小于比较值时，输出有效电平，为高，大于比较器值时输出为低
	timer_channel_output_mode_config(TIMER13, TIMER_CH_0, TIMER_OC_MODE_PWM0);
	
	//影子模式输出关闭
	timer_channel_output_shadow_config(TIMER13, TIMER_CH_0, TIMER_OC_SHADOW_DISABLE);
	
	//使能自动重装载
	timer_auto_reload_shadow_enable(TIMER13);
	
	//配置定时器为主要输出函数，所有通道使能
	timer_primary_output_config(TIMER13, ENABLE);
	
	//使能定时器
	timer_enable(TIMER13);	

	// 计算首次的DAC电压
	oscilloscope.DacVoltage = 3.3f * oscilloscope.DacDutyCycle / 1000;
}

/*
*   函数内容：设置PWM占空比
*	函数参数：无
*	返回值：  无
*/
void SetDacComparex(uint16_t value){
	timer_channel_output_pulse_value_config(TIMER13, TIMER_CH_0, value);	
	oscilloscope.DacVoltage = 3.3f * value / 1000;
	if(oscilloscope.DacVoltage < 0){
		oscilloscope.DacVoltage = 0;
	}
}

/*
*   函数内容：设置周期，对应设置频率
*	函数参数：无
*	返回值：	无
*/
void Set_Output2_Freq(uint32_t value){
    timer_autoreload_value_config(TIMER13,value);
}


// 设置DAC的开关
void SetDacState(uint8_t value){
	if(value >= 1){
		timer_enable(TIMER13);
		oscilloscope.IsDACOut = 1;
	}else{
		timer_disable(TIMER13);
		oscilloscope.IsDACOut = 0;
	}
}
