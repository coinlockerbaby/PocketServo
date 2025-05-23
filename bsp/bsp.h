/*
*********************************************************************************************************
*
*	模块名称 : BSP模块(For STM32FG474)
*	文件名称 : bsp.h
*	版    本 : V1.0
*	说    明 : 这是硬件底层驱动程序的主文件。每个c文件可以 #include "bsp.h" 来包含所有的外设驱动模块。
*			   bsp = Borad surport packet 板级支持包
*	修改记录 :
*		版本号  日期         作者       说明
*		V1.0    2024-12-06  astronG   正式发布
*
*	Copyright (C), 2018-2030, astronG
*
*********************************************************************************************************
*/

#ifndef _BSP_H_
#define _BSP_H_


#define HARDWARE_VERSION               "V1.0.0"
#define SOFTWARE_VERSION               "V1.0.0"

#define USE_FreeRTOS 1	/* 使能FreeRTOS */

/* 包含FreeRTOS头文件 */
#if USE_FreeRTOS == 1
	#include "FreeRTOS.h"
	#include "task.h"
	#include "event_groups.h"
	#include "queue.h"
	#include "semphr.h"
	#define DISABLE_INT()	taskENTER_CRITICAL() /* 禁止全局中断 */
	#define ENABLE_INT()	taskEXIT_CRITICAL()  /* 使能全局中断 */
#else
	#define DISABLE_INT()	__set_PRIMASK(1)	/* 禁止全局中断 */
	#define ENABLE_INT()	__set_PRIMASK(0)	/* 使能全局中断 */
#endif


#include "main.h"

/* 包含标准库头文件 */
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#ifndef TRUE
	#define TRUE  1
#endif

#ifndef FALSE
	#define FALSE 0
#endif

/* 通过取消注释或者添加注释的方式控制是否包含底层驱动模块 */
#include "bsp_led.h"
#include "bsp_usart.h"
#include "bsp_spi_mt6816.h"
#include "bsp_spi_st7789.h"
#include "bsp_dwt.h"
#include "bsp_key.h"
#include "bsp_spi3_manager.h"
/* 通过取消注释或者添加注释的方式控制是否包含第三方软件包 */
#include "chry_ringbuffer.h"
#include "usb_device.h"
#include "shell_port.h"
#include "uartShell.h"
#include "my_foc.h"
#include "lcd.h"
/* 提供给其他C文件调用的函数 */
void bsp_Init(void);
void bsp_Idle(void);
void bsp_RunPer1ms(void);
void bsp_RunPer10ms(void);
void PrintfLogo(void);
#endif

/************************************ astronG (END OF FILE) ************************************/
