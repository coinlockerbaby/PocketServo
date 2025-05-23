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

#include "esc_port.h"

void aw_ecat_uc_set_led(uint8_t run_led, uint8_t err_led)
{
    run_led ? HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_SET):
              HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_RESET);

//    err_led ? HAL_GPIO_WritePin(ERR_GPIO_Port, ERR_Pin, GPIO_PIN_RESET):
//              HAL_GPIO_WritePin(ERR_GPIO_Port, ERR_Pin, GPIO_PIN_SET);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
#if AL_EVENT_ENABLED
    if(GPIO_Pin == ECAT_INT_Pin)
    {
        extern void PDI_Isr();
        PDI_Isr();
    }
#endif
		
#if DC_SUPPORTED
    if(GPIO_Pin == SYNC0_Pin)
    {
        extern void Sync0_Isr();
        Sync0_Isr();
    }else if(GPIO_Pin == SYNC1_Pin)
    {
        extern void Sync1_Isr();
        Sync1_Isr();
    }
#endif
}
