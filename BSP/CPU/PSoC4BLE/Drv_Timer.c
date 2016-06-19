/*******************************************************************************
 *
 * @file Drv_Timer.c
 *
 * @author Murat Cakmak
 *
 * @brief Timer Driver Implementation.
 *
 *          [IMP] This timer is created for a PSoC Creator Project so it uses
 *          Auto-Generated Timer source files of PSoC Creator project.
 *
 *          PSoC 4 BLE does not include a 32 bit timer which counts up.
 *          Therefore, a customer timer HW module created using 32 bit counters.
 *          This custom timer HW is a one shot timer and stops itself on match.
 *          It needs to set Reset pin to start Timer.
 *          Timer counter is reset only when Timer is reset using reset pin.
 *          Timer module creates an interrupts only on match.
 *
 *			Timer resolution is 1 microsecond and Timer HW configured according
 *          to this setting in PSoC Creator IDE.
 *
 *        TODO
 *			- Timer Power should be closed when device enter sleep (Low Power)
 *
 * @see https://github.com/P-LATFORM/P-OS/wiki
 *
 ******************************************************************************
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 P-OS
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 ******************************************************************************/

/********************************* INCLUDES ***********************************/
#include "Drv_Timer.h"

#include "TIMER0.h"
#include "TIMER0_TIMER.h"
#include "TIMER0_IRQn.h"

#include "DRVConfig.h"

#include "Debug.h"
#include "postypes.h"

/***************************** MACRO DEFINITIONS ******************************/
/*
 * Number of HW Timers in used CPU.
 *
 *  [IMP] PSoC 4 support some HW Timer but pre-built timers do not meet our
 *  expectations (32 bit and up counting) so we created 1 custom timer for P-OS
 *  requirements.
 *
 *  TODO we need to create a different HW timer for user times. Problem is that
 *  there is no enough place for second timer.
 *  Timers may have channels to manage timings
 */
#define NUM_OF_HW_TIMERS					(1)

/*
 * We could not create a new Timer in PSoC 4 BLE but other IC' has more than one
 * timer and to avoid conflict between project, do not change following field in
 * in common area, just undef here to sync it with HW Timer Count
 */
#undef DRV_CONFIG_NUM_OF_USED_HW_TIMERS

/*
 * If upper layers do not specify HW Timer count, use all HW Timers
 */
#ifndef DRV_CONFIG_NUM_OF_USED_HW_TIMERS
#define DRV_CONFIG_NUM_OF_USED_HW_TIMERS	(NUM_OF_HW_TIMERS)
#endif	/* DRV_CONFIG_NUM_OF_USED_HW_TIMERS */

/*
 * Check External Configurations
 */
#if DRV_CONFIG_NUM_OF_USED_HW_TIMERS > NUM_OF_HW_TIMERS
#error "Unsupported HW Timer Number. P-OS created 2 HW Timer for PSoC 4 BLE!"
#endif

/* Just a wrapper for to be used timer number */
#define NUM_OF_TIMERS						DRV_CONFIG_NUM_OF_USED_HW_TIMERS

/* Local Wrapper for global debugging mode */
#define TIMER_DEBUG_MODE					ENABLE_DEBUG_ASSERT

#if TIMER_DEBUG_MODE
	/* Static checks for debugging mode  */

	/*
	 * When a HW Timer initialized and assigned to client using a handle,
	 * we set this value into the timer object. In this way, we can check both
	 * whether timer is valid (initialized) or not.
	 */
	#define TIMER_VALIDATION_KEY			(0x18273645)

	/* Sets validation key to mark timer as initialized. */
	#define TIMER_SET_VALIDATION_KEY(timer)	\
				{ timer->validationKey =  TIMER_VALIDATION_KEY; }

	/* Checks timer is valid. Checks timer objects includes validation key */
	#define TIMER_HANDLE_IS_VALID(timer) \
				(timer->validationKey == TIMER_VALIDATION_KEY)

#else	/* TIMER_DEBUG_MODE */

	/* Empty definitions to remove static check for release versions */
	#define TIMER_SET_VALIDATION_KEY(timer)
	#define TIMER_HANDLE_IS_VALID(timer)

#endif	/* TIMER_DEBUG_MODE */


/***************************** TYPE DEFINITIONS *******************************/
/*
 * Timer Object to provide SW Timer functionality.
 */
typedef struct
{
	/* Let's add static analysis items in debug mode */
#if TIMER_DEBUG_MODE
	/* Validation Key to mark Timer as initialized and valid. */
	uint32_t validationKey;
#endif /* TIMER_DEBUG_MODE */

	/* Client callback function to inform client about Timer Timeout */
	DrvTimerCallback callback;
} Timer;

/**************************** FUNCTION PROTOTYPES *****************************/

/******************************** VARIABLES ***********************************/
/*
 * All timer objects
 */
