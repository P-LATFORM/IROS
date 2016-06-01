/*******************************************************************************
 *
 * @file CPU.c
 *
 * @author Murat Cakmak
 *
 * @brief CPU specific (Cortex-M3) hardware abstraction implementation.
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

/********************************* INCLUDES ***********************************/
#include "CPU.h"
#include "CPU_Internal.h"

#include "GPIO.h"
#include "LPC17xx.h"
#include "postypes.h"

/***************************** MACRO DEFINITIONS ******************************/

/* Index of PENDSV Interrupt in System Handlers Priority (SCB->SHP) Register */
#define SCB_SHP_PENDSV_INDEX			(10)

/* Index of PENDSV Interrupt in System Handlers Priority (SCB->SHP) Register */
#define SCB_SHP_SYSTICK_INDEX			(11)

/* Initial Stack Value for Program Status Register (PSR) . */
#define TASK_INITIAL_PSR				(0x01000000)

/* 
 * PC (Program Counter) mask for task
 * Cortex-M spec offers that first bit (bit-0) of task start address should be 
 * zero. 
 */
#define TASK_START_ADDRESS_MASK			((reg32_t)0xfffffffeUL)

/* 
 * Priority of Kernel Interrupts. 
 *  Lowest priority!
 */
#define KERNEL_INTERRUPT_PRIORITY       (255)

/***************************** TYPE DEFINITIONS *******************************/

/**************************** FUNCTION PROTOTYPES *****************************/

/******************************** VARIABLES ***********************************/

/*
 * Current (Running) Task.
 *  Keeps reference of Running Task TCB.
 */
reg32_t* currentTask;

/*
 * To be handled next task. 
 * When a context switching occurs, currentTask will be replace with this item
 */
PRIVATE reg32_t* nextTask;

/**************************** PRIVATE FUNCTIONS ******************************/

/*
 * Switches Context from Running to Next (Selected) Task
 */
void SwitchContext(void)
{
    /* TODO allow context switch if scheduler suspended */
    
    /* TODO Check for stack overflow */
    
    currentTask = nextTask;
}

/*
 *  A task must not exit or return to its caller. If so break all execution. 
 */
PRIVATE void ErrorOnTaskExit(void)
{
	CPU_Halt();
}

/***************************** PUBLIC FUNCTIONS *******************************/
/*
 * Initializes actual CPU and its components/peripherals.
 *
 * @param none
 * @return none
 */
void CPU_Init(void)
{
	/*
	 * TODO Implement Core Initialization.
	 *
	 * Currently Core initialization done by System_Init function before
	 * main() function. We can move it here for coding consistancy.
	 */

	/* Initialize GPIO HW */
	GPIO_Init();
}

/*
 * Halts all system. 
 */
void CPU_Halt(void)
{
	/* Disable interrupts to avoid execution thru interrupts */
	__disable_irq();

	/* Block Execution */
	ENDLESS_WHILE_LOOP;
}

/*
 * Starts Context Switching
 *  Configures HW for CS and starts first task
 *  
 * @param initialTCB Initial (First) TCB (Task) for Context Switching
 *
 * @return none
 */
void CPU_CS_Start(reg32_t* initialTCB)
{
	/* For the first time let's assign current and next task using initial TCB */
    currentTask = initialTCB;
	nextTask = initialTCB;
    
	/* Disable interrupts to avoid any interruption during Context Switching Initialization */
	__disable_irq();
    
    /*
     * Set priorities for Kernel Interrupts using System Handlers Priority (SHP)
     *  SHB Registers are byte accessible. 
     *  - PENDSV    : SHB[10]
     *  - SYSTICK   : SHB[11]
     */
    /* TODO should we move to more common area than this function */
	SCB->SHP[SCB_SHP_PENDSV_INDEX] = KERNEL_INTERRUPT_PRIORITY;
    
    /* 
	 * Everything for CS is ready now let's start first task
	 * Switch to First */
    SwitchToFirstTask();
}

/*
 * Switches running task to provided new TCB
 *
 * @param newTCB to be switched TCB
 * @param none
 * 
 */
void CPU_CS_YieldTo(reg32_t* newTCB)
{
	/* Save TCB for task switching */
    nextTask = newTCB;
    
	/* Set a PendSV to request a context switch. */
    SCB->ICSR = (reg32_t)SCB_ICSR_PENDSVSET_Msk;
	
	/*     
     * Barriers are normally not required but do ensure the code is completely
	 * within the specified behaviour for the architecture. (Note From FreeRTOS)
     */
    __DMB();
}

/*
 * Initializes task stack according to Cortex-M3 Architecture.   
 *
 * @param stack to be initialized task stack
 * @param stackSize Stack Size
 * @param taskStartPoint Start Point (Function) of Task
 *
 * @return top of stack after initialization. 
 */
PUBLIC reg32_t* CPU_CS_InitializeTaskStack(uint8_t* stack, uint32_t stackSize, TaskStartPoint taskStartPoint)
{

	reg32_t* topOfStack = (reg32_t*)stack;
	uint32_t stackDepth = stackSize / sizeof(int);

	/* Calculate the top of user stack address which aligned */
	topOfStack = topOfStack + (stackDepth - 1);

	topOfStack = (reg32_t*)(((uintptr_t)topOfStack) & (~(0x0007)));

	topOfStack--;
	*topOfStack = TASK_INITIAL_PSR;  /* Offset added to account for the way the MCU uses the stack on entry/exit of interrupts. */
	topOfStack--;
	*topOfStack = ((uintptr_t)taskStartPoint) & TASK_START_ADDRESS_MASK;	/* PC */
	topOfStack--;
	*topOfStack = (uintptr_t)ErrorOnTaskExit; /* LR */
	topOfStack -= 5; 										/* R12, R3, R2 and R1. */
	*topOfStack = (uintptr_t)NULL; 							/* R0 */
	topOfStack -= 8; 	/* R11, R10, R9, R8, R7, R6, R5 and R4. */

	/* Save actual stack address for execution start */
	return topOfStack;
}
