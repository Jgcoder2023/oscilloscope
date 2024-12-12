#include "app.h"

StateValue sv;  //��ȡADC��ֵ
RefreshState rs = {
	Index,   //page
	1,       //IsRefresh
	0,   //��ؽ���
	16,      //x
	2,       //y
};

void AppInit(void){
	WTST = 0;		//���ó���ָ����ʱ��������ֵΪ0�ɽ�CPUִ��ָ����ٶ�����Ϊ���
	EAXSFR();		//��չSFR(XFR)����ʹ�� 
	CKCON = 0;      //��߷���XRAM�ٶ�

	
	RunLedConfig();
	UartConfig();
	
	AdcConfig();
	TimerConfig();
	PwmConfig();
	Init_Key_GPIO();
	
	EA = 1;

	SetBatNum('0');   //�����п��عر�
	
	OLED_Init();      //OLED��ʼ
  	OLED_Clear();     //����
}


void task_10ms(void){
	KeyInputScan();
		switch (rs.Page){
	case Index:
			sv.bat1_state = 0;
			sv.bat2_state = 0;
			sv.bat3_state = 0;
			sv.bat4_state = 0;
		if (R_KeyState(1)){
			rs.Page = Result1;
			rs.IsRefresh = 1;
			OLED_Clear();     //����
		}
		break;
	case Result1:
		if(R_KeyState(2)){rs.Page = Result2; rs.IsRefresh = 1;}
		if(R_KeyState(0)){rs.Page = Result4; rs.IsRefresh = 1;}
		if(R_KeyState(1)){rs.Page = Charge; rs.IsRefresh = 1;OLED_Clear();}
	break;
	case Result2:
		if(R_KeyState(2)){rs.Page = Result3; rs.IsRefresh = 1;}
		if(R_KeyState(0)){rs.Page = Result1; rs.IsRefresh = 1;}
		if(R_KeyState(1)){rs.Page = Charge; rs.IsRefresh = 1;OLED_Clear();}
	break;
	case Result3:
		if(R_KeyState(2)){rs.Page = Result4; rs.IsRefresh = 1;}
		if(R_KeyState(0)){rs.Page = Result2; rs.IsRefresh = 1;}
		if(R_KeyState(1)){rs.Page = Charge; rs.IsRefresh = 1;OLED_Clear();}
	break;
	case Result4:
		if(R_KeyState(2)){rs.Page = Result1; rs.IsRefresh = 1;}
		if(R_KeyState(0)){rs.Page = Result3; rs.IsRefresh = 1;}
		if(R_KeyState(1)){rs.Page = Charge; rs.IsRefresh = 1;OLED_Clear();}
	break;
	case Charge:
		if (L_KeyState(2)){
			SetBatNum(0);//���ݽ��������ü̵�������
			//SetVoltage(1);  //���ó���ѹ
			// ��λ����ҳ
			rs.Page = Index; 
			rs.IsRefresh = 1;
			rs.BatteryNum = 0;
			OLED_Clear();
		}

		// // ���Գ���
		// if(L_KeyState(1)){
		// 	if(rs.BatteryNum == 1){
		// 		sv.bat1_state = 1;
		// 	}else if(rs.BatteryNum == 2){
		// 		sv.bat1_state = 1;
		// 		sv.bat2_state = 1;
		// 	}else if(rs.BatteryNum == 3){
		// 		sv.bat1_state = 1;
		// 		sv.bat2_state = 1;
		// 		sv.bat3_state = 1;
		// 	}else if(rs.BatteryNum == 4){
		// 		sv.bat1_state = 1;
		// 		sv.bat2_state = 1;
		// 		sv.bat3_state = 1;
		// 		sv.bat4_state = 1;
		// 	}
		// }

		// if(L_KeyState(1)){
		// 	// ������

		// }
		
	break;
	default:
		break;
	}
}
void task_100ms(void){
	static u8 dotNum;
	char str[20];
	QueryAllStateValue(&sv);
	switch (rs.Page){
	case Index:
		if(rs.IsRefresh == 1){
			rs.IsRefresh = 0;
			OLED_ShowCHinese(rs.x*0,rs.y*0,0,0); //����:5.0V,2.1A
			OLED_ShowCHinese(rs.x*1,rs.y*0,1,0);

			OLED_ShowCHinese(rs.x*0,rs.y*1,0,0); //���:5.0V,2.1A
			OLED_ShowCHinese(rs.x*1,rs.y*1,2,0);

			// ��ؼ��
			OLED_ShowCHinese(rs.x*2,rs.y*3,3,0); //��ؼ��
			OLED_ShowCHinese(rs.x*3,rs.y*3,6,0); //��ؼ��
			OLED_ShowCHinese(rs.x*4,rs.y*3,7,0); //��ؼ��
			OLED_ShowCHinese(rs.x*5,rs.y*3,8,0); //��ؼ��
		}
		sprintf(str, ":%0.1fV,%0.1fA", sv.v_sun,sv.a_sun);  // ������ת��Ϊ�ַ���
		OLED_ShowString(rs.x*2,rs.y*0,&str,16,0);
		sprintf(str, ":%0.1fV,%0.1fA", sv.v_bat,sv.a_bat);  // ������ת��Ϊ�ַ���
		OLED_ShowString(rs.x*2,rs.y*1,&str,16,0);
		
		break;
	case Result1:
		if(rs.IsRefresh == 1){
			rs.IsRefresh = 0;
			rs.BatteryNum = 1;
			OLED_ShowCHinese(rs.x*2,rs.y*1,12,0); //һ�ڵ��
			OLED_ShowCHinese(rs.x*3,rs.y*1,16,0);
			OLED_ShowCHinese(rs.x*4,rs.y*1,3,0);
			OLED_ShowCHinese(rs.x*5,rs.y*1,6,0);
			
			OLED_ShowCHinese(rs.x*2,rs.y*3,9,0); //��ʼ���
			OLED_ShowCHinese(rs.x*3,rs.y*3,10,0);
			OLED_ShowCHinese(rs.x*4,rs.y*3,11,0);
			OLED_ShowCHinese(rs.x*5,rs.y*3,3,0);
		}
	break;
	case Result2:
		if(rs.IsRefresh == 1){
			rs.IsRefresh = 0;
			rs.BatteryNum = 2;
			OLED_ShowCHinese(rs.x*2,rs.y*1,13,0); //2�ڵ��
			OLED_ShowCHinese(rs.x*3,rs.y*1,16,0);
			OLED_ShowCHinese(rs.x*4,rs.y*1,3,0);
			OLED_ShowCHinese(rs.x*5,rs.y*1,6,0);
			
			OLED_ShowCHinese(rs.x*2,rs.y*3,9,0); //��ʼ���
			OLED_ShowCHinese(rs.x*3,rs.y*3,10,0);
			OLED_ShowCHinese(rs.x*4,rs.y*3,11,0);
			OLED_ShowCHinese(rs.x*5,rs.y*3,3,0);
		}
	break;
	case Result3:
		if(rs.IsRefresh == 1){
			rs.IsRefresh = 0;
			rs.BatteryNum = 3;
			OLED_ShowCHinese(rs.x*2,rs.y*1,14,0); //3�ڵ��
			OLED_ShowCHinese(rs.x*3,rs.y*1,16,0);
			OLED_ShowCHinese(rs.x*4,rs.y*1,3,0);
			OLED_ShowCHinese(rs.x*5,rs.y*1,6,0);
			
			OLED_ShowCHinese(rs.x*2,rs.y*3,9,0); //��ʼ���
			OLED_ShowCHinese(rs.x*3,rs.y*3,10,0);
			OLED_ShowCHinese(rs.x*4,rs.y*3,11,0);
			OLED_ShowCHinese(rs.x*5,rs.y*3,3,0);
		}
	break;
	case Result4:
		if(rs.IsRefresh == 1){
			rs.IsRefresh = 0;
			rs.BatteryNum = 4;
			OLED_ShowCHinese(rs.x*2,rs.y*1,15,0); //3�ڵ��
			OLED_ShowCHinese(rs.x*3,rs.y*1,16,0);
			OLED_ShowCHinese(rs.x*4,rs.y*1,3,0);
			OLED_ShowCHinese(rs.x*5,rs.y*1,6,0);
			
			OLED_ShowCHinese(rs.x*2,rs.y*3,9,0); //��ʼ���
			OLED_ShowCHinese(rs.x*3,rs.y*3,10,0);
			OLED_ShowCHinese(rs.x*4,rs.y*3,11,0);
			OLED_ShowCHinese(rs.x*5,rs.y*3,3,0);
		}
	break;
	case Charge:
		if(rs.IsRefresh == 1){
			rs.IsRefresh = 0;
			//SetBatNum(rs.BatteryNum);//���ݽ��������ü̵�������
			SetVoltage(rs.BatteryNum);  //���ó���ѹ


			OLED_ShowCHinese(rs.x*0,rs.y*0,0,0); //���
			OLED_ShowCHinese(rs.x*1,rs.y*0,2,0);

			OLED_ShowCHinese(rs.x*0,rs.y*1,17,0); //״̬
			OLED_ShowCHinese(rs.x*1,rs.y*1,18,0);

			OLED_ShowCHinese(rs.x*0,rs.y*2,16,0); //����
			OLED_ShowCHinese(rs.x*1,rs.y*2,19,0);

			sprintf(str, ":%d",rs.BatteryNum);  //��ؽ���
			OLED_ShowString(rs.x*2,rs.y*2,&str,16,0);

			OLED_ShowCHinese(rs.x*0,rs.y*3,3,0); //���
			OLED_ShowCHinese(rs.x*1,rs.y*3,6,0);
		}
		sprintf(str, ":%0.1fV,%0.1fA", sv.v_bat,sv.a_bat);  //����ѹ����
		OLED_ShowString(rs.x*2,rs.y*0,&str,16,0);
		if(sv.bat1_state&&sv.bat2_state&&sv.bat3_state&&sv.bat4_state){
			sprintf(str, ":OK     ");  //���״̬
		}else{
			if(dotNum == 0){
				sprintf(str, ":");  //���״̬
			}else if(dotNum == 1){
				sprintf(str, ":.");
			}else if(dotNum == 2){
				sprintf(str, ":..");
			}else if(dotNum == 3){
				sprintf(str, ":...");
			}else if(dotNum == 4){
				sprintf(str, ":....");
			}else if(dotNum == 5){
				sprintf(str, ":.....");
			}else if(dotNum == 6){
				sprintf(str, ":......");
			}else{
				sprintf(str, ":.......");
			}
			dotNum++;
			if(dotNum >= 8){
				dotNum = 0;
				sprintf(str, ":       ");
			}
		}
		OLED_ShowString(rs.x*2,rs.y*1,&str,16,0);

		sprintf(str, ":%d %d %d %d",sv.bat1_state,sv.bat2_state,sv.bat3_state,sv.bat4_state);
		OLED_ShowString(rs.x*2,rs.y*3,&str,16,0);
	break;
	
	default:
		break;
	}
}

