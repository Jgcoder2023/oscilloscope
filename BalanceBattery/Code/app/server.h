#ifndef		__SERVER_H
#define		__SERVER_H

#include "app.h"


//adc��ֵ
typedef struct{
	float v_sun;   //�ܵ�ѹ
	float a_sun;   //�ܵ���
	float v_bat;   //����ѹ
	float a_bat;   //������
	float v_bat1;  //���1��ѹ
	float v_bat2;  //���2��ѹ
	float v_bat3;  //���3��ѹ
	float v_bat4;  //���4��ѹ
	
	u8 CHRG:1;   //���ָʾ���͵�ƽ
	u8 DONE:1;   //����ָʾ �͵�ƽ
	u8 PG:1;     //��ѹ�յ�״̬
	u8 bat1_state:1;//���1��״̬ �Ƿ����
	u8 bat2_state:1;//���2��״̬
	u8 bat3_state:1;//���3��״̬
	u8 bat4_state:1;//���4��״̬
}StateValue;

void QueryAllStateValue(StateValue *sv);
void UpdatePwmDuty(u16 PWM7_Duty);
void SetVoltage(u8 key);
void SetBatNum(char num);





#endif