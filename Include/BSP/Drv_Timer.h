/*******************************************************************************
 *
 * @file Drv_Timer.h
 *
 * @author Murat Cakmak
 *
 * @brief Timer Driver Interface.
 *
 * 			Timer module provides one shot timers and you need to set timer for
 *			each time.
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
 *******************************************************************************/
#ifndef __DRV_TIMER_H
#define __DRV_TIMER_H

/********************************* INCLUDES ***********************************/

#include "postypes.h"

/***************************** MACRO DEFINITIONS ******************************/

/***************************** TYPE DEFINITIONS *******************************/
/* HW Timer no */
typedef uint32_t TimerNo;

/* Timer Handle to manage acquired timer */
typedef uint32_t TimerHandle;

/* Timer Timout Callback function type */
typedef void (*DrvTimerCallback)(void);

/*
 * Timer Prioritites
 *
 *  Timer interrupts preempts running task (main thread or another ISRs)
 *  according to its priority. Interrupts can only preempts the execution if
 *  running task's priority lower than timer's priority.
 */
typedef enum
{
	/* High priority. Should be used time critical timings */
	DRV_TIMER_PRI_HIGH,
	/* Normal priority*/
	DRV_TIMER_PRI_NORMAL,
	/* Low priority for trivial timings */
	DRV_TIMER_PRI_LOW,
	DRV_TIMER_PRI_NUM
} DrvTimerPriority;

/*************************** FUNCTION DEFINITIONS *****************************/

/*
 * Creates a SW Timer which matches with a HW Timer.
 *
 *  Client code specifies a HW Timer Number to acquire HW Timer.
 *  When a timer is allocated, it cannot used by another clients.
 *  (On the other hand, this check is done only in debug mode).
 *
 * @param timerNo 		to be acquired HW Timer Number. timerNo must not be
 *						higher than  DRV_CONFIG_NUM_OF_USED_HW_TIMERS value.
 * @param priority 		Timer Priorty
 * @param timerCallback	Client callback to inform client about Timer Timeout.
 *						Callback should not be NULL, otherwise timer is not
 * 						created.
 *
 * @return Timer Handle to manage timer. Client have to keep this handle,
 *		   otherwise cannot access its timer anymore.
 */
TimerHandle Drv_Timer_Create(TimerNo timerNo,
							 DrvTimerPriority priority,
                             DrvTimerCallback timerCallback);

/*
 * Starts a Timer.
 *
 *   Timer is a one shot timer : When timer timeout is occurred, client code is
 *   informed using its callback (registered in Drv_Timer_Create function)
 *   After that timer stop itself. Need to call Drv_Timer_Start() function start
 * 	 again.
 *
 * @param timerHandle	Handle of to be started Timer
 * @param timeoutInUs 	Timer Timeout value in microseconds. When time occurred,
 *        				client code is informed using its callback (registered
 *						in Drv_Timer_Create function)
 *
 * @return none
 *
 */
void Drv_Timer_Start(TimerHandle timerHandle, uint32_t timeoutInUs);

/*
 * Reads elapsed time in a Timer.
 *
 *   This function just return elapsed time from timer start even if timer is
 *   not started.
 *
 * @param timerHandle	Handle to get Elapsed Time of Timer
 *
 * @return Elapsed Time Elapsed time from timer start.
 */
uint32_t Drv_Timer_ReadElapsedTimeInUs(TimerHandle timerHandle);

#endif	/* __DRV_TIMER_H */
