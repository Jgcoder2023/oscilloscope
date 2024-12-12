/*
 * oled.c
 *
 *  Created on: 2022年7月24日
 *      Author: Unicorn_Li
 */
 
#include	"oled.h"

#include ".\app\app.h"

/**********************************************************
 * 初始化命令,根据芯片手册书写，详细步骤见上图以及注意事项
 ***********************************************************/
u8 CMD_Data[]={
0xAE, 0xD5, 0x80, 0xA8, 0x3F, 0xD3, 0x00, 0x40,0xA1, 0xC8, 0xDA,

0x12, 0x81, 0xCF, 0xD9, 0xF1, 0xDB, 0x40, 0xA4, 0xA6,0x8D, 0x14,

0xAF};



/**
 * @function: void OLED_Init(void)
 * @description: OLED初始化
 * @return {*}
 */
void OLED_Init(void)
{
	u8 i = 0;
//	HAL_Delay(200);
	delay_ms(200);
	
	for(i=0; i<23; i++)
	{
		OLED_WR_CMD(CMD_Data[i]);
	}
	
}

/**
 * @function: void OLED_WR_CMD(u8 cmd)
 * @description: 向设备写控制命令
 * @param {u8} cmd 芯片手册规定的命令
 * @return {*}
 */
void OLED_WR_CMD(u8 cmd)
{
	SI2C_WriteNbyte(0x78,0x00, &cmd, 1);
//	OLED_Data(0x78,0x00,cmd);
//	I2C_Mem_Write(I2C1,0x78,0x00,cmd);
//	HAL_I2C_Mem_Write(&hi2c1 ,0x78,0x00,I2C_MEMADD_SIZE_8BIT,&cmd,1,0x100);
}

/**
 * @function: void OLED_WR_DATA(u8 data)
 * @description: 向设备写控制数据
 * @param {u8} data 数据
 * @return {*}
 */
void OLED_WR_DATA(u8 d)
{
	SI2C_WriteNbyte(0x78,0x40, &d, 1);
	//OLED_Data(0x78,0x40,data);
//	I2C_Mem_Write(I2C1,0x78,0x40,data);
//	HAL_I2C_Mem_Write(&hi2c1 ,0x78,0x40,I2C_MEMADD_SIZE_8BIT,&data,1,0x100);
}

/**
 * @function: void OLED_On(void)
 * @description: 更新显示

 * @return {*}
 */
void OLED_On(void)
{
	u8 i,n;
	for(i=0;i<8;i++)
	{
		OLED_WR_CMD(0xb0+i);    //设置页地址（0~7）
		OLED_WR_CMD(0x00);      //设置显示位置—列低地址
		OLED_WR_CMD(0x10);      //设置显示位置—列高地址
		for(n=0;n<128;n++)
			OLED_WR_DATA(1);
	}
}


/**
 * @function: OLED_Clear(void)
 * @description: 清屏,整个屏幕是黑色的!和没点亮一样!!!
 * @return {*}
 */
void OLED_Clear(void)
{
	u8 i,n;
	for(i=0;i<8;i++)
	{
		OLED_WR_CMD(0xb0+i);    //设置页地址（0~7）
		OLED_WR_CMD(0x00);      //设置显示位置—列低地址
		OLED_WR_CMD(0x10);      //设置显示位置—列高地址
		for(n=0;n<128;n++)
			OLED_WR_DATA(0);
	}
}

/**
 * @function: void OLED_Display_On(void)
 * @description: 开启OLED显示
 * @return {*}
 */
void OLED_Display_On(void)
{
	OLED_WR_CMD(0X8D);  //SET DCDC命令
	OLED_WR_CMD(0X14);  //DCDC ON
	OLED_WR_CMD(0XAF);  //DISPLAY ON,打开显示
}


/**
 * @function: void OLED_Display_Off(void)
 * @description: 关闭OLED显示
 * @return {*}
 */
