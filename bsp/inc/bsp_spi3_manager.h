/*
*********************************************************************************************************
*
*	模块名称 : SPI3外设管理
*	文件名称 : bsp_spi3_manager.h
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
#ifndef __BSP_SPI3_MANAGER_H
#define __BSP_SPI3_MANAGER_H

/* SPI控制外设枚举 */
typedef enum {
    CONTROL_LCD     	= 0, // LCD屏幕
		CONTROL_ECAT      = 1, // ECAT ESC
} tSPICtrl;

/* 全局变量 */
extern volatile int32_t SPI3_Ctrl;

/* 提供给其他C文件调用的函数 */
void bsp_spi3_manager_init(void);
void bsp_spi3_Lcd_Init(void);
void bsp_spi3_ECAT_Init(void);

#endif
