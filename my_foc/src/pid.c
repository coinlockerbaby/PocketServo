/*
*********************************************************************************************************
*
*	模块名称 : PID控制器 模块
*	文件名称 : pid.c
*	版    本 : V1.0
*	说    明 : 
*	修改记录 :
*		版本号  日期         作者      说明
*		V1.0    2024-11-28  astronG   正式发布
*
*	Copyright (C), 2020-2030, astronG
*
*********************************************************************************************************
*/

#include "my_foc.h"
#include "utils.h"


/*
*********************************************************************************************************
*	函 数 名: pid_init
*	功能说明: 初始化PID控制器结构体，设置比例、积分、微分参数以及输出限制参数。
*	形    参: pid - 指向PID控制器结构体的指针
*            p - 比例参数
*            i - 积分参数
*            d - 微分参数
*            ramp - 输出限制的斜率参数，用于平滑输出限制, 如果ramp为0，则不限制输出
*            limit - 输出的最大限制值
*	返 回 值: 无
*	说    明: 此函数用于初始化PID控制器，设置其参数，并初始化内部状态变量。
*********************************************************************************************************
*/
void pid_init(pid_t* pid, float p, float i, float d, float ramp, float limit)
{
  pid->p = p; /* 比例参数 */
  pid->i = i;	/* 积分参数 */
  pid->d = d;	/* 微分参数 */

  pid->output_ramp = ramp;	 /* 设置输出限制的斜率参数，用于平滑输出限制 */
  pid->limit = limit;	/* 设置输出的最大限制值 */

  pid->error_prev = 0.0;	/* 初始化上一次误差值 */
  pid->output_prev = 0.0;	/* 初始化上一次输出值 */
  pid->integral_prev = 0.0;	/* 初始化上一次积分值 */
  // pid->timestamp_prev = HAL_GetTick();	/* 初始化上一次的时间戳，用于计算时间差 */
}

/*
*********************************************************************************************************
*	函 数 名: pid_reset
*	功能说明: 重置PID控制器的内部状态，将积分、输出和误差的前值重置为0。
*	形    参: pid - 指向PID控制器结构体的指针
*	返 回 值: 无
*	说    明: 此函数用于在需要时（例如在系统启动或某些关键状态变化时）重置PID控制器的状态，
*			以便从已知的状态开始控制过程。这可以防止积分项累积导致的偏差。
*********************************************************************************************************
*/
void pid_reset(pid_t* pid)
{
  pid->integral_prev = 0.0;
  pid->output_prev = 0.0;
  pid->error_prev = 0.0;
}

/*
*********************************************************************************************************
*	函 数 名: pid_calc
*	功能说明: 根据PID控制器的参数和当前的误差值，计算PID控制器的输出。
*	形    参: pid - 指向PID控制器结构体的指针
*            error - 当前的误差值
*            ts - 时间间隔，单位为秒
*	返 回 值: float - PID控制器的输出值
*	说    明: 此函数用于计算PID控制器的输出。首先获取当前时间戳并计算与上一次的时间差。
*			然后根据比例、积分、微分参数和误差值计算输出。积分项和输出值都受到限制，以防止过大的值。
*			如果设置了输出限制斜率，还会检查输出变化率，确保其不超过限制斜率。
*********************************************************************************************************
*/
float pid_calc(pid_t* pid, float error, float ts)
{
  // float ts;
  // uint32_t timestamp_now = HAL_GetTick();
  // /* 计算时间差，单位转换为秒 */
  // ts = (float)(timestamp_now - pid->timestamp_prev) * 1e-3;
  // /* 如果时间差为负值，则设置为1毫秒（防止时间回绕）*/
  // if (ts <= 0 || ts > 0.5f)
  //   ts = 1e-3;
  /* 计算比例项 */
  float proportional = pid->p * error;
  /* 计算积分项，使用梯形积分法 */
  float integral =
		pid->integral_prev + pid->i * ts * 0.5f * (error + pid->error_prev);
  /* 限制积分项不超过设定的限制 */
  integral = FOC_CLAMP(integral, -pid->limit, pid->limit);
  /* 计算微分项 */
  float derivative = pid->d * (error - pid->error_prev) / ts;

  /* 计算PID输出 */
  float output = proportional + integral + derivative;

  /* 限制PID输出不超过设定的限制 */
  output = FOC_CLAMP(output, -pid->limit, pid->limit);

  /* 如果设置了输出斜率限制 */
  if (pid->output_ramp > 0)
	{
    /* 计算输出变化率 */
    float output_rate = (output - pid->output_prev) / ts;
    /* 如果输出变化率超过设定的斜率限制，则限制输出变化 */
    if (output_rate > pid->output_ramp)
      output = pid->output_prev + pid->output_ramp * ts;
    else if (output_rate < -pid->output_ramp)
      output = pid->output_prev - pid->output_ramp * ts;
  }
	
  /* 更新积分项 */
  pid->integral_prev = integral;
  /* 更新上一次输出值 */
  pid->output_prev = output;
  /* 更新上一次误差值 */
  pid->error_prev = error;
  /* 更新上一次时间戳 */
  // pid->timestamp_prev = timestamp_now;
  /* 返回PID计算结果 */
  return output;
}
