/*******************************************************************************
 *
 * @file Kernel.h
 *
 * @author Murat Cakmak
 *
 * @brief P-OS Kernel Interface for User Applications. 
 *
 *		  USER GUIDE
 *
 *		  1. CREATE USER TASKS and ADD to STARTUP
 *
 *				//User function for task execution
 *				OS_USER_TASK_START_POINT(MyTaskFunc)
 *				{
 *					while (true) 
 *					{
 *						// User Code
 *					}
 *				}
 *
 *				//Create an user task with name MyTask1 and 256 Stack Size using MyTaskFunc function
 *				OS_USER_TASK(MyTask1, MyTaskFunc, 256);
 *				//Create an another user task with name MyTask2 and 512 Stack Size using MyTaskFunc function
 *				OS_USER_TASK(MyTask2, MyTaskFunc, 512);
 *	
 *				// Add MyTask1 and MyTask2 to startup. 
 *				STARTUP_APPLICATIONS
 *				(
 *					USER_TASK_PREFIX(MyTask1);
 *					USER_TASK_PREFIX(MyTask2);
 *				)
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
 * Scheduler Types
 */
#define OS_SCHEDULER_NON_PREEMPTIVE_RR		(1)
#define OS_SCHEDULER_EDF					(2)
#define OS_SCHEDULER_COOPARATIVE			(3)
#define OS_SCHEDULER_PREEMPTIVE				(4)
#define OS_SCHEDULER_ADAPTIVE				(5)

/*
 * User Task
 *
 * User creates an task using this definition. 
 * 
 * NOTE
 * 	If you need to change TaskName##Type structure, you have to pay attention to
 * 	change also order or initialization order of 'TaskName'
 *
 * @param TaskName Name of user task. 
 * @param StartPoint Start point (function) for user tasks. User task started 
 *		  from that function. 
 * @param StackSize Stack Size of User Task. 
 * 
 */
#define OS_USER_TASK(TaskName, StartPoint, StackSize) \
typedef struct \
{ \
    OSUserTaskStartPoint __task; \
    uint32_t __stackSize; \
    uint8_t __stack[StackSize]; \
} TaskName##Type; \
static TaskName##Type TaskName = { StartPoint, StackSize, { 0 } };

/*
 * Prefix for User Task. 
 * 
 * [IMP] When you add a User Task to Startup Applications, you should use this 
 * macro with user task name. 
 * 
 * @param UserTask User Task
 *
 * TODO Find a more suitable name than Prefix
 */
#define USER_TASK_PREFIX(UserTask)	&(UserTask)

/*
 * User Task Start point Prototype.
 *
 * User space applications can use this macro to create compatible function 
 * (Start Points)
 *
 * @param TaskStartPoint (Function) Name of User Task Start Point
 *
 */
#define OS_USER_TASK_START_POINT(TaskStartPoint) \
			void TaskStartPoint(void* args)

/*
 * Static Startup Applications
 * 
 * You can add an user task to system startup. Kernel starts startup functions 
 * after kernel initialization.
 *
 * @param ... List of Startup Functions. 
 *			  IMP you should pass user task using USER_TASK_PREFIX() macro.
 * 
 */
#define STARTUP_APPLICATIONS(...) \
static void* startupApplications[] = { __VA_ARGS__ };

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

/* User Task Signature */
typedef void(*OSUserTaskStartPoint)(void*);

/*************************** FUNCTION DEFINITIONS *****************************/

/*
 * Interrupts running task execution and switches execution to next task. 
 * 
 * @param none
 * @return none
 */
void OS_Yield(void);

/*
 * IMP : User space have to implement this function.
 * Kernel uses this function to initialize User Space Area before starts User Tasks
 * 
 * @param none
 * @return none
 */
extern void OS_InitializeUserSpace(void);

#endif	/* __KERNEL_H */
