/*******************************************************************************
 *
 * @file Kernel.c
 *
 * @author Murat Cakmak
 *
 * @brief Kernel Implementation. 
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

#include "Drv_CPUCore.h"
#include "Scheduler.h"
#include "Board.h"
#include "UserStartupInfo.h"

#include "postypes.h"

/***************************** MACRO DEFINITIONS ******************************/

/*
 * Number of Kernel Tasks
 *  
 *  - We just have an idle task.
 */
#define NUM_OF_KERNEL_TASKS				(1)

/* 
 * Returns Numbers of User Tasks
 */
#define NUM_OF_USER_TASKS \
			(sizeof(startupApplications) / sizeof(void*))

/*
 * Number of all task including kernel and user tasks
 */
#define NUM_OF_ALL_TASKS \
			(NUM_OF_KERNEL_TASKS + NUM_OF_USER_TASKS)

/*
 * Size of idle stack. 
 *
 *  128 byte should be enough
 */
#define SIZE_OF_IDLE_TASK_STACK			(128)

/*
 * Kernel Task Creater Definition
 */
#define KERNEL_TASK						OS_USER_TASK

/*
 * Start Point Definition (Function Prototype) for Kernel Tasks
 */
#define KERNEL_TASK_START_POINT			OS_USER_TASK_START_POINT

/***************************** TYPE DEFINITIONS *******************************/

/*
 * Base type for User Task
 * 
 *  Each static user task implements its type implicitly using OS_USER_TASK() 
 *  macro and specifies task specific features. e.g stack size. 
 *  We can handle task specific differencies using this base type.
 */
typedef struct
{
	/* 
	 * User Task start point. 
	 *
	 * User task starts with that point (function).
	 * Program Counter (PC) is set to this value when task is started.
	 */
	OSUserTaskStartPoint taskStartPoint;
	/* 
	 * Stack size of User Task
	 */
	uint32_t stackSize;
	/*
	 * Start address of task stack. 
	 *
	 * We use this variable for just start point of user task.
	 * Boundary check can be done by stackSize variable.
	 *
	 * NOTE : While each task has different stack size, we define size as 
	 * '1' to get start point of user stack. It could be zero but zero-sized 
	 * arrays are not portable and some platforms does not support it.
	 *
	 */
	uint8_t stack[1];
} UserTaskBaseType;

/*
 * Task Control Block (TCB)
 *
 *  Includes all task specific information including required information for 
 *	Context Switching. 
 */
typedef struct TCB
{
	/*
	 * Actual top address of stack of task. 
	 * 
	 *  When a user task is started, task starts to use stack from end of
	 *  stack in descending order. 
	 *  When a task preempted, we need to save actual position of stack to 
	 *  backup next execution of task. 
	 *
	 *	[IMP] This value must be first item in that data structure. When we pass
	 *	a TCB to context switcher mechanism, HW looks for first address. 
	 */
	reg32_t* topOfStack;
	
	/*
	 * User defined task Information. 
	 */
	UserTaskBaseType* userTaskInfo;
} TaskInfo, TCB;

/**************************** FUNCTION PROTOTYPES *****************************/
/* 
 * IDLE Task Definitions
 */
PRIVATE OS_USER_TASK_START_POINT(IdleTaskFunc);

/******************************** VARIABLES ***********************************/

/*
 * User Space Applications which are started at startup
 * (after system initialization)
 * 
 * We use following user-defined container to access user applications
 */
extern void* startupApplications[];

/*
 * Idle Task
 * 
 *  We use idle task for side things which maintains system
 */
KERNEL_TASK(IdleTask, IdleTaskFunc, SIZE_OF_IDLE_TASK_STACK);

/*
 * Task Pool. 
 * 
 *  Keeps all kernel and user tasks. 
 */
PRIVATE TaskInfo kernelTaskPool[NUM_OF_ALL_TASKS];

/**************************** PRIVATE FUNCTIONS ******************************/

/*
 * TODO Remove
 * Hack code until we have a Scheduler. 
 * Below function returns all items sequentially from task pool. 
 */
#define Scheduler_GetNextTCB Hack_GetNextTCBFromScheduler
PRIVATE TCB* Hack_GetNextTCBFromScheduler(void)
{
    static int nextTaskIndex = 0;
    TCB* nextTCB;
    
    nextTaskIndex = (nextTaskIndex + 1) % NUM_OF_ALL_TASKS;
    
    nextTCB = &kernelTaskPool[nextTaskIndex];
    
    return nextTCB;
}

/**
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
PRIVATE INLINE void InitializeNewTask(TCB* newTCB)
{
	UserTaskBaseType* userTask = newTCB->userTaskInfo;
    
	/* Initialize stack of user task according to CPU architecture */
	newTCB->topOfStack = Drv_CPUCore_CSInitializeTaskStack(userTask->stack,
                                                           userTask->stackSize,
                                                           userTask->taskStartPoint);
}

/**
 * Initializes all tasks (TCB) for scheduling. 
 *
 * @param none
 *
 * @return none
 */
PRIVATE INLINE void InitializeAllTasks(void)
{
	//UserTaskBaseType* userTask;
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

	/* Loop for only User Tasks. Kernel Tasks are placed at the end of task pool. */
	while (taskIndex < NUM_OF_USER_TASKS)
	{
		/* Save User Task Info into TCB */
		tcb->userTaskInfo = (UserTaskBaseType*)*appPtr;

		/* Initialize New Task */
        InitializeNewTask(tcb);

		/* Increase pointer to get references of next items */
		tcb++;
        appPtr++;
		taskIndex++;
	}

	/* Put idle task to end of task pool and initialize */
	tcb->userTaskInfo = (UserTaskBaseType*)USER_TASK_PREFIX(IdleTask);
    InitializeNewTask(tcb);
}

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
	/* Initialize all tasks before starting scheduling */
	InitializeAllTasks();

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
	TCB* firstTask = &kernelTaskPool[0];

	//kernelSettings.flags.schedulerRunning = 1;	

	Drv_CPUCore_CSStart((reg32_t*)firstTask);

	/* TODO When Scheduler is implemented */
	//Scheduler_Start();
}

/***************************** PUBLIC FUNCTIONS *******************************/
PUBLIC void OS_Yield(void)
{
    TCB* nextTCB = Scheduler_GetNextTCB();
    
    Drv_CPUCore_CSYieldTo((reg32_t*)nextTCB);
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

	/* Start Kernel */
	StartKernel();

	return 0;
}
