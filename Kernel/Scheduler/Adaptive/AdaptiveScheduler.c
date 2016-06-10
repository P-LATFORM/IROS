/*******************************************************************************
 *
 * @file AdaptiveScheduler.c
 *
 * @author Murat Cakmak
 *
 * @brief Adaptive Scheduler Implementation.
 *
 *          This file implements innovative scheduling paradigms which apply
 *          Control Theory approaches.
 *          Founders are A. Leva, M. Maggio, A. V. Papadopulos and F. Terraneo
 *          Please see following book for details.
 *           - Control-Based Operating System Design
 *              ( http://www.theiet.org/resources/books/control/cbosd.cfm )
 *
 *
 *          Herein, we just implement an I+PI Task scheduler which suggested at
 *          mentioned book in section 5.2 and see Algorithm 5.1
 *          (I + PI Algorithm)
 *
 *          References
 *          [1] Control-Based Operating System Design, A. Leva, M. Maggio,
 *              A. V. Papadopulos and F. Terraneo, 2013
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
#include "OSConfig.h"

#if (OS_SCHEDULER == OS_SCHEDULER_ADAPTIVE)

/********************************* INCLUDES ***********************************/
#include "Kernel.h"
#include "Kernel_Internal.h"
#include "Scheduler.h"

#include "AdaptiveScheduler_Internal.h"

#include "Debug.h"
#include "postypes.h"

/***************************** MACRO DEFINITIONS ******************************/
#if OS_TASK_CREATION == OS_TASK_CREATION_STATIC

#define AS_ENABLE_REINIT_REGULATOR          0

#else

#define AS_ENABLE_REINIT_REGULATOR          1

#endif

/* Task count */
#define TASK_COUNT                          NUM_OF_USER_TASKS

/*
 * Boundary check for burst value and return burst value in valid range
 *
 * Burst value must NOT be
 *  - smaller than minimum burst time
 *  - higher  than maximum burst time
 */
#define AS_BURST_BOUNDARY_FIX(burst) \
            MATH_MIN(MATH_MAX(burst, (int32_t)(AS_BURST_MIN_IN_US * AS_MULT_FACTOR)), \
						 (int32_t)(AS_BURST_MAX_IN_US * AS_MULT_FACTOR));

/*
 * Adaptive scheduler is an internal object and external objects
 * (e.g. User Application) cannot access and cannot pass parameter into
 * this module. And assume that OS modules are robust and do not pass erronous
 * parameters so no need to check parameter for public function to avoid
 * unnecessary checks.
 */
#define AS_PARAMETER_CHECK(...)

/***************************** TYPE DEFINITIONS *******************************/

/*
 * Controller State Variables/Parameters
 *
 *  Collects variables of Inner Loop which are used in I + PI controller.
 *
 *  Please see sections 5.1 and 5.2 in [1] for details of variables.
 */
typedef struct
{
    /* Actual (Measured) Processing (Burst) Time of Task */
	uint32_t tProcess;
    /* Set point for burst time */
	uint32_t tProcessSetPoint;
    /* Previous burst time */
	uint32_t tBurstOld;
    /*
     * Task's alpha to distrubute burst correction onto tasks.
     *  See section 5.2.2 (Outer loop) in [1] for details.
     */
	float alpha;
} TaskStateVariables;

/*
 * Task Information
 *
 *  TODO [IMP] This type definition keeps common task info for all kernel so should be
 *  moved to common area.
 *
 */
typedef struct TaskInfo
{
    /* Task flags */
	struct
	{
		uint32_t ready : 1;
        uint32_t __reserved : 31;
	} flags;

    /* Controller State Variables for Task */
    TaskStateVariables stateVariables;

    /* Task State */
    OSTaskState state;

    /*
     * Task Control Block (TCB) Reference.
     *  Kernel interests only TCB for context switching, when scheduler finds
     *  the next task, just provides next task's TCB to kernel.
     */
    TCB* tcb;
} TaskInfo;

