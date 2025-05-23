/*******************************************************************************
*                                 AWorks
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-present Guangzhou ZHIYUAN Electronics Co., Ltd.
* ALL rights reserved.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
*
* The License of this software follows LGPL v2.1, See the LICENSE for more details:
* https://opensource.org/licenses/LGPL-2.1
*
* Contact information:
* web site:    http://www.zlg.cn/
*******************************************************************************/

#ifndef _TIMER_PORT_H_
#define _TIMER_PORT_H_

#include "tim.h"
#include "ecat_def.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __GLOBAL_ECAT_TIMER_VAR__
#define PROTO
#else
#define PROTO extern
#endif

PROTO uint16_t __g_ecat_timer_cnt;

#undef PROTO

#define aw_ecat_timer_inc_p_ms  1u

#if ECAT_TIMER_INT
#define aw_ecat_timer_intr_enable() HAL_NVIC_EnableIRQ(TIM2_IRQn)

#define aw_ecat_timer_intr_disable() HAL_NVIC_DisableIRQ(TIM2_IRQn)
#else
#define aw_ecat_timer_intr_enable()

#define aw_ecat_timer_intr_disable()
#endif

#define aw_ecat_timer_get() (__g_ecat_timer_cnt)

#define aw_ecat_timer_clear()   \
            do                  \
            {                   \
                __g_ecat_timer_cnt = 0;\
            } while (0)

#define aw_ecat_timer_start() HAL_TIM_Base_Start_IT(&htim2)

#ifdef __cplusplus
}
#endif

#endif /* _TIMER_PORT_H_ */
