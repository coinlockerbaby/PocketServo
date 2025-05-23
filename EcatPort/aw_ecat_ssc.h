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
#ifndef __AW_ECAT_SSC_H
#define __AW_ECAT_SSC_H

#include <stdlib.h>
#include "beckhoff_drv.h"
#include "timer_port.h"
#include "esc_port.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ESC_RD       0x02
#define ESC_WR       0x04

/**< \brief Returns the first 16bit of the AL Event register(0x220)*/
#define HW_GetALEventRegister()                   aw_ecat_esc_get_alevent_reg()
#define HW_GetALEventRegister_Isr()               aw_ecat_esc_get_alevent_reg_isr()

/**< \brief Generic ESC (register and DPRAM) read access.*/
#define HW_EscRead(pData, Address, Len)           aw_ecat_esc_read(pData, Address, Len) 
#define HW_EscReadIsr(pData, Address, Len)        aw_ecat_esc_read_isr(pData, Address, Len) 
/**< \brief Generic ESC (register and DPRAM) write access.*/
#define HW_EscWrite(pData, Address, Len)          aw_ecat_esc_write(pData, Address, Len)
#define HW_EscWriteIsr(pData, Address, Len)       aw_ecat_esc_write_isr(pData, Address, Len)

/**< \brief 16Bit specific ESC (register and DPRAM) read access.*/
#define HW_EscReadWord(WordValue, Address)        aw_ecat_esc_read((void*)(&WordValue), Address, 2)
#define HW_EscReadWordIsr(WordValue, Address)     aw_ecat_esc_read_isr((void*)(&WordValue), Address, 2)
/**< \brief 16Bit specific ESC (register and DPRAM) write access.*/
#define HW_EscWriteWord(WordValue, Address)       aw_ecat_esc_write((void*)(&WordValue), Address, 2)
#define HW_EscWriteWordIsr(WordValue, Address)    aw_ecat_esc_write_isr((void*)(&WordValue), Address, 2)

/**< \brief 32Bit specific ESC (register and DPRAM) read access.*/
#define HW_EscReadDWord(DWordValue, Address)      aw_ecat_esc_read((void*)(&DWordValue), Address, 4)
#define HW_EscReadDWordIsr(DWordValue, Address)   aw_ecat_esc_read_isr((void*)(&DWordValue), Address, 4)
/**< \brief 32Bit specific ESC (register and DPRAM) write access.*/
#define HW_EscWriteDWord(DWordValue, Address)     aw_ecat_esc_write((void*)(&WordValue), Address, 4)
#define HW_EscWriteDWordIsr(DWordValue, Address)  aw_ecat_esc_write_isr((void*)(&WordValue), Address, 4)

/**< \brief Macro to copy data from the application mailbox memory(not the ESC memory).*/
#define HW_EscReadMbxMem(pData, Address, Len)     aw_ecat_esc_read(pData, Address, Len) 
/**< \brief Macro to copy data from the application mailbox memory (not the ESC memory).*/
#define HW_EscWriteMbxMem(pData, Address, Len)    aw_ecat_esc_write(pData, Address, Len)

/**< \brief disabled all ecat interrupt(PDI,SYNCx,Timer)*/
#define ENABLE_ESC_INT()                    \
        do                                  \
        {                                   \
            aw_ecat_esc_pdi_intr_enable();  \
            aw_ecat_esc_syncx_intr_enable();\
            aw_ecat_timer_intr_enable();    \
        }while(0)

#define DISABLE_ESC_INT()                    \
        do                                   \
        {                                    \
            aw_ecat_esc_pdi_intr_disable();  \
            aw_ecat_esc_syncx_intr_disable();\
            aw_ecat_timer_intr_disable();    \
        }while(0)

/**< \brief Stack control led, to display the slaver's status*/
#define HW_SetLed(RunLed, ErrLed) aw_ecat_uc_set_led(RunLed, ErrLed)

/**< \brief get or clear timer tick*/
#define HW_GetTimer()       aw_ecat_timer_get()
#define HW_ClearTimer()     aw_ecat_timer_clear()       

#define ECAT_TIMER_INC_P_MS aw_ecat_timer_inc_p_ms

#define aw_ecat_sys_malloc  malloc
#define aw_ecat_sys_free    free

#define aw_ecat_sys_memcpy  memcpy
#define aw_ecat_sys_memset  memset

#ifdef __cplusplus
}
#endif

#endif