void task_500ms(void){
	static u8 blink = 0;
	switch (rs.Page){
	case Index:
	case Result1:
	case Result2:
	case Result3:
	case Result4:
		if(blink){
			OLED_ShowString(rs.x*1,rs.y*3," >",16,0);OLED_ShowString(rs.x*6,rs.y*3,"<",16,0);
		}else{
			OLED_ShowString(rs.x*1,rs.y*3,"  ",16,0);OLED_ShowString(rs.x*6,rs.y*3," ",16,0);
		}
		blink = !blink;
		break;
	default:
		break;
	}
}

// 1sʱ��
void taskLed(void){
	RunLed = ~RunLed;
}

//������
#define TaskMaxNum 4
static TaskInfo task[TaskMaxNum] = {
  {10,10,0,task_10ms},
  {100,100,0,task_100ms},
  {200,200,0,task_500ms},
  {1000,1000,0,taskLed},
};


void main(void){
	AppInit();
	while(1){
		u8 i = 0;
		for (i = 0; i < TaskMaxNum; i++){
      if(task[i].RunFalg == 1){
        task[i].pTaskHook();
        task[i].RunFalg = 0;
      }
    }
	}
}

// ����
void heartbeat(void){
	u8 i = 0;
  for (i = 0; i < TaskMaxNum; i++){
    if(task[i].Timer > 0 && task[i].RunFalg == 0){
      task[i].Timer--;
      if(task[i].Timer == 0){
        task[i].RunFalg = 1;
        task[i].Timer = task[i].SpaceTime;
      }
    }
  }
}


