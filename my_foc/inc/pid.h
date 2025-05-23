/*
*********************************************************************************************************
*
*	模块名称 : PID控制器 模块
*	文件名称 : pid.h
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

#ifndef _PID_H_
#define _PID_H_

typedef struct
{
  float p;           // 比例增益
  float i;           // 积分增益
  float d;           // 微分增益
  float output_ramp; // 输出值的最大变化速度
  float limit;       // 最大输出值

  float error_prev;        // 跟踪最后一次误差值
  float output_prev;       // 最后一次PID输出值
  float integral_prev;     // 最后一次积分值
  uint32_t timestamp_prev; // 记录最后一次时间戳
} pid_t;

/* 提供给其他C文件调用的函数 */
void pid_init(pid_t* pid, float p, float i, float d, float ramp, float limit);
void pid_reset(pid_t* pid);
float pid_calc(pid_t* pid, float error, float ts);

#endif
