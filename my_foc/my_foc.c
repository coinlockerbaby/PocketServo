/*
*********************************************************************************************************
*
*	模块名称 : my_foc项目管理文件
*	文件名称 : my_foc.c
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
#include "shell_port.h"

void MyFoc_init(void)
{
	USR_CONFIG_set_default_config();
	shellPrint(&shell, "[FOC_Init]:Usr_Config set to default\r\n");
	MCT_init();
	FOC_init();
	Encoder_init();

	LCD_Draw_FOC_Calibration();
	Calibration_init();
	LCD_Fill(0, 0, 240, 320, BLACK);
	PWM_Cur_Init();
	if (!UsrConfig.is_calibrated)
	{
		
		shellPrint(&shell, "[FOC_Init]:Motor not calibrated, please calibrate, \r\n");
	}
	else
	{
		USR_CONFIG_print_config();
		shellPrint(&shell, "[FOC_Init]:Motor initialization complete, please press Enter\r\n");
	}
	shellPrint(&shell, "*************************************************************\r\n");
}


void USR_CONFIG_print_config(void)
{
	// 打印表头
	shellPrint(&shell, "+----------------------+-------------------+\r\n");
	shellPrint(&shell, "| Parameter            | Value             |\r\n");
	shellPrint(&shell, "+----------------------+-------------------+\r\n");
	// 打印 motor_pole_pairs
	shellPrint(&shell, "| Motor Pole Pairs     | %-17d |\r\n", UsrConfig.motor_pole_pairs);
	// 打印 zero_electric_angle
	shellPrint(&shell, "| Zero Electric Angle  | %-17.2f |\r\n", UsrConfig.zero_electric_angle);
	// 打印 encoder_dir
	shellPrint(&shell, "| Encoder Direction    | %-17s |\r\n", UsrConfig.encoder_dir == 1 ? "CW" : "CCW");

	// 打印 control_mode
	switch (UsrConfig.control_mode)
	{
		case CONTROL_MODE_TORQUE_RAMP:
			shellPrint(&shell, "| Control Mode         | %-17s |\r\n", "TORQUE_RAMP");
			break;
		case CONTROL_MODE_VELOCITY_RAMP:
			shellPrint(&shell, "| Control Mode         | %-17s |\r\n", "VELOCITY_RAMP");
			break;
		case CONTROL_MODE_POSITION_RAMP:
			shellPrint(&shell, "| Control Mode         | %-17s |\r\n", "POSITION_RAMP");
			break;
		case CONTROL_MODE_POSITION_PROFILE:
			shellPrint(&shell, "| Control Mode         | %-17s |\r\n", "POSITION_PROFILE");
		break;
		default:
			break;
	}
	// 打印 current_limit
	shellPrint(&shell, "| Current Limit        | %-17.2f |\r\n", UsrConfig.current_limit);
	// 打印 velocity_limit
	shellPrint(&shell, "| Velocity Limit       | %-17.2f |\r\n", UsrConfig.velocity_limit);
	// 打印表尾
	shellPrint(&shell, "+----------------------+-------------------+\r\n");
}
