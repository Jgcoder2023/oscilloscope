#include "timerPwm.h"

/*
*	�������ݣ���ʼ����ʱ�������PWM�ź�
*	������������
*	����ֵ��	��
*/
void Init_PWM_Output(uint32_t period,uint32_t pulse)
{
	//��ʱ����������ṹ��
	timer_oc_parameter_struct timer_ocinitpara;
	
	//��ʱ����ʼ�������ṹ��
    timer_parameter_struct timer_initpara;
	
	//ʹ��ʱ��
	rcu_periph_clock_enable(RCU_GPIOA);
	
	//ʹ�ܶ�ʱ��14ʱ��
	rcu_periph_clock_enable(RCU_TIMER14);
	
	//GPIO����ģʽ����--PA2-TIMER14_CH0
	gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_2);
	
	//�����������
	gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_2);
	
	//����ģʽ0
	gpio_af_set(GPIOA, GPIO_AF_0, GPIO_PIN_2);
	
	//��λ��ʱ��14
	timer_deinit(TIMER14);
	
	//��ʼ����ʱ���ṹ�����
	timer_struct_para_init(&timer_initpara);
	
	timer_initpara.prescaler         = 71;									//Ԥ��Ƶ������
	timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;	                //���ض���
	timer_initpara.counterdirection  = TIMER_COUNTER_UP;		            //���ϼ���
	timer_initpara.period            = period-1;								//����
	timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;		            //ʱ�ӷ�Ƶ
	timer_initpara.repetitioncounter = 0;								    //��װ��ֵ
	timer_init(TIMER14, &timer_initpara);
	
	//��ʼ����ʱ��ͨ����������ṹ��
	timer_channel_output_struct_para_init(&timer_ocinitpara);
	
	timer_ocinitpara.outputstate  = TIMER_CCX_ENABLE;			//���״̬�������ͨ������
	timer_ocinitpara.outputnstate = TIMER_CCXN_DISABLE;			//�������״̬�ر�
	timer_ocinitpara.ocpolarity   = TIMER_OC_POLARITY_HIGH;		//�������Ϊ��
	timer_ocinitpara.ocnpolarity  = TIMER_OCN_POLARITY_HIGH;	//�����������Ϊ��
	timer_ocinitpara.ocidlestate  = TIMER_OC_IDLE_STATE_LOW;	//����״̬ͨ�������
	timer_ocinitpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;	//����״̬����ͨ�������
	
	//ͨ���������
	timer_channel_output_config(TIMER14, TIMER_CH_0, &timer_ocinitpara);
	
	//����Ƚ�ֵ
	timer_channel_output_pulse_value_config(TIMER14, TIMER_CH_0, pulse);
	
	//���ģʽ0������ʱ��С�ڱȽ�ֵʱ�������Ч��ƽ��Ϊ�ߣ����ڱȽ���ֵʱ���Ϊ��
	timer_channel_output_mode_config(TIMER14, TIMER_CH_0, TIMER_OC_MODE_PWM0);
	
	//Ӱ��ģʽ����ر�
	timer_channel_output_shadow_config(TIMER14, TIMER_CH_0, TIMER_OC_SHADOW_DISABLE);
	
	//ʹ���Զ���װ��
	timer_auto_reload_shadow_enable(TIMER14);
	
	//���ö�ʱ��Ϊ��Ҫ�������������ͨ��ʹ��
	timer_primary_output_config(TIMER14, ENABLE);
	
	//ʹ�ܶ�ʱ��
	timer_enable(TIMER14);	
}


// ����PWM�Ŀ���
void SetPwmState(uint8_t value){
	if(value >= 1){
		timer_enable(TIMER14);
		oscilloscope.IsPwmOut = 1;
	}else{
		timer_disable(TIMER14);
		oscilloscope.IsPwmOut = 0;
	}
}


/*
*   �������ݣ�����PWMռ�ձ�
*	������������
*	����ֵ��  ��
*/
void Set_Output_PWMComparex(uint16_t value){
	timer_channel_output_pulse_value_config(TIMER14, TIMER_CH_0, value);
	oscilloscope.pwmDutyCycle = value;
}

/*
*   �������ݣ��������ڣ���Ӧ����Ƶ��
*	������������
*	����ֵ��	��
*/
void Set_Output_Freq(uint32_t value){
  timer_autoreload_value_config(TIMER14,value);
	oscilloscope.PwmOutFreq = (uint16_t)(1000000 / value);
}

