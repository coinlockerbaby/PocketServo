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
#include "bsp.h"
#include "aw_ecat_ssc.h"

#define __ESC_AL_EVENTMASK_OFFSET (0x0204U)

uint16_t HW_Init(void){

    uint32_t intMask;

    do{
        intMask = 0x93;
#if ESC_32BIT_ACCESS
        HW_EscWriteDWord(intMask, __ESC_AL_EVENTMASK_OFFSET);
#else
        HW_EscWrite(&intMask, __ESC_AL_EVENTMASK_OFFSET, sizeof(intMask));
#endif /* ESC_32BIT_ACCESS */
        intMask = 0;
#if ESC_32BIT_ACCESS
        HW_EscReadDWord(intMask, __ESC_AL_EVENTMASK_OFFSET);
#else
        HW_EscRead(&intMask, __ESC_AL_EVENTMASK_OFFSET, sizeof(intMask));
#endif /* ESC_32BIT_ACCESS */
    }while(intMask != 0x93);

    intMask = 0;

#if ESC_32BIT_ACCESS
    HW_EscWriteDWord(intMask, __ESC_AL_EVENTMASK_OFFSET);
#else /* ESC_32BIT_ACCESS */
    HW_EscWrite(&intMask, __ESC_AL_EVENTMASK_OFFSET, sizeof(intMask));
#endif

    aw_ecat_timer_start();

    return 0;
}

void ecat_Init(void)
{
    HW_Init();
    extern void MainInit();
    MainInit();
    extern BOOL bRunApplication;
    bRunApplication = TRUE;
    
    // do
    // {
    //     extern void MainLoop();
    //     MainLoop();
    // } while (bRunApplication == TRUE);
}


void ecat_main(void)
{
    extern BOOL bRunApplication;
    if (bRunApplication == TRUE)
    {
        extern void MainLoop();
        MainLoop();
    }
}
