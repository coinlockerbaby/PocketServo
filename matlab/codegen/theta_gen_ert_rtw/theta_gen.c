/*
 * File: theta_gen.c
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

#include "theta_gen.h"

/* Block signals and states (default storage) */
DW rtDW;

/* External inputs (root inport signals with default storage) */
ExtU rtU;

/* External outputs (root outports fed by signals with default storage) */
ExtY rtY;

/* Model step function */
void theta_gen_step(void)
{
    /* MATLAB Function: '<Root>/mod2pi_fun' incorporates:
     *  DiscreteIntegrator: '<Root>/Discrete-Time Integrator'
     */
    rtY.theta = rtDW.DiscreteTimeIntegrator_DSTATE;
    while (rtY.theta < 0.0F)
    {
        rtY.theta += 6.28318548F;
    }

    while (rtY.theta >= 6.28318548F)
    {
        rtY.theta -= 6.28318548F;
    }

    /* End of MATLAB Function: '<Root>/mod2pi_fun' */

    /* Update for DiscreteIntegrator: '<Root>/Discrete-Time Integrator' incorporates:
     *  Gain: '<Root>/Gain'
     *  Inport: '<Root>/freq'
     */
    rtDW.DiscreteTimeIntegrator_DSTATE += 6.28318548F * rtU.freq * 1.0E-5F;
}

/* Model initialize function */
void theta_gen_initialize(void)
{
    /* (no initialization code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
