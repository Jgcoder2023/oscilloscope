#include "server.h"

// 205 -> 4.2
// 233 -> 8.4
// 262 -> 12.6
// 296 -> 16.8

//����PWMռ�ձ�
void UpdatePwmDuty(u16 PWM7_Duty){
   PWMB_CCR7H = (u8)(PWM7_Duty >> 8); //����ռ�ձ�ʱ��
   PWMB_CCR7L = (u8)(PWM7_Duty);
}


u16 value[4]={205,233,262,296};
// ���ó���ѹ
void SetVoltage(u8 key){
	if(key<1 || key > 4){
		return;
	}
	UpdatePwmDuty(value[key-1]);
}


// ��ѯ���е�����
void QueryAllStateValue(StateValue *sv){
	//��ȡ�ܵ�ѹADC
	u16 adc_value;
	adc_value = Get_ADCResult(3);
	if(adc_value < 4096){
		sv->v_sun = (adc_value * 0.00513f);  //V
	}

	//��ȡ�ܵ���ADC
	adc_value = Get_ADCResult(2);
	if(adc_value < 4096){
		sv->a_sun = (adc_value * 8.1380283f/10000); //A
	}

	//����ѹADC
	adc_value = Get_ADCResult(1);
	if(adc_value < 4096){
		sv->v_bat = (adc_value * 0.00514f);  //V
	}

	//������ADC
	adc_value = Get_ADCResult(4);
	if(adc_value < 4096){
		sv->a_bat = (adc_value * 8.1380283f/10000); //A
	}

	//bat1
	adc_value = Get_ADCResult(14);
	if(adc_value < 4096){
		sv->v_bat1 = (adc_value * 0.0012207f);  //V
	}

	//bat2
	adc_value = Get_ADCResult(13);
	if(adc_value < 4096){
		sv->v_bat1 = (adc_value * 0.0024414f);  //V
	}
	
	//bat3
	adc_value = Get_ADCResult(12);
	if(adc_value < 4096){
		sv->v_bat1 = (adc_value * 0.00361424f);  //V
	}

	//bat4
	adc_value = Get_ADCResult(11);
	if(adc_value < 4096){
		sv->v_bat1 = (adc_value * 0.004811f);  //V
	}

	sv->CHRG = P00?1:0;  //����
	sv->DONE = P01?1:0;  //����
	sv->PG = P02?1:0; //�յ���ѹ�Ƿ�ok

	// ��ȡÿ�ڵ�صĳ��״̬
	sv->bat4_state = sv->v_bat1>4.15?1:0;
	sv->bat3_state = sv->v_bat2>4.15?1:0;
	sv->bat2_state = sv->v_bat3>4.15?1:0;
	sv->bat1_state = sv->v_bat4>4.15?1:0;
}


// ����ÿ�ڵ�ص�״̬����ƽ����
void HandleBalanceBattery(StateValue *sv){
	if(sv->bat4_state){
		if(Bat1OnOff && Bat2OnOff && Bat3OnOff){
			MainOnOff = 0;
		}else{
			Bat4OnOff = 1;
		}
	}

	if(sv->bat3_state){
		if(Bat1OnOff && Bat2OnOff && Bat4OnOff){
			MainOnOff = 0;
		}else{
			Bat3OnOff = 1;//�ضϳ��
		}
	}

	if(sv->bat2_state){
		if(Bat1OnOff && Bat3OnOff && Bat4OnOff){
			MainOnOff = 0;
		}else{
			Bat2OnOff = 1;//�ضϳ��
		}
	}

	if(sv->bat1_state){
		if(Bat4OnOff && Bat2OnOff && Bat3OnOff){
			MainOnOff = 0;
		}else{
			Bat1OnOff = 1; //�ضϳ��
		}
	}
}


// �Զ����������
u8 TestBatteryNum(void){
	StateValue sv;
	MainOnOff = 1;

	//�ȼ���Ƿ�4�ڣ����Ծ��Ǵ��ܿ��أ�Ȼ��ֻ�����ӿ���4��Ȼ����ѹ�Ƿ���� 3.5*4=14 ������4��
	Bat1OnOff = 0;
	Bat2OnOff = 0;
	Bat3OnOff = 0;
	Bat4OnOff = 1;
	QueryAllStateValue(&sv); //��ȡ���е�״ֵ̬
	if(sv.v_bat4 >= 14){
		return 4;
	}


	//�ȼ���Ƿ�3�ڣ����Ծ��Ǵ��ܿ��أ�Ȼ��ֻ�����ӿ���4�Ϳ���3��Ȼ����ѹ�Ƿ���� 3.5*3=10.5 ������3��
	Bat1OnOff = 0;
	Bat2OnOff = 0;
	Bat3OnOff = 1;
	Bat4OnOff = 0;
	QueryAllStateValue(&sv); //��ȡ���е�״ֵ̬
	if(sv.v_bat3 >= 10.5f){
		return 3;
	}

	//������ͬ  3.5*2=7
	Bat1OnOff = 0;
	Bat2OnOff = 1;
	Bat3OnOff = 0;
	Bat4OnOff = 0;
	QueryAllStateValue(&sv); //��ȡ���е�״ֵ̬
	if(sv.v_bat3 >= 7){
		return 2;
	}

	//������ͬ  3.5*1=3.5
	Bat1OnOff = 1;
	Bat2OnOff = 0;
	Bat3OnOff = 0;
	Bat4OnOff = 0;
	QueryAllStateValue(&sv); //��ȡ���е�״ֵ̬
	if(sv.v_bat3 >= 3.5f){
		return 1;
	}
	return 0;   //��ʾ�㲻��
}



// ���ݵ�ؽ���������������翪��
void SetBatNum(char num){
	switch (num){
	case '0':   //ȫ��
	case 0:
		MainOnOff = 0;
		Bat1OnOff = 0;
		Bat2OnOff = 0;
		Bat3OnOff = 0;
		Bat4OnOff = 0;
		break;
	case '1':   //����һ�� ���������������������ŵ�1��
	case 1:
		Bat1OnOff = 0;
		Bat2OnOff = 1;
		Bat3OnOff = 1;
		Bat4OnOff = 1;
		MainOnOff = 1;
		break;
	case '2':   //����2��
	case 2:
		Bat1OnOff = 0;
		Bat2OnOff = 0;
		Bat3OnOff = 1;
		Bat4OnOff = 1;
		MainOnOff = 1;
		break;
	case '3':
	case 3:
		Bat1OnOff = 0;
		Bat2OnOff = 0;
		Bat3OnOff = 0;
		Bat4OnOff = 1;
		MainOnOff = 1;
		break;
	case '4':
	case 4:
		Bat1OnOff = 0;
		Bat2OnOff = 0;
		Bat3OnOff = 0;
		Bat4OnOff = 0;
		MainOnOff = 1;
		break;
	default:
		MainOnOff = 0;
		break;
	}
}