void OLED_Display_Off(void)
{
	OLED_WR_CMD(0X8D);  //SET DCDC命令
	OLED_WR_CMD(0X10);  //DCDC OFF
	OLED_WR_CMD(0XAE);  //DISPLAY OFF，关闭显示
}

/**
 * @function: void OLED_Set_Pos(u8 x, u8 y)
 * @description: 坐标设置
 * @param {u8} x,y
 * @return {*}
 */
void OLED_Set_Pos(u8 x, u8 y)
{
	OLED_WR_CMD(0xb0+y);	//设置页地址（0~7）
	OLED_WR_CMD(((x&0xf0)>>4)|0x10); //设置显示位置—列高地址
	OLED_WR_CMD(x&0x0f);	//设置显示位置—列低地址
}


/**
 * @function: unsigned int oled_pow(u8 m,u8 n)
 * @description: m^n函数
 * @param {u8} m,n
 * @return {unsigned int} result
 */
unsigned int oled_pow(u8 m,u8 n)
{
	unsigned int result=1;
	while(n--)result*=m;
	return result;
}

/**
 * @function: void OLED_ShowChar(u8 x, u8 y, u8 chr, u8 Char_Size,u8 Color_Turn)
 * @description: 在OLED12864特定位置开始显示一个字符
 * @param {u8} x字符开始显示的横坐标
 * @param {u8} y字符开始显示的纵坐标
 * @param {u8} chr待显示的字符
 * @param {u8} Char_Size待显示字符的字体大小,选择字体 16/12
 * @param {u8} Color_Turn是否反相显示(1反相、0不反相)
 * @return {*}
 */
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size,u8 Color_Turn)
{
	unsigned char c=0,i=0;
		c=chr-' ';//得到偏移后的值
		if(x>128-1){x=0;y=y+2;}
		if(Char_Size ==16)
		{
			OLED_Set_Pos(x,y);
			for(i=0;i<8;i++)
				{
				  if(Color_Turn)
					  OLED_WR_DATA(~F8X16[c*16+i]);
				  else
					  OLED_WR_DATA(F8X16[c*16+i]);
				}
			OLED_Set_Pos(x,y+1);
			for(i=0;i<8;i++)
			    {
				  if(Color_Turn)
					  OLED_WR_DATA(~F8X16[c*16+i+8]);
				  else
					  OLED_WR_DATA(F8X16[c*16+i+8]);
			    }

			}
	     else
	     {
				OLED_Set_Pos(x,y);
				for(i=0;i<6;i++)
			    {
				  if(Color_Turn)
					  OLED_WR_DATA(~F6x8[c][i]);
				  else
					  OLED_WR_DATA(F6x8[c][i]);
			    }
		  }
}

/**
 * @function: void OLED_ShowString(u8 x, u8 y, u8 *chr, uint8_tChar_Size, u8 Color_Turn)
 * @description: 在OLED12864特定位置开始显示字符串
 * @param {u8} x待显示字符串的开始横坐标x:0~127
 * @param {u8} y待显示字符串的开始纵坐标 y:0~7，若选择字体大小为16，则两行数字之间需要间隔2，若选择字体大小为12，间隔1
 * @param {u8} *chr待显示的字符串
 * @param {u8} Char_Size待显示字符串的字体大小,选择字体 16/12，16为8X16，12为6x8
 * @param {u8} Color_Turn是否反相显示(1反相、0不反相)
 * @return {*}
 */
void OLED_ShowString(u8 x,u8 y,char*chr,u8 Char_Size, u8 Color_Turn)
{
	u8  j=0;
	while (chr[j]!='\0')
	{		OLED_ShowChar(x,y,chr[j],Char_Size, Color_Turn);
			if (Char_Size == 12) //6X8的字体列加6，显示下一个字符
				x += 6;
			else  //8X16的字体列加8，显示下一个字符
				x += 8;

			if (x > 122 && Char_Size==12) //TextSize6x8如果一行不够显示了，从下一行继续显示
			{
				x = 0;
				y++;
			}
			if (x > 120 && Char_Size== 16) //TextSize8x16如果一行不够显示了，从下一行继续显示
			{
				x = 0;
				y++;
			}
			j++;
	}
}

