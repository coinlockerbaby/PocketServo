/*
*********************************************************************************************************
*
*	模块名称 : ST7789 屏幕
*	文件名称 : bsp_spi_st7789.c
*	版    本 : V1.0
*	说    明 : 
*	修改记录 :
*		版本号  日期         作者      说明
*		V1.0    2025-03-08  astronG   正式发布
*
*	Copyright (C), 2020-2030, astronG
*
*********************************************************************************************************
*/
#include "bsp.h"
#include "spi.h"


/* 设置横屏或者竖屏显示 0或1为竖屏 2或3为横屏 */
#define USE_HORIZONTAL 3


#if USE_HORIZONTAL==0||USE_HORIZONTAL==1
#define LCD_W 135
#define LCD_H 240

#else
#define LCD_W 240
#define LCD_H 135
#endif

/*
*********************************************************************************************************
*	函 数 名: Delay_ms
*	功能说明: 粗略延时函数
*	形    参: ms 延时的时间，单位是毫秒
*	返 回 值: 无
*********************************************************************************************************
*/
static void Delay_ms(uint32_t ms)
{
  uint32_t i, j;
  for (i = ms; i > 0; i--)
  {
    for (j = 0; j < 1000; j++)
    {
        // 空循环，消耗时间
    }
  }
}

