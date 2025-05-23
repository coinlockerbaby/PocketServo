/*
*********************************************************************************************************
*
*	模块名称 : my_foc 工具函数
*	文件名称 : utils.h
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

#ifndef _UTILS_H_
#define _UTILS_H_


#define M_PI                                          (3.14159265358f)
#define M_2PI                                         (6.28318530716f)
#define ONE_BY_SQRT3                                  (0.57735026919f)
#define TWO_BY_SQRT3                                  (2.0f * 0.57735026919f)
#define SQRT3_BY_2                                    (0.86602540378f)
#define _3PI_BY_2                                     (4.71238898038f)

#define FOC_MIN(a,b) 				                          (((a)<(b))?(a):(b))
#define FOC_MAX(a,b) 				                          (((a)>(b))?(a):(b))
#define FOC_ABS(x)                                    ((x) > 0 ? (x) : -(x))
#define FOC_CLAMP(x, lower, upper)                    (FOC_MIN(upper, FOC_MAX(x, lower)))
#define FOC_FLOAT_EQU(floatA, floatB)                 ((FOC_ABS((floatA) - (floatB))) < 0.000001f)

/*
*********************************************************************************************************
*	宏    名: UTILS_LP_FAST
*	功能描述: 实现低通滤波器的快速计算，用于平滑信号并滤除高频噪声。
*	参数说明: value: 当前滤波器的输出值
*				sample: 当前采样值
*				filter_constant: 滤波常数，范围在0.0到1.0之间，1.0表示无滤波
*********************************************************************************************************
*/
#define UTILS_LP_FAST(value, sample, filter_constant) (value -= (filter_constant) * ((value) - (sample)))
/*
*********************************************************************************************************
*	宏    名: UTILS_LP_MOVING_AVG_APPROX
*	功能描述: 实现移动平均滤波器的快速计算，用于平滑信号并滤除高频噪声。
*	参数说明: value: 当前滤波器的输出值
*				sample: 当前采样值
*				N: 滤波器阶数，即平滑窗口的大小
*********************************************************************************************************
*/
#define UTILS_LP_MOVING_AVG_APPROX(value, sample, N)  UTILS_LP_FAST(value, sample, 2.0f / ((N) + 1.0f))

/* 提供给其他C文件调用的函数 */
void clarke_transform(float Ia, float Ib, float Ic, float *Ialpha, float *Ibeta);
void park_transform(float Ialpha, float Ibeta, float Theta, float *Id, float *Iq);
void inverse_park_transform(float mod_d, float mod_q, float Theta, float *mod_alpha, float *mod_beta);
void inverse_clarke_transform(float Ualpha, float Ubeta, float *Ua, float *Ub, float *Uc);
void svpwm_generate(float Ua, float Ub, float Uc, float *SVM_a, float *SVM_b, float *SVM_c);
float normalize_angle(float angle);
void transform_test(void);
void normalize_angle_test(void);

#endif
