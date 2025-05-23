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

#define __GLOBAL_ECAT_TIMER_VAR__ 1
#include "timer_port.h"
#undef __GLOBAL_ECAT_TIMER_VAR__

void ECAT_TIM_CallBack(void)
{
#if ECAT_TIMER_INT
        extern void ECAT_CheckTimer();
        ECAT_CheckTimer();
#endif
        __g_ecat_timer_cnt += aw_ecat_timer_inc_p_ms;
}
