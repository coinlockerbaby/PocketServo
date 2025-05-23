/*
*********************************************************************************************************
*
*	模块名称 : FOC基础算法
*	文件名称 : foc.h
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

#ifndef _FOC_H_
#define _FOC_H_


/* 该结构体用于存储FOC控制算法所需的各种参数和状态信息 */
typedef struct sFOC
{
  // 电机是否已使能
  uint8_t is_armed;

  // 直流母线电压及其滤波值
  float v_bus, v_bus_filt;
  // 三相电流及其滤波值
  float i_a, i_b, i_c;
  // 电流环的dq轴电流及其滤波值、母线电流及其滤波值、功率滤波值
  float i_q, i_q_filt, i_d, i_d_filt, i_bus, i_bus_filt, power_filt;
  

  // 三相输出电压
  float u_a, u_b, u_c;
  // 目标电流
  float target_i_q;
  float target_i_d;
  // 目标速度、实际速度
  float target_velocity, velocity, velocity_filt;
  // 目标位置、实际位置
  float target_position, position;
  
  // 电流环PID参数
  pid_t pid_i_d, pid_i_q;
  // 速度环PID参数
  pid_t pid_velocity;
  // 位置-电流PID参数
  pid_t pid_position;
  // 位置-速度PID参数
  pid_t pid_position_profile;
} tFOC;

extern volatile tFOC Foc;
/* 提供给其他C文件调用的函数 */
void FOC_init(void);
void FOC_voltage(float Ud_set, float Uq_set, float phase);
void FOC_current(float Id_set, float Iq_set, float phase);
void FOC_Velocity(float target_velocity);
void FOC_Position(float target_position);
void FOC_Position_Profile(float target_position);
void velocityOpenloop(float target_velocity);

#endif

/************************************ astronG (END OF FILE) ************************************/