/*
*********************************************************************************************************
*	函 数 名: LCD_Writ_Bus
*	功能说明: 向LCD写一个字节
*	形    参: dat 要写入的数据
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_Writ_Bus(uint8_t dat)
{	
	LCD_CS_Clr();
  HAL_SPI_Transmit(&hspi3, &dat, 1, HAL_MAX_DELAY);
  LCD_CS_Set();
}

/*
*********************************************************************************************************
*	函 数 名: LCD_WR_DATA8
*	功能说明: 向LCD写一个字节
*	形    参: dat 要写入的数据
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_WR_DATA8(uint8_t dat)
{
	LCD_Writ_Bus(dat);
}

/*
*********************************************************************************************************
*	函 数 名: LCD_WR_DATA
*	功能说明: 向LCD写两个字节
*	形    参: dat 要写入的数据
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_WR_DATA(uint16_t dat)
{
	LCD_Writ_Bus(dat>>8);
	LCD_Writ_Bus(dat);
}

/*
*********************************************************************************************************
*	函 数 名: LCD_WR_REG
*	功能说明: 向LCD写一个命令
*	形    参: dat 命令
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_WR_REG(uint8_t dat)
{
	LCD_DC_Clr();
	LCD_Writ_Bus(dat);
	LCD_DC_Set();
}

/*
*********************************************************************************************************
*	函 数 名: LCD_Address_Set
*	功能说明: 设置光标起始位置
*	形    参: x1,y1 起始坐标
*			 x2,y2 终止坐标
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_Address_Set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	// 判断液晶屏的横屏模式
	if(USE_HORIZONTAL == 0)
	{
		// 设置列地址
		LCD_WR_REG(0x2a); // 发送列地址设置命令
		LCD_WR_DATA(x1 + 52); // 发送起始列地址
		LCD_WR_DATA(x2 + 52); // 发送结束列地址

		// 设置行地址
		LCD_WR_REG(0x2b); // 发送行地址设置命令
		LCD_WR_DATA(y1 + 40); // 发送起始行地址
		LCD_WR_DATA(y2 + 40); // 发送结束行地址

		// 设置写入数据命令
		LCD_WR_REG(0x2c); // 发送写入数据命令
	}
	else if(USE_HORIZONTAL == 1)
	{
		// 设置列地址
		LCD_WR_REG(0x2a); // 发送列地址设置命令
		LCD_WR_DATA(x1 + 53); // 发送起始列地址
		LCD_WR_DATA(x2 + 53); // 发送结束列地址

		// 设置行地址
		LCD_WR_REG(0x2b); // 发送行地址设置命令
		LCD_WR_DATA(y1 + 40); // 发送起始行地址
		LCD_WR_DATA(y2 + 40); // 发送结束行地址

		// 设置写入数据命令
		LCD_WR_REG(0x2c); // 发送写入数据命令
	}
	else if(USE_HORIZONTAL == 2)
	{
		// 设置列地址
		LCD_WR_REG(0x2a); // 发送列地址设置命令
		LCD_WR_DATA(x1 + 40); // 发送起始列地址
		LCD_WR_DATA(x2 + 40); // 发送结束列地址

		// 设置行地址
		LCD_WR_REG(0x2b); // 发送行地址设置命令
		LCD_WR_DATA(y1 + 53); // 发送起始行地址
		LCD_WR_DATA(y2 + 53); // 发送结束行地址

		// 设置写入数据命令
		LCD_WR_REG(0x2c); // 发送写入数据命令
	}
	else
	{
		// 设置列地址
		LCD_WR_REG(0x2a); // 发送列地址设置命令
		LCD_WR_DATA(x1 + 40); // 发送起始列地址
		LCD_WR_DATA(x2 + 40); // 发送结束列地址

		// 设置行地址
		LCD_WR_REG(0x2b); // 发送行地址设置命令
		LCD_WR_DATA(y1 + 52); // 发送起始行地址
		LCD_WR_DATA(y2 + 52); // 发送结束行地址

		// 设置写入数据命令
		LCD_WR_REG(0x2c); // 发送写入数据命令
	}
}


/*
*********************************************************************************************************
*	函 数 名: LCD_Init
*	功能说明: 初始化LCD
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_Init(void)
{
	LCD_RES_Clr();
	Delay_ms(100);
	LCD_RES_Set();
	Delay_ms(100);
	
	LCD_BLK_Set();
  Delay_ms(100);
	
	LCD_WR_REG(0x11); 
	Delay_ms(120); 
	LCD_WR_REG(0x36); 
	if(USE_HORIZONTAL==0)LCD_WR_DATA8(0x00);
	else if(USE_HORIZONTAL==1)LCD_WR_DATA8(0xC0);
	else if(USE_HORIZONTAL==2)LCD_WR_DATA8(0x70);
	else LCD_WR_DATA8(0xA0);

	LCD_WR_REG(0x3A);
	LCD_WR_DATA8(0x05);

	LCD_WR_REG(0xB2);
	LCD_WR_DATA8(0x0C);
	LCD_WR_DATA8(0x0C);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x33);
	LCD_WR_DATA8(0x33); 

	LCD_WR_REG(0xB7); 
	LCD_WR_DATA8(0x35);  

	LCD_WR_REG(0xBB);
	LCD_WR_DATA8(0x19);

	LCD_WR_REG(0xC0);
	LCD_WR_DATA8(0x2C);

	LCD_WR_REG(0xC2);
	LCD_WR_DATA8(0x01);

	LCD_WR_REG(0xC3);
	LCD_WR_DATA8(0x12);   

	LCD_WR_REG(0xC4);
	LCD_WR_DATA8(0x20);  

	LCD_WR_REG(0xC6); 
	LCD_WR_DATA8(0x0F);    

	LCD_WR_REG(0xD0); 
	LCD_WR_DATA8(0xA4);
	LCD_WR_DATA8(0xA1);

	LCD_WR_REG(0xE0);
	LCD_WR_DATA8(0xD0);
	LCD_WR_DATA8(0x04);
	LCD_WR_DATA8(0x0D);
	LCD_WR_DATA8(0x11);
	LCD_WR_DATA8(0x13);
	LCD_WR_DATA8(0x2B);
	LCD_WR_DATA8(0x3F);
	LCD_WR_DATA8(0x54);
	LCD_WR_DATA8(0x4C);
	LCD_WR_DATA8(0x18);
	LCD_WR_DATA8(0x0D);
	LCD_WR_DATA8(0x0B);
	LCD_WR_DATA8(0x1F);
	LCD_WR_DATA8(0x23);

	LCD_WR_REG(0xE1);
	LCD_WR_DATA8(0xD0);
	LCD_WR_DATA8(0x04);
	LCD_WR_DATA8(0x0C);
	LCD_WR_DATA8(0x11);
	LCD_WR_DATA8(0x13);
	LCD_WR_DATA8(0x2C);
	LCD_WR_DATA8(0x3F);
	LCD_WR_DATA8(0x44);
	LCD_WR_DATA8(0x51);
	LCD_WR_DATA8(0x2F);
	LCD_WR_DATA8(0x1F);
	LCD_WR_DATA8(0x1F);
	LCD_WR_DATA8(0x20);
	LCD_WR_DATA8(0x23);

	LCD_WR_REG(0x21); 

	LCD_WR_REG(0x29);
	LCD_Fill(0, 0, 240, 320, BLACK);
}
/*********************************** astronG (END OF FILE) **********************************************/
