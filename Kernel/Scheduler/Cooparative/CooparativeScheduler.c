/*******************************************************************************
 *
 * @file CooparativeScheduler.c
 *
 * @author Murat Cakmak
 *
 * @brief Basic Cooparative Scheduler Implementation.
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

#include "OSConfig.h"

#if (OS_SCHEDULER == OS_SCHEDULER_COOPARATIVE)

/********************************* INCLUDES ***********************************/
#include "Kernel.h"
#include "Scheduler.h"

#include "Kernel_Internal.h"

#include "Debug.h"
#include "postypes.h"

/***************************** MACRO DEFINITIONS ******************************/
/* Task count */
#define TASK_COUNT              NUM_OF_USER_TASKS

/***************************** TYPE DEFINITIONS *******************************/
/*
 * Cooparative Scheduler Internal Data Structure
 */
typedef struct
{
    /* Task pool for all user tasks */
    TCB* taskPool;
    /* Idle task TCB reference*/
    TCB* idleTask;
    /* Kernel callback to inform Kernel core about context switching */
    SchedulerCSCallback csCallback;
    /* Task index to track current task in cooparative scheduling */
    int taskIndex;
} CooparativeScheduler;
/**************************** FUNCTION PROTOTYPES *****************************/

/******************************** VARIABLES ***********************************/
/*
 * Cooparative scheduler internal data
 */
PRIVATE CooparativeScheduler scheduler;
/**************************** PRIVATE FUNCTIONS *******************************/

/***************************** PUBLIC FUNCTIONS *******************************/
/*
 * Initializes Scheduler
 */
PUBLIC void Scheduler_Init(TCB* tcbList, TCB* idleTCB, SchedulerCSCallback csCallback)
{
    scheduler.taskPool = tcbList;
    scheduler.idleTask = idleTCB;
    scheduler.csCallback = csCallback;
    scheduler.taskIndex = 0;
}

/*
 * Yields task in Scheduler side.
 *
 *  Primitive implementation for Cooparative scheduling.
 */
PUBLIC void Scheduler_Yield(void)
{
    TCB* nextTCB;

    /* Get next (ready) task from task pool */
    nextTCB = &scheduler.taskPool[scheduler.taskIndex];

    /* Calculate task indexx for next yield */
    scheduler.taskIndex = (scheduler.taskIndex + 1) % TASK_COUNT;

    /* Inform kernel about next task */
    scheduler.csCallback(nextTCB);
}

#endif /* #if (OS_SCHEDULER == OS_SCHEDULER_COOPARATIVE) */
