#ifndef OLED_OLED_H_
#define OLED_OLED_H_

#include ".\app\app.h"


void OLED_WR_CMD(u8 cmd);
void OLED_WR_DATA(u8 d);
void OLED_Init(void);
void OLED_Clear(void);
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Set_Pos(u8 x, u8 y);
void OLED_On(void);
void OLED_ShowNum(u8 x,u8 y,unsigned int num,u8 len,u8 size2,u8 Color_Turn);
void OLED_Showdecimal(u8 x,u8 y,float num,u8 z_len,u8 f_len,u8 size2, u8 Color_Turn);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size,u8 Color_Turn);
void OLED_ShowString(u8 x,u8 y,char*chr,u8 Char_Size,u8 Color_Turn);
void OLED_ShowCHinese(u8 x,u8 y,u8 no,u8 Color_Turn);
void OLED_DrawBMP(u8 x0, u8 y0, u8 x1, u8 y1, u8 *  BMP,u8 Color_Turn);
void OLED_HorizontalShift(u8 direction);
void OLED_Some_HorizontalShift(u8 direction,u8 start,u8 end);
void OLED_VerticalAndHorizontalShift(u8 direction);
void OLED_DisplayMode(u8 mode);
void OLED_IntensityControl(u8 intensity);



#endif /* OLED_OLED_H_ */

