/*
*********************************************************************************************************
*
*	模块名称 : my_foc 工具函数
*	文件名称 : utils.c
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
#include "utils.h"
#include "arm_math.h"
//#include "theta_gen.h"
/*
*********************************************************************************************************
*	函 数 名: clarke_transform
*	功能说明: 执行克拉克变换，将ABC轴上的电流值转换为alpha-beta轴上的电流值。
*	形    参: Ia    - A轴上的电流值
*           Ib    - B轴上的电流值
*           Ic    - C轴上的电流值
*           Ialpha - 指向存储转换后的alpha轴电流值的浮点数指针
*           Ibeta  - 指向存储转换后的beta轴电流值的浮点数指针
*	返 回 值: 无
*********************************************************************************************************
*/
inline void clarke_transform(float Ia, float Ib, float Ic, float *Ialpha, float *Ibeta)
{
	*Ialpha = Ia;
	*Ibeta  = (Ib - Ic) * ONE_BY_SQRT3;
}

/*
*********************************************************************************************************
*	函 数 名: park_transform
*	功能说明: 执行帕克变换，将alpha-beta轴上的电流值转换为dq轴上的电流值。
*	形    参: Ialpha - alpha轴上的电流值
*           Ibeta  - beta轴上的电流值
*           Theta  - 旋转角度
*           Id     - 指向存储转换后的d轴电流值的浮点数指针
*           Iq     - 指向存储转换后的q轴电流值的浮点数指针
*	返 回 值: 无
*********************************************************************************************************
*/
inline void park_transform(float Ialpha, float Ibeta, float Theta, float *Id, float *Iq)
{
	float s = arm_sin_f32(Theta);
	float c = arm_cos_f32(Theta);
	*Id     = Ialpha * c + Ibeta * s;
	*Iq     = -Ialpha * s + Ibeta * c;
}

/*
*********************************************************************************************************
*	函 数 名: inverse_park
*	功能说明: 执行逆帕克变换，将dq轴上的电压值转换回alpha-beta轴上的电压值。
*	形    参: Ud    - d轴上的电压值
*           Uq    - q轴上的电压值
*           Theta - 旋转角度
*           Ualpha - 指向存储转换后的alpha轴电压值的浮点数指针
*           Ubeta  - 指向存储转换后的beta轴电压值的浮点数指针
*	返 回 值: 无
*********************************************************************************************************
*/
inline void inverse_park_transform(float Ud, float Uq, float Theta, float *Ualpha, float *Ubeta)
{
	float s    = arm_sin_f32(Theta);
	float c    = arm_cos_f32(Theta);
	*Ualpha = Ud * c - Uq * s;
	*Ubeta  = Ud * s + Uq * c;
}

/*
*********************************************************************************************************
*	函 数 名: inverse_clarke_transform
*	功能说明: 执行逆克拉克变换，将alpha-beta轴上的电压值转换回ABC轴上的电压值。
*	形    参: Ualpha - alpha轴上的电压值
*           Ubeta  - beta轴上的电压值
*           Ua     - 指向存储转换后的A轴电压值的浮点数指针
*           Ub     - 指向存储转换后的B轴电压值的浮点数指针
*           Uc     - 指向存储转换后的C轴电压值的浮点数指针
*	返 回 值: 无
*********************************************************************************************************
*/
inline void inverse_clarke_transform(float Ualpha, float Ubeta, float *Ua, float *Ub, float *Uc)
{
	*Ua = Ualpha;
	*Ub = -0.5f * Ualpha + SQRT3_BY_2 * Ubeta;
	*Uc = -0.5f * Ualpha - SQRT3_BY_2 * Ubeta;
}

/*
*********************************************************************************************************
*	函 数 名: svpwm_generate
*	功能说明: 执行SVPWM调制，根据给定的电压值生成SVPWM信号。
*	形    参: Ua - A轴上的电压值
*           Ub - B轴上的电压值
*           Uc - C轴上的电压值
*           SVM_a - 指向存储生成的SVPWM信号A相的浮点数指针
*           SVM_b - 指向存储生成的SVPWM信号B相的浮点数指针
*           SVM_c - 指向存储生成的SVPWM信号C相的浮点数指针
*	返 回 值: 无
*********************************************************************************************************
*/
inline void svpwm_generate(float Ua, float Ub, float Uc, float *SVM_a, float *SVM_b, float *SVM_c)
{
	float Umax, Umin, Ucom;
	if (Ua > Ub)
	{
		Umax = Ua;
		Umin = Ub;
	}
	else
	{
		Umax = Ub;
		Umin = Ua;
	}
	if (Uc > Umax)
	{
		Umax = Uc;
	}
	else if (Uc < Umin)
	{
		Umin = Uc;
	}
	Ucom = 0.5f * (Umax + Umin);
	*SVM_a = Ua - Ucom;
	*SVM_b = Ub - Ucom;
	*SVM_c = Uc - Ucom;
}

///*
//*********************************************************************************************************
//*	函 数 名: transform_test
//*	功能说明: 测试函数，用于验证变换函数的正确性。
//*	形    参: 无
//*	返 回 值: 无
//*********************************************************************************************************
//*/
//void transform_test(void)
//{
//	float Ud = 0.0f; 
//	float Uq = 1.0f;
//	float Ualpha = 0.0f;
//	float Ubeta = 0.0f;
//	float Ua = 0.0f;
//	float Ub = 0.0f;
//	float Uc = 0.0f;

//	/* matlab theta_gen function */
//	rtU.freq = 1000.0f;
//	theta_gen_step();

//	inverse_park_transform(Ud, Uq, rtY.theta, &Ualpha, &Ubeta);
//	inverse_clarke_transform(Ualpha, Ubeta, &Ua, &Ub, &Uc);

//	float SVM_a, SVM_b, SVM_c;
//	svpwm_generate(Ua, Ub, Uc, &SVM_a, &SVM_b, &SVM_c);
//	debug_print("samples:%f, %f, %f\n", SVM_a, SVM_b, SVM_c);
//}

/*
*********************************************************************************************************
*	函 数 名: normalize_angle
*	功能说明: 角度归一化[0,2PI]
*	形    参: angle - 需要归一化的角度
*	返 回 值: 归一化后的角度
*********************************************************************************************************
*/
inline float normalize_angle(float angle)
{
  float a = fmod(angle, 2*M_PI);   //取余运算可以用于归一化，列出特殊值例子算便知
  return a >= 0 ? a : (a + 2*M_PI); 
}

/*
*********************************************************************************************************
*	函 数 名: normalize_angle_test
*	功能说明: 角度归一化测试函数
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void normalize_angle_test(void)
{
	float angle = 8.30f;
	float norm_angle = normalize_angle(angle);
	debug_print("angle: %f, norm_angle: %f\n", angle, norm_angle);
}
