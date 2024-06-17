#ifndef __TFT_INIT_H
#define __TFT_INIT_H

#include "gd32e23x.h"
#include "systick.h"
#include <stdio.h>

#define USE_HORIZONTAL 2  //���ú�������������ʾ 0��1Ϊ���� 2��3Ϊ����


#if USE_HORIZONTAL==0||USE_HORIZONTAL==1
#define LCD_W 128
#define LCD_H 160

#else
#define LCD_W 160
#define LCD_H 128
#endif

void Init_SPI0_GPIO(void);
void TFT_WR_DATA8(uint8_t data);
void TFT_WR_DATA(uint16_t data);
void TFT_WR_DATA16(uint16_t data);
void TFT_WR_REG(uint8_t reg);
void TFT_WR_REG16(uint16_t reg);
void TFT_Address_Set(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2);
void TFT_Address_Set16(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2);
void TFT_Init(void);
void spi_dma_fill_transmit(uint16_t color,int size);

#endif