#include "server.h"

// 205 -> 4.2
// 233 -> 8.4
// 262 -> 12.6
// 296 -> 16.8

//更新PWM占空比
void UpdatePwmDuty(u16 PWM7_Duty){
   PWMB_CCR7H = (u8)(PWM7_Duty >> 8); //设置占空比时间
   PWMB_CCR7L = (u8)(PWM7_Duty);
}


u16 value[4]={205,233,262,296};
// 设置充电电压
void SetVoltage(u8 key){
	if(key<1 || key > 4){
		return;
	}
	UpdatePwmDuty(value[key-1]);
}


// 查询所有的数据
void QueryAllStateValue(StateValue *sv){
	//获取总电压ADC
	u16 adc_value;
	adc_value = Get_ADCResult(3);
	if(adc_value < 4096){
		sv->v_sun = (adc_value * 0.00513f);  //V
	}

	//获取总电流ADC
	adc_value = Get_ADCResult(2);
	if(adc_value < 4096){
		sv->a_sun = (adc_value * 8.1380283f/10000); //A
	}

	//充电电压ADC
	adc_value = Get_ADCResult(1);
	if(adc_value < 4096){
		sv->v_bat = (adc_value * 0.00514f);  //V
	}

	//充电电流ADC
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

	sv->CHRG = P00?1:0;  //充中
	sv->DONE = P01?1:0;  //充满
	sv->PG = P02?1:0; //诱导电压是否ok

	// 获取每节电池的充电状态
	sv->bat4_state = sv->v_bat1>4.15?1:0;
	sv->bat3_state = sv->v_bat2>4.15?1:0;
	sv->bat2_state = sv->v_bat3>4.15?1:0;
	sv->bat1_state = sv->v_bat4>4.15?1:0;
}


// 根据每节电池的状态进行平衡充电
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
			Bat3OnOff = 1;//截断充电
		}
	}

	if(sv->bat2_state){
		if(Bat1OnOff && Bat3OnOff && Bat4OnOff){
			MainOnOff = 0;
		}else{
			Bat2OnOff = 1;//截断充电
		}
	}

	if(sv->bat1_state){
		if(Bat4OnOff && Bat2OnOff && Bat3OnOff){
			MainOnOff = 0;
		}else{
			Bat1OnOff = 1; //截断充电
		}
	}
}


// 自动检测检测数量
u8 TestBatteryNum(void){
	StateValue sv;
	MainOnOff = 1;

	//先检测是否4节，策略就是打开总开关，然后只吸合子开关4，然后测电压是否大于 3.5*4=14 是则有4节
	Bat1OnOff = 0;
	Bat2OnOff = 0;
	Bat3OnOff = 0;
	Bat4OnOff = 1;
	QueryAllStateValue(&sv); //获取所有的状态值
	if(sv.v_bat4 >= 14){
		return 4;
	}


	//先检测是否3节，策略就是打开总开关，然后只吸合子开关4和开关3，然后测电压是否大于 3.5*3=10.5 是则有3节
	Bat1OnOff = 0;
	Bat2OnOff = 0;
	Bat3OnOff = 1;
	Bat4OnOff = 0;
	QueryAllStateValue(&sv); //获取所有的状态值
	if(sv.v_bat3 >= 10.5f){
		return 3;
	}

	//与上相同  3.5*2=7
	Bat1OnOff = 0;
	Bat2OnOff = 1;
	Bat3OnOff = 0;
	Bat4OnOff = 0;
	QueryAllStateValue(&sv); //获取所有的状态值
	if(sv.v_bat3 >= 7){
		return 2;
	}

	//与上相同  3.5*1=3.5
	Bat1OnOff = 1;
	Bat2OnOff = 0;
	Bat3OnOff = 0;
	Bat4OnOff = 0;
	QueryAllStateValue(&sv); //获取所有的状态值
	if(sv.v_bat3 >= 3.5f){
		return 1;
	}
	return 0;   //表示搞不清
}



// 根据电池节数来决定开启充电开关
void SetBatNum(char num){
	switch (num){
	case '0':   //全关
	case 0:
		MainOnOff = 0;
		Bat1OnOff = 0;
		Bat2OnOff = 0;
		Bat3OnOff = 0;
		Bat4OnOff = 0;
		break;
	case '1':   //开启一节 就是吸合上面三个，开放第1个
	case 1:
		Bat1OnOff = 0;
		Bat2OnOff = 1;
		Bat3OnOff = 1;
		Bat4OnOff = 1;
		MainOnOff = 1;
		break;
	case '2':   //开启2节
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