/**
 * @function: void OLED_ShowNum(u8 x,u8 y,unsigned int num,u8 len,u8 size2, Color_Turn)
 * @description: 显示数字
 * @param {u8} x待显示的数字起始横坐标,x:0~126
 * @param {u8} y待显示的数字起始纵坐标, y:0~7，若选择字体大小为16，则两行数字之间需要间隔2，若选择字体大小为12，间隔1
 * @param {unsigned int} num:输入的数据
 * @param {u8 } len:输入的数据位数
 * @param {u8} size2:输入的数据大小，选择 16/12，16为8X16，12为6x8
 * @param {u8} Color_Turn是否反相显示(1反相、0不反相)
 * @return {*}
 */
void OLED_ShowNum(u8 x,u8 y,unsigned int num,u8 len,u8 size2, u8 Color_Turn)
{
	u8 t,temp;
	u8 enshow=0;
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size2/2)*t,y,' ',size2, Color_Turn);
				continue;
			}else enshow=1;

		}
	 	OLED_ShowChar(x+(size2/2)*t,y,temp+'0',size2, Color_Turn);
	}
}


/**
 * @function: void OLED_Showdecimal(u8 x,u8 y,float num,u8 z_len,u8 f_len,u8 size2, u8 Color_Turn)
 * @description: 显示正负浮点数
 * @param {u8} x待显示的数字起始横坐标,x:0~126
 * @param {u8} y待显示的数字起始纵坐标, y:0~7，若选择字体大小为16，则两行数字之间需要间隔2，若选择字体大小为12，间隔1
 * @param {float} num:输入的浮点型数据
 * @param {u8 } z_ len:整数部分的位数
 * @param {u8 } f_len: 小数部分的位数
 * @param {u8} size2:输入的数据大小，选择 16/12，16为8X16，12为6x8
 * @param {u8} Color_Turn是否反相显示(1反相、0不反相)
 * @return {*}
 */
void OLED_Showdecimal(u8 x,u8 y,float num,u8 z_len,u8 f_len,u8 size2, u8 Color_Turn)
{
	u8 t,temp,i=0;//i为负数标志位
	u8 enshow;
	int z_temp,f_temp;
	if(num<0)
	{
		z_len+=1;
		i=1;
		num=-num;
	}
	z_temp=(int)num;
	//整数部分
	for(t=0;t<z_len;t++)
	{
		temp=(z_temp/oled_pow(10,z_len-t-1))%10;
		if(enshow==0 && t<(z_len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size2/2)*t,y,' ',size2, Color_Turn);
				continue;
			}
			else
			enshow=1;
		}
		OLED_ShowChar(x+(size2/2)*t,y,temp+'0',size2, Color_Turn);
	}
	//小数点
	OLED_ShowChar(x+(size2/2)*(z_len),y,'.',size2, Color_Turn);

	f_temp=(int)((num-z_temp)*(oled_pow(10,f_len)));
  //小数部分
	for(t=0;t<f_len;t++)
	{
		temp=(f_temp/oled_pow(10,f_len-t-1))%10;
		OLED_ShowChar(x+(size2/2)*(t+z_len)+5,y,temp+'0',size2, Color_Turn);
	}
	if(i==1)//如果为负，就将最前的一位赋值‘-’
	{
		OLED_ShowChar(x,y,'-',size2, Color_Turn);
		i=0;
	}
}



/**
 * @function: void OLED_ShowCHinese(u8 x,u8 y,u8 no, u8 Color_Turn)
 * @description: 在OLED特定位置开始显示16X16汉字
 * @param {u8} x待显示的汉字起始横坐标x: 0~112，两列汉字之间需要间隔16
 * @param {u8} y待显示的汉字起始纵坐标 y: 0~6 , 两行汉字之间需要间隔2
 * @param {u8} no待显示的汉字编号
 * @param {u8} Color_Turn是否反相显示(1反相、0不反相)
 * @return {*}
 */