/*
 * Controller state variables/parameters.
 *
 *  Collects variables of Outer Loop which are used in I + PI controller.
 *
 *  Please see sections 5.1 and 5.2 in [1] for details of variables.
 */
typedef struct
{
    /* Actual (Measured) round time */
    uint32_t tRound;
    /* Set point for round */
    uint32_t tRoundSetPoint;
    /* Previous Burst Correction Value */
    int32_t burstCorrectionOld;
    /* Previous Round Error */
    int32_t errRoundOld;
} SchedulerStateVariables;

/*
 * Scheduler Data Structure.
 */
typedef struct
{
    /* Scheduler flags */
	struct
	{
        /* Indicates whether if Scheduler initialized or not */
		uint32_t initialized : 1;
        /* Indicates whether if current task is idle or not */
		uint32_t taskIsIdle : 1;
        /* Indicates whether if all ready tasks are saturated or not*/
		uint32_t allReadyTasksSaturated : 1;
        /* Indicates whether if there is a request to re-init regulator */
		uint32_t reInitRegulator : 1;
        uint32_t __reserved : 28;
	} flags;

    /*
     * Burst Timer
     * This timer preempts the actual task when task burst time is completed.
     */
    KernelTimerHandle timer;

    /* Client (Kernel) callback function to notify kernel for to be run task */
    SchedulerCSCallback csCallback;

    /* Task List (Pool) to collect all user tasks. */
    TaskInfo taskList[TASK_COUNT];
    /* Idle task */
	TaskInfo idleTask;
    /* Reference to Current (Running) task*/
    TaskInfo* currentTask;

    SchedulerStateVariables stateVariables;
} SchedulerData;
/**************************** FUNCTION PROTOTYPES *****************************/

/******************************** VARIABLES ***********************************/

/*
 * Scheduler (Internal) Data
 */
PRIVATE SchedulerData scheduler;

/*
 * Just a flag to understand we reached to end of task list.
 */
PRIVATE const TaskInfo* const LAST_TASK = &scheduler.taskList[TASK_COUNT];

/**************************** PRIVATE FUNCTIONS ******************************/

/*
 * Interrupt Service Routine (ISR) to handle Timer Timouts
 */
void ISR_BurstTimer(void) NO_INLINE;
void ISR_BurstTimer(void)
{
    /*
     * Running task's burst time is ended and preempted.
     * Therfore, yield to next task
     */
    Scheduler_Yield();
}

/*
 * Sets preemption timer for next Tasks.
 *
 * @param burstTimeInUs timer timeout value in microseconds to preempt running
 *        task at the end of its burst time
 *
 * @return none
 */
PRIVATE ALWAYS_INLINE void SetBurstTimer(uint32_t burstTimeInUs)
{
	/* Set timer for IDLE Task Burst */
    Kernel_StartPreemptionTimer(scheduler.timer, burstTimeInUs);
}

#if AS_ENABLE_REINIT_REGULATOR
/*
 * Resets Regulator and state variables
 *
 * @param none
 * @return none
 */
PRIVATE ALWAYS_INLINE void ResetRegulator(void)
{
    SchedulerStateVariables* state = &scheduler.stateVariables;

    state->tRound = 0;
    state->errRoundOld = 0;
    state->burstCorrectionOld = 0;
}
#endif

/*
 * Runs internal regulator to tune system parameters.
 *
 *  Called at the begining of the round.
 *
 * @param none
 * @return none
 */
