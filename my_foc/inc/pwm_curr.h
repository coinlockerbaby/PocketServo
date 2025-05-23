/*
*********************************************************************************************************
*
*	模块名称 : foc底层硬件接口
*	文件名称 : pwm_curr.h
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

#ifndef _PWM_CURR_H_
#define _PWM_CURR_H_

#include "tim.h"
#include "adc.h"

/* 母线电压 */
#define BUS_VOLTAGE            (19.0f)
/* 电压到PWM占空比转换系数，计算公式：(HALF_PWM_PERIOD_CYCLES / 母线电压) */
#define VOLTAGE_FACTOR_TO_PWM_DUTY  ((float) (HALF_PWM_PERIOD_CYCLES/BUS_VOLTAGE))
/* 采样电阻 */
#define SHUNT_RESISTENCE       (0.002f)
/* 电流采样到实际电流的转换系数，计算公式：(3.3V / 4095 / 采样电阻 / 运放增益) */
#define I_SCALE                ((float) ((3.3f / 4095.0f) / SHUNT_RESISTENCE / 20.0f))

/* 三相电流采样的偏移量 */
extern volatile int16_t  phase_a_adc_offset;
extern volatile int16_t  phase_b_adc_offset;
extern volatile int16_t  phase_c_adc_offset;

static inline void set_a_voltage(float voltage)
{
  htim1.Instance->CCR1 = (voltage * VOLTAGE_FACTOR_TO_PWM_DUTY) + (HALF_PWM_PERIOD_CYCLES/2);
}

static inline void set_b_voltage(float voltage)
{
  htim1.Instance->CCR2 = (voltage * VOLTAGE_FACTOR_TO_PWM_DUTY) + (HALF_PWM_PERIOD_CYCLES/2);
}

static inline void set_c_voltage(float voltage)
{
  htim1.Instance->CCR3 = (voltage * VOLTAGE_FACTOR_TO_PWM_DUTY) + (HALF_PWM_PERIOD_CYCLES/2);
}

static inline float read_iphase_a(void)
{
  return (float)((uint16_t)hadc1.Instance->JDR1 - phase_a_adc_offset) * I_SCALE;
}

static inline float read_iphase_b(void)
{
  return (float)((uint16_t)hadc1.Instance->JDR2 - phase_b_adc_offset) * I_SCALE;
}

static inline float read_iphase_c(void)
{
  return (float)((uint16_t)hadc1.Instance->JDR3 - phase_c_adc_offset) * I_SCALE;
}

/* 提供给其他C文件调用的函数 */
void PWM_Cur_Init(void);
void PWM_SwitchOnPWM(void);
void PWM_SwitchOffPWM(void);
void PWM_TurnOnLowSides(void);

#endif

/************************************ astronG (END OF FILE) ************************************/
