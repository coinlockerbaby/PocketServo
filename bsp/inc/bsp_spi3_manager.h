/*
*********************************************************************************************************
*
*	ģ������ : SPI3�������
*	�ļ����� : bsp_spi3_manager.h
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
#ifndef __BSP_SPI3_MANAGER_H
#define __BSP_SPI3_MANAGER_H

/* SPI��������ö�� */
typedef enum {
    CONTROL_LCD     	= 0, // LCD��Ļ
		CONTROL_ECAT      = 1, // ECAT ESC
} tSPICtrl;

/* ȫ�ֱ��� */
extern volatile int32_t SPI3_Ctrl;

/* �ṩ������C�ļ����õĺ��� */
void bsp_spi3_manager_init(void);
void bsp_spi3_Lcd_Init(void);
void bsp_spi3_ECAT_Init(void);

#endif
