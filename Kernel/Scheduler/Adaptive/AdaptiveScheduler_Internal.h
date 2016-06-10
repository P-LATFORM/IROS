/*******************************************************************************
 *
 * @file AdaptiveScheduler.h
 *
 * @author Murat Cakmak
 *
 * @brief Adaptive scheduler internal definitions and configurations.
 *
 *        NOTE AS prefix refers to 'Adaptive Scheduler'
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
#ifndef __ADAPTIVE_SCHEDULER_INTERNAL_H
#define __ADAPTIVE_SCHEDULER_INTERNAL_H

/********************************* INCLUDES ***********************************/
#include "postypes.h"

/***************************** MACRO DEFINITIONS ******************************/


/*******************************************************************************
 * Coefficients for Controller
 ******************************************************************************/

/* Coefficient for PI Controller (Outer Loop) */
#define AS_KPI						(0.5f)
/*
 * KRR and ZRR parameters.
 *
 * For details, please see following research :
 *  - A new perspective proposal for preemptive feedback scheduling.
 *    M.Maggio, A. Leva. (2010)
 */
#define AS_KRR						(0.9f)
#define AS_ZRR						(0.88f)

/******************************************************************************/

/* Burst time for idle thread */
#define AS_IDLE_THREAD_BURST_IN_US	(10000)

/*
 * Nominal burst time for tasks.
 * Tasks starts with nominal burst time and scheduler tries to find optimal
 * burst in time.
 */
#define AS_BURST_NOMINAL_IN_US		(4000)

/*
 * Minimum burst time for a task.
 *
 * Miosix Note : To avoid inefficiency by context switch
 */
#define AS_BURST_MIN_IN_US			(200)

/*
 * Maximum burst time for a task.
 *
 * Miosix Note : To avoid losing responsiveness
 */
#define AS_BURST_MAX_IN_US			(20000)

/*
 * Multiplication Factor
 *
 * Miosix Note : Implementation detail resulting from a fixed point
 * implementation of the inner integral regulators. Never change this, change
 * KPI instead.
 */
#define AS_MULT_FACTOR				(1.0f / AS_KPI)

/***************************** TYPE DEFINITIONS *******************************/

/*************************** FUNCTION DEFINITIONS *****************************/

#endif /* __ADAPTIVE_SCHEDULER_INTERNAL_H */
