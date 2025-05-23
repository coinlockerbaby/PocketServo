/*
*********************************************************************************************************
*
*	模块名称 : ST7789 屏幕
*	文件名称 : bsp_spi_st7789.h
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
#ifndef __BSP_SPI_ST7789_H
#define __BSP_SPI_ST7789_H


/* SPI LCD屏幕的复位(RES)， PA15 */
#define LCD_RES_Clr()					LCD_RES_GPIO_Port->BSRR = ((uint32_t)LCD_RES_Pin << 16U)
#define LCD_RES_Set()					LCD_RES_GPIO_Port->BSRR = LCD_RES_Pin
/* SPI LCD屏幕的命令(DC)， PC11 */
#define LCD_DC_Clr()   				LCD_DC_GPIO_Port->BSRR = ((uint32_t)LCD_DC_Pin << 16U)
#define LCD_DC_Set()          LCD_DC_GPIO_Port->BSRR = LCD_DC_Pin
/* SPI LCD屏幕的片选(CS)， PB6 */
#define LCD_CS_Clr()   				LCD_CS_GPIO_Port->BSRR = ((uint32_t)LCD_CS_Pin << 16U)
#define LCD_CS_Set()   				LCD_CS_GPIO_Port->BSRR = LCD_CS_Pin
/* SPI LCD屏幕的背光(BLK)， PB7 */
#define LCD_BLK_Clr()  				LCD_BLK_GPIO_Port->BSRR = ((uint32_t)LCD_BLK_Pin << 16U)
#define LCD_BLK_Set()  				LCD_BLK_GPIO_Port->BSRR = LCD_BLK_Pin

/* 提供给其他C文件调用的函数 */
void LCD_Writ_Bus(uint8_t dat);
void LCD_WR_DATA8(uint8_t dat);
void LCD_WR_DATA(uint16_t dat);
void LCD_WR_REG(uint8_t dat);
void LCD_Address_Set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void LCD_Init(void);
#endif
