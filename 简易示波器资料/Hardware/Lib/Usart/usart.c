#include "usart.h"
#include "main.h"

#define USART0_RDATA_ADDRESS      ((uint32_t)&USART_RDATA(USART0))
#define USART0_TDATA_ADDRESS      ((uint32_t)&USART_TDATA(USART0))  
#define ARRAYNUM(arr_nanme)      (uint32_t)(sizeof(arr_nanme) / sizeof(*(arr_nanme)))

//接收完成标志位
static uint8_t receive_flag = 0;
//接受数据数量
static uint8_t rx_count = 0;
//接收数据缓冲区
static uint8_t rxbuffer[256];
static uint8_t txbuffer[256];

/*
*   函数内容：初始化打印串口
*   函数参数：bound--波特率
*   返回值：  无
*/
void Init_USART(uint32_t bound){
    //使能时钟
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_USART0);
    
    //复用功能设置
    gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_9);
    gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_10);
    
    //端口模式设置
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_9);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_9);
    
    //端口模式设置
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_10);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_10);
    
    //复位USART0
    usart_deinit(USART0);
    usart_word_length_set(USART0, USART_WL_8BIT);       //8位数据位宽
    usart_stop_bit_set(USART0, USART_STB_1BIT);         //1位停止位
    usart_parity_config(USART0, USART_PM_NONE);         //无校验位
    usart_baudrate_set(USART0, bound);                  //波特率
    usart_receive_config(USART0, USART_RECEIVE_ENABLE); //接收使能
    usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);//发送使能
    usart_dma_receive_config(USART0, USART_DENR_ENABLE);//DAM接收使能
    //使能串口0
    usart_enable(USART0);
    
    //等待串口空闲
    while(RESET == usart_flag_get(USART0, USART_FLAG_IDLE));
    //清除空闲标志位
    usart_flag_clear(USART0, USART_FLAG_IDLE);
    //空闲中断使能
    usart_interrupt_enable(USART0,USART_INT_IDLE);

    Init_USART0RecDMA();
}

/*
*   函数内容：初始化串口0接收DMA
*   函数参数：无
*   返回值：  无
*/
void Init_USART0RecDMA(void){
    //DMA参数结构体
    dma_parameter_struct dma_init_struct;
    
    //中断使能
    nvic_irq_enable(USART0_IRQn, 0); 
    
    //DMA时钟使能
    rcu_periph_clock_enable(RCU_DMA);
	
	 /* initialize DMA channel1 */
    dma_deinit(DMA_CH1);
    dma_struct_para_init(&dma_init_struct);
    
    dma_init_struct.direction = DMA_MEMORY_TO_PERIPHERAL;
    dma_init_struct.memory_addr = (uint32_t)txbuffer;
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
    dma_init_struct.number = ARRAYNUM(txbuffer);
    dma_init_struct.periph_addr = USART0_TDATA_ADDRESS;
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
    dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
    dma_init(DMA_CH1, &dma_init_struct);
	
	
	
    
    //复位DMA通道，DMA通道2对应串口0的接收
    dma_deinit(DMA_CH2);
    dma_init_struct.direction = DMA_PERIPHERAL_TO_MEMORY;       //从外设到内存
    dma_init_struct.memory_addr = (uint32_t)rxbuffer;           //内存地址
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;    //内存地址自增
    dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;       //内存地址宽度-8bit
    dma_init_struct.number = 256;                               //DMA最大传输数量
    dma_init_struct.periph_addr = USART0_RDATA_ADDRESS;         //外设地址
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;   //外设地址不自增
    dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;   //外设地址宽度
    dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;         //中断优先级高
    dma_init(DMA_CH2, &dma_init_struct);                        //初始化DMA通道2
    
    //DAM循环模式失能
    dma_circulation_disable(DMA_CH2);
	dma_circulation_disable(DMA_CH1);
		
    //使能DMA通道2
    dma_channel_enable(DMA_CH2);
	dma_channel_enable(DMA_CH1);
}

/*
*   函数内容：USART0发送函数
*   函数参数：char *SendData-发送数据指针
*   返回值：  无
*/
void USART0_SendData(char *SendData){
    usart_flag_clear(USART0,USART_FLAG_TC);
    while(*SendData != '\0'){
       usart_data_transmit(USART0, (uint8_t)*SendData);
       while(RESET == usart_flag_get(USART0, USART_FLAG_TBE));
       usart_flag_clear(USART0,USART_FLAG_TC);
       SendData++;
    }
}

