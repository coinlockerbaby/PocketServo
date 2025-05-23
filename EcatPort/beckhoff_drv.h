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

#ifndef _BECKHOFF_DRV_H_
#define _BECKHOFF_DRV_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

uint16_t aw_ecat_esc_get_alevent_reg(void);
uint16_t aw_ecat_esc_get_alevent_reg_isr(void);
void aw_ecat_esc_read(void *p_data, uint16_t address, uint16_t len);
void aw_ecat_esc_read_isr(void *p_data, uint16_t address, uint16_t len);
void aw_ecat_esc_write(void *p_data, uint16_t address, uint16_t len);
void aw_ecat_esc_write_isr(void *p_data, uint16_t address, uint16_t len);

#ifdef __cplusplus
}
#endif

#endif /* _BECKHOFF_DRV_H_ */
