/*
*********************************************************************************************************
*
*	ģ������ : ST7789 ��Ļ
*	�ļ����� : bsp_spi_st7789.h
*	��    �� : V1.0
*	˵    �� : 
*	�޸ļ�¼ :
*		�汾��  ����         ����      ˵��
*		V1.0    2025-03-08  astronG   ��ʽ����
*
*	Copyright (C), 2020-2030, astronG
*
*********************************************************************************************************
*/
#ifndef __BSP_SPI_ST7789_H
#define __BSP_SPI_ST7789_H


/* SPI LCD��Ļ�ĸ�λ(RES)�� PA15 */
#define LCD_RES_Clr()					LCD_RES_GPIO_Port->BSRR = ((uint32_t)LCD_RES_Pin << 16U)
#define LCD_RES_Set()					LCD_RES_GPIO_Port->BSRR = LCD_RES_Pin
/* SPI LCD��Ļ������(DC)�� PC11 */
#define LCD_DC_Clr()   				LCD_DC_GPIO_Port->BSRR = ((uint32_t)LCD_DC_Pin << 16U)
#define LCD_DC_Set()          LCD_DC_GPIO_Port->BSRR = LCD_DC_Pin
/* SPI LCD��Ļ��Ƭѡ(CS)�� PB6 */
#define LCD_CS_Clr()   				LCD_CS_GPIO_Port->BSRR = ((uint32_t)LCD_CS_Pin << 16U)
#define LCD_CS_Set()   				LCD_CS_GPIO_Port->BSRR = LCD_CS_Pin
/* SPI LCD��Ļ�ı���(BLK)�� PB7 */
#define LCD_BLK_Clr()  				LCD_BLK_GPIO_Port->BSRR = ((uint32_t)LCD_BLK_Pin << 16U)
#define LCD_BLK_Set()  				LCD_BLK_GPIO_Port->BSRR = LCD_BLK_Pin

/* �ṩ������C�ļ����õĺ��� */
void LCD_Writ_Bus(uint8_t dat);
void LCD_WR_DATA8(uint8_t dat);
void LCD_WR_DATA(uint16_t dat);
void LCD_WR_REG(uint8_t dat);
void LCD_Address_Set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void LCD_Init(void);
#endif
