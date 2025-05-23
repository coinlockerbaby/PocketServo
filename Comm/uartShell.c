/*
*********************************************************************************************************
*
*	模块名称 : 串口Shell
*	文件名称 : uartShell.c
*	版    本 : V1.0
*	说    明 : 串口Shell自定义命令
*
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2025-02-16 astronG  正式发布
*
*	Copyright (C), 2015-2030, astronG
*
*********************************************************************************************************
*/

#include "bsp.h"

void set_ctrl_mode(char * mode)
{
  // 获取互斥锁，保护 FOC参数 资源
  xSemaphoreTake(xFocMutex, portMAX_DELAY);
  if (strcmp(mode, "TORQUE_RAMP") == 0)
  {
    UsrConfig.control_mode = CONTROL_MODE_TORQUE_RAMP;
  }
  else if (strcmp(mode, "VELOCITY_RAMP") == 0)
  {
    UsrConfig.control_mode = CONTROL_MODE_VELOCITY_RAMP;
  }
  else if (strcmp(mode, "POSITION_RAMP") == 0)
  {
    UsrConfig.control_mode = CONTROL_MODE_POSITION_RAMP;
  }
  else if (strcmp(mode, "POSITION_PROFILE") == 0)
  {
    UsrConfig.control_mode = CONTROL_MODE_POSITION_PROFILE;
  }
  else
  {
    shellPrint(&shell, "no this mode");
  }
  // 释放互斥锁
  xSemaphoreGive(xFocMutex);
}
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), set_ctrl_mode, set_ctrl_mode, 
set motor ctrl mode\r\ncan be set to TORQUE_RAMP VELOCITY_RAMP POSITION_RAMP POSITION_PROFILE);

void set_vel(float target_vel)
{
  // 获取互斥锁，保护 FOC参数 资源
  xSemaphoreTake(xFocMutex, portMAX_DELAY);
  switch (UsrConfig.control_mode)
	{
    /* 如果当前状态为速度-电流 则设置目标速度 */
    case CONTROL_MODE_VELOCITY_RAMP:
      Foc.target_velocity = target_vel;
      break;
    /* 如果当前状态为位置-速度-电流 则设置位置环PID输出上限 */
    case CONTROL_MODE_POSITION_PROFILE:
      Foc.pid_position_profile.limit = target_vel;
      break;
    default:
      shellPrint(&shell, "current mode can't set velocity\r\n");
      break;
  }
  // 释放互斥锁
  xSemaphoreGive(xFocMutex);
}
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), set_vel, set_vel, 
set motor velocity);

void set_pos(float target_pos)
{
  // 获取互斥锁，保护 FOC参数 资源
  xSemaphoreTake(xFocMutex, portMAX_DELAY);
  switch (UsrConfig.control_mode)
	{
    /* 如果当前状态为位置-电流 则设置目标位置 */
    case CONTROL_MODE_POSITION_RAMP:
      Foc.target_position = target_pos;
      break;
    /* 如果当前状态为位置-速度-电流 则设置目标位置 */
    case CONTROL_MODE_POSITION_PROFILE:
      Foc.target_position = target_pos;
      break;
    default:
      shellPrint(&shell, "current mode can't set position\r\n");
      break;
  }
  // 释放互斥锁
  xSemaphoreGive(xFocMutex);
}
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), set_pos, set_pos, 
set motor position);


void set_cur(float target_cur)
{
  // 获取互斥锁，保护 FOC参数 资源
  xSemaphoreTake(xFocMutex, portMAX_DELAY);
  switch (UsrConfig.control_mode)
	{
    /* 如果当前状态为电流环 则设置目标电流 */
    case CONTROL_MODE_TORQUE_RAMP:
      Foc.target_i_q = target_cur;
      break;
    /* 如果当前状态为速度-电流 则设置速度环PID输出上限 */
    case CONTROL_MODE_VELOCITY_RAMP:
      Foc.pid_velocity.limit = target_cur;
      break;
    /* 如果当前状态为位置-电流 则设置位置环PID输出上限 */
    case CONTROL_MODE_POSITION_RAMP:
      Foc.pid_position.limit = target_cur;
      break;
    /* 如果当前状态为位置-速度-电流 则设置速度环PID输出上限 */
    case CONTROL_MODE_POSITION_PROFILE:
      Foc.pid_velocity.limit = target_cur;
      break;
    default:
      shellPrint(&shell, "current mode can't set current\r\n");
      break;
  }
  // 释放互斥锁
  xSemaphoreGive(xFocMutex);
}

SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), set_cur, set_cur, 
set motor current limit);

