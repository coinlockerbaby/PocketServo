/*
*********************************************************************************************************
*
*	模块名称 : FOC基础算法
*	文件名称 : foc.c
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
#include "tim.h"
#include "adc.h"
#include "utils.h"

volatile tFOC Foc;

/*
*********************************************************************************************************
*	函 数 名: FOC_init
*	功能说明: 初始化FOC算法
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void FOC_init(void)
{
	Foc.is_armed = FALSE;
	
	Foc.v_bus = 0.0f;
	Foc.v_bus_filt = 0.0f;
	Foc.i_q = 0.0f;
	Foc.i_q_filt = 0.0f;
	Foc.i_d = 0.0f;
	Foc.i_d_filt = 0.0f;
	Foc.i_bus = 0.0f;
	Foc.i_bus_filt = 0.0f;
	Foc.power_filt = 0.0f;

	Foc.target_i_d = 0.0f;
	Foc.target_i_q = 0.0f;
	Foc.target_velocity = 0.0f;
	Foc.target_position = 0.0f;

	pid_init((pid_t *)&Foc.pid_i_d, 0.05f, 25.0f, 0.0f, 100000.0f, 4.0f);
	pid_init((pid_t *)&Foc.pid_i_q, 0.05f, 25.0f, 0.0f, 100000.0f, 4.0f);
	pid_init((pid_t *)&Foc.pid_velocity, 0.05f, 0.01f, 0.0f, 100000.0f, 2.0f);
	
//	pid_init((pid_t *)&Foc.pid_position, 1.0f, 0.0f, 0.03f, 100000.0f, 2.0f);
	pid_init((pid_t *)&Foc.pid_position, 1.0f, 0.0f, 0.03f, 100000.0f, 5.0f);
	pid_init((pid_t *)&Foc.pid_position_profile, 50.0f, 0.0f, 0.0f, 100000.0f, 300.0f);
}

/*
*********************************************************************************************************
*	函 数 名: FOC_voltage
*	功能说明: 根据Ud_set、Uq_set、phase输出电压
*	形    参: Ud_set: 输出电压的d轴分量
*					  Uq_set: 输出电压的q轴分量
*	          phase: 当前电角度
*	返 回 值: 无
*********************************************************************************************************
*/
void FOC_voltage(float Ud_set, float Uq_set, float phase)
{
	/* 限幅 */
	Ud_set = FOC_CLAMP(Ud_set, -BUS_VOLTAGE/2.0f, BUS_VOLTAGE/2.0f);
	Uq_set = FOC_CLAMP(Uq_set, -BUS_VOLTAGE/2.0f, BUS_VOLTAGE/2.0f);
	float Uq_max = sqrt( (BUS_VOLTAGE * BUS_VOLTAGE) - (Ud_set * Ud_set) );
	if (Uq_set > Uq_max)
	{
		Uq_set = Uq_max;
	}
	else if (Uq_set < -Uq_max)
	{
		Uq_set = -Uq_max;
	}

	float Ualpha, Ubeta;
	float Ua, Ub, Uc;
	inverse_park_transform(Ud_set, Uq_set, phase, &Ualpha, &Ubeta);
	inverse_clarke_transform(Ualpha, Ubeta, &Ua, &Ub, &Uc);
	float SVM_a, SVM_b, SVM_c;
	svpwm_generate(Ua, Ub, Uc, &SVM_a, &SVM_b, &SVM_c);
	/* 计算输出电压对应的CCR值 */
	Foc.u_a = SVM_a;
	Foc.u_b = SVM_b;
	Foc.u_c = SVM_c;
	/* 输出电压 */
	set_a_voltage(Foc.u_a);
	set_b_voltage(Foc.u_b);
	set_c_voltage(Foc.u_c);
}

