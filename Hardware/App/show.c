#include "show.h"


/*
*   �������ݣ�PWM�ľ�ֹUI����
*   ������������
*   ����ֵ��  ��
*/
void PWM_StaticUI(void){
	static enum MenuState MyMenu;
    uint16_t i=0,j=0;
    char showData[12]={0};

    if(oscilloscope.KnobValue > 0){
        //�ҷ�ҳ 
        oscilloscope.ThisMenu = DAC;
        oscilloscope.KnobValue = 0;
    } 
    if(oscilloscope.KnobValue < 0){
    	// ��ҳ
        oscilloscope.ThisMenu = SetInput;
        oscilloscope.KnobValue = 0;
    }
    if(F_KeyState(0)){   //������ȥ����
        oscilloscope.ThisMenu = SetPwmOut;
    }
	// ��ֹ�ظ�ˢ��
	if(MyMenu == oscilloscope.ThisMenu){
		return;
	}
	MyMenu = oscilloscope.ThisMenu;

    TFT_ShowChinese(10,0,(uint8_t *)"����ʾ����",BLACK,GREEN,16,0);
    
    sprintf(showData,"  PWM ");
    TFT_ShowString(110,0,(uint8_t *)showData,RED,YELLOW,16,0);  
    memset(showData,0,12);
    
    TFT_ShowChinese(110,20,(uint8_t *)"���״̬",WHITE,PURPLE,12,0);
    
    sprintf(showData,"      ");
    TFT_ShowString(110,36,(uint8_t *)showData,BLACK,YELLOW,16,0);  
    memset(showData,0,12);
    
    TFT_ShowChinese(110,56,(uint8_t *)"���Ƶ��",WHITE,PURPLE,12,0);
    
    sprintf(showData,"      ");
    TFT_ShowString(110,72,(uint8_t *)showData,BLACK,YELLOW,16,0);  
    memset(showData,0,12);
    
    sprintf(showData,"        ");
    TFT_ShowString(110,92,(uint8_t *)showData,WHITE,PURPLE,12,0);  
    memset(showData,0,12);
    TFT_ShowChinese(118,92,(uint8_t *)"ռ�ձ�",WHITE,PURPLE,12,0);
    
    sprintf(showData,"      ");
    TFT_ShowString(110,106,(uint8_t *)showData,BLACK,YELLOW,16,0);  
    memset(showData,0,12);
    
    TFT_ShowChinese(5,92,(uint8_t *)"�����ֵ",WHITE,PURPLE,12,0);
    
    TFT_ShowChinese(55,92,(uint8_t *)"����Ƶ��",WHITE,PURPLE,12,0);
    
    for(i=0;i<=128;i=i+2){
        TFT_DrawPoint(106,i,YELLOW);
    }
    
    for(i=0;i<100;i++){
        TFT_DrawPoint(i,81,GREEN);
    }
    for(i=30;i<=80;i++){
        TFT_DrawPoint(0,i,GREEN);
    }
    for(i=0;i<10;i++){
        TFT_DrawPoint((i*10)+2,82,GREEN); 
        TFT_DrawPoint((i*10)+3,82,GREEN); 
    }
    for(i=0;i<10;i++){
        TFT_DrawPoint((i*10)+2,83,GREEN); 
        TFT_DrawPoint((i*10)+3,83,GREEN);
    }
}