void OLED_ShowCHinese(u8 x,u8 y,u8 no, u8 Color_Turn)
{
	u8 t=0;
	OLED_Set_Pos(x,y);
    for(t=0;t<16;t++)
		{
				if (Color_Turn)
					OLED_WR_DATA(~Hzk[2*no][t]); //显示汉字的上半部分
				else
					OLED_WR_DATA(Hzk[2*no][t]); //显示汉字的上半部分
        }

		OLED_Set_Pos(x,y+1);
    for(t=0;t<16;t++)
		{
				if (Color_Turn)
					OLED_WR_DATA(~Hzk[2*no+1][t]); //显示汉字的上半部分
				else
					OLED_WR_DATA(Hzk[2*no+1][t]);//显示汉字的上半部分

         }
}

/**
 * @function: void OLED_DrawBMP(u8 x0, u8 y0, u8 x1, u8 y1, u8 *  BMP,u8 Color_Turn)
 * @description: 在OLED特定区域显示BMP图片
 * @param {u8} x0图像开始显示横坐标  x0:0~127
 * @param {u8} y0图像开始显示纵坐标  y0:0~7
 * @param {u8} x1图像结束显示横坐标  x1:1~128
 * @param {u8} y1图像结束显示纵坐标  y1:1~8
 * @param {u8} *BMP待显示的图像数据
 * @param {u8} Color_Turn是否反相显示(1反相、0不反相)
 * @return {*}
 */
void OLED_DrawBMP(u8 x0, u8 y0, u8 x1, u8 y1, u8 *  BMP,u8 Color_Turn)
{
   u32 j = 0;
   u8 x = 0, y = 0;

  if(y1%8==0)
		y = y1/8;
  else
		y = y1/8 + 1;
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
    for(x=x0;x<x1;x++)
		{
			if (Color_Turn)
				OLED_WR_DATA(~BMP[j++]);//显示反相图片
			else
				OLED_WR_DATA(BMP[j++]);//显示图片

		}
	}
}


/**
 * @function: void OLED_HorizontalShift(u8 direction)
 * @description: 屏幕内容水平全屏滚动播放
 * @param {u8} direction			LEFT	   0x27     	RIGHT  0x26
 * @return {*}
 */
void OLED_HorizontalShift(u8 direction)

{
	OLED_WR_CMD(0x2e);//停止滚动
	OLED_WR_CMD(direction);//设置滚动方向
	OLED_WR_CMD(0x00);//虚拟字节设置，默认为0x00
	OLED_WR_CMD(0x00);//设置开始页地址
	OLED_WR_CMD(0x07);//设置每个滚动步骤之间的时间间隔的帧频
    //  0x00-5帧， 0x01-64帧， 0x02-128帧， 0x03-256帧， 0x04-3帧， 0x05-4帧， 0x06-25帧， 0x07-2帧，
	OLED_WR_CMD(0x07);//设置结束页地址
	OLED_WR_CMD(0x00);//虚拟字节设置，默认为0x00
	OLED_WR_CMD(0xff);//虚拟字节设置，默认为0xff
	OLED_WR_CMD(0x2f);//开启滚动-0x2f，禁用滚动-0x2e，禁用需要重写数据
}

/**
 * @function: void OLED_Some_HorizontalShift(u8 direction,u8 start,u8 end)
 * @description: 屏幕部分内容水平滚动播放
 * @param {u8} direction			LEFT	   0x27     	RIGHT  0x26
 * @param {u8} start 开始页地址  0x00~0x07
 * @param {u8} end  结束页地址  0x01~0x07
 * @return {*}
 */
