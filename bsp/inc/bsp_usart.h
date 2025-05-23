/*
*********************************************************************************************************
*
*	模块名称 : USART调试串口驱动模块
*	文件名称 : bsp_usart.h
*	版    本 : V1.0
*	说    明 : 调试串口重定向
*
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2024-12-06 astronG  正式发布
*
*	Copyright (C), 2015-2030, astronG
*
*********************************************************************************************************
*/

#ifndef _BSP_USART_H_
#define _BSP_USART_H_

#define CH_COUNT 10
struct Frame
{
	float fdata[CH_COUNT];
	unsigned char tail[4];
};
extern volatile struct Frame vofaFrame;
extern SemaphoreHandle_t xTxSemaphore;  // 用于发送完成的信号量
extern SemaphoreHandle_t xUartMutex;    // 用于保护串口资源的互斥锁
/* 供外部调用的函数声明 */
void debug_print(const char *const fmt, ...);
void vofa_print(void);

#endif

/************************************ astronG (END OF FILE) ************************************/