/*
*   函数内容：fputc端口重定向
*   函数参数：无
*   返回值：  无
*/
int fputc(int ch, FILE *f){
    usart_data_transmit(USART0, (uint8_t) ch);
    while(RESET == usart_flag_get(USART0, USART_FLAG_TBE));
    return ch;
}

/*
*   函数内容：串口0中断服务函数
*   函数参数: 无
*   返回值：  无
*/
void USART0_IRQHandler(void){
    if(RESET != usart_interrupt_flag_get(USART0, USART_INT_FLAG_IDLE)){
        usart_interrupt_flag_clear(USART0, USART_INT_FLAG_IDLE);
        
        //接收数据数量
        rx_count = 256 - (dma_transfer_number_get(DMA_CH2));
        //接收完成标志位
        receive_flag = 1;
        
        //失能DMA通道
        dma_channel_disable(DMA_CH2);
        //重新配置DMA计数
        dma_transfer_number_config(DMA_CH2, 256);
        //使能DMA通道
        dma_channel_enable(DMA_CH2);
    }
}




/*
*   函数内容：串口0接收数据处理函数
*   函数参数: 无
*   返回值：  无
*/
void USART0_RecHandle(void){
    //如果数据接收完成
    if(receive_flag == 1){
			//直接在中断处理
			if((rxbuffer[0] == 0x5B) && (rxbuffer[7] = 0x5D)){
				//说明验证通过
				oscilloscope.IsComSend = 0;
				//设置pwm开关
				SetPwmState(rxbuffer[1] & 0x01);
				
				//设置dac开关
				SetDacState(rxbuffer[1] & 0x02);
				//设置交流直流
				SetSignalAcOrDc(rxbuffer[1] & 0x04);
				//设置衰减
				SetSignalDecay(rxbuffer[1] & 0x08);
				
				//设置PWM频率
				oscilloscope.PwmSetFreqValue = (uint16_t)(1000000/((rxbuffer[2] << 8) + rxbuffer[3]));
        Set_Output_Freq(oscilloscope.PwmSetFreqValue);
				
				//设置DAC电压
				oscilloscope.DacDutyCycle = (uint16_t)(((rxbuffer[4] << 8) + rxbuffer[5])*3.0304f);
        SetDacComparex(oscilloscope.DacDutyCycle);
				
				//设置PWM占空比
				oscilloscope.pwmDutyCycle = (rxbuffer[6]*10u);
        Set_Output_PWMComparex(oscilloscope.pwmDutyCycle);
				
			}
			
        memset(rxbuffer,0,256);
        receive_flag=0;
        rx_count=0;
				oscilloscope.IsComSend = 1;
    }
}

// 发送业务数据
void SendAllData(void){
		if(oscilloscope.IsComSend == 0){
			return;
		}
    UpData upData = {
        .sud.Head = 0x5B,
        .sud.IsPwmOut = oscilloscope.IsPwmOut,
        .sud.IsDACOut = oscilloscope.IsDACOut,
        .sud.IsACDC = oscilloscope.IsDACDC,
        .sud.IsDecay = oscilloscope.IsDecay,
        .sud.Vpp = (uint16_t)(oscilloscope.vpp * 100u),
        .sud.GatherFreq = (uint16_t)(oscilloscope.gatherFreq),
        .sud.PwmFreq = oscilloscope.PwmOutFreq,
        .sud.PwmDutyCycle = (uint8_t)(oscilloscope.pwmDutyCycle / 10),   //数据回来要乘10
        .sud.DacVoltage = (uint16_t)(oscilloscope.DacVoltage * 1000u),
        .sud.Tail = 0x5D
    };

    for (uint16_t i = 0; i < 300; i++){
        upData.sud.AdcV[i] = (uint16_t)(oscilloscope.voltageValue[i] * 100);
    }


    // 开始串口发送
    usart_flag_clear(USART0,USART_FLAG_TC);
    for (uint16_t i = 0; i < 306*2; i++){
        usart_data_transmit(USART0, upData.cud[i]);
        while(RESET == usart_flag_get(USART0, USART_FLAG_TBE));
        usart_flag_clear(USART0,USART_FLAG_TC);
    }
}