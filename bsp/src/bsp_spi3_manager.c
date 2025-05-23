/*
*********************************************************************************************************
*
*	ģ������ : SPI3���豸����
*	�ļ����� : bsp_spi3_manager.c
*	��    �� : V1.0
*	˵    �� : 
*	�޸ļ�¼ :
*		�汾��  ����         ����      ˵��
*		V1.0    2025-03-09  astronG   ��ʽ����
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
	/* ��λSPI */
	if(HAL_SPI_DeInit(&hspi3) != HAL_OK)
	{
		Error_Handler();
	}
	/* ����SPI */
	MX_SPI3_Init();
	
	/* CUBEMX���ɵĳ�ʼ���������Ļ�ģ�û������ʧ��MISO���ŵĴ��룬��Ҫ�ֶ� */
	HAL_GPIO_DeInit(LCD_DC_GPIO_Port, LCD_DC_Pin); 
	/* �ֶ���ʼ��LCD_DC���� */
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = LCD_DC_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LCD_DC_GPIO_Port, &GPIO_InitStruct);
}

void bsp_spi3_ECAT_Init(void)
{
	/* ��λSPI */
	if(HAL_SPI_DeInit(&hspi3) != HAL_OK)
	{
		Error_Handler();
	}
	/* ����SPI */
	MX_SPI3_Init();
}
