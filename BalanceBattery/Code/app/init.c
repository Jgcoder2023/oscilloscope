#include "init.h"
#include "app.h"


/***************	����˵��	****************

����ʱ, ѡ��ʱ�� 24MHz (�����������ļ�"config.h"���޸�).
******************************************/

void RunLedConfig(void){	 
	P2_MODE_OUT_PP(GPIO_Pin_2);   //��ʼ��led��gpio
	P2_MODE_IO_PU(GPIO_Pin_3 | GPIO_Pin_4);	//��ʼ����i2c��	//P3.2,P3.3 ����Ϊ׼˫���
	P0_MODE_IN_HIZ(GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2);   //�����ʾ��io��
	P3_MODE_IN_HIZ(GPIO_Pin_5);   //bat1������ʾ
	P1_MODE_IN_HIZ(GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);   //bat2-4������ʾ

	P5_MODE_OUT_PP(GPIO_Pin_3);   //����ܿ���
	P3_MODE_OUT_PP(GPIO_Pin_3 | GPIO_Pin_4);   //����ӿ���
	P4_MODE_OUT_PP(GPIO_Pin_3 | GPIO_Pin_7);   //����ӿ���
}

/************************ ��ʱ������ ****************************/
void TimerConfig(void){
	TIM_InitTypeDef		TIM_InitStructure;					//�ṹ����
	TIM_InitStructure.TIM_Mode      = TIM_16BitAutoReload;	//ָ������ģʽ,   TIM_16BitAutoReload,TIM_16Bit,TIM_8BitAutoReload,TIM_16BitAutoReloadNoMask
	TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_1T;		//ָ��ʱ��Դ,     TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
	TIM_InitStructure.TIM_ClkOut    = DISABLE;				//�Ƿ������������, ENABLE��DISABLE
	TIM_InitStructure.TIM_Value     = (u16)(65536UL - (MAIN_Fosc / 2000));		//��ֵ,(��ʱ1ms)
	TIM_InitStructure.TIM_PS        = 0;					//8λԤ��Ƶ��(n+1), 0~255
	TIM_InitStructure.TIM_Run       = ENABLE;				//�Ƿ��ʼ����������ʱ��, ENABLE��DISABLE
	Timer_Inilize(Timer0,&TIM_InitStructure);				//��ʼ��Timer0	  Timer0,Timer1,Timer2,Timer3,Timer4
	NVIC_Timer0_Init(ENABLE,Priority_0);		//�ж�ʹ��, ENABLE/DISABLE; ���ȼ�(�͵���) Priority_0,Priority_1,Priority_2,Priority_3
}


/***************  ���ڳ�ʼ������ *****************/
void UartConfig(void){	
	COMx_InitDefine	COMx_InitStructure;		//�ṹ����
	
	P3_MODE_IO_PU(GPIO_Pin_0 | GPIO_Pin_1); //��ʼ������gpio
	
	
	COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;	//ģʽ, UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
	COMx_InitStructure.UART_BRT_Use   = BRT_Timer1;			//ѡ�����ʷ�����, BRT_Timer1, BRT_Timer2 (ע��: ����2�̶�ʹ��BRT_Timer2)
	COMx_InitStructure.UART_BaudRate  = 115200ul;			//������, һ�� 110 ~ 115200
	COMx_InitStructure.UART_RxEnable  = ENABLE;				//��������,   ENABLE��DISABLE
	COMx_InitStructure.BaudRateDouble = DISABLE;			//�����ʼӱ�, ENABLE��DISABLE
	UART_Configuration(UART1, &COMx_InitStructure);		//��ʼ������1 UART1,UART2,UART3,UART4
	NVIC_UART1_Init(ENABLE,Priority_1);		//�ж�ʹ��, ENABLE/DISABLE; ���ȼ�(�͵���) Priority_0,Priority_1,Priority_2,Priority_3
	UART1_SW(UART1_SW_P30_P31);		//UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17,UART1_SW_P43_P44
}



