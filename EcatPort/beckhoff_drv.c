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
#include "beckhoff_drv.h"
#include "esc_port.h"

typedef union
{
    uint8_t  byte[2];
    uint16_t word;
}alevent;

#define __ESC_RD                     (0x02)
#define __ESC_RWS                    (0x03U)
#define __ESC_WR                     (0x04U)
#define __ESC_ALEVENT_OFFSET         (0x220U)


static void __handle_setup_phase(uint16_t address, uint8_t command)
{
#define __ADDR_MODE_BYTE_MAX (4U)

    uint8_t tx_data[__ADDR_MODE_BYTE_MAX] = {0, 0, 0, 0}, tx_len = 0;

    tx_data[0] = address >> 5;
    tx_len++;
    /**
     * \brief check address is 2byte or 3byte mode
    */
    if((address + 8) & ~0x1fff)
    {
        tx_data[1] = (address << 3) + 0x06;
        tx_data[2] = ((address >> 8) & 0x0e) + (command << 2);
        tx_len += 2;
    }
    else
    {
        tx_data[1] = (address << 3) | command;
        tx_len++;
    }
    /**
     * \brief add wait state byte
    */
    if(__ESC_RWS == command)
    {
        tx_data[tx_len] = 0xff;
        tx_len++;
    }
    __ECAT_SPI_WRITE(tx_data, tx_len);
}

void aw_ecat_esc_read(void *p_data, uint16_t address, uint16_t len)
{
    uint8_t tx_data = 0xFF;
		
    while(len-- > 0)
    {
        __ECAT_DISABLE_ALL_IRQ();
        __ECAT_SPI_CS_SELECT();
        /**
         * \brief send address and cmd
        */
        __handle_setup_phase(address, 
                            __ESC_RWS
                            );
        /**
         * \brief read data one by one,
         * it can be interrupted by PDI_Isr/Syncx_Isr
         * for realtime
        */
        __ECAT_SPI_WRITE_AND_READ(&tx_data, p_data, 1);
        __ECAT_SPI_CS_DESELECT();
        __ECAT_ENABLE_ALL_IRQ();
        p_data = ((uint8_t*)p_data) + 1;
        address++;
    }
}

void aw_ecat_esc_read_isr(void *p_data, uint16_t address, uint16_t len)
{
    uint8_t tx_data = 0x00U;
    __ECAT_DISABLE_ALL_IRQ();
    __ECAT_SPI_CS_SELECT();
    /**
     * \brief send address and cmd
    */
    __handle_setup_phase(address,
                        __ESC_RWS
                        );
    /**
     * \brief read data
    */
    while(len-- > 1)
    {
        __ECAT_SPI_WRITE_AND_READ(&tx_data, p_data, 1);
        p_data = ((uint8_t*)p_data) + 1;
    }
    /**
     * \brief send terminate data
    */
    tx_data = 0xFFU;
    __ECAT_SPI_WRITE_AND_READ(&tx_data, (uint8_t*)p_data, 1);
    __ECAT_SPI_CS_DESELECT();
    __ECAT_ENABLE_ALL_IRQ();
}

void aw_ecat_esc_write(void *p_data, uint16_t address, uint16_t len)
{
    while(len-- > 0)
    {
        __ECAT_DISABLE_ALL_IRQ();
        __ECAT_SPI_CS_SELECT();
        /**
         * \brief send address and cmd
        */
        __handle_setup_phase(address, __ESC_WR);
        __ECAT_SPI_WRITE(p_data, 1);
        __ECAT_SPI_CS_DESELECT();
        __ECAT_ENABLE_ALL_IRQ();
        p_data = ((uint8_t*)p_data) + 1;
        address++;
    }
}

void aw_ecat_esc_write_isr(void *p_data, uint16_t address, uint16_t len)
{
    __ECAT_DISABLE_ALL_IRQ();
    __ECAT_SPI_CS_SELECT();
    /**
     * \brief send address and cmd
    */
    __handle_setup_phase(address, __ESC_WR);
    __ECAT_SPI_WRITE(p_data, len);
    __ECAT_SPI_CS_DESELECT();
    __ECAT_ENABLE_ALL_IRQ();
}
/* aw_ecat_esc function define */
uint16_t aw_ecat_esc_get_alevent_reg(void)
{
    alevent esc_alevent_reg;
    aw_ecat_esc_read((void*)(&(esc_alevent_reg.word)),
                        __ESC_ALEVENT_OFFSET,
                        sizeof(alevent));
    return esc_alevent_reg.word;
}

uint16_t aw_ecat_esc_get_alevent_reg_isr(void)
{
    alevent esc_alevent_reg;
    aw_ecat_esc_read_isr((void*)(&(esc_alevent_reg.word)),
                        __ESC_ALEVENT_OFFSET,
                        sizeof(alevent));
    return esc_alevent_reg.word;
}