/*
*   �������ݣ�DAC�ľ�ֹUI����
*   ������������
*   ����ֵ��  ��
*/
void DAC_StaticUI(void){
	static enum MenuState MyMenu;
    uint16_t i=0,j=0;
    char showData[12]={0};

	if(oscilloscope.KnobValue > 0){
        //�ҷ�ҳ 
    	oscilloscope.ThisMenu = SetInput;
        oscilloscope.KnobValue = 0;
    } 
    if(oscilloscope.KnobValue < 0){
        // ��ҳ
        oscilloscope.ThisMenu = PWM;
        oscilloscope.KnobValue = 0;
    }
    if(F_KeyState(0)){   //������ȥ����
        oscilloscope.ThisMenu = SetDacOut;
    }

	// ��ֹ�ظ�ˢ��
	if(MyMenu == oscilloscope.ThisMenu){
		return;
	}
	MyMenu = oscilloscope.ThisMenu;
    
    // TFT_ShowChinese(10,0,(uint8_t *)"����ʾ����",BLACK,GREEN,16,0);
    
    sprintf(showData,"  DAC ");
    TFT_ShowString(110,0,(uint8_t *)showData,RED,YELLOW,16,0);  
    memset(showData,0,12);
    
    TFT_ShowChinese(110,20,(uint8_t *)"���״̬",WHITE,PURPLE,12,0);
    
    sprintf(showData,"      ");
    TFT_ShowString(110,36,(uint8_t *)showData,BLACK,YELLOW,16,0);  
    memset(showData,0,12);
    
    TFT_ShowChinese(110,56,(uint8_t *)"�����ѹ",WHITE,PURPLE,12,0);
    
    sprintf(showData,"      ");
    TFT_ShowString(110,72,(uint8_t *)showData,BLACK,YELLOW,16,0);  
    memset(showData,0,12);
    
    sprintf(showData,"        ");
    TFT_ShowString(110,92,(uint8_t *)showData,WHITE,PURPLE,12,0);  
    memset(showData,0,12);

    // TFT_ShowChinese(118,92,(uint8_t *)"ռ�ձ�",BLACK,BLACK,12,0);
    
    sprintf(showData,"      ");
    TFT_ShowString(110,106,(uint8_t *)showData,BLACK,BLACK,16,0);  
    memset(showData,0,12);
    
    TFT_ShowChinese(5,92,(uint8_t *)"�����ֵ",WHITE,PURPLE,12,0);
    
    TFT_ShowChinese(55,92,(uint8_t *)"����Ƶ��",WHITE,PURPLE,12,0);
    
    for(i=0;i<=128;i=i+2){
        TFT_DrawPoint(106,i,YELLOW);
    }
    
    for(i=0;i<100;i++){
        TFT_DrawPoint(i,81,GREEN);
    }
    for(i=30;i<=80;i++){
        TFT_DrawPoint(0,i,GREEN);
    }
    for(i=0;i<10;i++){
        TFT_DrawPoint((i*10)+2,82,GREEN); 
        TFT_DrawPoint((i*10)+3,82,GREEN); 
    }
    for(i=0;i<10;i++){
        TFT_DrawPoint((i*10)+2,83,GREEN); 
        TFT_DrawPoint((i*10)+3,83,GREEN);
    }
}




/*
*   �������ݣ�SET�ľ�ֹUI����
*   ������������
*   ����ֵ��  ��
*/
void SET_StaticUI(void){
	static enum MenuState MyMenu;
    uint16_t i=0,j=0;
    char showData[12]={0};

	
	if(oscilloscope.KnobValue > 0){
        //�ҷ�ҳ 
        oscilloscope.ThisMenu = PWM;
        oscilloscope.KnobValue = 0;
    } 
    if(oscilloscope.KnobValue < 0){
        // ��ҳ
        oscilloscope.ThisMenu = DAC;
        oscilloscope.KnobValue = 0;
    }

	// ��ֹ�ظ�ˢ��
	if(MyMenu == oscilloscope.ThisMenu){
		return;
	}
	MyMenu = oscilloscope.ThisMenu;
    
    // TFT_ShowChinese(10,0,(uint8_t *)"����ʾ����",BLACK,GREEN,16,0);
    
    sprintf(showData,"  SET ");
    TFT_ShowString(110,0,(uint8_t *)showData,RED,YELLOW,16,0);  
    memset(showData,0,12);
    
    TFT_ShowChinese(110,20,(uint8_t *)"���״̬",WHITE,PURPLE,12,0);
    
    sprintf(showData,"      ");
    TFT_ShowString(110,36,(uint8_t *)showData,BLACK,YELLOW,16,0);  
    memset(showData,0,12);
    
    TFT_ShowChinese(110,56,(uint8_t *)"˥��״̬",WHITE,PURPLE,12,0);
    
    sprintf(showData,"      ");
    TFT_ShowString(110,72,(uint8_t *)showData,BLACK,YELLOW,16,0);  
    memset(showData,0,12);
    
    sprintf(showData,"        ");
    TFT_ShowString(110,92,(uint8_t *)showData,WHITE,PURPLE,12,0);  
    memset(showData,0,12);

    // TFT_ShowChinese(118,92,(uint8_t *)"ռ�ձ�",BLACK,BLACK,12,0);
    
    sprintf(showData,"      ");
    TFT_ShowString(110,106,(uint8_t *)showData,BLACK,BLACK,16,0);  
    memset(showData,0,12);
    
    TFT_ShowChinese(5,92,(uint8_t *)"�����ֵ",WHITE,PURPLE,12,0);
    
    TFT_ShowChinese(55,92,(uint8_t *)"����Ƶ��",WHITE,PURPLE,12,0);
    
    for(i=0;i<=128;i=i+2){
        TFT_DrawPoint(106,i,YELLOW);
    }
    
    for(i=0;i<100;i++){
        TFT_DrawPoint(i,81,GREEN);
    }
    for(i=30;i<=80;i++){
        TFT_DrawPoint(0,i,GREEN);
    }
    for(i=0;i<10;i++){
        TFT_DrawPoint((i*10)+2,82,GREEN); 
        TFT_DrawPoint((i*10)+3,82,GREEN); 
    }
    for(i=0;i<10;i++){
        TFT_DrawPoint((i*10)+2,83,GREEN); 
        TFT_DrawPoint((i*10)+3,83,GREEN);
    }
}


