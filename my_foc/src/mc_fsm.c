/*
*********************************************************************************************************
*
*	模块名称 : 电机控制状态机
*	文件名称 : mc_fsm.c
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

#include "my_foc.h"
#include "utils.h"

typedef struct sFSM
{
	tFSMState state;
	tFSMState state_next;
	uint8_t state_next_ready;
} tFSM;

static volatile tFSM mFSM;

/*
*********************************************************************************************************
*	函 数 名: MCT_init
*	功能说明: 初始化状态机
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void MCT_init(void)
{
	mFSM.state = BOOT_UP;
	mFSM.state_next = BOOT_UP;
	mFSM.state_next_ready = 0;
}

/*
*********************************************************************************************************
*	函 数 名: MCT_get_state
*	功能说明: 获取状态机当前状态
*	形    参: 无
*	返 回 值: 当前状态
*********************************************************************************************************
*/
tFSMState MCT_get_state(void)
{
	return mFSM.state;
}

/*
*********************************************************************************************************
*	函 数 名: MCT_set_state
*	功能说明: 设置状态机当前状态
*	形    参: state - 要设置的状态
*	返 回 值: 0 - 设置成功，-1 - 设置失败
*********************************************************************************************************
*/
int MCT_set_state(tFSMState state)
{
	int ret;
	switch (state)
	{
		case BOOT_UP:
			mFSM.state_next = BOOT_UP;
			break;
		case IDLE:
			mFSM.state_next = IDLE;
			break;
		case RUN:
			mFSM.state_next = RUN;
			break;
		case M_ERR:
			mFSM.state_next = M_ERR;
			break;
		default:
			break;
	}

	mFSM.state_next_ready = 0;
	return ret;
}

/*
*********************************************************************************************************
*	函 数 名: exit_state
*	功能说明: 退出状态
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void exit_state(void)
{
	/* exit state */
	switch (mFSM.state)
	{
		case BOOT_UP:
				mFSM.state_next_ready = 1;
				break;

		case IDLE:
				mFSM.state_next_ready = 1;
				break;

		case RUN:
				mFSM.state_next_ready = 1;
				break;

		case M_ERR:
				mFSM.state_next_ready = 1;
				break;
		
		default:
				break;
	}
}

/*
*********************************************************************************************************
*	函 数 名: enter_state
*	功能说明: 进入状态
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void enter_state(void)
{
	/* enter state */
	switch (mFSM.state)
	{
		case BOOT_UP:
				break;

		case IDLE:
				break;

		case RUN:
				PWM_SwitchOnPWM();
				Foc.is_armed = TRUE;
				break;
		case M_ERR:
				PWM_SwitchOffPWM();
				Foc.is_armed = FALSE;
				break;
		default:
				break;
	}
}

/*
*********************************************************************************************************
*	函 数 名: CONTROLLER_loop
*	功能说明: 控制循环
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void CONTROLLER_loop(void)
{
	/* 初始控制状态为 速度-电流 */
	static int32_t control_mode_last = 0;
	if (control_mode_last == 0)
		control_mode_last = UsrConfig.control_mode; /* 设置初始状态 */

	const int8_t velocity_ctl_div = 2; 				 /* 速度-电流环 分频 */
	const int8_t position_ctl_div = 2; 				 /* 位置-电流环 分频 */
	const int8_t position_profile_ctl_div = 4; /* 位置-速度-电流环 分频 */
	static int32_t DivCntCopy = 0;
	/* 控制状态切换，清除积分并设置分频数 */
	if (UsrConfig.control_mode != control_mode_last)
	{
		Foc.pid_i_d.integral_prev = 0.0f;
		Foc.pid_i_q.integral_prev = 0.0f;
		Foc.pid_position.integral_prev = 0.0f;
		Foc.pid_velocity.integral_prev = 0.0f;
		Foc.pid_position_profile.integral_prev = 0.0f;
		Foc.target_i_d = 0.0f;
		Foc.target_i_q = 0.0f;
		Foc.target_velocity = 0.0f;
		Foc.target_position = Foc.position;
		
		control_mode_last = UsrConfig.control_mode;
//		switch (control_mode_last)
//		{
//			case CONTROL_MODE_VELOCITY_RAMP:
//			{
//				DivCntCopy = velocity_ctl_div;
//			}
//			break;
//			case CONTROL_MODE_POSITION_RAMP:
//			{
//				DivCntCopy = position_ctl_div;
//			}
//			break;
//			case CONTROL_MODE_POSITION_PROFILE:
//			{
//				DivCntCopy = position_profile_ctl_div;
//			}
//			break;
//			default:
//				break;
//		}
	}

	/* 根据控制模式执行控制 */
	switch (control_mode_last)
	{
		case CONTROL_MODE_VELOCITY_RAMP:
		{
			if (--DivCntCopy <= 0)
			{
				/* do something */
				FOC_Velocity(Foc.target_velocity);
				DivCntCopy = velocity_ctl_div;
			}
		}
		break;
		case CONTROL_MODE_POSITION_RAMP:
		{
		  if (--DivCntCopy <= 0)
			{
				/* do something */
				FOC_Position(Foc.target_position);
				DivCntCopy = position_ctl_div;
			}
		}
		break;
		case CONTROL_MODE_POSITION_PROFILE:
		{
			/* 运行位置环-速度环 */
			static int32_t DivCntCopy2 = 0;
		  if (--DivCntCopy <= 0)
			{
				/* do something */
				FOC_Position_Profile(Foc.target_position);
				DivCntCopy = position_profile_ctl_div;
			}
			if (--DivCntCopy2 <= 0)
			{
				/* do something */
				FOC_Velocity(Foc.target_velocity);
				DivCntCopy2 = velocity_ctl_div;
			}
		}
		break;
		default:
				break;
	}

	FOC_current(Foc.target_i_d, Foc.target_i_q, Encoder_electrical_angle());
}


/*
*********************************************************************************************************
*	函 数 名: MCT_high_frequency_task
*	功能说明: 高频任务
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void MCT_high_frequency_task(void)
{
	/* 状态机状态切换 */
	if (mFSM.state_next != mFSM.state)
	{
		exit_state();
		if (mFSM.state_next_ready)
		{
			mFSM.state = mFSM.state_next;
			enter_state();
		}
	}

	/* 获取电流, 取流入绕组电流方向为正方向 */
	Foc.i_a = -read_iphase_a();
	Foc.i_b = -read_iphase_b();
	Foc.i_c = -read_iphase_c();

	/* 获取编码器 */
	Encoder_update(); /* 读取编码器数据 */
	Foc.position = Encoder_getAngle();
	Foc.velocity = Encoder_getVelocity();
	
	/* 根据当前状态运行对应操作 */
	switch (mFSM.state)
	{
		case BOOT_UP:
				if (UsrConfig.is_calibrated)
				{
					MCT_set_state(RUN);
				}
				else
				{
					MCT_set_state(M_ERR);
				}
				break;

		case IDLE:
				__NOP();
				break;

		case RUN:
				CONTROLLER_loop();
				
				break;

		case M_ERR:
				break;
		default:
				break;
	}
}
