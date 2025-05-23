/*
*********************************************************************************************************
*
*	模块名称 : foc底层硬件接口(PWM和电流采样)
*	文件名称 : pwm_curr.c
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

volatile int16_t phase_a_adc_offset = 0;
volatile int16_t phase_b_adc_offset = 0;
volatile int16_t phase_c_adc_offset = 0;

/*
*********************************************************************************************************
*	函 数 名: PWM_Cur_Init
*	功能说明: PWM和电流采样初始化
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void PWM_Cur_Init(void)
{
	/* ADC1 calibration */
	HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
	HAL_ADCEx_InjectedStart_IT(&hadc1);

	/* Set all duty to 50% */
	htim1.Instance->CCR1 = ((uint32_t) HALF_PWM_PERIOD_CYCLES / (uint32_t) 2);
	htim1.Instance->CCR2 = ((uint32_t) HALF_PWM_PERIOD_CYCLES / (uint32_t) 2);
	htim1.Instance->CCR3 = ((uint32_t) HALF_PWM_PERIOD_CYCLES / (uint32_t) 2);
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_3);
	HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_1);
	HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_2);
	HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_3);
	
  /* 定时器通道4触发ADC采样 */
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);
}

/*
*********************************************************************************************************
*	函 数 名: PWM_SwitchOffPWM
*	功能说明: 打开PWM输出
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void PWM_SwitchOnPWM(void)
{
	/* Set all duty to 50% */
	htim1.Instance->CCR1 = ((uint32_t) HALF_PWM_PERIOD_CYCLES / (uint32_t) 2);
	htim1.Instance->CCR2 = ((uint32_t) HALF_PWM_PERIOD_CYCLES / (uint32_t) 2);
	htim1.Instance->CCR3 = ((uint32_t) HALF_PWM_PERIOD_CYCLES / (uint32_t) 2);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
	HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);
	HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_3);
}

/*
*********************************************************************************************************
*	函 数 名: PWM_SwitchOffPWM
*	功能说明: 关闭PWM输出
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void PWM_SwitchOffPWM(void)
{
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_3);
	HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_1);
	HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_2);
	HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_3);
}

/*
*********************************************************************************************************
*	函 数 名: PWMC_TurnOnLowSides
*	功能说明: 打开低侧PWM输出
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void PWM_TurnOnLowSides(void)
{
	htim1.Instance->CCR1 = 0;
	htim1.Instance->CCR2 = 0;
	htim1.Instance->CCR3 = 0;
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
	HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);
	HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_3);
}
