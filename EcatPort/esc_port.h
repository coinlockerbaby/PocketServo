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

#ifndef _PORT_H_
#define _PORT_H_

#include "spi.h"
#include "gpio.h"
#include "ecat_def.h"
#include "bsp.h"

#ifdef __cplusplus
extern "C" {
#endif

#define __ECAT_SPI_READ(p_rx_data, rx_len) \
            HAL_SPI_Receive(&hspi3, p_rx_data, rx_len, HAL_MAX_DELAY)

#define __ECAT_SPI_WRITE(p_tx_data, tx_len) \
            HAL_SPI_Transmit(&hspi3, p_tx_data, tx_len, HAL_MAX_DELAY)

#define __ECAT_SPI_WRITE_AND_READ(p_tx_data, p_rx_data, size) \
            HAL_SPI_TransmitReceive(&hspi3, p_tx_data, p_rx_data, size, HAL_MAX_DELAY)

#define __ECAT_SPI_CS_SELECT() \
            HAL_GPIO_WritePin(ECAT_CS_GPIO_Port, ECAT_CS_Pin, GPIO_PIN_RESET)

#define __ECAT_SPI_CS_DESELECT() \
            HAL_GPIO_WritePin(ECAT_CS_GPIO_Port, ECAT_CS_Pin, GPIO_PIN_SET)

#define __ECAT_ENABLE_ALL_IRQ() __set_PRIMASK(0) /* 使能全局中断 */
#define __ECAT_DISABLE_ALL_IRQ() __set_PRIMASK(1) /* 禁止全局中断 */

#if AL_EVENT_ENABLED
#define aw_ecat_esc_pdi_intr_enable()  HAL_NVIC_EnableIRQ(EXTI4_IRQn)

#define aw_ecat_esc_pdi_intr_disable()  HAL_NVIC_DisableIRQ(EXTI4_IRQn)
#else
#define aw_ecat_esc_pdi_intr_enable()

#define aw_ecat_esc_pdi_intr_disable()
#endif

#define aw_ecat_esc_syncx_intr_enable()  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);HAL_NVIC_EnableIRQ(EXTI9_5_IRQn)

#define aw_ecat_esc_syncx_intr_disable()  HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);HAL_NVIC_DisableIRQ(EXTI9_5_IRQn)

void aw_ecat_uc_set_led(uint8_t run_led, uint8_t err_led);

#ifdef __cplusplus
}
#endif

#endif /* _PORT_H_ */
