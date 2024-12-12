#include "init.h"
#include "app.h"


/***************	功能说明	****************

下载时, 选择时钟 24MHz (可以在配置文件"config.h"中修改).
******************************************/

void RunLedConfig(void){	 
	P2_MODE_OUT_PP(GPIO_Pin_2);   //初始化led的gpio
	P2_MODE_IO_PU(GPIO_Pin_3 | GPIO_Pin_4);	//初始化软i2c口	//P3.2,P3.3 设置为准双向口
	P0_MODE_IN_HIZ(GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2);   //充电提示的io口
	P3_MODE_IN_HIZ(GPIO_Pin_5);   //bat1充满提示
	P1_MODE_IN_HIZ(GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);   //bat2-4充满提示

	P5_MODE_OUT_PP(GPIO_Pin_3);   //充电总开关
	P3_MODE_OUT_PP(GPIO_Pin_3 | GPIO_Pin_4);   //充电子开关
	P4_MODE_OUT_PP(GPIO_Pin_3 | GPIO_Pin_7);   //充电子开关
}

/************************ 定时器配置 ****************************/
void TimerConfig(void){
	TIM_InitTypeDef		TIM_InitStructure;					//结构定义
	TIM_InitStructure.TIM_Mode      = TIM_16BitAutoReload;	//指定工作模式,   TIM_16BitAutoReload,TIM_16Bit,TIM_8BitAutoReload,TIM_16BitAutoReloadNoMask
	TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_1T;		//指定时钟源,     TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
	TIM_InitStructure.TIM_ClkOut    = DISABLE;				//是否输出高速脉冲, ENABLE或DISABLE
	TIM_InitStructure.TIM_Value     = (u16)(65536UL - (MAIN_Fosc / 2000));		//初值,(定时1ms)
	TIM_InitStructure.TIM_PS        = 0;					//8位预分频器(n+1), 0~255
	TIM_InitStructure.TIM_Run       = ENABLE;				//是否初始化后启动定时器, ENABLE或DISABLE
	Timer_Inilize(Timer0,&TIM_InitStructure);				//初始化Timer0	  Timer0,Timer1,Timer2,Timer3,Timer4
	NVIC_Timer0_Init(ENABLE,Priority_0);		//中断使能, ENABLE/DISABLE; 优先级(低到高) Priority_0,Priority_1,Priority_2,Priority_3
}


/***************  串口初始化函数 *****************/
void UartConfig(void){	
	COMx_InitDefine	COMx_InitStructure;		//结构定义
	
	P3_MODE_IO_PU(GPIO_Pin_0 | GPIO_Pin_1); //初始化串口gpio
	
	
	COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;	//模式, UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
	COMx_InitStructure.UART_BRT_Use   = BRT_Timer1;			//选择波特率发生器, BRT_Timer1, BRT_Timer2 (注意: 串口2固定使用BRT_Timer2)
	COMx_InitStructure.UART_BaudRate  = 115200ul;			//波特率, 一般 110 ~ 115200
	COMx_InitStructure.UART_RxEnable  = ENABLE;				//接收允许,   ENABLE或DISABLE
	COMx_InitStructure.BaudRateDouble = DISABLE;			//波特率加倍, ENABLE或DISABLE
	UART_Configuration(UART1, &COMx_InitStructure);		//初始化串口1 UART1,UART2,UART3,UART4
	NVIC_UART1_Init(ENABLE,Priority_1);		//中断使能, ENABLE/DISABLE; 优先级(低到高) Priority_0,Priority_1,Priority_2,Priority_3
	UART1_SW(UART1_SW_P30_P31);		//UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17,UART1_SW_P43_P44
}



