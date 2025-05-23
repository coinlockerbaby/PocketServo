/*
*********************************************************************************************************
*
*	模块名称 : letter-shell移植
*	文件名称 : shell_port.c
*	版    本 : V1.0
*	说    明 : 
*	修改记录 :
*		版本号  日期         作者       说明
*		V1.0    2025-02-06  astronG   正式发布
*
*	Copyright (C), 2018-2030, astronG
*
*********************************************************************************************************
*/

#include "bsp.h"
#include "usart.h"
#include "shell.h"
#include "shell_port.h"

/* 1. 创建shell对象，开辟shell缓冲区 */
Shell shell;
char shell_tx_buffer[512];
char shell_rx_ch = 0;
uint8_t shell_rx_mempool[512];
chry_ringbuffer_t shell_rx_rb;

/* 2. 自己实现shell写函数 */
signed short User_Shell_Write(char *data, unsigned short len)
{
  // 获取互斥锁，保护串口资源
  xSemaphoreTake(xUartMutex, portMAX_DELAY);
	HAL_UART_Transmit_DMA(&huart3, (uint8_t*)data, len);
	// 等待发送完成信号量
  xSemaphoreTake(xTxSemaphore, portMAX_DELAY);
	// 释放互斥锁
  xSemaphoreGive(xUartMutex);
	return len;
}

signed short User_Shell_Read(char *data, unsigned short len)
{
  return chry_ringbuffer_read(&shell_rx_rb, data, len);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart->Instance == USART3)
	{
    chry_ringbuffer_write(&shell_rx_rb, &shell_rx_ch, 1);
    HAL_UART_Receive_IT(&huart3, (uint8_t *)&shell_rx_ch, 1);
	}
}

/* 3. 编写初始化函数 */
void User_Shell_Init(void)
{
	chry_ringbuffer_init(&shell_rx_rb, shell_rx_mempool, 512);
	//注册自己实现的写函数
  shell.write = User_Shell_Write;
  shell.read = User_Shell_Read;
  //调用shell初始化函数
  shellInit(&shell, shell_tx_buffer, 512);

  HAL_UART_Receive_IT(&huart3, (uint8_t *)&shell_rx_ch, 1);
}
