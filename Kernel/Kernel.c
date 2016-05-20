/*******************************************************************************
 *
 * @file Startup.c
 *
 * @author Murat Cakmak
 *
 * @brief P-OS Kernel.
 *
 *		- Initializes Hardware
 *		- Initializes Kernel
 *		- Starts Kernel
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

/********************************* INCLUDES ***********************************/
#include "Kernel.h"
#include "Scheduler.h"
#include "Board.h"
#include "postypes.h"

/***************************** MACRO DEFINITIONS ******************************/

/***************************** TYPE DEFINITIONS *******************************/

/**************************** FUNCTION PROTOTYPES *****************************/

/******************************** VARIABLES ***********************************/

/**************************** PRIVATE FUNCTIONS ******************************/
/**
 * Initializes Kernel and OS Components. Initializes also User Space.
 * INLINED to avoid function call overhead.
 *
 * @param none
 *
 * @return none
 */
PRIVATE INLINE void InitializeKernel(void)
{
	/* Initialize Scheduler */
	Scheduler_Init();

	/*
	 * TODO Other initializations
	 */

	/* Initialize User Space */
	OS_InitializeUserSpace();
}

/**
 * Starts Kernel after initialization
 * INLINED to avoid function call overhead.
 *
 * @param none
 *
 * @return none
 */
PRIVATE INLINE void StartKernel(void)
{
	Scheduler_Start();
}

/***************************** PUBLIC FUNCTIONS *******************************/
/**
 * Creates a task.
 *
 * @param task User Task
 *
 * @return Handle for user task.
 */
PUBLIC OSTaskHandle OS_CreateTask(OSTask task)
{
	/* Just add user task to scheduler */
	return Scheduler_AddTask(task);
}

/**
 * Terminates a task.
 *
 * @param taskHandle to be terminated Task Handle
 *
 * @return none
 */
PUBLIC void OS_TerminateTask(OSTaskHandle taskHandle)
{
	/* Just remove from Scheduler */
	Scheduler_RemoveTask(taskHandle);
}

/**
 * Sets State of Task
 *
 * @param taskHandle to be set Task Handle
 * @param newState New state of Task
 *
 * @return none
 */
PUBLIC void OS_SetTaskState(OSTaskHandle taskHandle, OSTaskState newState)
{
	Scheduler_SetTaskState(taskHandle, newState);
}

/**
 * Start point.
 * Initialize whole system and starts Kernel.
 *
 * @param none
 *
 */
int main(void)
{
	/* Initialize Board First */
	Board_Init();

	/* Initialize Kernel */
	InitializeKernel();

	/* Start Kernel */
	StartKernel();

	return 0;
}

