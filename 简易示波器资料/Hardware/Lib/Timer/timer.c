#include "timer.h"
#include "main.h"
#include "led.h"

/*
*   �������ݣ���ʼ��ͨ�ö�ʱ��15
*   ������������
*   ����ֵ��  ��
*/
void Init_GeneralTimer5(void){   
    //ʹ��ʱ��
    rcu_periph_clock_enable(RCU_TIMER5);
    
    //��ʱ����ʼ�������ṹ��
    timer_parameter_struct timer_initpara;
    
    //��λ��ʱ��15
	timer_deinit(TIMER5);
    
    //��������Ԥ��Ƶ��
    timer_prescaler_config(TIMER5,1,TIMER_PSC_RELOAD_NOW);
    
    // ��ʼ���ṹ��
    timer_struct_para_init(&timer_initpara);
    
    // ���ö�ʱ������ 
	timer_initpara.prescaler        = 719;                 //  ʱ��Ԥ��Ƶֵ 0-65535  psc_clk = CK_TIMER / pre
	timer_initpara.alignedmode      = TIMER_COUNTER_EDGE;   // ��Ե����
	timer_initpara.counterdirection = TIMER_COUNTER_UP;     // ���ϼ���
	timer_initpara.period           = 99;                   // ����
	//�����벶���ʱ��ʹ��  �����˲���ʹ�õĲ���Ƶ��֮��ķ�Ƶ���� 
	timer_initpara.clockdivision = TIMER_CKDIV_DIV1;        // ��Ƶ����
	//ֻ�и߼���ʱ������ ����Ϊx�����ظ�x+1�ν����ж� 
	timer_initpara.repetitioncounter = 0; // �ظ������� 0-255  
	timer_init(TIMER5,&timer_initpara); // ��ʼ����ʱ��
    
    //��ʱ�������жϱ�־λ���
    timer_interrupt_flag_clear(TIMER5,TIMER_INT_FLAG_UP);
    
    //�ж����ȼ�
    nvic_irq_enable(TIMER5_IRQn,0);
    
    //ʹ�ܸ����¼��ж�
    timer_interrupt_enable(TIMER5,TIMER_INT_UP); 

    timer_enable(TIMER5);
}
// �����Ļص�
void TIMER5_IRQHandler(void){
    if(timer_interrupt_flag_get(TIMER5,TIMER_INT_FLAG_UP) == SET){
		heartbeat();

        //��ʱ�������жϱ�־λ���
        timer_interrupt_flag_clear(TIMER5,TIMER_INT_FLAG_UP);
    }
}