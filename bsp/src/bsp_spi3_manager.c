/*
*********************************************************************************************************
*
*	模块名称 : SPI3从设备管理
*	文件名称 : bsp_spi3_manager.c
*	版    本 : V1.0
*	说    明 : 
*	修改记录 :
*		版本号  日期         作者      说明
*		V1.0    2025-03-09  astronG   正式发布
*
*	Copyright (C), 2020-2030, astronG
*
*********************************************************************************************************
*/
#include "bsp.h"
#include "spi.h"

volatile int32_t SPI3_Ctrl;
void bsp_spi3_manager_init(void)
{
	SPI3_Ctrl = CONTROL_LCD;

}

void bsp_spi3_Lcd_Init(void)
{
	/* 复位SPI */
	if(HAL_SPI_DeInit(&hspi3) != HAL_OK)
	{
		Error_Handler();
	}
	/* 设置SPI */
	MX_SPI3_Init();
	
	/* CUBEMX生成的初始化是针对屏幕的，没有生成失能MISO引脚的代码，需要手动 */
	HAL_GPIO_DeInit(LCD_DC_GPIO_Port, LCD_DC_Pin); 
	/* 手动初始化LCD_DC引脚 */
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = LCD_DC_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LCD_DC_GPIO_Port, &GPIO_InitStruct);
}

void bsp_spi3_ECAT_Init(void)
{
	/* 复位SPI */
	if(HAL_SPI_DeInit(&hspi3) != HAL_OK)
	{
		Error_Handler();
	}
	/* 设置SPI */
	MX_SPI3_Init();
}
