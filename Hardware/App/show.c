#include "show.h"


/*
*   函数内容：PWM的静止UI界面
*   函数参数：无
*   返回值：  无
*/
void PWM_StaticUI(void){
	static enum MenuState MyMenu;
    uint16_t i=0,j=0;
    char showData[12]={0};

    if(oscilloscope.KnobValue > 0){
        //右翻页 
        oscilloscope.ThisMenu = DAC;
        oscilloscope.KnobValue = 0;
    } 
    if(oscilloscope.KnobValue < 0){
    	// 左翻页
        oscilloscope.ThisMenu = SetInput;
        oscilloscope.KnobValue = 0;
    }
    if(F_KeyState(0)){   //单击进去设置
        oscilloscope.ThisMenu = SetPwmOut;
    }
	// 防止重复刷新
	if(MyMenu == oscilloscope.ThisMenu){
		return;
	}
	MyMenu = oscilloscope.ThisMenu;

    TFT_ShowChinese(10,0,(uint8_t *)"简易示波器",BLACK,GREEN,16,0);
    
    sprintf(showData,"  PWM ");
    TFT_ShowString(110,0,(uint8_t *)showData,RED,YELLOW,16,0);  
    memset(showData,0,12);
    
    TFT_ShowChinese(110,20,(uint8_t *)"输出状态",WHITE,PURPLE,12,0);
    
    sprintf(showData,"      ");
    TFT_ShowString(110,36,(uint8_t *)showData,BLACK,YELLOW,16,0);  
    memset(showData,0,12);
    
    TFT_ShowChinese(110,56,(uint8_t *)"输出频率",WHITE,PURPLE,12,0);
    
    sprintf(showData,"      ");
    TFT_ShowString(110,72,(uint8_t *)showData,BLACK,YELLOW,16,0);  
    memset(showData,0,12);
    
    sprintf(showData,"        ");
    TFT_ShowString(110,92,(uint8_t *)showData,WHITE,PURPLE,12,0);  
    memset(showData,0,12);
    TFT_ShowChinese(118,92,(uint8_t *)"占空比",WHITE,PURPLE,12,0);
    
    sprintf(showData,"      ");
    TFT_ShowString(110,106,(uint8_t *)showData,BLACK,YELLOW,16,0);  
    memset(showData,0,12);
    
    TFT_ShowChinese(5,92,(uint8_t *)"输入幅值",WHITE,PURPLE,12,0);
    
    TFT_ShowChinese(55,92,(uint8_t *)"输入频率",WHITE,PURPLE,12,0);
    
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
*   函数内容：DAC的静止UI界面
*   函数参数：无
*   返回值：  无
*/
void DAC_StaticUI(void){
	static enum MenuState MyMenu;
    uint16_t i=0,j=0;
    char showData[12]={0};

	if(oscilloscope.KnobValue > 0){
        //右翻页 
    	oscilloscope.ThisMenu = SetInput;
        oscilloscope.KnobValue = 0;
    } 
    if(oscilloscope.KnobValue < 0){
        // 左翻页
        oscilloscope.ThisMenu = PWM;
        oscilloscope.KnobValue = 0;
    }
    if(F_KeyState(0)){   //单击进去设置
        oscilloscope.ThisMenu = SetDacOut;
    }

	// 防止重复刷新
	if(MyMenu == oscilloscope.ThisMenu){
		return;
	}
	MyMenu = oscilloscope.ThisMenu;
    
    // TFT_ShowChinese(10,0,(uint8_t *)"简易示波器",BLACK,GREEN,16,0);
    
    sprintf(showData,"  DAC ");
    TFT_ShowString(110,0,(uint8_t *)showData,RED,YELLOW,16,0);  
    memset(showData,0,12);
    
    TFT_ShowChinese(110,20,(uint8_t *)"输出状态",WHITE,PURPLE,12,0);
    
    sprintf(showData,"      ");
    TFT_ShowString(110,36,(uint8_t *)showData,BLACK,YELLOW,16,0);  
    memset(showData,0,12);
    
    TFT_ShowChinese(110,56,(uint8_t *)"输出电压",WHITE,PURPLE,12,0);
    
    sprintf(showData,"      ");
    TFT_ShowString(110,72,(uint8_t *)showData,BLACK,YELLOW,16,0);  
    memset(showData,0,12);
    
    sprintf(showData,"        ");
    TFT_ShowString(110,92,(uint8_t *)showData,WHITE,PURPLE,12,0);  
    memset(showData,0,12);

    // TFT_ShowChinese(118,92,(uint8_t *)"占空比",BLACK,BLACK,12,0);
    
    sprintf(showData,"      ");
    TFT_ShowString(110,106,(uint8_t *)showData,BLACK,BLACK,16,0);  
    memset(showData,0,12);
    
    TFT_ShowChinese(5,92,(uint8_t *)"输入幅值",WHITE,PURPLE,12,0);
    
    TFT_ShowChinese(55,92,(uint8_t *)"输入频率",WHITE,PURPLE,12,0);
    
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
*   函数内容：SET的静止UI界面
*   函数参数：无
*   返回值：  无
*/
void SET_StaticUI(void){
	static enum MenuState MyMenu;
    uint16_t i=0,j=0;
    char showData[12]={0};

	
	if(oscilloscope.KnobValue > 0){
        //右翻页 
        oscilloscope.ThisMenu = PWM;
        oscilloscope.KnobValue = 0;
    } 
    if(oscilloscope.KnobValue < 0){
        // 左翻页
        oscilloscope.ThisMenu = DAC;
        oscilloscope.KnobValue = 0;
    }

	// 防止重复刷新
	if(MyMenu == oscilloscope.ThisMenu){
		return;
	}
	MyMenu = oscilloscope.ThisMenu;
    
    // TFT_ShowChinese(10,0,(uint8_t *)"简易示波器",BLACK,GREEN,16,0);
    
    sprintf(showData,"  SET ");
    TFT_ShowString(110,0,(uint8_t *)showData,RED,YELLOW,16,0);  
    memset(showData,0,12);
    
    TFT_ShowChinese(110,20,(uint8_t *)"耦合状态",WHITE,PURPLE,12,0);
    
    sprintf(showData,"      ");
    TFT_ShowString(110,36,(uint8_t *)showData,BLACK,YELLOW,16,0);  
    memset(showData,0,12);
    
    TFT_ShowChinese(110,56,(uint8_t *)"衰减状态",WHITE,PURPLE,12,0);
    
    sprintf(showData,"      ");
    TFT_ShowString(110,72,(uint8_t *)showData,BLACK,YELLOW,16,0);  
    memset(showData,0,12);
    
    sprintf(showData,"        ");
    TFT_ShowString(110,92,(uint8_t *)showData,WHITE,PURPLE,12,0);  
    memset(showData,0,12);

    // TFT_ShowChinese(118,92,(uint8_t *)"占空比",BLACK,BLACK,12,0);
    
    sprintf(showData,"      ");
    TFT_ShowString(110,106,(uint8_t *)showData,BLACK,BLACK,16,0);  
    memset(showData,0,12);
    
    TFT_ShowChinese(5,92,(uint8_t *)"输入幅值",WHITE,PURPLE,12,0);
    
    TFT_ShowChinese(55,92,(uint8_t *)"输入频率",WHITE,PURPLE,12,0);
    
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


// 设置pwm输出按键扫描
void SetPwmOutFc(void){
	if(oscilloscope.KnobValue > 0){//右翻页 
        oscilloscope.ThisMenu = SetPwmFreq;
        oscilloscope.KnobValue = 0;
		return;
    } 
    if(oscilloscope.KnobValue < 0){// 左翻页
        oscilloscope.ThisMenu = PWM;
        oscilloscope.KnobValue = 0;
		return;
    }
	if(F_KeyState(0)){
		oscilloscope.ThisMenu = SetPwmOutAck;
		return;
	}
}
// 设置pwm频率按键扫描
void SetPwmFreqFc(void){
	if(oscilloscope.KnobValue > 0){ //右翻页 
        oscilloscope.ThisMenu = SetPwmDutyCycle;
        oscilloscope.KnobValue = 0;
		return;
    } 
    if(oscilloscope.KnobValue < 0){  // 左翻页
        oscilloscope.ThisMenu = SetPwmOut;
        oscilloscope.KnobValue = 0;
		return;
    }
	if(F_KeyState(0)){
		oscilloscope.ThisMenu = SetPwmFreqAck;
		return;
	}
}
//设置pwm占空比按键扫描
void SetPwmDutyCycleFc(void){
	if(oscilloscope.KnobValue > 0){ //右翻页 
        oscilloscope.ThisMenu = PWM;
        oscilloscope.KnobValue = 0;
		return;
    } 
    if(oscilloscope.KnobValue < 0){  // 左翻页
        oscilloscope.ThisMenu = SetPwmFreq;
        oscilloscope.KnobValue = 0;
		return;
    }
	if(F_KeyState(0)){
		oscilloscope.ThisMenu = SetPwmDutyCycleAck;
		return;
	}
}
// 设置DAC输出（按键扫描）
void SetDacOutFc(void){
	if(oscilloscope.KnobValue > 0){ //右翻页 
        oscilloscope.ThisMenu = SetDacDutyCycle;
        oscilloscope.KnobValue = 0;
		return;
    } 
    if(oscilloscope.KnobValue < 0){  // 左翻页
        oscilloscope.ThisMenu = DAC;
        oscilloscope.KnobValue = 0;
		return;
    }
	if(F_KeyState(0)){
		oscilloscope.ThisMenu = SetDacOutAck;
		return;
	}
}
// 设置DAC的占空比
void SetDacDutyCycleFc(void){
	if(oscilloscope.KnobValue > 0){ //右翻页 
        oscilloscope.ThisMenu = DAC;
        oscilloscope.KnobValue = 0;
		return;
    } 
    if(oscilloscope.KnobValue < 0){  // 左翻页
        oscilloscope.ThisMenu = SetDacOut;
        oscilloscope.KnobValue = 0;
		return;
    }
	if(F_KeyState(0)){
		oscilloscope.ThisMenu = SetDacDutyCycleAck;
		return;
	}
}
// 设置交直流耦合
void SetDcAcFc(void){
    if(oscilloscope.KnobValue > 0){ //右翻页 
        oscilloscope.ThisMenu = SetDecay;
        oscilloscope.KnobValue = 0;
		return;
    } 
    if(oscilloscope.KnobValue < 0){  // 左翻页
        oscilloscope.ThisMenu = SetInput;
        oscilloscope.KnobValue = 0;
		return;
    }
	if(F_KeyState(0)){
		oscilloscope.ThisMenu = SetDcAcAck;
		return;
	}
}
// 设置衰减
void SetDecayFc(void){
    if(oscilloscope.KnobValue > 0){ //右翻页 
        oscilloscope.ThisMenu = SetInput;
        oscilloscope.KnobValue = 0;
		return;
    } 
    if(oscilloscope.KnobValue < 0){  // 左翻页
        oscilloscope.ThisMenu = SetDcAc;
        oscilloscope.KnobValue = 0;
		return;
    }
	if(F_KeyState(0)){
		oscilloscope.ThisMenu = SetDecayAck;
		return;
	}
}


// 设置pwm输出按键扫描（闪烁状态）
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
// 设置pwm频率(闪烁)
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
// 设置占空比（闪烁）
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
// 设置DAC输出(闪烁)
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
// 设置DAC占空比闪烁
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
// 设置交直流耦合闪烁
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
// 设置衰减闪烁
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

// 修改PWM时开关状态时，后面两个的刷新
void SetPwmAfterRefresh(bool ResetColor){
    char showData[10]= "";
    // 第二个输出
    DisplayFreq(BLACK,YELLOW);
	// 占空比
	sprintf(showData,"%3.1f%%  ",((oscilloscope.pwmDutyCycle)/(oscilloscope.timerPeriod+0.0f))*100);
	TFT_ShowString(110,106,(uint8_t *)showData,BLACK,YELLOW,16,0);  
	memset(showData,0,10);
    if(ResetColor){
        TFT_ShowString(110,0,(uint8_t *)("  PWM "),RED,YELLOW,16,0); 
    }else{
        TFT_ShowString(110,0,(uint8_t *)("  PWM "),BLACK,YELLOW,16,0); //恢复原来的颜色
    } 
}

// 修改PWM时频率，中间两个的刷新
void SetPwmMiddleRefresh(void){
    char showData[10]= "";
    TFT_ShowChinese(118,36,(uint8_t *)(oscilloscope.IsPwmOut == 1?"打开":"关闭"),BLACK,YELLOW,16,0);
	// 占空比
	sprintf(showData,"%3.1f%%  ",((oscilloscope.pwmDutyCycle)/(oscilloscope.timerPeriod+0.0f))*100);
	TFT_ShowString(110,106,(uint8_t *)showData,BLACK,YELLOW,16,0);  
	memset(showData,0,10);      
    TFT_ShowString(110,0,(uint8_t *)("  PWM "),BLACK,YELLOW,16,0); //恢复原来的颜色
}

// 修改PWM占空比，前面两个刷新
void SetPwmBeforeRefresh(void){
    char showData[10]= "";
    TFT_ShowChinese(118,36,(uint8_t *)(oscilloscope.IsPwmOut == 1?"打开":"关闭"),BLACK,YELLOW,16,0);
    DisplayFreq(BLACK,YELLOW);
    TFT_ShowString(110,0,(uint8_t *)("  PWM "),BLACK,YELLOW,16,0); //恢复原来的颜色
}


// 修改DAC状态（刷新后面两组）
void SetDacAfterRefresh(bool ResetColor){
    char showData[10]= "";
	// 输出电压
	sprintf(showData,"%1.2fV ",oscilloscope.DacVoltage);
	TFT_ShowString(110,72,(uint8_t *)showData,BLACK,YELLOW,16,0);  
	// 占空比(这里无内容)
	TFT_ShowString(110,106,(uint8_t *)"      ",YELLOW,YELLOW,16,0);
    if(ResetColor){
        TFT_ShowString(110,0,(uint8_t *)("  DAC "),RED,YELLOW,16,0); 
    }else{
        TFT_ShowString(110,0,(uint8_t *)("  DAC "),BLACK,YELLOW,16,0); //恢复原来的颜色
    } 
}
// 修改DAC占空比刷新后面两组
void SetDacBeforeRefresh(void){
    TFT_ShowChinese(118,36,(uint8_t *)(oscilloscope.IsDACOut == 1?"打开":"关闭"),BLACK,YELLOW,16,0);  
    TFT_ShowString(110,106,(uint8_t *)"      ",YELLOW,YELLOW,16,0);// 占空比(这里无内容)
}





// 显示频率的函数封装
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
*   函数内容：  显示字符串
*   函数参数：  uint16_t vpp--峰峰值
*               uint16_t freq-频率
*               float DoBias--直流偏执信号
*   返回值：    无
*/
void TFT_ShowUI(void){
    static uint16_t counter;
    counter++;
	// 输入电压
    char showData[10]= "";
    sprintf(showData,"%1.2fV ",oscilloscope.vpp);
    TFT_ShowString(5,106,(uint8_t *)showData,BLACK,GREEN,16,0);  
    memset(showData,0,10);
    
	// 输入频率
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
        // 第一个状态
        TFT_ShowChinese(118,36,(uint8_t *)(oscilloscope.IsPwmOut == 1?"打开":"关闭"),BLACK,YELLOW,16,0);   
        SetPwmAfterRefresh(true);
		break;
    case SetPwmOut: //设置PWM输出状态(选择阶段)
        TFT_ShowChinese(118,36,(uint8_t *)(oscilloscope.IsPwmOut == 1?"打开":"关闭"),RED,YELLOW,16,0);   
        SetPwmAfterRefresh(false);
        break;
    case SetPwmOutAck:  //设置PWM输出状态(闪烁)
        if(counter == 2){
            TFT_ShowChinese(118,36,(uint8_t *)(oscilloscope.IsPwmOut == 1?"打开":"关闭"),RED,BLUE,16,0);   
        }
        if(counter >= 4){
            counter = 0;
            TFT_ShowChinese(118,36,(uint8_t *)(oscilloscope.IsPwmOut == 1?"打开":"关闭"),RED,YELLOW,16,0);  
        }
        SetPwmAfterRefresh(false);
        break;
   	case SetPwmFreq://设置Pwm频率
        DisplayFreq(RED,YELLOW);
        SetPwmMiddleRefresh();
        break;
    case SetPwmFreqAck: //设置Pwm频率(闪烁)
        if(counter == 2){DisplayFreq(RED,BLUE);}
        if(counter >= 4){
            counter = 0;
            DisplayFreq(RED,YELLOW);
        }
        SetPwmMiddleRefresh();
        break;

   	case SetPwmDutyCycle://设置PWM占空比
        sprintf(showData,"%3.1f%%  ",((oscilloscope.pwmDutyCycle)/(oscilloscope.timerPeriod+0.0f))*100);
        TFT_ShowString(110,106,(uint8_t *)showData,RED,YELLOW,16,0);    
        SetPwmBeforeRefresh();
        break;
    case SetPwmDutyCycleAck:  //设置占空比（闪烁）
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

    // 开始处理DAC
	case DAC:
		// 第一个状态
		TFT_ShowChinese(118,36,(uint8_t *)(oscilloscope.IsDACOut == 1?"打开":"关闭"),BLACK,YELLOW,16,0);   
        SetDacAfterRefresh(true);
		break;
    case SetDacOut:
		TFT_ShowChinese(118,36,(uint8_t *)(oscilloscope.IsDACOut == 1?"打开":"关闭"),RED,YELLOW,16,0);   
        SetDacAfterRefresh(false);
        break;
    case SetDacOutAck:
        if(counter == 2){
            TFT_ShowChinese(118,36,(uint8_t *)(oscilloscope.IsDACOut == 1?"打开":"关闭"),RED,YELLOW,16,0);  
        }
        if(counter >= 4){
            counter = 0;
            TFT_ShowChinese(118,36,(uint8_t *)(oscilloscope.IsDACOut == 1?"打开":"关闭"),BLACK,YELLOW,16,0);  
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
		TFT_ShowChinese(118,36,(uint8_t *)(oscilloscope.IsDACDC == 1?"直流":"交流"),BLACK,YELLOW,16,0);// 第一个状态
		TFT_ShowString(110,72,(uint8_t *)(oscilloscope.IsDecay == 0 ? " 1/50":"  No "),BLACK,YELLOW,16,0); // 有无衰减
		TFT_ShowString(110,106,(uint8_t *)"      ",YELLOW,YELLOW,16,0);  // 占空比(这里无内容)
		break;
	case SetDcAc:
        TFT_ShowString(110,0,(uint8_t *)("  SET "),BLACK,YELLOW,16,0);  
        TFT_ShowChinese(118,36,(uint8_t *)(oscilloscope.IsDACDC == 1?"直流":"交流"),RED,YELLOW,16,0);// 第一个状态
		TFT_ShowString(110,72,(uint8_t *)(oscilloscope.IsDecay == 0 ? " 1/50":"  No "),BLACK,YELLOW,16,0); // 有无衰减
		TFT_ShowString(110,106,(uint8_t *)"      ",YELLOW,YELLOW,16,0);  // 占空比(这里无内容)
    break;
    case SetDcAcAck:
        if(counter == 2){
            TFT_ShowChinese(118,36,(uint8_t *)(oscilloscope.IsDACDC == 1?"直流":"交流"),RED,YELLOW,16,0);
        }
        if(counter >= 4){
            counter = 0;
            TFT_ShowChinese(118,36,(uint8_t *)(oscilloscope.IsDACDC == 1?"直流":"交流"),BLACK,YELLOW,16,0);
        }
        TFT_ShowString(110,0,(uint8_t *)("  SET "),BLACK,YELLOW,16,0);  
		TFT_ShowString(110,72,(uint8_t *)(oscilloscope.IsDecay == 0 ? " 1/50":"  No "),BLACK,YELLOW,16,0); // 有无衰减
		TFT_ShowString(110,106,(uint8_t *)"      ",YELLOW,YELLOW,16,0);  // 占空比(这里无内容)
    break;
    case SetDecay:
        TFT_ShowString(110,0,(uint8_t *)("  SET "),BLACK,YELLOW,16,0);  
        TFT_ShowChinese(118,36,(uint8_t *)(oscilloscope.IsDACDC == 1?"直流":"交流"),BLACK,YELLOW,16,0);// 第一个状态
		TFT_ShowString(110,72,(uint8_t *)(oscilloscope.IsDecay == 0 ? " 1/50":"  No "),RED,YELLOW,16,0); // 有无衰减
		TFT_ShowString(110,106,(uint8_t *)"      ",YELLOW,YELLOW,16,0);  // 占空比(这里无内容)
    break;
    case SetDecayAck:
        if(counter == 2){
            TFT_ShowString(110,72,(uint8_t *)(oscilloscope.IsDecay == 0 ? " 1/50":"  No "),RED,YELLOW,16,0); // 有无衰减
        }
        if(counter >= 4){
            counter = 0;
            TFT_ShowString(110,72,(uint8_t *)(oscilloscope.IsDecay == 0 ? " 1/50":"  No "),BLACK,YELLOW,16,0); // 有无衰减
        }
        TFT_ShowString(110,0,(uint8_t *)("  SET "),BLACK,YELLOW,16,0);  
        TFT_ShowChinese(118,36,(uint8_t *)(oscilloscope.IsDACDC == 1?"直流":"交流"),BLACK,YELLOW,16,0);// 第一个状态
		TFT_ShowString(110,106,(uint8_t *)"      ",YELLOW,YELLOW,16,0);  // 占空比(这里无内容)
    break;

	default:
		break;
	}
    memset(showData,0,10);  
}