void sysinfo(void)
{
	uint8_t pcWriteBuffer[500];
	shellPrint(&shell, "=================================================\r\n");
	shellPrint(&shell, "Task_Name      State Priority Rem_Stack TaskID\r\n");
	vTaskList((char *)&pcWriteBuffer);
	shellPrint(&shell, "%s\r\n", pcWriteBuffer);
	shellPrint(&shell, "Task_Name      Run_Count        Used\r\n");
	vTaskGetRunTimeStats((char *)&pcWriteBuffer);
	shellPrint(&shell, "%s\r\n", pcWriteBuffer);
}
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), sysinfo, sysinfo, system information);

void motinfo(void)
{
  USR_CONFIG_print_config();
}
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), motinfo, motinfo, motor information);

void focinfo(void)
{

  // 打印表头
	shellPrint(&shell, "+----------------------+-------------------+\r\n");
	shellPrint(&shell, "| Parameter            | Value             |\r\n");
	shellPrint(&shell, "+----------------------+-------------------+\r\n");
  // 根据 UsrConfig.control_mode 打印不同参数
	switch (UsrConfig.control_mode)
	{
		case CONTROL_MODE_TORQUE_RAMP:
			shellPrint(&shell, "| Control Mode         | %-17s |\r\n", "TORQUE_RAMP");
      shellPrint(&shell, "| Target Torque        | %-17.2f |\r\n", Foc.target_i_q);
      shellPrint(&shell, "| Current Torque       | %-17.2f |\r\n", Foc.i_q);
      shellPrint(&shell, "| Torque_P             | %-17.2f |\r\n", Foc.pid_i_q.p);
      shellPrint(&shell, "| Torque_I             | %-17.2f |\r\n", Foc.pid_i_q.i);
      shellPrint(&shell, "| Voltage_Limit        | %-17.2f |\r\n", Foc.pid_i_q.limit);
			break;
		case CONTROL_MODE_VELOCITY_RAMP:
			shellPrint(&shell, "| Control Mode         | %-17s |\r\n", "VELOCITY_RAMP");
      shellPrint(&shell, "| Target Velocity      | %-17.2f |\r\n", Foc.target_velocity);
      shellPrint(&shell, "| Current Velocity     | %-17.2f |\r\n", Foc.velocity);
      shellPrint(&shell, "| Velocity_P           | %-17.2f |\r\n", Foc.pid_velocity.p);
      shellPrint(&shell, "| Velocity_I           | %-17.2f |\r\n", Foc.pid_velocity.i);
      shellPrint(&shell, "| Current_Limit        | %-17.2f |\r\n", Foc.pid_velocity.limit);
			break;
		case CONTROL_MODE_POSITION_RAMP:
			shellPrint(&shell, "| Control Mode         | %-17s |\r\n", "POSITION_RAMP");
      shellPrint(&shell, "| Target Position      | %-17.2f |\r\n", Foc.target_position);
      shellPrint(&shell, "| Current Position     | %-17.2f |\r\n", Foc.position);
      shellPrint(&shell, "| Position_P           | %-17.2f |\r\n", Foc.pid_position.p);
      shellPrint(&shell, "| Position_I           | %-17.2f |\r\n", Foc.pid_position.i);
      shellPrint(&shell, "| Position_D           | %-17.2f |\r\n", Foc.pid_position.d);
      shellPrint(&shell, "| Current_Limit        | %-17.2f |\r\n", Foc.pid_position.limit);
			break;
		case CONTROL_MODE_POSITION_PROFILE:
			shellPrint(&shell, "| Control Mode         | %-17s |\r\n", "POSITION_PROFILE");
      shellPrint(&shell, "| Target Position      | %-17.2f |\r\n", Foc.target_position);
      shellPrint(&shell, "| Current Position     | %-17.2f |\r\n", Foc.position);
      shellPrint(&shell, "| Position_Profile_P   | %-17.2f |\r\n", Foc.pid_position_profile.p);
      shellPrint(&shell, "| Position_Profile_I   | %-17.2f |\r\n", Foc.pid_position_profile.i);
      shellPrint(&shell, "| Velocity_P           | %-17.2f |\r\n", Foc.pid_velocity.p);
      shellPrint(&shell, "| Velocity_I           | %-17.2f |\r\n", Foc.pid_velocity.i);
      shellPrint(&shell, "| Vel_Limit            | %-17.2f |\r\n", Foc.pid_position_profile.limit);
      shellPrint(&shell, "| Current_Limit        | %-17.2f |\r\n", Foc.pid_velocity.limit);
		break;
		default:
			break;
	}
	// 打印表尾
	shellPrint(&shell, "+----------------------+-------------------+\r\n");
}
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), focinfo, focinfo, FOC information);
