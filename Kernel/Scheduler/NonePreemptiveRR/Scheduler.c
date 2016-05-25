/*******************************************************************************
 *
 * @file Scheduler.c
 *
 * @author Murat Cakmak
 *
 * @brief Non-Preemptive Round Robin Scheduler. 
 *		- Most primitive task scheduler and acts like a single thread. 
 *		- It is also known as "SUPER LOOP". 
 *		- This scheduler different than cooperative scheduler because
 *		  a task does not yield itself when finished its job. 
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

#include "OS_Config.h"

#if (OS_SCHEDULER == OS_SCHEDULER_NON_PREEMPTIVE_RR)

/********************************* INCLUDES ***********************************/
#include "Kernel.h"
#include "Scheduler.h"

#include "Debug.h"
#include "postypes.h"

/***************************** MACRO DEFINITIONS ******************************/

/***************************** TYPE DEFINITIONS *******************************/

/**************************** FUNCTION PROTOTYPES *****************************/

/******************************** VARIABLES ***********************************/

/**************************** PRIVATE FUNCTIONS *******************************/


/***************************** PUBLIC FUNCTIONS *******************************/
/**
 * Initializes Scheduler
 *
 * @param none
 *
 * @return 0 initialization successful success otherwise -1
 */
void Scheduler_Init(void)
{
}

/**
 * Initializes Scheduler
 *
 * @param none
 *
 * @return 0 scheduler started otherwise -1
 */
void Scheduler_Start(void)
{
}

#endif /* #if (OS_SCHEDULER == OS_SCHEDULER_NON_PREEMPTIVE_RR) */
