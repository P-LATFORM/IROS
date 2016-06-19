/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

#include <project.h>

/*************************** MACRO DEFINITIONS ********************************/
/**
 * @brief Timer Reset Values
 */
#define TIMER0_CONTROL_RESET_DISABLE        (0)
#define TIMER0_CONTROL_RESET_ENABLE         (1)

/**
 * @brief Calculates divider value for timer clock.
 *
 * IMP: PSoC 4 does not allow to use clocks as input in counter so a simple
 * circuit(TFF and DFF) is used to convert clock as input. This circuit divides
 * input clock so we have to divide tick count by two also as below.
 *
 */
#define TIMER_CLK_DIVIDER_VALUE(us_per_tick) \
            ((CYDEV_BCLK__HFCLK__MHZ * (us_per_tick)) / 2 )

/**************************** PUBLIC FUNCTIONS ********************************/

void `$INSTANCE_NAME`_Start(void)
{
     /* Start Timer */
    `$INSTANCE_NAME`_TIMER_Start();
}

void `$INSTANCE_NAME`_Stop(void)
{
    /* Start Timer */
    `$INSTANCE_NAME`_TIMER_Stop();
}

void `$INSTANCE_NAME`_Reset(void)
{
    `$INSTANCE_NAME`_TIMER_CONTROL_Write(TIMER0_CONTROL_RESET_ENABLE);
     asm("nop");                     /* just wait a cycle */
    `$INSTANCE_NAME`_TIMER_CONTROL_Write(TIMER0_CONTROL_RESET_DISABLE);
}

uint32 `$INSTANCE_NAME`_ReadCounter(void)
{
    return `$INSTANCE_NAME`_TIMER_ReadCounter();
}

void `$INSTANCE_NAME`_SetCompareValue(uint32 value)
{
    `$INSTANCE_NAME`_TIMER_WriteCompare(value);
}
