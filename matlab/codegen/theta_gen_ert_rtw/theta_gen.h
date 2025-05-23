/*
 * File: theta_gen.h
 *
 * Code generated for Simulink model 'theta_gen'.
 *
 * Model version                  : 1.16
 * Simulink Coder version         : 9.9 (R2023a) 19-Nov-2022
 * C/C++ source code generated on : Fri Feb  7 13:23:39 2025
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. Traceability
 * Validation result: Not run
 */

#ifndef RTW_HEADER_theta_gen_h_
#define RTW_HEADER_theta_gen_h_
#ifndef theta_gen_COMMON_INCLUDES_
#define theta_gen_COMMON_INCLUDES_
#include <stdbool.h>
#include <stdint.h>
#endif                                 /* theta_gen_COMMON_INCLUDES_ */

/* Block signals and states (default storage) for system '<Root>' */
typedef struct
{
    float DiscreteTimeIntegrator_DSTATE;/* '<Root>/Discrete-Time Integrator' */
}
DW;

/* External inputs (root inport signals with default storage) */
typedef struct
{
    float freq;                        /* '<Root>/freq' */
}
ExtU;

/* External outputs (root outports fed by signals with default storage) */
typedef struct
{
    float theta;                       /* '<Root>/theta' */
}
ExtY;

/* Block signals and states (default storage) */
extern DW rtDW;

/* External inputs (root inport signals with default storage) */
extern ExtU rtU;

/* External outputs (root outports fed by signals with default storage) */
extern ExtY rtY;

/* Model entry point functions */
extern void theta_gen_initialize(void);
extern void theta_gen_step(void);

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<Root>/Data Type Conversion' : Eliminate redundant data type conversion
 */

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'theta_gen'
 * '<S1>'   : 'theta_gen/mod2pi_fun'
 */

/*-
 * Requirements for '<Root>': theta_gen

 */
#endif                                 /* RTW_HEADER_theta_gen_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
