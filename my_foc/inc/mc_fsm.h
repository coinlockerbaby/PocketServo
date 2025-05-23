/*
*********************************************************************************************************
*
*	模块名称 : 电机控制状态机
*	文件名称 : mc_fsm.h
*	版    本 : V1.0
*	说    明 : 
*	修改记录 :
*		版本号  日期         作者       说明
*		V1.0    2025-02-06  astronG   正式发布
*
*	Copyright (C), 2018-2030, astronG
*
*********************************************************************************************************
*/

#ifndef _MC_FSM_H_
#define _MC_FSM_H_

// Motor controler state
typedef enum eFSMState {
	BOOT_UP     = 0,
	IDLE        = 1,
	RUN         = 2,
	M_ERR       = 3
} tFSMState;

/* 提供给其他C文件调用的函数 */
void MCT_init(void);
tFSMState MCT_get_state(void);
int MCT_set_state(tFSMState state);
void MCT_high_frequency_task(void);

#endif

/************************************ astronG (END OF FILE) ************************************/
