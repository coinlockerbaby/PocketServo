/*
*********************************************************************************************************
*
*	模块名称 : 编码器
*	文件名称 : encoder.c
*	版    本 : V1.0
*	说    明 : 
*	修改记录 :
*		版本号  日期         作者      说明
*		V1.0    2024-11-25  astronG   正式发布
*
*	Copyright (C), 2020-2030, astronG
*
*********************************************************************************************************
*/

#include "my_foc.h"
#include "utils.h"
tEncoder   Encoder;

/*
*********************************************************************************************************
*	函 数 名: Encoder_init
*	功能说明: 初始化编码器
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void Encoder_init(void)
{
  Encoder.offline = TRUE; /* 初始化Encoder状态为离线 */
  Encoder.angle = 0.0f;
  Encoder.full_rotations = 0;		/* 初始 当前位置 圈数为0 */
	Encoder.vel_full_rotations = 0; /* 初始 速度计算起点 相关的圈数为0 */
  /* 获取 速度计算起点 相关的初始角度值，并记录当前的时间戳 */
  encoder_get_mechanical_angle();
	Encoder.vel_angle_prev = Encoder.angle;
	// Encoder.vel_angle_prev_ts = HAL_GetTick();
	/* 获取 当前位置 的初始角度值，并记录当前的时间戳 */
	encoder_get_mechanical_angle();
	Encoder.angle_prev = Encoder.angle;
	// Encoder.angle_prev_ts = HAL_GetTick();
}

/*
*********************************************************************************************************
*	函 数 名: Encoder_update
*	功能说明: 更新编码器状态
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void Encoder_update(void)
{
  encoder_get_mechanical_angle();
  float val = Encoder.angle;
  // Encoder.angle_prev_ts = HAL_GetTick();
  float d_angle = val - Encoder.angle_prev;
  // 圈数检测
  if(FOC_ABS(d_angle) > (0.8f*M_2PI) ) Encoder.full_rotations += ( d_angle > 0 ) ? -1 : 1; 
  Encoder.angle_prev = val;
}

/*
*********************************************************************************************************
*	函 数 名: Encoder_getMechanicalAngle
*	功能说明: 获取编码器机械角度(不带圈数)
*	形    参: 无
*	返 回 值: 返回编码器的角度值，单位为弧度
*********************************************************************************************************
*/
float Encoder_getMechanicalAngle(void)
{
  return Encoder.angle;
}

/*
*********************************************************************************************************
*	函 数 名: Encoder_getAngle
*	功能说明: 获取编码器多圈角度
*	形    参: 无
*	返 回 值: 返回编码器的多圈角度值，单位为弧度
*********************************************************************************************************
*/
float Encoder_getAngle(void)
{
  return (float)Encoder.full_rotations * M_2PI + Encoder.angle_prev;
}

/*
*********************************************************************************************************
*	函 数 名: Encoder_getVelocity
*	功能说明: 获取编码器的速度
*	形    参: 无
*	返 回 值: 返回编码器的速度值，单位为弧度/秒
*********************************************************************************************************
*/
float Encoder_getVelocity(void)
{
	float Ts;

  // // 计算采样时间
  // Ts = (Encoder.angle_prev_ts - Encoder.vel_angle_prev_ts)*1e-3;
  // // 如果溢出则设置Ts为1ms
  // if(Ts <= 0) Ts = 1e-3f;

  /* 编码器采样频率 */
  Ts = 1.0f / PWM_FREQUENCY;

  // 速度计算
  float vel = ( (float)(Encoder.full_rotations - Encoder.vel_full_rotations)*M_2PI + 
    (Encoder.angle_prev - Encoder.vel_angle_prev) ) / Ts;    
  // 保存变量以待将来使用
  Encoder.vel_angle_prev = Encoder.angle_prev;
  Encoder.vel_full_rotations = Encoder.full_rotations;
  Encoder.vel_angle_prev_ts = Encoder.angle_prev_ts;
  return vel;
}

/*
*********************************************************************************************************
*	函 数 名: Encoder_electrical_angle
*	功能说明: 获取编码器电角度
*	形    参: 无
*	返 回 值: 返回编码器的电角度值
*********************************************************************************************************
*/
inline float Encoder_electrical_angle(void)
{
	return  normalize_angle((float)(UsrConfig.encoder_dir * UsrConfig.motor_pole_pairs)
                      * Encoder_getMechanicalAngle() - UsrConfig.zero_electric_angle);
}