/******************** ADC 配置 ********************/
void AdcConfig(void){
	ADC_InitTypeDef		ADC_InitStructure;		//结构定义
//	DMA_ADC_InitTypeDef		DMA_ADC_InitStructure;		//结构定义
	
	//gpio
	P0_MODE_IN_HIZ(GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6);		//P0.3~P0.6 设置为高阻输入
	P1_MODE_IN_HIZ(GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4);

	
	ADC_InitStructure.ADC_SMPduty   = 31;		//ADC 模拟信号采样时间控制, 0~31（注意： SMPDUTY 一定不能设置小于 10）
	ADC_InitStructure.ADC_CsSetup   = 0;		//ADC 通道选择时间控制 0(默认),1
	ADC_InitStructure.ADC_CsHold    = 1;		//ADC 通道选择保持时间控制 0,1(默认),2,3
	ADC_InitStructure.ADC_Speed     = ADC_SPEED_2X16T;		//设置 ADC 工作时钟频率	ADC_SPEED_2X1T~ADC_SPEED_2X16T
	ADC_InitStructure.ADC_AdjResult = ADC_RIGHT_JUSTIFIED;	//ADC结果调整,	ADC_LEFT_JUSTIFIED,ADC_RIGHT_JUSTIFIED
	ADC_Inilize(&ADC_InitStructure);		//初始化
	ADC_PowerControl(ENABLE);						//ADC电源开关, ENABLE或DISABLE
	NVIC_ADC_Init(DISABLE,Priority_0);		//中断使能, ENABLE/DISABLE; 优先级(低到高) Priority_0,Priority_1,Priority_2,Priority_3
	
	
//	/******************** DMA 配置 ********************/
//	DMA_ADC_InitStructure.DMA_Enable = ENABLE;			//DMA使能  	ENABLE,DISABLE
//	DMA_ADC_InitStructure.DMA_Channel = 0xffff;			//ADC通道使能寄存器, 1:使能, bit15~bit0 对应 ADC15~ADC0
//	DMA_ADC_InitStructure.DMA_Buffer = (u16)DmaAdBuffer;	//ADC转换数据存储地址
//	DMA_ADC_InitStructure.DMA_Times = ADC_4_Times;	//每个通道转换次数, ADC_1_Times,ADC_2_Times,ADC_4_Times,ADC_8_Times,ADC_16_Times,ADC_32_Times,ADC_64_Times,ADC_128_Times,ADC_256_Times
//	DMA_ADC_Inilize(&DMA_ADC_InitStructure);		//初始化
//	NVIC_DMA_ADC_Init(ENABLE,Priority_0,Priority_0);		//中断使能, ENABLE/DISABLE; 优先级(低到高) Priority_0~Priority_3; 总线优先级(低到高) Priority_0~Priority_3
//	DMA_ADC_TRIG();		//触发启动转换
}


/*************  本地常量声明    **************/

#define PWM_PERIOD  1023    //设置周期值
#define PWM7_3      0x30	//P5.2
/*************  本地变量声明    **************/

void PwmConfig(void){
	P5_MODE_IO_PU(GPIO_Pin_2);
	
	PWMB_CCER1 = 0x00; //写 CCMRx 前必须先清零 CCxE 关闭通道
    PWMB_CCER2 = 0x00;
    PWMB_CCMR1 = 0x60; //通道模式配置
    PWMB_CCMR2 = 0x60;
    PWMB_CCMR3 = 0x60;
    PWMB_CCMR4 = 0x60;
	
    PWMB_CCER1 = 0x33; //配置通道输出使能和极性
    PWMB_CCER2 = 0x33;
    PWMB_CCMR1 |= 0x08; //开启PWMB_CCR5预转载功能(需要CC5E=1才可写)
    PWMB_CCMR2 |= 0x08;
    PWMB_CCMR3 |= 0x08;
    PWMB_CCMR4 |= 0x08;

    PWMB_ARRH = (u8)(PWM_PERIOD >> 8); //设置周期时间
    PWMB_ARRL = (u8)PWM_PERIOD;

    PWMB_ENO = 0x00;
    PWMB_ENO |= ENO7P; //使能输出

    PWMB_PS = 0x00;    //高级 PWM 通道输出脚选择位
    PWMB_PS |= PWM7_3; //选择 PWM7_0 通道

    PWMB_BKR = 0x80;   //使能主输出
    PWMB_CR1 |= 0x81;  //使能ARR预装载，开始计时
		
	PWMB_CCR7H = (u8)(0 >> 8); //设置占空比时间
    PWMB_CCR7L = (u8)(0);
}




