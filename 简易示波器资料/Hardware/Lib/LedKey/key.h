#ifndef __KEY_H__
#define __KEY_H__

#include "main.h"


#define MaxButtonNumber              1  //��������,����32,����u16Bit_Key_structλ��Ϊ32λ

// #define Read_PROBE1_KEY              gpio_input_bit_get(GPIOB,GPIO_PIN_13)

// #define Read_PROBE2_KEY              gpio_input_bit_get(GPIOB,GPIO_PIN_14)

// #define Read_PROBE3_KEY              gpio_input_bit_get(GPIOB,GPIO_PIN_15)

#define Read_encoder_KEY             gpio_input_bit_get(GPIOB,GPIO_PIN_9)



//**********************************************************
struct uint32_t_Bit_Key_struct{
	uint32_t Bit0 : 1;
	uint32_t Bit1 : 1;
	uint32_t Bit2 : 1;
	uint32_t Bit3 : 1;
	uint32_t Bit4 : 1;
	uint32_t Bit5 : 1;
	uint32_t Bit6 : 1;
	uint32_t Bit7 : 1;
	uint32_t Bit8 : 1;
	uint32_t Bit9 : 1;
	uint32_t Bit10: 1;
	uint32_t Bit11: 1;
	uint32_t Bit12: 1;
	uint32_t Bit13: 1;
	uint32_t Bit14: 1;
	uint32_t Bit15: 1;
	uint32_t Bit16: 1;
	uint32_t Bit17: 1;
	uint32_t Bit18: 1;
	uint32_t Bit19: 1;
	uint32_t Bit20: 1;
	uint32_t Bit21: 1;
	uint32_t Bit22: 1;
	uint32_t Bit23: 1;
	uint32_t Bit24: 1;
	uint32_t Bit25: 1;
	uint32_t Bit26: 1;
	uint32_t Bit27: 1;
	uint32_t Bit28: 1;
	uint32_t Bit29: 1;
	uint32_t Bit30: 1;
	uint32_t Bit31: 1;
};
//*************
union uint32_t_Bit_Key_union{
	struct uint32_t_Bit_Key_struct uint32_t_Bit;
	uint32_t All;
};

#define  PROBE1_KEY      0//����1
#define  PROBE2_KEY      1//����2
#define  PROBE3_KEY      2//����3


void Init_Key_GPIO(void);
void KeyInputScan(void);     //������ɨ������
void KeyValueCalculate(void);//��ֵ����
uint8_t KeyState(uint8_t KeyNumber);// ���ؼ�ֵ�ĵ�ǰ״̬
uint8_t R_KeyState(uint8_t KeyNumber);//���ؼ�ֵ��������״̬
uint8_t F_KeyState(uint8_t KeyNumber);//���ؼ�ֵ���½���״̬
uint8_t L_KeyState(uint8_t KeyNumber);//���ؼ�ֵ�ĳ���״̬
uint8_t D_KeyState(uint8_t KeyNumber);//���ؼ�ֵ�Ķ���״̬
uint8_t DC_KeyState(uint8_t KeyNumber);//˫��֮�󴥷�һ��
#endif


/**
 * 
 * ʹ��ָ��
 * 1. �ȵ��� KeyIO_Init ����io�ڳ�ʼ��
 * 2. ÿ10ms ���� KeyInputScan ���а���ɨ��
 * 3. ÿ10ms ���� KeyValueCalculate ���а���״̬����
 * 4. ���ö�Ӧ���� ���ض�Ӧ������Ӧ��״̬
 * 
*/