PRIVATE ALWAYS_INLINE void RunRegulator(void)
{
    /* Shortcuts to state variables*/
    SchedulerStateVariables* state = &scheduler.stateVariables;
    TaskStateVariables* taskState;
    float nextRoundTime;
	TaskInfo* task;
	int32_t errorTProcess = 0;
	int32_t burst = 0;
    int32_t errRound = 0;
    int32_t burstCorrection = 0;

#if AS_ENABLE_REINIT_REGULATOR
	if (scheduler.flags.reInitRegulator == BOOL_FALSE)
#endif /* AS_ENABLE_REINIT_REGULATOR */
	{
        /*
         * Calculate burst correction in PI controller (outer loop)
         *
         *   bc(k) = bc(k - 1) +
         *           KRR * ( Tr0(k - 1) - Tr(k-1) ) -
         *           KRR * ZRR * (Tr0(k - 2) - Tr(k - 2))
         *
         */

        /* Get Round Error : Tr0(k - 1) - Tr(k-1) */
		errRound = state->tRoundSetPoint - state->tRound;

		burstCorrection = state->burstCorrectionOld +
                          (int32_t)( (AS_KRR * errRound) - (AS_KRR * AS_ZRR * state->errRoundOld));

        /* Apply Saturations */
		if (scheduler.flags.allReadyTasksSaturated == BOOL_TRUE)
        {
            /* Clear flag first */
            scheduler.flags.allReadyTasksSaturated = BOOL_FALSE;

            /*
             * Miosix Note : If all inner regulators reached upper saturation,
             * allow only a decrease in the burst correction.
             */
            if (burstCorrection < state->burstCorrectionOld)
			{
				state->burstCorrectionOld = burstCorrection;
			}
		}
		else
		{
            /* Save burst correction value for next iteration in I Controller */
			state->burstCorrectionOld = burstCorrection;
		}

        /* Boundary check and fix for burst correction */
		state->burstCorrectionOld =
            MATH_MIN((uint32_t)MATH_MAX(state->burstCorrectionOld, -(int32_t)state->tRound),
					 (uint32_t)AS_BURST_MAX_IN_US * TASK_COUNT);

        /* Calculate Next Round time using burst correction value */
		nextRoundTime = (float)(state->tRound + state->burstCorrectionOld);

        /* Save Round Error for next iteration in Controller */
		state->errRoundOld = errRound;

        /* Reset (actual/measured)round time */
		state->tRound = 0;

        /* Visit all tasks to calculate their burst times */
        for (task = &scheduler.taskList[0]; task != LAST_TASK; task++)
		{
            taskState = &task->stateVariables;

            /*
             * Calculate set point for process (burst) time.
             * Each task has its alpha and round time is shared between task
             * using alpha.
             *
             *   Tt(k) = a(k) * (Tr(k) + bc(k))
             */
            taskState->tProcessSetPoint = (uint32_t)(taskState->alpha * nextRoundTime);


            /*
             * Calculate Burst Time in I Controller (Inner Loop)
             *
             *   b(k) = b(k - 1) + kI (Tt0(k - 1) - Tt(k - 1))
             */
            /* Process (Burst) Time Error Tt0(k - 1) - Tt(k - 1) */
			errorTProcess = taskState->tProcessSetPoint - taskState->tProcess;

            /* Task Burst Time for next Round */
			burst = taskState->tBurstOld + errorTProcess;

            /* Boundary check for burst time */
			taskState->tBurstOld = AS_BURST_BOUNDARY_FIX(burst);
		}
	}
#if AS_ENABLE_REINIT_REGULATOR
	else
	{
        /* Clear flag first */
		scheduler.flags.reInitRegulator = BOOL_FALSE;

		/* Reset Regulator first in case of reinitialization of regulator */
        ResetRegulator();

        /* Visit all tasks to calculate their burst times */
		for (task = &scheduler.taskList[0]; task != LAST_TASK; task++)
		{
            taskState = &task->stateVariables;

            /* Reset Process set point to initial value using Round Set Point */
			taskState->tProcessSetPoint = (uint32_t)(taskState->alpha * state->tRoundSetPoint);

            /* Reset to initial Burst value*/
			burst = (int32_t)(taskState->tProcessSetPoint * AS_MULT_FACTOR);

            /* Boundary check for burst time */
			taskState->tBurstOld = AS_BURST_BOUNDARY_FIX(burst);
		}
	}
#endif /* AS_ENABLE_REINIT_REGULATOR */

}