/******************** ADC ���� ********************/
void AdcConfig(void){
	ADC_InitTypeDef		ADC_InitStructure;		//�ṹ����
//	DMA_ADC_InitTypeDef		DMA_ADC_InitStructure;		//�ṹ����
	
	//gpio
	P0_MODE_IN_HIZ(GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6);		//P0.3~P0.6 ����Ϊ��������
	P1_MODE_IN_HIZ(GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4);

	
	ADC_InitStructure.ADC_SMPduty   = 31;		//ADC ģ���źŲ���ʱ�����, 0~31��ע�⣺ SMPDUTY һ����������С�� 10��
	ADC_InitStructure.ADC_CsSetup   = 0;		//ADC ͨ��ѡ��ʱ����� 0(Ĭ��),1
	ADC_InitStructure.ADC_CsHold    = 1;		//ADC ͨ��ѡ�񱣳�ʱ����� 0,1(Ĭ��),2,3
	ADC_InitStructure.ADC_Speed     = ADC_SPEED_2X16T;		//���� ADC ����ʱ��Ƶ��	ADC_SPEED_2X1T~ADC_SPEED_2X16T
	ADC_InitStructure.ADC_AdjResult = ADC_RIGHT_JUSTIFIED;	//ADC�������,	ADC_LEFT_JUSTIFIED,ADC_RIGHT_JUSTIFIED
	ADC_Inilize(&ADC_InitStructure);		//��ʼ��
	ADC_PowerControl(ENABLE);						//ADC��Դ����, ENABLE��DISABLE
	NVIC_ADC_Init(DISABLE,Priority_0);		//�ж�ʹ��, ENABLE/DISABLE; ���ȼ�(�͵���) Priority_0,Priority_1,Priority_2,Priority_3
	
	
//	/******************** DMA ���� ********************/
//	DMA_ADC_InitStructure.DMA_Enable = ENABLE;			//DMAʹ��  	ENABLE,DISABLE
//	DMA_ADC_InitStructure.DMA_Channel = 0xffff;			//ADCͨ��ʹ�ܼĴ���, 1:ʹ��, bit15~bit0 ��Ӧ ADC15~ADC0
//	DMA_ADC_InitStructure.DMA_Buffer = (u16)DmaAdBuffer;	//ADCת�����ݴ洢��ַ
//	DMA_ADC_InitStructure.DMA_Times = ADC_4_Times;	//ÿ��ͨ��ת������, ADC_1_Times,ADC_2_Times,ADC_4_Times,ADC_8_Times,ADC_16_Times,ADC_32_Times,ADC_64_Times,ADC_128_Times,ADC_256_Times
//	DMA_ADC_Inilize(&DMA_ADC_InitStructure);		//��ʼ��
//	NVIC_DMA_ADC_Init(ENABLE,Priority_0,Priority_0);		//�ж�ʹ��, ENABLE/DISABLE; ���ȼ�(�͵���) Priority_0~Priority_3; �������ȼ�(�͵���) Priority_0~Priority_3
//	DMA_ADC_TRIG();		//��������ת��
}


/*************  ���س�������    **************/

#define PWM_PERIOD  1023    //��������ֵ
#define PWM7_3      0x30	//P5.2
/*************  ���ر�������    **************/

void PwmConfig(void){
	P5_MODE_IO_PU(GPIO_Pin_2);
	
	PWMB_CCER1 = 0x00; //д CCMRx ǰ���������� CCxE �ر�ͨ��
    PWMB_CCER2 = 0x00;
    PWMB_CCMR1 = 0x60; //ͨ��ģʽ����
    PWMB_CCMR2 = 0x60;
    PWMB_CCMR3 = 0x60;
    PWMB_CCMR4 = 0x60;
	
    PWMB_CCER1 = 0x33; //����ͨ�����ʹ�ܺͼ���
    PWMB_CCER2 = 0x33;
    PWMB_CCMR1 |= 0x08; //����PWMB_CCR5Ԥת�ع���(��ҪCC5E=1�ſ�д)
    PWMB_CCMR2 |= 0x08;
    PWMB_CCMR3 |= 0x08;
    PWMB_CCMR4 |= 0x08;

    PWMB_ARRH = (u8)(PWM_PERIOD >> 8); //��������ʱ��
    PWMB_ARRL = (u8)PWM_PERIOD;

    PWMB_ENO = 0x00;
    PWMB_ENO |= ENO7P; //ʹ�����

    PWMB_PS = 0x00;    //�߼� PWM ͨ�������ѡ��λ
    PWMB_PS |= PWM7_3; //ѡ�� PWM7_0 ͨ��

    PWMB_BKR = 0x80;   //ʹ�������
    PWMB_CR1 |= 0x81;  //ʹ��ARRԤװ�أ���ʼ��ʱ
		
	PWMB_CCR7H = (u8)(0 >> 8); //����ռ�ձ�ʱ��
    PWMB_CCR7L = (u8)(0);
}




