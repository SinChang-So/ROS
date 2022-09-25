/**********************************************************************************************************************
 * \file GPT12_PWM_Generation.c
 * \copyright Copyright (C) Infineon Technologies AG 2019
 *
 * Use of this file is subject to the terms of use agreed between (i) you or the company in which ordinary course of
 * business you are acting and (ii) Infineon Technologies AG or its licensees. If and as long as no such terms of use
 * are agreed, use of this file is subject to following:
 *
 * Boost Software License - Version 1.0 - August 17th, 2003
 *
 * Permission is hereby granted, free of charge, to any person or organization obtaining a copy of the software and
 * accompanying documentation covered by this license (the "Software") to use, reproduce, display, distribute, execute,
 * and transmit the Software, and to prepare derivative works of the Software, and to permit third-parties to whom the
 * Software is furnished to do so, all subject to the following:
 *
 * The copyright notices in the Software and this entire statement, including the above license grant, this restriction
 * and the following disclaimer, must be included in all copies of the Software, in whole or in part, and all
 * derivative works of the Software, unless such copies or derivative works are solely in the form of
 * machine-executable object code generated by a source language processor.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *********************************************************************************************************************/

/*********************************************************************************************************************/
/*-----------------------------------------------------Includes------------------------------------------------------*/
/*********************************************************************************************************************/
#include "GPT12_PWM_Generation.h"
#include "IfxVadc_Adc.h"
#include "Ifx_Types.h"
#include "IfxGpt12.h"
#include "IfxPort.h"
#include "Bsp.h"

/*********************************************************************************************************************/
/*------------------------------------------------------Macros-------------------------------------------------------*/
/*********************************************************************************************************************/
#define ISR_PRIORITY_GPT12_TIMER    6                       /* Define the GPT12 Timer interrupt priority            */
#define ISR_PROVIDER_GPT12_TIMER    IfxSrc_Tos_cpu0         /* Interrupt provider                                   */
#define PWM_FREQUENCY               50.0f                    /* Frequency of the PWM signal in Hz                    */
#define PWM_DUTY_CYCLE              7.25                      /* Duty cycle of the PWM signal in percentage           */
#define LED                         &MODULE_P00, 5          /* LED which toggles in the Interrupt Service Routine   */
#define GPT1_BLOCK_PRESCALER        32                      /* GPT1 block prescaler value                           */
#define TIMER_T3_INPUT_PRESCALER    32                      /* Timer input prescaler value                          */
#define TEST_PIN1                   &MODULE_P02, 1
#define SEC_TO_USEC                     1000000

/*********************************************************************************************************************/
/*--------------------------------------------Function Implementations-----------------------------------------------*/
/*********************************************************************************************************************/
/* Macro to define Interrupt Service Routine */
IFX_INTERRUPT(interruptGpt12, 0, ISR_PRIORITY_GPT12_TIMER);
//extern float32 test_value;
float32 fTimer;
float angle = 0.0;


/* Interrupt Service Routine of the GPT12 */
void interruptGpt12(void)
{
    /* Toggle the state of the LED */
    IfxPort_togglePin(LED);
    IfxPort_togglePin(TEST_PIN1);
    //IfxPort_setPinHigh(TEST_PIN1);
}

