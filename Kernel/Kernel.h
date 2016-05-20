/*******************************************************************************
 *
 * @file Kernel.h
 *
 * @author Murat Cakmak
 *
 * @brief P-OS Kernel Interface
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
#ifndef __KERNEL_H
#define __KERNEL_H

/********************************* INCLUDES ***********************************/
#include "postypes.h"

/***************************** MACRO DEFINITIONS ******************************/
/*
 * User Application Signature.
 *
 * User space applications can use this macro to create compatible User
 * Applications
 */
#define OS_USER_APPLICATION(appName)		void appName(void)

/*
 * Invalid Task Handle.
 * In case of task creation fail, OS returns this value.
 * */
#define OS_TASK_INVALID_HANDLE				(-1)


/*
 * Scheduler Types
 */
/* Non-Preemptive Round Robin Scheduler */
#define OS_SCHEDULER_NON_PREEMPTIVE_RR		(1)
#define OS_SCHEDULER_EDF					(2)
#define OS_SCHEDULER_COOPARATIVE			(3)
#define OS_SCHEDULER_PREEMPTIVE				(4)
#define OS_SCHEDULER_ADAPTIVE				(5)

/***************************** TYPE DEFINITIONS *******************************/

/*
 * Task States
 */
typedef enum
{
	OSTaskState_NoState = 0,
	OSTaskState_Terminated = OSTaskState_NoState,	/* The process has finished execution */
	OSTaskState_New = 1,							/* The process is being created */
	OSTaskState_Running,							/* Instructions are being executed */
	OSTaskState_Waiting,							/* The process is waiting for some event to occur */
	OSTaskState_Ready,								/* The process is waiting to be assigned to a processor */
} OSTaskState;

/* Task Handle Type */
typedef int32_t OSTaskHandle;

/* User Task Signature */
typedef void(*OSTask)(void);

/*************************** FUNCTION DEFINITIONS *****************************/
OSTaskHandle OS_CreateTask(OSTask task);
void OS_TerminateTask(OSTaskHandle taskHandle);
void OS_SetTaskState(OSTaskHandle taskHandle, OSTaskState newState);

/*
 * IMP : User space have to implement this function.
 * Kernel uses this function to initialize User Space Area before start User Tasks
 */
extern void OS_InitializeUserSpace(void);

#endif	/* __KERNEL_H */
