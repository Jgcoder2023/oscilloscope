#include "ec11.h"
/*
*   �������ݣ���ʼ��EC11����,DE��B��������ͨ����һ��
*   ������������
*   ����ֵ��  ��
*/
void Init_EC11_GPIO(void){
    //ʹ��ʱ��
    rcu_periph_clock_enable(RCU_GPIOB);
    //�������ģʽ������
    gpio_mode_set(GPIOB, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, GPIO_PIN_3);

}

/*
*   �������ݣ���ʼ��EC11����,��ת�ж�����A��Ҫʹ���ж�
*   ������������
*   ����ֵ��  ��
*/
void Init_EC11_EXITGPIO(void){
	Init_EC11_GPIO();
	
	//ʹ��ʱ��GPIOB��CMP
    rcu_periph_clock_enable(RCU_GPIOB);
	rcu_periph_clock_enable(RCU_CFGCMP);
	
	//��������ģʽ������
	gpio_mode_set(GPIOB,GPIO_MODE_INPUT,GPIO_PUPD_PULLUP,GPIO_PIN_4);
	
	//�ж�ʹ��
	nvic_irq_enable(EXTI4_15_IRQn,1U);
	
	//�����ж���
	syscfg_exti_line_config(EXTI_SOURCE_GPIOB,EXTI_SOURCE_PIN4);
	
	//��ʼ���ж��ߣ�����Ϊ�ж�ģʽ�������ش���
	exti_init(EXTI_4,EXTI_INTERRUPT,EXTI_TRIG_FALLING);
	
    //�ж���ʹ��
    exti_interrupt_enable(EXTI_4);
    
	//�жϱ�־λ���
	exti_interrupt_flag_clear(EXTI_4);    
}


/*
*   �������ݣ��ж���4-15�жϷ�����
*   ������������
*   ����ֵ��  ��
*/
void EXTI4_15_IRQHandler(void){
   if(RESET != exti_interrupt_flag_get(EXTI_4)){
        startTime15();  //������ʱ����1ms��ʱ
        exti_interrupt_flag_clear(EXTI_4);  //�жϱ�־λ���
    }
}

// ��ʱ��15�Ļص�(������ר��ȥ������ʾ��ʱ��1ms)
void TIMER15_IRQHandler(void){
    if(timer_interrupt_flag_get(TIMER15,TIMER_INT_FLAG_UP) == SET){
        timer_disable(TIMER15);   //���ö�ʱ��
        timer_interrupt_flag_clear(TIMER15,TIMER_INT_FLAG_UP);//��ʱ�������жϱ�־λ���
        if(gpio_input_bit_get(GPIOB,GPIO_PIN_3) == RESET){
            oscilloscope.KnobValue += 1;
        }else{
            oscilloscope.KnobValue -= 1;
        }
    }
}