/*
 * Find a (next) ready task from Task Pool.
 *  It is also set the preemption timer for next tasks
 *
 * @param setBurstTimer Flag to set Burst Timer for preemption or not.
 * @return none
 *
 */
PRIVATE void FindNextTask(uint32_t setBurstTimer)
{
    TaskInfo* nextTask;
    uint32_t tProcess;
	uint32_t nextBurstTime;

	/* Do not evaluate idle task */
    if (scheduler.flags.taskIsIdle == 0)
	{
	/* Measure actual burst time */
        tProcess = Kernel_GetPreemptionTimeStamp(scheduler.timer);

        /* Save burst time into task */
        scheduler.currentTask->stateVariables.tProcess = tProcess;

        /* Add burst time to obtain actual round time */
		scheduler.stateVariables.tRound += tProcess;
	}
    else
    {
        /* Clear IDLE Task flag */
        scheduler.flags.taskIsIdle = BOOL_FALSE;
    }

    /* Start with current task to find next task */
    nextTask = scheduler.currentTask;

	while (1)
	{
		/* Get next in the task list */
        nextTask++;

        /* If we reach the last task, we need to re-evaluate task list. */
        if (nextTask == LAST_TASK)
		{
            /*
             * TODO
             * 	- Find ready task count.
             * 	  If there is no ready task, run the idle task.
             *
             * 	  TODO For now, do not run idle task until support task states.
             *
             * 	- Find out whether all threads are saturated or not.
             * 	  if All task's tBurstOld value is higher than BURST_MAX_IN_US,
             * 	  it means all tasks are saturated and this information should
             * 	  be passed to regulator.
             *
             * 	  [IMP] : This block works for each round so cost of finding
             * 	  ready task count and saturation check is amortized to number
             * 	  of tasks. On the other hand, even if it is amortized, it still
             *    increases the grown rate asymptotically by n so it adds
             *    additional cost for context switching at the beginning of each
             *    round. Instead, we may distribute these findings to each
             *    context switching or we may find out a way which does not need
             *    to visit all task to get ready task count or saturations.
             */

            if (0) /* Do not run idle task until */
            //TODO if (NUM of READ TASKS EQUALS TO 0)
            {
				/* If there is no ready task, we run idle task */
                nextTask = &scheduler.idleTask;

				/* Set Burst Time for Idle Task */
                nextBurstTime = AS_IDLE_THREAD_BURST_IN_US;

                /* Notify about IDLE Task */
				scheduler.flags.taskIsIdle = BOOL_TRUE;

                /* next task (idle) is found so break to search */
				break;
			}

			/* Round is completed, turn back to first task for new round*/
            nextTask = &scheduler.taskList[0];

			/* Run regulator to tune system parameters */
            RunRegulator();
		}

        /*
         * TODO [IMP] Until we support task state feature, all task are in
         * ready state for now.
         */
		if (nextTask->state == OSTaskState_Ready)
		{
			/* Calculate Burst Time for Next Task */
            nextBurstTime = nextTask->stateVariables.tBurstOld / AS_MULT_FACTOR;

            /* next task (and its burst time) is found so break the search */
            break;
		}
		else
		{
			/*
			 * This task is not ready so it is not run in current round.
			 * So, it is processing (burst) time is just zero.
			 */
			nextTask->stateVariables.tProcess = 0;

			/*
			 * Do not break searching here and continue to find a ready task.
			 */
		}
	}

	/* Save next task*/
    scheduler.currentTask = nextTask;

    /*
     * Set burst timer of current task to preempt task at the end of
     * its assigned burst time.
     *
     * TODO Why do we set time in FindNextTask() function? It can be set in
     *      different place.
     */
    if (setBurstTimer == BOOL_TRUE)
    {
        SetBurstTimer(nextBurstTime);
    }
}

/*
 * Initializes tasks for Adaptive Scheduling and prepares task state variables
 *
 * @param tcbList list of to be initialized tasks (TCBs)
 *
 */