// ����pwm�������ɨ��
void SetPwmOutFc(void){
	if(oscilloscope.KnobValue > 0){//�ҷ�ҳ 
        oscilloscope.ThisMenu = SetPwmFreq;
        oscilloscope.KnobValue = 0;
		return;
    } 
    if(oscilloscope.KnobValue < 0){// ��ҳ
        oscilloscope.ThisMenu = PWM;
        oscilloscope.KnobValue = 0;
		return;
    }
	if(F_KeyState(0)){
		oscilloscope.ThisMenu = SetPwmOutAck;
		return;
	}
}
// ����pwmƵ�ʰ���ɨ��
void SetPwmFreqFc(void){
	if(oscilloscope.KnobValue > 0){ //�ҷ�ҳ 
        oscilloscope.ThisMenu = SetPwmDutyCycle;
        oscilloscope.KnobValue = 0;
		return;
    } 
    if(oscilloscope.KnobValue < 0){  // ��ҳ
        oscilloscope.ThisMenu = SetPwmOut;
        oscilloscope.KnobValue = 0;
		return;
    }
	if(F_KeyState(0)){
		oscilloscope.ThisMenu = SetPwmFreqAck;
		return;
	}
}
//����pwmռ�ձȰ���ɨ��
void SetPwmDutyCycleFc(void){
	if(oscilloscope.KnobValue > 0){ //�ҷ�ҳ 
        oscilloscope.ThisMenu = PWM;
        oscilloscope.KnobValue = 0;
		return;
    } 
    if(oscilloscope.KnobValue < 0){  // ��ҳ
        oscilloscope.ThisMenu = SetPwmFreq;
        oscilloscope.KnobValue = 0;
		return;
    }
	if(F_KeyState(0)){
		oscilloscope.ThisMenu = SetPwmDutyCycleAck;
		return;
	}
}
// ����DAC���������ɨ�裩
void SetDacOutFc(void){
	if(oscilloscope.KnobValue > 0){ //�ҷ�ҳ 
        oscilloscope.ThisMenu = SetDacDutyCycle;
        oscilloscope.KnobValue = 0;
		return;
    } 
    if(oscilloscope.KnobValue < 0){  // ��ҳ
        oscilloscope.ThisMenu = DAC;
        oscilloscope.KnobValue = 0;
		return;
    }
	if(F_KeyState(0)){
		oscilloscope.ThisMenu = SetDacOutAck;
		return;
	}
}
// ����DAC��ռ�ձ�
void SetDacDutyCycleFc(void){
	if(oscilloscope.KnobValue > 0){ //�ҷ�ҳ 
        oscilloscope.ThisMenu = DAC;
        oscilloscope.KnobValue = 0;
		return;
    } 
    if(oscilloscope.KnobValue < 0){  // ��ҳ
        oscilloscope.ThisMenu = SetDacOut;
        oscilloscope.KnobValue = 0;
		return;
    }
	if(F_KeyState(0)){
		oscilloscope.ThisMenu = SetDacDutyCycleAck;
		return;
	}
}
// ���ý�ֱ�����
void SetDcAcFc(void){
    if(oscilloscope.KnobValue > 0){ //�ҷ�ҳ 
        oscilloscope.ThisMenu = SetDecay;
        oscilloscope.KnobValue = 0;
		return;
    } 
    if(oscilloscope.KnobValue < 0){  // ��ҳ
        oscilloscope.ThisMenu = SetInput;
        oscilloscope.KnobValue = 0;
		return;
    }
	if(F_KeyState(0)){
		oscilloscope.ThisMenu = SetDcAcAck;
		return;
	}
}
// ����˥��
void SetDecayFc(void){
    if(oscilloscope.KnobValue > 0){ //�ҷ�ҳ 
        oscilloscope.ThisMenu = SetInput;
        oscilloscope.KnobValue = 0;
		return;
    } 
    if(oscilloscope.KnobValue < 0){  // ��ҳ
        oscilloscope.ThisMenu = SetDcAc;
        oscilloscope.KnobValue = 0;
		return;
    }
	if(F_KeyState(0)){
		oscilloscope.ThisMenu = SetDecayAck;
		return;
	}
}


