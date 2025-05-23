/*
*********************************************************************************************************
*
*	模块名称 : 电机参数识别和编码器校准 模块
*	文件名称 : calibration.c
*	版    本 : V1.0
*	说    明 : 
*	修改记录 :
*		版本号  日期         作者       说明
*		V1.0    2025-02-10  astronG   正式发布
*
*	Copyright (C), 2018-2030, astronG
*
*********************************************************************************************************
*/

#include "my_foc.h"
#include "utils.h"

/*
*********************************************************************************************************
*	函 数 名: Calibration_init
*	功能说明: 电机参数识别和编码器校准
*	形    参: 无
*	返 回 值: 无
*	注    意: 请勿在中断中运行
*********************************************************************************************************
*/
void Calibration_init(void)
{
	shellPrint(&shell, "[FOC_Init]MOT: Calibration Start\r\n");
  /* 开启PWM输出 */
  PWM_SwitchOnPWM();
  // find natural direction
  // move one electrical revolution forward
  for (int i = 0; i <=500; i++ )
  {
    float angle = _3PI_BY_2 + M_2PI * i / 500.0f;
    FOC_voltage(0.0f, 0.5f,  angle);
    Encoder_update();
    vTaskDelay(2);
  }
  Encoder_update();
  // take and angle in the middle
  float mid_angle = Encoder_getAngle();
  // move one electrical revolution backwards
  for (int i = 500; i >=0; i-- )
  {
    float angle = _3PI_BY_2 + M_2PI * i / 500.0f ;
    FOC_voltage(0.0f, 0.5f,  angle);
    Encoder_update();
    vTaskDelay(2);
  }
  Encoder_update();
  float end_angle = Encoder_getAngle();
  FOC_voltage(0.0f, 0.0f, 0.0f);
  vTaskDelay(200);
  // determine the direction the sensor moved
  if (mid_angle == end_angle)
  {
    /* 关闭PWM输出 */
    PWM_SwitchOffPWM();
    shellPrint(&shell, "[FOC_Init]MOT: Failed to notice movement\r\n");
    return; // failed calibration
  }
  else if (mid_angle < end_angle)
  {
		shellPrint(&shell, "[FOC_Init]MOT: sensor_direction==CCW\r\n");
    UsrConfig.encoder_dir = -1; // 逆时针
  }
  else
  {
    shellPrint(&shell, "[FOC_Init]MOT: sensor_direction==CW\r\n");
    UsrConfig.encoder_dir = 1; // 顺时针
  }
  // check pole pair number
  float moved = FOC_ABS(mid_angle - end_angle);
  uint8_t pp_check_result = !(FOC_ABS(moved*UsrConfig.motor_pole_pairs - M_2PI) > 0.5f);  // 0.5f is arbitrary number it can be lower or higher!
  if (pp_check_result == FALSE)
  {
    UsrConfig.motor_pole_pairs = M_2PI/moved;
    shellPrint(&shell, "[FOC_Init]MOT: PP check: fail - estimated pp: %d\r\n", UsrConfig.motor_pole_pairs);
	}
  else
  {
    shellPrint(&shell, "[FOC_Init]MOT: PP check: OK!\r\n");
  }

  // align the electrical phases of the motor and sensor
  // set angle -90(270 = 3PI/2) degrees
  FOC_voltage(0.0f, 0.5f, _3PI_BY_2);
  vTaskDelay(700);
  Encoder_update();
  /* 保存电角度为0时的机械角度 */
  UsrConfig.encoder_offset = Encoder_getMechanicalAngle();
  UsrConfig.zero_electric_angle = normalize_angle(
				(float)(UsrConfig.encoder_dir * UsrConfig.motor_pole_pairs) * UsrConfig.encoder_offset);
  shellPrint(&shell, "[FOC_Init]MOT: Zero elec. angle: %f\r\n", UsrConfig.zero_electric_angle);
  // stop everything
  FOC_voltage(0.0f, 0.0f, 0.0f);
  vTaskDelay(200);
  UsrConfig.is_calibrated = TRUE;
  /* 关闭PWM输出 */
  PWM_SwitchOffPWM();
	shellPrint(&shell, "[FOC_Init]MOT: Calibration OK!\r\n");
}
