#include "usart.h"
#include "main.h"

#define USART0_RDATA_ADDRESS      ((uint32_t)&USART_RDATA(USART0))
#define USART0_TDATA_ADDRESS      ((uint32_t)&USART_TDATA(USART0))  
#define ARRAYNUM(arr_nanme)      (uint32_t)(sizeof(arr_nanme) / sizeof(*(arr_nanme)))

//������ɱ�־λ
static uint8_t receive_flag = 0;
//������������
static uint8_t rx_count = 0;
//�������ݻ�����
static uint8_t rxbuffer[256];
static uint8_t txbuffer[256];

/*
*   �������ݣ���ʼ����ӡ����
*   ����������bound--������
*   ����ֵ��  ��
*/
void Init_USART(uint32_t bound){
    //ʹ��ʱ��
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_USART0);
    
    //���ù�������
    gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_9);
    gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_10);
    
    //�˿�ģʽ����
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_9);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_9);
    
    //�˿�ģʽ����
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_10);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_10);
    
    //��λUSART0
    usart_deinit(USART0);
    usart_word_length_set(USART0, USART_WL_8BIT);       //8λ����λ��
    usart_stop_bit_set(USART0, USART_STB_1BIT);         //1λֹͣλ
    usart_parity_config(USART0, USART_PM_NONE);         //��У��λ
    usart_baudrate_set(USART0, bound);                  //������
    usart_receive_config(USART0, USART_RECEIVE_ENABLE); //����ʹ��
    usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);//����ʹ��
    usart_dma_receive_config(USART0, USART_DENR_ENABLE);//DAM����ʹ��
    //ʹ�ܴ���0
    usart_enable(USART0);
    
    //�ȴ����ڿ���
    while(RESET == usart_flag_get(USART0, USART_FLAG_IDLE));
    //������б�־λ
    usart_flag_clear(USART0, USART_FLAG_IDLE);
    //�����ж�ʹ��
    usart_interrupt_enable(USART0,USART_INT_IDLE);

    Init_USART0RecDMA();
}

/*
*   �������ݣ���ʼ������0����DMA
*   ������������
*   ����ֵ��  ��
*/
void Init_USART0RecDMA(void){
    //DMA�����ṹ��
    dma_parameter_struct dma_init_struct;
    
    //�ж�ʹ��
    nvic_irq_enable(USART0_IRQn, 0); 
    
    //DMAʱ��ʹ��
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
	
	
	
    
    //��λDMAͨ����DMAͨ��2��Ӧ����0�Ľ���
    dma_deinit(DMA_CH2);
    dma_init_struct.direction = DMA_PERIPHERAL_TO_MEMORY;       //�����赽�ڴ�
    dma_init_struct.memory_addr = (uint32_t)rxbuffer;           //�ڴ��ַ
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;    //�ڴ��ַ����
    dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;       //�ڴ��ַ���-8bit
    dma_init_struct.number = 256;                               //DMA���������
    dma_init_struct.periph_addr = USART0_RDATA_ADDRESS;         //�����ַ
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;   //�����ַ������
    dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;   //�����ַ���
    dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;         //�ж����ȼ���
    dma_init(DMA_CH2, &dma_init_struct);                        //��ʼ��DMAͨ��2
    
    //DAMѭ��ģʽʧ��
    dma_circulation_disable(DMA_CH2);
	dma_circulation_disable(DMA_CH1);
		
    //ʹ��DMAͨ��2
    dma_channel_enable(DMA_CH2);
	dma_channel_enable(DMA_CH1);
}

/*
*   �������ݣ�USART0���ͺ���
*   ����������char *SendData-��������ָ��
*   ����ֵ��  ��
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
*   �������ݣ�fputc�˿��ض���
*   ������������
*   ����ֵ��  ��
*/
int fputc(int ch, FILE *f){
    usart_data_transmit(USART0, (uint8_t) ch);
    while(RESET == usart_flag_get(USART0, USART_FLAG_TBE));
    return ch;
}

/*
*   �������ݣ�����0�жϷ�����
*   ��������: ��
*   ����ֵ��  ��
*/
void USART0_IRQHandler(void){
    if(RESET != usart_interrupt_flag_get(USART0, USART_INT_FLAG_IDLE)){
        usart_interrupt_flag_clear(USART0, USART_INT_FLAG_IDLE);
        
        //������������
        rx_count = 256 - (dma_transfer_number_get(DMA_CH2));
        //������ɱ�־λ
        receive_flag = 1;
        
        //ʧ��DMAͨ��
        dma_channel_disable(DMA_CH2);
        //��������DMA����
        dma_transfer_number_config(DMA_CH2, 256);
        //ʹ��DMAͨ��
        dma_channel_enable(DMA_CH2);
    }
}




/*
*   �������ݣ�����0�������ݴ�����
*   ��������: ��
*   ����ֵ��  ��
*/
void USART0_RecHandle(void){
    //������ݽ������
    if(receive_flag == 1){
			//ֱ�����жϴ���
			if((rxbuffer[0] == 0x5B) && (rxbuffer[7] = 0x5D)){
				//˵����֤ͨ��
				oscilloscope.IsComSend = 0;
				//����pwm����
				SetPwmState(rxbuffer[1] & 0x01);
				
				//����dac����
				SetDacState(rxbuffer[1] & 0x02);
				//���ý���ֱ��
				SetSignalAcOrDc(rxbuffer[1] & 0x04);
				//����˥��
				SetSignalDecay(rxbuffer[1] & 0x08);
				
				//����PWMƵ��
				oscilloscope.PwmSetFreqValue = (uint16_t)(1000000/((rxbuffer[2] << 8) + rxbuffer[3]));
        Set_Output_Freq(oscilloscope.PwmSetFreqValue);
				
				//����DAC��ѹ
				oscilloscope.DacDutyCycle = (uint16_t)(((rxbuffer[4] << 8) + rxbuffer[5])*3.0304f);
        SetDacComparex(oscilloscope.DacDutyCycle);
				
				//����PWMռ�ձ�
				oscilloscope.pwmDutyCycle = (rxbuffer[6]*10u);
        Set_Output_PWMComparex(oscilloscope.pwmDutyCycle);
				
			}
			
        memset(rxbuffer,0,256);
        receive_flag=0;
        rx_count=0;
				oscilloscope.IsComSend = 1;
    }
}

// ����ҵ������
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
        .sud.PwmDutyCycle = (uint8_t)(oscilloscope.pwmDutyCycle / 10),   //���ݻ���Ҫ��10
        .sud.DacVoltage = (uint16_t)(oscilloscope.DacVoltage * 1000u),
        .sud.Tail = 0x5D
    };

    for (uint16_t i = 0; i < 300; i++){
        upData.sud.AdcV[i] = (uint16_t)(oscilloscope.voltageValue[i] * 100);
    }


    // ��ʼ���ڷ���
    usart_flag_clear(USART0,USART_FLAG_TC);
    for (uint16_t i = 0; i < 306*2; i++){
        usart_data_transmit(USART0, upData.cud[i]);
        while(RESET == usart_flag_get(USART0, USART_FLAG_TBE));
        usart_flag_clear(USART0,USART_FLAG_TC);
    }
}