// ����pwm�������ɨ�裨��˸״̬��
void SetPwmOutAckFc(void){
    if(oscilloscope.KnobValue != 0){
        oscilloscope.IsPwmOut = !oscilloscope.IsPwmOut;
        oscilloscope.KnobValue = 0;
        SetPwmState(oscilloscope.IsPwmOut);
        return;
    }
	if(F_KeyState(0)){
		oscilloscope.ThisMenu = SetPwmOut;
		return;
	}
}
// ����pwmƵ��(��˸)
void SetPwmFreqAckFc(void){
    if(oscilloscope.KnobValue != 0){
        oscilloscope.PwmSetFreqValue -= oscilloscope.KnobValue*10;
        oscilloscope.KnobValue = 0;
        Set_Output_Freq(oscilloscope.PwmSetFreqValue);
        return;
    }
	if(F_KeyState(0)){
		oscilloscope.ThisMenu = SetPwmFreq;
		return;
	}
}
// ����ռ�ձȣ���˸��
void SetPwmDutyCycleAckFc(void){
    if(oscilloscope.KnobValue != 0){
        oscilloscope.pwmDutyCycle += oscilloscope.KnobValue*10;
        oscilloscope.KnobValue = 0;
        Set_Output_PWMComparex(oscilloscope.pwmDutyCycle);
        return;
    }

	if(F_KeyState(0)){
		oscilloscope.ThisMenu = SetPwmDutyCycle;
		return;
	}
}
// ����DAC���(��˸)
void SetDacOutAckFc(void){
    if(oscilloscope.KnobValue != 0){
        oscilloscope.IsDACOut = !oscilloscope.IsDACOut;
        oscilloscope.KnobValue = 0;
        SetDacState(oscilloscope.IsDACOut);
        return;
    }
	if(F_KeyState(0)){
		oscilloscope.ThisMenu = SetDacOut;
		return;
	}
}
// ����DACռ�ձ���˸
void SetDacDutyCycleAckFc(void){
    if(oscilloscope.KnobValue != 0){
        oscilloscope.DacDutyCycle += oscilloscope.KnobValue*10;
        oscilloscope.KnobValue = 0;
        SetDacComparex(oscilloscope.DacDutyCycle);
        return;
    }
	if(F_KeyState(0)){
		oscilloscope.ThisMenu = SetDacDutyCycle;
		return;
	}
}
// ���ý�ֱ�������˸
void SetDcAcAckFc(void){
    if(oscilloscope.KnobValue != 0){
        oscilloscope.KnobValue = 0;
        SetSignalAcOrDc(!oscilloscope.IsDACDC);
        return;
    }
	if(F_KeyState(0)){
		oscilloscope.ThisMenu = SetDcAc;
		return;
	}
}
// ����˥����˸
void SetDecayAckFc(void){
    if(oscilloscope.KnobValue != 0){
        oscilloscope.KnobValue = 0;
        SetSignalDecay(!oscilloscope.IsDecay);
        return;
    }
	if(F_KeyState(0)){
		oscilloscope.ThisMenu = SetDcAc;
		return;
	}
}

// �޸�PWMʱ����״̬ʱ������������ˢ��
void SetPwmAfterRefresh(bool ResetColor){
    char showData[10]= "";
    // �ڶ������
    DisplayFreq(BLACK,YELLOW);
	// ռ�ձ�
	sprintf(showData,"%3.1f%%  ",((oscilloscope.pwmDutyCycle)/(oscilloscope.timerPeriod+0.0f))*100);
	TFT_ShowString(110,106,(uint8_t *)showData,BLACK,YELLOW,16,0);  
	memset(showData,0,10);
    if(ResetColor){
        TFT_ShowString(110,0,(uint8_t *)("  PWM "),RED,YELLOW,16,0); 
    }else{
        TFT_ShowString(110,0,(uint8_t *)("  PWM "),BLACK,YELLOW,16,0); //�ָ�ԭ������ɫ
    } 
}