PRIVATE ALWAYS_INLINE void InitializeTasks(TCB* tcbList)
{
    /* User TCB */
    TCB* tcb = &tcbList[0];
    /* Internal task object for adaptive scheduling */
    TaskInfo* task;
    /* Keeps sum of priorities of all tasks */
    uint32_t sumOfPriorities = 0;

    for (task = &scheduler.taskList[0]; task != LAST_TASK; task++, tcb++)
    {
        /* First save user tcb into internal task object */
        task->tcb = tcb;

        /* When a task is created, it should be in ready state */
        task->state = OSTaskState_Ready;

        /* Initial burst value for Task */
        task->stateVariables.tBurstOld = AS_BURST_NOMINAL_IN_US * AS_MULT_FACTOR;

        /* Calculate the sum of Priorities */
        sumOfPriorities += tcb->userTaskInfo->priority + 1;
    }

    /*
     * Find the alpha for each task.
     *  alpha = TaskPriority / TotalPriority
     */
    for (task = &scheduler.taskList[0]; task != LAST_TASK; task++)
    {
        task->stateVariables.alpha = ((float)(task->tcb->userTaskInfo->priority + 1)) / sumOfPriorities;
    }

#if AS_ENABLE_REINIT_REGULATOR
    /* Reinit Regulator for the first time */
    scheduler.flags.reInitRegulator = BOOL_TRUE;
#endif /* AS_ENABLE_REINIT_REGULATOR */
}

/*
 * Initializes Scheduler and its state variables.
 *
 * @param none
 * @return none
 *
 */
PRIVATE ALWAYS_INLINE void InitializeScheduler(void)
{
    /* For now, Set point for Round Time  is constant */
    scheduler.stateVariables.tRoundSetPoint = TASK_COUNT * AS_BURST_NOMINAL_IN_US;

    /*
     * IMP Following initializations seem hacky but to avoid unnecesssary if checks
     * for each context switch, they are helpfull.
     */

    /*
     * FindNextTask() function increases the current task (to get next task)
     * first so let's start with -1 indexed item to run first
     */
    scheduler.currentTask = scheduler.taskList;
    scheduler.currentTask--;

    /*
     * FindNextTask measures elapsed time for running task but for the first
     * time, there is no running time so we should discard time measurement.
     * FindNextTask only discard measurement if running task is idle.
     */
    scheduler.flags.taskIsIdle = 1;
}

/***************************** PUBLIC FUNCTIONS *******************************/
/*
 * Initializes Adaptive Scheduler
 *
 *  There is no implementation specific details for developers.
 */
PUBLIC void Scheduler_Init(TCB* tcbList, TCB* idleTCB, SchedulerCSCallback csCallback)
{
    AS_PARAMETER_CHECK(tcbList, idleTCB, csCallback);

    /* Save client(kernel) callback to notify when Context Switching required */
    scheduler.csCallback = csCallback;

    /*
     * Create timer to preempt running task.
     */
    scheduler.timer = Kernel_CreatePreemptionTimer(SYSTEM_TIMER_KERNEL,
                                                   KERNEL_TIMER_PRIORITY,
                                                   ISR_BurstTimer);
    /* Save Idle TCB to run idle task if there is no ready task in next round*/
    scheduler.idleTask.tcb = idleTCB;

    /* Initialize Tasks for Adaptive Scheduling */
    InitializeTasks(tcbList);

    /* Initialize Scheduler and state variables for Adaptive Scheduler */
    InitializeScheduler();
}

PUBLIC void Scheduler_Yield(void)
{
	/* First find next task */
    FindNextTask(BOOL_TRUE);

    /* Notify kernel and pass next task (TCB) */
    scheduler.csCallback(scheduler.currentTask->tcb);
}

PUBLIC TCB* Scheduler_GetNextTCBs(void)
{
    FindNextTask(BOOL_TRUE);

    return scheduler.currentTask->tcb;
}

#endif /* #if (OS_SCHEDULER == OS_SCHEDULER_ADAPTIVE) */
