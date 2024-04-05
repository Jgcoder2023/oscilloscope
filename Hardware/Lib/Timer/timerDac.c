#include "timerDac.h"

/*
*	�������ݣ���ʼ����ʱ�������PWM�ź�
*	������������
*	����ֵ��	��
13 4ͨ��
*/
void Init_Dac_Output(uint32_t period,uint32_t pulse)
{
	//��ʱ����������ṹ��
	timer_oc_parameter_struct timer_ocinitpara;
	
	//��ʱ����ʼ�������ṹ��
    timer_parameter_struct timer_initpara;
	
	//ʹ��ʱ��
	rcu_periph_clock_enable(RCU_GPIOA);
	
	//ʹ�ܶ�ʱ��13ʱ��
	rcu_periph_clock_enable(RCU_TIMER13);
	
	//GPIO����ģʽ����--PA4-TIMER13_CH0
	gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_4);
	
	//�����������
	gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_4);
	
	//����ģʽ4
	gpio_af_set(GPIOA, GPIO_AF_4, GPIO_PIN_4);
	
	//��λ��ʱ��13
	timer_deinit(TIMER13);
	
	//��ʼ����ʱ���ṹ�����
	timer_struct_para_init(&timer_initpara);
	
	timer_initpara.prescaler         = 71;									//Ԥ��Ƶ������
	timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;	                //���ض���
	timer_initpara.counterdirection  = TIMER_COUNTER_UP;		            //���ϼ���
	timer_initpara.period            = period - 1;								//����
	timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;		            //ʱ�ӷ�Ƶ
	timer_initpara.repetitioncounter = 0;								    //��װ��ֵ
	timer_init(TIMER13, &timer_initpara);
	
	//��ʼ����ʱ��ͨ����������ṹ��
	timer_channel_output_struct_para_init(&timer_ocinitpara);
	
	timer_ocinitpara.outputstate  = TIMER_CCX_ENABLE;			//���״̬�������ͨ������
	timer_ocinitpara.outputnstate = TIMER_CCXN_DISABLE;			//�������״̬�ر�
	timer_ocinitpara.ocpolarity   = TIMER_OC_POLARITY_HIGH;		//�������Ϊ��
	timer_ocinitpara.ocnpolarity  = TIMER_OCN_POLARITY_HIGH;	//�����������Ϊ��
	timer_ocinitpara.ocidlestate  = TIMER_OC_IDLE_STATE_LOW;	//����״̬ͨ�������
	timer_ocinitpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;	//����״̬����ͨ�������
	
	//ͨ���������
	timer_channel_output_config(TIMER13, TIMER_CH_0, &timer_ocinitpara);
	
	//����Ƚ�ֵ
	timer_channel_output_pulse_value_config(TIMER13, TIMER_CH_0, pulse);
	
	//���ģʽ0������ʱ��С�ڱȽ�ֵʱ�������Ч��ƽ��Ϊ�ߣ����ڱȽ���ֵʱ���Ϊ��
	timer_channel_output_mode_config(TIMER13, TIMER_CH_0, TIMER_OC_MODE_PWM0);
	
	//Ӱ��ģʽ����ر�
	timer_channel_output_shadow_config(TIMER13, TIMER_CH_0, TIMER_OC_SHADOW_DISABLE);
	
	//ʹ���Զ���װ��
	timer_auto_reload_shadow_enable(TIMER13);
	
	//���ö�ʱ��Ϊ��Ҫ�������������ͨ��ʹ��
	timer_primary_output_config(TIMER13, ENABLE);
	
	//ʹ�ܶ�ʱ��
	timer_enable(TIMER13);	

	// �����״ε�DAC��ѹ
	oscilloscope.DacVoltage = 3.3f * oscilloscope.DacDutyCycle / 1000;
}

/*
*   �������ݣ�����PWMռ�ձ�
*	������������
*	����ֵ��  ��
*/
void SetDacComparex(uint16_t value){
	timer_channel_output_pulse_value_config(TIMER13, TIMER_CH_0, value);	
	oscilloscope.DacVoltage = 3.3f * value / 1000;
	if(oscilloscope.DacVoltage < 0){
		oscilloscope.DacVoltage = 0;
	}
}

/*
*   �������ݣ��������ڣ���Ӧ����Ƶ��
*	������������
*	����ֵ��	��
*/
void Set_Output2_Freq(uint32_t value){
    timer_autoreload_value_config(TIMER13,value);
}


// ����DAC�Ŀ���
void SetDacState(uint8_t value){
	if(value >= 1){
		timer_enable(TIMER13);
		oscilloscope.IsDACOut = 1;
	}else{
		timer_disable(TIMER13);
		oscilloscope.IsDACOut = 0;
	}
}