// �޸�PWMʱƵ�ʣ��м�������ˢ��
void SetPwmMiddleRefresh(void){
    char showData[10]= "";
    TFT_ShowChinese(118,36,(uint8_t *)(oscilloscope.IsPwmOut == 1?"��":"�ر�"),BLACK,YELLOW,16,0);
	// ռ�ձ�
	sprintf(showData,"%3.1f%%  ",((oscilloscope.pwmDutyCycle)/(oscilloscope.timerPeriod+0.0f))*100);
	TFT_ShowString(110,106,(uint8_t *)showData,BLACK,YELLOW,16,0);  
	memset(showData,0,10);      
    TFT_ShowString(110,0,(uint8_t *)("  PWM "),BLACK,YELLOW,16,0); //�ָ�ԭ������ɫ
}

// �޸�PWMռ�ձȣ�ǰ������ˢ��
void SetPwmBeforeRefresh(void){
    char showData[10]= "";
    TFT_ShowChinese(118,36,(uint8_t *)(oscilloscope.IsPwmOut == 1?"��":"�ر�"),BLACK,YELLOW,16,0);
    DisplayFreq(BLACK,YELLOW);
    TFT_ShowString(110,0,(uint8_t *)("  PWM "),BLACK,YELLOW,16,0); //�ָ�ԭ������ɫ
}


// �޸�DAC״̬��ˢ�º������飩
void SetDacAfterRefresh(bool ResetColor){
    char showData[10]= "";
	// �����ѹ
	sprintf(showData,"%1.2fV ",oscilloscope.DacVoltage);
	TFT_ShowString(110,72,(uint8_t *)showData,BLACK,YELLOW,16,0);  
	// ռ�ձ�(����������)
	TFT_ShowString(110,106,(uint8_t *)"      ",YELLOW,YELLOW,16,0);
    if(ResetColor){
        TFT_ShowString(110,0,(uint8_t *)("  DAC "),RED,YELLOW,16,0); 
    }else{
        TFT_ShowString(110,0,(uint8_t *)("  DAC "),BLACK,YELLOW,16,0); //�ָ�ԭ������ɫ
    } 
}
// �޸�DACռ�ձ�ˢ�º�������
void SetDacBeforeRefresh(void){
    TFT_ShowChinese(118,36,(uint8_t *)(oscilloscope.IsDACOut == 1?"��":"�ر�"),BLACK,YELLOW,16,0);  
    TFT_ShowString(110,106,(uint8_t *)"      ",YELLOW,YELLOW,16,0);// ռ�ձ�(����������)
}





// ��ʾƵ�ʵĺ�����װ
void DisplayFreq(uint16_t fc,uint16_t bc){
    char showData[10]= "";
    if(oscilloscope.PwmOutFreq>=1000){
        sprintf(showData,"%1.2fK ",oscilloscope.PwmOutFreq/1000.0f);
        TFT_ShowString(110,72,(uint8_t *)showData,fc,bc,16,0);  
        memset(showData,0,10); 
    }else{
        sprintf(showData," %3dHz",oscilloscope.PwmOutFreq);
        TFT_ShowString(110,72,(uint8_t *)showData,fc,bc,16,0);  
        memset(showData,0,10);
    }
}


