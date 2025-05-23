/*
*********************************************************************************************************
*
*	模块名称 : FOC用户配置文件
*	文件名称 : usr_config.c
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

tUsrConfig UsrConfig;

/*
*********************************************************************************************************
*	函 数 名: usr_config_init
*	功能说明: 初始化用户配置
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void USR_CONFIG_set_default_config(void)
{
  // Motor
  UsrConfig.motor_pole_pairs = 7;
  UsrConfig.motor_phase_resistance = 0.0f;
  UsrConfig.motor_phase_inductance = 0.0f;
  UsrConfig.current_limit = 8.0f;
  UsrConfig.velocity_limit = 60.0f;
  // Calibration
  UsrConfig.is_calibrated = FALSE;
  UsrConfig.zero_electric_angle = 0.0f;
  // Controller
  UsrConfig.control_mode = CONTROL_MODE_VELOCITY_RAMP;
  UsrConfig.current_ctrl_bw = 2000.0f;
  // Protect
  UsrConfig.protect_under_voltage = 10.0f;
  UsrConfig.protect_over_voltage = 14.0f;
  // COMM
  // Encoder
  UsrConfig.encoder_dir = 1;
  UsrConfig.encoder_offset = 0.0f;
}
