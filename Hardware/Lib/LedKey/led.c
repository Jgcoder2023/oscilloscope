#include "led.h"

/*
*   �������ݣ���ʼ��LED����
*   ������������
*   ����ֵ��  ��
*/
void Init_LED_GPIO(void){
    //ʹ��ʱ��
    rcu_periph_clock_enable(RCU_GPIOC);
    //�������ģʽ������
    gpio_mode_set(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLDOWN, GPIO_PIN_13);
    //�������ģʽ����������
    //gpio_mode_set(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_15);
    //����������ͣ����������50Mhz
    gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_13);
    //����������ͣ����������50Mhz
    //gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_15);
    //Ĭ�ϲ�����
}