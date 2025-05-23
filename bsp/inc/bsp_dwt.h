/*
*********************************************************************************************************
*
*	模块名称 : DWT内核定时器 模块
*	文件名称 : bsp_dwt.h
*	版    本 : V1.0
*	说    明 : 
*	修改记录 :
*		版本号  日期         作者       说明
*		V1.0    2025-02-08  astronG   正式发布
*
*	Copyright (C), 2018-2030, astronG
*
*********************************************************************************************************
*/

#ifndef __BSP_DWT_H
#define __BSP_DWT_H

static inline void bsp_dwt_start(void)
{
	/* 使能DWT外设 */
	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;

	/* DWT CYCCNT寄存器计数清0 */
	DWT->CYCCNT = (uint32_t)0u;

	/* 使能Cortex-M DWT CYCCNT寄存器 */
	DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

static inline uint32_t bsp_dwt_read(void)
{
	/* 返回DWT CYCCNT寄存器的值 */
	return DWT->CYCCNT;
}

static inline void bsp_DelayUS(uint32_t _ulDelayTime)
{
    uint32_t tCnt, tDelayCnt;
    uint32_t tStart;
         
    tStart = DWT->CYCCNT;                                     /* 刚进入时的计数器值 */
    tCnt = 0;
    tDelayCnt = _ulDelayTime * (SystemCoreClock / 1000000);  /* 需要的节拍数 */            
 
    while(tCnt < tDelayCnt)
    {
        tCnt = DWT->CYCCNT - tStart; /* 求减过程中，如果发生第一次32位计数器重新计数，依然可以正确计算 */ 
    }
}

#endif
/************************************ astronG (END OF FILE) ************************************/
