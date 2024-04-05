#include "ec11.h"
/*
*   函数内容：初始化EC11引脚,DE与B引脚与普通引脚一样
*   函数参数：无
*   返回值：  无
*/
void Init_EC11_GPIO(void){
    //使能时钟
    rcu_periph_clock_enable(RCU_GPIOB);
    //设置输出模式，上拉
    gpio_mode_set(GPIOB, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, GPIO_PIN_3);

}

/*
*   函数内容：初始化EC11引脚,旋转判断引脚A需要使用中断
*   函数参数：无
*   返回值：  无
*/
void Init_EC11_EXITGPIO(void){
	Init_EC11_GPIO();
	
	//使能时钟GPIOB，CMP
    rcu_periph_clock_enable(RCU_GPIOB);
	rcu_periph_clock_enable(RCU_CFGCMP);
	
	//设置引脚模式，上拉
	gpio_mode_set(GPIOB,GPIO_MODE_INPUT,GPIO_PUPD_PULLUP,GPIO_PIN_4);
	
	//中断使能
	nvic_irq_enable(EXTI4_15_IRQn,1U);
	
	//配置中断线
	syscfg_exti_line_config(EXTI_SOURCE_GPIOB,EXTI_SOURCE_PIN4);
	
	//初始化中断线，设置为中断模式，下升沿触发
	exti_init(EXTI_4,EXTI_INTERRUPT,EXTI_TRIG_FALLING);
	
    //中断线使能
    exti_interrupt_enable(EXTI_4);
    
	//中断标志位清除
	exti_interrupt_flag_clear(EXTI_4);    
}


/*
*   函数内容：中断线4-15中断服务函数
*   函数参数：无
*   返回值：  无
*/
void EXTI4_15_IRQHandler(void){
   if(RESET != exti_interrupt_flag_get(EXTI_4)){
        startTime15();  //开启定时器的1ms延时
        exti_interrupt_flag_clear(EXTI_4);  //中断标志位清除
    }
}

// 定时器15的回调(编码器专用去抖的演示定时器1ms)
void TIMER15_IRQHandler(void){
    if(timer_interrupt_flag_get(TIMER15,TIMER_INT_FLAG_UP) == SET){
        timer_disable(TIMER15);   //禁用定时器
        timer_interrupt_flag_clear(TIMER15,TIMER_INT_FLAG_UP);//定时器更新中断标志位清除
        if(gpio_input_bit_get(GPIOB,GPIO_PIN_3) == RESET){
            oscilloscope.KnobValue += 1;
        }else{
            oscilloscope.KnobValue -= 1;
        }
    }
}