/*
*********************************************************************************************************
*	函 数 名: FOC_current
*	功能说明: 根据Id_set、Iq_set输出电流
*	形    参: Id_set: 输出电流的d轴分量
*					  Iq_set: 输出电流的q轴分量
*	          phase: 当前电角度
*	返 回 值: 无
*********************************************************************************************************
*/
void FOC_current(float Id_set, float Iq_set, float phase)
{
	/* 根据当前读取到的Ia Ib Ic 算出 Id Iq */
	float Ialpha, Ibeta;
	clarke_transform(Foc.i_a, Foc.i_b, Foc.i_c, &Ialpha, &Ibeta);
	park_transform(Ialpha, Ibeta, phase, (float *)&Foc.i_d, (float *)&Foc.i_q);

	UTILS_LP_FAST(Foc.i_d_filt, Foc.i_d, 0.01f);
	UTILS_LP_FAST(Foc.i_q_filt, Foc.i_q, 0.01f);
	
	/* 电流环频率 */
	float ts = 1.0f/PWM_FREQUENCY;

	float pid_out_ud = pid_calc((pid_t *)&Foc.pid_i_d, Id_set - Foc.i_d_filt, ts);
	float pid_out_uq = pid_calc((pid_t *)&Foc.pid_i_q, Iq_set - Foc.i_q_filt, ts);
	FOC_voltage(pid_out_ud, pid_out_uq, phase);
}

/*
*********************************************************************************************************
*	函 数 名: FOC_Velocity
*	功能说明: 速度-电流控制
*	形    参: target_velocity: 目标速度
*	返 回 值: 无
*********************************************************************************************************
*/
void FOC_Velocity(float target_velocity)
{
	UTILS_LP_MOVING_AVG_APPROX(Foc.velocity_filt, Foc.velocity, 5);
	
	float ts = 1.0f/(PWM_FREQUENCY/2); /* 频率为电流环的一半 */

	float pid_out_iq = pid_calc((pid_t *)&Foc.pid_velocity, target_velocity - Foc.velocity_filt, ts);
	Foc.target_i_d = 0.0f;
	Foc.target_i_q = pid_out_iq;
}

/*
*********************************************************************************************************
*	函 数 名: FOC_Position
*	功能说明: 位置-电流控制
*	形    参: target_position: 目标位置
*	返 回 值: 无
*********************************************************************************************************
*/
void FOC_Position(float target_position)
{	
	float ts = 1.0f/(PWM_FREQUENCY/2); /* 频率为电流环的一半 */

	float pid_out_iq = pid_calc((pid_t *)&Foc.pid_position, target_position - Foc.position, ts);
	Foc.target_i_d = 0.0f;
	Foc.target_i_q = pid_out_iq;
}

/*
*********************************************************************************************************
*	函 数 名: FOC_Position_Profile
*	功能说明: 位置-速度-电流控制
*	形    参: target_position: 目标位置
*	返 回 值: 无
*********************************************************************************************************
*/
void FOC_Position_Profile(float target_position)
{
	float ts = 1.0f/(PWM_FREQUENCY/4); /* 频率为电流环的四分之一 */
	float pid_out_vel = pid_calc((pid_t *)&Foc.pid_position_profile, target_position - Foc.position, ts);
	Foc.target_velocity = pid_out_vel;
}

/*
*********************************************************************************************************
*	函 数 名: velocityOpenloop
*	功能说明: 开环速度控制
*	形    参: target_velocity: 目标速度
*	返 回 值: 无
*	说    明: 
*********************************************************************************************************
*/
void velocityOpenloop(float target_velocity)
{
	static float shaft_angle = 0.0f;
	// static uint32_t open_loop_timestamp = 0;
	// uint32_t now_ms = HAL_GetTick();
	// //计算当前每个Loop的运行时间间隔
  // float Ts = (now_ms - open_loop_timestamp) * 1e-3f;
	float Ts = 1.0f/PWM_FREQUENCY;
	// if(Ts <= 0 || Ts > 0.5f) Ts = 1e-3f;
	// 通过乘以时间间隔和目标速度来计算需要转动的机械角度，存储在 shaft_angle 变量中。在此之前，还需要对轴角度进行归一化，以确保其值在 0 到 2π 之间。
  shaft_angle = normalize_angle(shaft_angle + target_velocity*Ts);
	float Uq = 1.0f;
	FOC_voltage(0.0f, Uq, normalize_angle(shaft_angle * UsrConfig.motor_pole_pairs));
	// open_loop_timestamp = now_ms;  //用于计算下一个时间间隔
}
