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
#ifndef OS_MAX_TASK_COUNT
#error " Allowed Max Task Count (OS_MAX_TASK_COUNT) must be defined!"
#endif

/* 
 * Checks Task Handler and breaks execution in case of invalid task handle if 
 * Debug Assertion is enabled by projects
 */
#define CHECK_TASK_HANDLE(handle) \
			DEBUG_ASSERT_MESSAGE(!(taskHandle < 0 || taskHandle >= OS_MAX_TASK_COUNT), "Invalid Task Handle");

/***************************** TYPE DEFINITIONS *******************************/
/*
 * Internal Task Information
 */
typedef struct
{
    OSTask task;					/* User Task */
	volatile OSTaskState state;		/* Current State of Task (New, Ready, Running, Waiting, Terminated)*/
} OSTaskInfo;
/**************************** FUNCTION PROTOTYPES *****************************/

/******************************** VARIABLES ***********************************/
/* All registered tasks (Task Pool) */
PRIVATE OSTaskInfo taskPool[OS_MAX_TASK_COUNT] = { { 0 } };

/**************************** PRIVATE FUNCTIONS *******************************/
/**
 * Runs Non-Preemptive Round-Robin Scheduler. 
 *  Visits all tasks and checks whether task is ready for execution or not. 
 *
 * @param none
 * @return none
 */
PRIVATE INLINE void RunScheduler(void)
{
	OSTaskInfo* task;
	int taskIndex;

	/* SUPER LOOP for General System Execution */
	SUPER_LOOP
	{
		task = &taskPool[0];
		taskIndex = 0;
		
		/* Visit all tasks in task pool */
		while (taskIndex++ < OS_MAX_TASK_COUNT)
		{
			/* If task is ready to execution */
			if (task->state == OSTaskState_Ready)
			{
				/* Task is in Running state now */
				task->state = OSTaskState_Running;

				/* Run Task */
				task->task();

				/* Task left execution due to waiting an event so pass task to Waiting State */
				task->state = OSTaskState_Waiting;
			}

			/* Get Next Task */
			++task;
		}
	}
}

/***************************** PUBLIC FUNCTIONS *******************************/
/**
 * Initializes Scheduler
 *
 * @param none
 *
 * @return 0 initialization successful success otherwise -1
 */
int Scheduler_Init(void)
{
	return RESULT_SUCCESS;
}

/**
 * Initializes Scheduler
 *
 * @param none
 *
 * @return 0 scheduler started otherwise -1
 */
int Scheduler_Start(void)
{
	RunScheduler();

	return RESULT_SUCCESS;
}

/**
 * Adds a task to schedule Task Execution. 
 *
 * @param userTask User Defined Task
 *
 * @return Handle for User Task. User manages task with this handle later. 
 *		   In case of fail, Returns invalid handle (OS_TASK_INVALID_HANDLE). 
 *		   Client code should check task handle.
 */
OSTaskHandle Scheduler_AddTask(OSTask userTask)
{
	OSTaskInfo* currTaskSlot = &taskPool[0];
	uint32_t taskIndex = 0;

	DEBUG_ASSERT_MESSAGE(userTask != NULL, "Invalid Task");

	/* Visit all task pool to find a empty slot for new task */
	while (taskIndex < OS_MAX_TASK_COUNT)
	{
		/* 
		 * Check for state of task slot. 
		 * NoState means slot is empty and can be used for new task 
		 * NoState and Terminated states are same so when we terminate
		 * a task, its slot can be used for new tasks. 
		 */
		if (currTaskSlot->state == OSTaskState_NoState)
		{
			/* Save User Task */
			currTaskSlot->task = userTask;

			/* It is new task so set state as new*/
			currTaskSlot->state = OSTaskState_New;

			break;
		}

		/* Get Next Task Slot in Task pool*/
		++currTaskSlot;
		++taskIndex;
	}

	if (taskIndex == OS_MAX_TASK_COUNT)
	{
		/* Return invalid handle if there is no empty slot in task pool */
		return OS_TASK_INVALID_HANDLE;
	}

	/* Return slot index as Task Handle */
	return taskIndex;
}

/**
 * Removes a task from task pool. It also means task (process) termination.
 *
 * @param taskHandle to be removed Task Handle
 *
 * @return none
 */
void Scheduler_RemoveTask(OSTaskHandle taskHandle)
{
	CHECK_TASK_HANDLE(taskHandle);

	taskPool[taskHandle].state = OSTaskState_Terminated;
}

/**
 * Sets state of a Task. 
 *
 * @param taskHandle handle to set task state
 * @param newState New State of Task
 *
 * @return none
 */
void Scheduler_SetTaskState(OSTaskHandle taskHandle, OSTaskState newState)
{
	CHECK_TASK_HANDLE(taskHandle);
	
	/*TODO Should we enqueue incoming Ready States while Task is already in Ready state */

	/* Set task state */
	taskPool[taskHandle].state = newState;
}

#endif /* #if (OS_SCHEDULER == OS_SCHEDULER_NON_PREEMPTIVE_RR) */
