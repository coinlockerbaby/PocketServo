/*
*********************************************************************************************************
*
*	模块名称 : my_foc项目管理文件
*	文件名称 : foc.h
*	版    本 : V1.0
*	说    明 : 这是my_foc项目的总头文件
*	修改记录 :
*		版本号  日期         作者       说明
*		V1.0    2025-02-06  astronG   正式发布
*
*	Copyright (C), 2018-2030, astronG
*
*********************************************************************************************************
*/

#ifndef _MY_FOC_H_
#define _MY_FOC_H_

#define MYFOC_SOFTWARE_VERSION               "V1.0.0"


/* 通过取消注释或者添加注释的方式控制是否包含底层驱动模块 */
#include "bsp.h"

/* 通过取消注释或者添加注释的方式控制是否包含FOC功能模块 */
#include "usr_config.h"
#include "pid.h"
#include "foc.h"
#include "mc_fsm.h"
#include "pwm_curr.h"
#include "encoder.h"
#include "calibration.h"

/* 提供给其他C文件调用的函数 */
void MyFoc_init(void);		//初始化my_foc项目
void USR_CONFIG_print_config(void);

#endif

/************************************ astronG (END OF FILE) ************************************/
