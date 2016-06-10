/*******************************************************************************
 *
 * @file Kernel.c
 *
 * @author Murat Cakmak
 *
 * @brief Kernel Core Implementation.
 *
 *		- Initializes Hardware
 *		- Initializes Kernel
 *		- Initializes User Space Area
 *		- Starts Kernel
 *		- Starts User Space Applications
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
#include "Kernel_Internal.h"
#include "Scheduler.h"

#include "Board.h"

#include "postypes.h"

/***************************** MACRO DEFINITIONS ******************************/

/***************************** TYPE DEFINITIONS *******************************/

/**************************** FUNCTION PROTOTYPES *****************************/
/* 
 * IDLE Task Definitions
 */
PRIVATE OS_USER_TASK_START_POINT(IdleTaskFunc);

/******************************** VARIABLES ***********************************/
/*
 * Idle Task
 * 
 *  We use idle task for side things which maintains system
 */
KERNEL_TASK(IdleTask, IdleTaskFunc, IDLE_TASK_STACK_SIZE, IDLE_TASK_PRIORITY);

/*
 * Task Pool.
 * 
 *  Keeps all kernel and user tasks.
 */
PRIVATE TCB kernelTaskPool[NUM_OF_USER_TASKS];

/*
 * Task Pool. 
 * 
 *  Keeps all kernel and user tasks. 
 */
PRIVATE TCB idleTaskTCB;

/**************************** PRIVATE FUNCTIONS ******************************/

/*
 * Context Switching callback.
 *
 *  When a context switching is required, Scheduler notifies Kernel using
 *  this callback.
 */
PRIVATE void ContextSwitch_Callback(TCB* nextTCB)
{
    /* Just switch to next TCB which specified from scheduler */
    Kernel_SwitchTo((reg32_t*)nextTCB);
}

/*
 * Idle System Task Code Block 
 *
 */
PRIVATE KERNEL_TASK_START_POINT(IdleTaskFunc)
{
    while (1)
    {
        /* For now just yield */
        OS_Yield();
    }
}

/*
 * Initialize a new task
 * 
 * @param tcb to be initialized new task (TSB)
 */
PRIVATE ALWAYS_INLINE void InitializeNewTask(TCB* newTCB)
{
	UserTaskBaseType* userTask = newTCB->userTaskInfo;
    
	/* Initialize stack of user task according to CPU architecture */
	newTCB->topOfStack = Kernel_InitializeTaskStack(userTask->stack,
                                                    userTask->stackSize,
                                                    userTask->taskStartPoint);
}

/**
 * Starts Kernel after initialization
 * INLINED to avoid function call overhead.
 *
 * @param none
 *
 * @return none
 */
PRIVATE ALWAYS_INLINE void StartScheduling(void)
{
	Kernel_StartContextSwitching((reg32_t*)&idleTaskTCB);
}

/**
 * Initializes all tasks (TCB) for scheduling. 
 *
 * @param none
 *
 * @return none
 */
PRIVATE ALWAYS_INLINE void InitializeAllTasks(void)
{
	TCB* tcb = &kernelTaskPool[0];
	int32_t taskIndex = 0;
	/*
	 * Each User task creates its type and we collect user tasks in a single
	 * container (startupApplications) to manage startup applications.
	 * To do that we defined this container type as "void* array".
	 * While we can not use pointer operations on "void* arrays", we cast this
	 * array to a non - typeless array (int* as a word of a CPU which  most
	 * suitable type for pointer array arithmetic)
	 *
	 */
    int* appPtr = (int*)startupApplications;

	/* Initialize user tasks */
    for (taskIndex = 0; taskIndex < NUM_OF_USER_TASKS; taskIndex++, tcb++, appPtr++)
	{
		/* Save User Task Info into TCB */
		tcb->userTaskInfo = (UserTaskBaseType*)*appPtr;

		/* Initialize New Task */
        InitializeNewTask(tcb);
	}

	/* Initialize idle task */
	idleTaskTCB.userTaskInfo = (UserTaskBaseType*)OS_USER_TASK_PREFIX(IdleTask);
    InitializeNewTask(&idleTaskTCB);
}

/**
 * Initializes Kernel and OS Components. Initializes also User Space.
 * INLINED to avoid function call overhead.
 *
 * @param none
 *
 * @return none
 */
PRIVATE ALWAYS_INLINE void InitializeKernel(void)
{
	/* Initialize all tasks before starting scheduling */
	InitializeAllTasks();

	/* Initialize Scheduler */
	Scheduler_Init(kernelTaskPool, &idleTaskTCB, ContextSwitch_Callback);

	/* Initialize User Space */
	OS_InitializeUserSpace();
}

/***************************** PUBLIC FUNCTIONS *******************************/
PUBLIC void OS_Yield(void)
{
    /*
     * Just call the Scheduler Yield function, Scheduler also notifies
     * about next task using callback function.
     */
    Scheduler_Yield();
}

/**
 * Kernel Start point.
 * Kernel is the owner of main function to start itself after system power-up. 
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

	/* Start Task Scheduling */
	StartScheduling();

	return 0;
}