/*
*   �������ݣ�  ��ʾ�ַ���
*   ����������  uint16_t vpp--���ֵ
*               uint16_t freq-Ƶ��
*               float DoBias--ֱ��ƫִ�ź�
*   ����ֵ��    ��
*/
void TFT_ShowUI(void){
    static uint16_t counter;
    counter++;
	// �����ѹ
    char showData[10]= "";
    sprintf(showData,"%1.2fV ",oscilloscope.vpp);
    TFT_ShowString(5,106,(uint8_t *)showData,BLACK,GREEN,16,0);  
    memset(showData,0,10);
    
	// ����Ƶ��
    if(oscilloscope.gatherFreq>=1000){
        sprintf(showData,"%1.1fKHz",oscilloscope.gatherFreq/1000.0f);
        TFT_ShowString(55,106,(uint8_t *)showData,BLACK,GREEN,16,0);  
        memset(showData,0,10);
    }else{
        sprintf(showData,"%3.0fHz ",oscilloscope.gatherFreq);
        TFT_ShowString(55,106,(uint8_t *)showData,BLACK,GREEN,16,0);  
        memset(showData,0,10);  
    }
	
	switch (oscilloscope.ThisMenu){
	case PWM:
        // ��һ��״̬
        TFT_ShowChinese(118,36,(uint8_t *)(oscilloscope.IsPwmOut == 1?"��":"�ر�"),BLACK,YELLOW,16,0);   
        SetPwmAfterRefresh(true);
		break;
    case SetPwmOut: //����PWM���״̬(ѡ��׶�)
        TFT_ShowChinese(118,36,(uint8_t *)(oscilloscope.IsPwmOut == 1?"��":"�ر�"),RED,YELLOW,16,0);   
        SetPwmAfterRefresh(false);
        break;
    case SetPwmOutAck:  //����PWM���״̬(��˸)
        if(counter == 2){
            TFT_ShowChinese(118,36,(uint8_t *)(oscilloscope.IsPwmOut == 1?"��":"�ر�"),RED,BLUE,16,0);   
        }
        if(counter >= 4){
            counter = 0;
            TFT_ShowChinese(118,36,(uint8_t *)(oscilloscope.IsPwmOut == 1?"��":"�ر�"),RED,YELLOW,16,0);  
        }
        SetPwmAfterRefresh(false);
        break;
   	case SetPwmFreq://����PwmƵ��
        DisplayFreq(RED,YELLOW);
        SetPwmMiddleRefresh();
        break;
    case SetPwmFreqAck: //����PwmƵ��(��˸)
        if(counter == 2){DisplayFreq(RED,BLUE);}
        if(counter >= 4){
            counter = 0;
            DisplayFreq(RED,YELLOW);
        }
        SetPwmMiddleRefresh();
        break;

   	case SetPwmDutyCycle://����PWMռ�ձ�
        sprintf(showData,"%3.1f%%  ",((oscilloscope.pwmDutyCycle)/(oscilloscope.timerPeriod+0.0f))*100);
        TFT_ShowString(110,106,(uint8_t *)showData,RED,YELLOW,16,0);    
        SetPwmBeforeRefresh();
        break;
    case SetPwmDutyCycleAck:  //����ռ�ձȣ���˸��
        sprintf(showData,"%3.1f%%  ",((oscilloscope.pwmDutyCycle)/(oscilloscope.timerPeriod+0.0f))*100);
        if(counter == 2){
            TFT_ShowString(110,106,(uint8_t *)showData,RED,BLUE,16,0);  
        }
        if(counter >= 4){
            counter = 0;
            TFT_ShowString(110,106,(uint8_t *)showData,RED,YELLOW,16,0);  
        }  
        SetPwmBeforeRefresh();
        break;

    // ��ʼ����DAC
	case DAC:
		// ��һ��״̬
		TFT_ShowChinese(118,36,(uint8_t *)(oscilloscope.IsDACOut == 1?"��":"�ر�"),BLACK,YELLOW,16,0);   
        SetDacAfterRefresh(true);
		break;
    case SetDacOut:
		TFT_ShowChinese(118,36,(uint8_t *)(oscilloscope.IsDACOut == 1?"��":"�ر�"),RED,YELLOW,16,0);   
        SetDacAfterRefresh(false);
        break;
    case SetDacOutAck:
        if(counter == 2){
            TFT_ShowChinese(118,36,(uint8_t *)(oscilloscope.IsDACOut == 1?"��":"�ر�"),RED,YELLOW,16,0);  
        }
        if(counter >= 4){
            counter = 0;
            TFT_ShowChinese(118,36,(uint8_t *)(oscilloscope.IsDACOut == 1?"��":"�ر�"),BLACK,YELLOW,16,0);  
        }
        SetDacAfterRefresh(false);
        break;
    case SetDacDutyCycle:
        sprintf(showData,"%1.2fV ",oscilloscope.DacVoltage);
        TFT_ShowString(110,72,(uint8_t *)showData,RED,YELLOW,16,0); 
        SetDacBeforeRefresh();
        break;
    case SetDacDutyCycleAck:
        sprintf(showData,"%1.2fV ",oscilloscope.DacVoltage);
        if(counter == 2){
            TFT_ShowString(110,72,(uint8_t *)showData,RED,YELLOW,16,0); 
        }
        if(counter >= 4){
            counter = 0;
            TFT_ShowString(110,72,(uint8_t *)showData,BLACK,YELLOW,16,0); 
        }
        SetDacBeforeRefresh();
        break;
	case SetInput:
        TFT_ShowString(110,0,(uint8_t *)("  SET "),RED,YELLOW,16,0);  
		TFT_ShowChinese(118,36,(uint8_t *)(oscilloscope.IsDACDC == 1?"ֱ��":"����"),BLACK,YELLOW,16,0);// ��һ��״̬
		TFT_ShowString(110,72,(uint8_t *)(oscilloscope.IsDecay == 0 ? " 1/50":"  No "),BLACK,YELLOW,16,0); // ����˥��
		TFT_ShowString(110,106,(uint8_t *)"      ",YELLOW,YELLOW,16,0);  // ռ�ձ�(����������)
		break;
	case SetDcAc:
        TFT_ShowString(110,0,(uint8_t *)("  SET "),BLACK,YELLOW,16,0);  
        TFT_ShowChinese(118,36,(uint8_t *)(oscilloscope.IsDACDC == 1?"ֱ��":"����"),RED,YELLOW,16,0);// ��һ��״̬
		TFT_ShowString(110,72,(uint8_t *)(oscilloscope.IsDecay == 0 ? " 1/50":"  No "),BLACK,YELLOW,16,0); // ����˥��
		TFT_ShowString(110,106,(uint8_t *)"      ",YELLOW,YELLOW,16,0);  // ռ�ձ�(����������)
    break;
    case SetDcAcAck:
        if(counter == 2){
            TFT_ShowChinese(118,36,(uint8_t *)(oscilloscope.IsDACDC == 1?"ֱ��":"����"),RED,YELLOW,16,0);
        }
        if(counter >= 4){
            counter = 0;
            TFT_ShowChinese(118,36,(uint8_t *)(oscilloscope.IsDACDC == 1?"ֱ��":"����"),BLACK,YELLOW,16,0);
        }
        TFT_ShowString(110,0,(uint8_t *)("  SET "),BLACK,YELLOW,16,0);  
		TFT_ShowString(110,72,(uint8_t *)(oscilloscope.IsDecay == 0 ? " 1/50":"  No "),BLACK,YELLOW,16,0); // ����˥��
		TFT_ShowString(110,106,(uint8_t *)"      ",YELLOW,YELLOW,16,0);  // ռ�ձ�(����������)
    break;
    case SetDecay:
        TFT_ShowString(110,0,(uint8_t *)("  SET "),BLACK,YELLOW,16,0);  
        TFT_ShowChinese(118,36,(uint8_t *)(oscilloscope.IsDACDC == 1?"ֱ��":"����"),BLACK,YELLOW,16,0);// ��һ��״̬
		TFT_ShowString(110,72,(uint8_t *)(oscilloscope.IsDecay == 0 ? " 1/50":"  No "),RED,YELLOW,16,0); // ����˥��
		TFT_ShowString(110,106,(uint8_t *)"      ",YELLOW,YELLOW,16,0);  // ռ�ձ�(����������)
    break;
    case SetDecayAck:
        if(counter == 2){
            TFT_ShowString(110,72,(uint8_t *)(oscilloscope.IsDecay == 0 ? " 1/50":"  No "),RED,YELLOW,16,0); // ����˥��
        }
        if(counter >= 4){
            counter = 0;
            TFT_ShowString(110,72,(uint8_t *)(oscilloscope.IsDecay == 0 ? " 1/50":"  No "),BLACK,YELLOW,16,0); // ����˥��
        }
        TFT_ShowString(110,0,(uint8_t *)("  SET "),BLACK,YELLOW,16,0);  
        TFT_ShowChinese(118,36,(uint8_t *)(oscilloscope.IsDACDC == 1?"ֱ��":"����"),BLACK,YELLOW,16,0);// ��һ��״̬
		TFT_ShowString(110,106,(uint8_t *)"      ",YELLOW,YELLOW,16,0);  // ռ�ձ�(����������)
    break;

	default:
		break;
	}
    memset(showData,0,10);  
}