void OLED_Some_HorizontalShift(u8 direction,u8 start,u8 end)
{
	OLED_WR_CMD(0x2e);//停止滚动
	OLED_WR_CMD(direction);//设置滚动方向
	OLED_WR_CMD(0x00);//虚拟字节设置，默认为0x00
	OLED_WR_CMD(start);//设置开始页地址
	OLED_WR_CMD(0x07);//设置每个滚动步骤之间的时间间隔的帧频,0x07即滚动速度2帧
	OLED_WR_CMD(end);//设置结束页地址
	OLED_WR_CMD(0x00);//虚拟字节设置，默认为0x00
	OLED_WR_CMD(0xff);//虚拟字节设置，默认为0xff
	OLED_WR_CMD(0x2f);//开启滚动-0x2f，禁用滚动-0x2e，禁用需要重写数据

}

/**
 * @function: void OLED_VerticalAndHorizontalShift(u8 direction)
 * @description: 屏幕内容垂直水平全屏滚动播放
 * @param {u8} direction				右上滚动	 0x29
 *                                                            左上滚动   0x2A
 * @return {*}
 */
void OLED_VerticalAndHorizontalShift(u8 direction)
{
	OLED_WR_CMD(0x2e);//停止滚动
	OLED_WR_CMD(direction);//设置滚动方向
	OLED_WR_CMD(0x01);//虚拟字节设置
	OLED_WR_CMD(0x00);//设置开始页地址
	OLED_WR_CMD(0x07);//设置每个滚动步骤之间的时间间隔的帧频，即滚动速度
	OLED_WR_CMD(0x07);//设置结束页地址
	OLED_WR_CMD(0x01);//垂直滚动偏移量
	OLED_WR_CMD(0x00);//虚拟字节设置，默认为0x00
	OLED_WR_CMD(0xff);//虚拟字节设置，默认为0xff
	OLED_WR_CMD(0x2f);//开启滚动-0x2f，禁用滚动-0x2e，禁用需要重写数据
}

/**
 * @function: void OLED_DisplayMode(u8 mode)
 * @description: 屏幕内容取反显示
 * @param {u8} direction			ON	0xA7  ，
 *                                                          OFF	0xA6	默认此模式，设置像素点亮
 * @return {*}
 */
void OLED_DisplayMode(u8 mode)
{
	OLED_WR_CMD(mode);
}

/**
 * @function: void OLED_IntensityControl(u8 intensity)
 * @description: 屏幕亮度调节
 * @param  {u8} intensity	0x00~0xFF,RESET=0x7F
 * @return {*}
 */
void OLED_IntensityControl(u8 intensity)
{
	OLED_WR_CMD(0x81);
	OLED_WR_CMD(intensity);
}






/**
 * 使用案例
 * 
 */
// //oled的初始化
//   OLED_Init();                           //OLED初始
//  OLED_Clear();                         //清屏
// //上面的初始化以及清屏的代码在一开始处一定要写
//  OLED_ShowString(0,0,"Jgcoder",16, 1);    //反相显示8X16字符串
//  OLED_ShowString(0,2,"unicorn_li_123",12,0);//正相显示6X8字符串
 
//  OLED_ShowCHinese(0,4,0,1); //反相显示汉字“独”
//  OLED_ShowCHinese(16,4,1,1);//反相显示汉字“角”
//  OLED_ShowCHinese(32,4,2,1);//反相显示汉字“兽”
//  OLED_ShowCHinese(0,6,0,0); //正相显示汉字“独”
//  OLED_ShowCHinese(16,6,1,0);//正相显示汉字“角”
//  OLED_ShowCHinese(32,6,2,0);//正相显示汉字“兽”

//  OLED_ShowNum(48,4,6,1,16, 0);//正相显示1位8X16数字“6”
//  OLED_ShowNum(48,7,77,2,12, 1);//反相显示2位6X8数字“77”
//  OLED_DrawBMP(90,0,122, 4,BMP1,0);//正相显示图片BMP1
//  OLED_DrawBMP(90,4,122, 8,BMP1,1);//反相显示图片BMP1
// 
//  OLED_HorizontalShift(0x26);//全屏水平向右滚动播放