PRIVATE Timer timers[NUM_OF_TIMERS];

/*
 * Custom (Emprically Defined) IRQ Priorities for HW Timers
 * 		Cortex M3 NVIC allows priorities between 0~31
 *  	max = 0, min = 31
 */
PRIVATE const uint32_t timerIRQPriorities[] =
{
	3,		/* High Priority */
	9,		/* Midd Priority */
	15		/* Low Priority  */
};

/**************************** PRIVATE FUNCTIONS *******************************/

CY_ISR(TIMER0_IRQHandler)
{
    /*
     * Read Status register in order to clear the sticky Terminal Count (TC) bit
	 * in the status register. Note that the function is not called, but rather
	 * the status is read directly.
	 */
    TIMER0_TIMER_STATUS;

    /* Call user callback to inform about Timer Timeout */
    timers[0].callback();
}

/***************************** PUBLIC FUNCTIONS *******************************/
/*
 * Creates a SW Timer which matches with a HW Timer.
 *
 *  There is no special note about internal implementation details.
 *  See header files to function description.
 */
PUBLIC TimerHandle Drv_Timer_Create(TimerNo timerNo, DrvTimerPriority priority, DrvTimerCallback timerCallback)
{
	Timer* timer;

    /* Internal Checks for debug mode */
	DEBUG_ASSERT_MESSAGE((timerNo < NUM_OF_TIMERS), "Invalid Timer No!");

	/*
	 * Get timer objects to fill client info.
     *  Get here because other internal checks may use timer object
	 */
	timer = &timers[timerNo];

	/* Rest of internal checks */
	DEBUG_ASSERT_MESSAGE(TIMER_HANDLE_IS_VALID(timer) == 0, "Timer is already assigned before!");
	DEBUG_ASSERT_MESSAGE((priority < DRV_TIMER_PRI_NUM), "Invalid Timer Priority!");
	DEBUG_ASSERT_MESSAGE(timerCallback != NULL, "Invalid (NULL) Callback!");

    /* Save user timer callback into timer object */
    timer->callback = timerCallback;

    /* Just simple optimization until we have more than one HW Timer */
#if NUM_OF_TIMERS > 1
    if (timerNo == 0)
#endif
    {
        /* Set interrupt priority using client's priority request */
        TIMER0_IRQn_SetPriority(timerIRQPriorities[priority]);
        /* Register Timer0 interrupt to handle Timer Timouts */
        TIMER0_IRQn_StartEx(TIMER0_IRQHandler);

        TIMER0_Start();
    }

    /* We initialized timer so we can mark it as validated (initialized) */
	TIMER_SET_VALIDATION_KEY(timer);

    /*
	 * Return internal reference as an integer number as Timer Handle.
	 * This hidden internal reference simplies to manage internal objects
	 */
	return (TimerHandle)timer;
}

/*
 * Starts a Timer.
 *
 *  There is no special note about internal implementation details.
 *  See header files to function description.
 */
PUBLIC void Drv_Timer_Start(TimerHandle timerHandle, uint32_t timeoutInUs)
{
    /* Just simple optimization until we have more than one HW Timer */
#if NUM_OF_HW_TIMERS > 1
    /* Get internal timer using timer handle */
	Timer* timer = (Timer*)timerHandle;
#else
    (void)timerHandle;
#endif

	/* Internal Checks for debug mode */
	DEBUG_ASSERT_MESSAGE(TIMER_HANDLE_IS_VALID(timer), "Invalid Timer Handle");

    /* Just simple optimization until we have more than one HW Timer */
#if NUM_OF_TIMERS > 1
    if (timerNo == 0)
#endif
    {
        /* Set compare value */
        TIMER0_SetCompareValue(timeoutInUs);

        /* Our HW Timer module is a one shot timer and needs reset to start */
        TIMER0_Reset();
    }
}

/*
 * Reads elapsed time in a Timer.
 *
 *  Our resolution is 1 microseconds so 1 tick means 1 us so we can directly
 *  return timer tick counter as elapsed time.
 */
PUBLIC uint32_t Drv_Timer_ReadElapsedTimeInUs(TimerHandle timerHandle)
{
    uint32_t timerCounter;

    /* Just simple optimization until we have more than one HW Timer */
#if NUM_OF_HW_TIMERS > 1
    /* Get internal timer using timer handle */
	Timer* timer = (Timer*)timerHandle;
#else
    (void)timerHandle;
#endif

	/* Internal checks for debug mode */
	DEBUG_ASSERT_MESSAGE(TIMER_HANDLE_IS_VALID(timer), "Invalid Timer Handle");

    /* Just simple optimization until we have more than one HW Timer */
#if NUM_OF_TIMERS > 1
    if (timerNo == 0)
#endif
    {
        timerCounter = TIMER0_ReadCounter();
    }

	/* Return just timer count (1 tick = 1 us) as elapsed time */
	return timerCounter;
}