/* Function to initialize the GPT12 module and the LED */
void initGpt12PWM(void)
{
    /* Enable the GPT12 module */
        IfxGpt12_enableModule(&MODULE_GPT120);
        /* Set GPT1 block prescaler to divide the module frequency */
        IfxGpt12_setGpt1BlockPrescaler(&MODULE_GPT120, IfxGpt12_Gpt1BlockPrescaler_32);
        /* Set the GPT12 timer T3 in timer mode */
        IfxGpt12_T3_setMode(&MODULE_GPT120, IfxGpt12_Mode_timer);
        /* Set the Timer T3 direction   */
        IfxGpt12_T3_setTimerDirection(&MODULE_GPT120, IfxGpt12_TimerDirection_down);
        /* Set timer T3 input prescaler to divide the timer frequency */
        IfxGpt12_T3_setTimerPrescaler(&MODULE_GPT120, IfxGpt12_TimerInputPrescaler_32);

        /* Calculate dutyUpTime and dutyDownTime for reloading timer T3 */
        float32 moduleFreq = IfxGpt12_getModuleFrequency(&MODULE_GPT120);
        fTimer = (moduleFreq / (GPT1_BLOCK_PRESCALER * TIMER_T3_INPUT_PRESCALER));
        //test_value = fTimer;
        uint16 dutyUpTime = (fTimer * (PWM_DUTY_CYCLE / 100.0f)) / PWM_FREQUENCY;
        uint16 dutyDownTime = (fTimer * (1 - (PWM_DUTY_CYCLE / 100.0f))) / PWM_FREQUENCY;

        /* Set timer T3 value */
        IfxGpt12_T3_setTimerValue(&MODULE_GPT120, dutyDownTime);

        /* Timer T2: reloads the value dutyDownTime in timer T3 */
        /* Set the timer T2 in reload mode */
        IfxGpt12_T2_setMode(&MODULE_GPT120, IfxGpt12_Mode_reload);
        /* Set the T2 input parameter: Negative transition (falling edge) of T3 toggle latch T3OTL */
        IfxGpt12_T2_setReloadInputMode(&MODULE_GPT120, IfxGpt12_ReloadInputMode_fallingEdgeTxOTL);
        /* Set timer T2 value (the value that is reloaded in T3 on negative transition) */
        IfxGpt12_T2_setTimerValue(&MODULE_GPT120, dutyDownTime);

        /* Timer T4: reloads the value dutyUpTime in timer T3 */
        /* Set timer T4 in reload mode */
        IfxGpt12_T4_setMode(&MODULE_GPT120, IfxGpt12_Mode_reload);
        /* Set the T4 input parameter: Positive transition (rising edge) of T3 toggle latch T3OTL */
        IfxGpt12_T4_setReloadInputMode(&MODULE_GPT120, IfxGpt12_ReloadInputMode_risingEdgeTxOTL);
        /* Set timer T4 value (the value that is reloaded in T3 on positive transition) */
        IfxGpt12_T4_setTimerValue(&MODULE_GPT120, dutyUpTime);

        /* Configure the GPT12 interrupt */
        volatile Ifx_SRC_SRCR *src = IfxGpt12_T3_getSrc(&MODULE_GPT120);            /* Get interrupt address            */
        IfxSrc_init(src, ISR_PROVIDER_GPT12_TIMER, ISR_PRIORITY_GPT12_TIMER);       /* Initialize the service request   */
        IfxSrc_enable(src);                                                         /* Enable GPT12 interrupt           */

        /* Initialize the LED */
        IfxPort_setPinModeOutput(LED, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
        IfxPort_setPinModeOutput(TEST_PIN1, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
}

/* Function to start the GPT12 timer */
void runGpt12PWM(void)
{
    IfxGpt12_T3_run(&MODULE_GPT120, IfxGpt12_TimerRun_start);                   /* Start the timer T3               */
}
float map_angle = 0.0;
float map_ag(int x, float in_min, float in_max, float out_min, float out_max)
{

    map_angle = (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    return map_angle;
}


void Set_GptPWM_Duty(float pwm_duty)
{
    angle = map_ag(pwm_duty, 0.0, 180.0, 2.5, 12.0);
    double dutyUpTime = (fTimer * (angle / 100.0f)) / PWM_FREQUENCY;
    double dutyDownTime = (fTimer * (1 - (angle / 100.0f))) / PWM_FREQUENCY;
    IfxGpt12_T2_setTimerValue(&MODULE_GPT120, dutyDownTime);
    IfxGpt12_T4_setTimerValue(&MODULE_GPT120, dutyUpTime);
}

