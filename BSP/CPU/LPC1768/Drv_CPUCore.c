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
#include "Drv_CPUCore.h"

#include "Drv_CPUCore_Internal.h"
#include "LPC17xx.h"

#include "postypes.h"

/***************************** MACRO DEFINITIONS ******************************/

/* Index of PENDSV Interrupt in System Handlers Priority (SCB->SHP) Register */
#define SCB_SHP_PENDSV_INDEX			(10)

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
/*
 * Map for Stack Initialization of a Task Stack
 *
 * This struct is a map of a stack so all items should be consequent without
 * gaps. Therefore, we need to pack it to avoid CPU specific alignment and
 * padding even packing has overhead.
 */
TYPEDEF_STRUCT_PACKED
{
	reg32_t R4;
	reg32_t R5;
	reg32_t R6;
	reg32_t R7;
	reg32_t R8;
	reg32_t R9;
	reg32_t R10;
	reg32_t R11;
	reg32_t R0;
	reg32_t R1;
	reg32_t R2;
	reg32_t R3;
	reg32_t R12;
	reg32_t LR;  		/* Link Register */
	reg32_t PC;	 		/* Program Counter */
	reg32_t PSR; 		/* Program Status Register */
} TaskStackMap;

/**************************** FUNCTION PROTOTYPES *****************************/

/******************************** VARIABLES ***********************************/

/*
 * Reference of Current(Running) Task TCB.
 *
 * This variable is marked as internal because Assembly functions which located at different
 * files use this variable for context switching.
 */
INTERNAL reg32_t* currentTCB;

/*
 * To be switched next task.
 * When a context switch occurs, currentTCB is replaced with this TCB
 */
PRIVATE reg32_t* nextTCB;

/**************************** PRIVATE FUNCTIONS ******************************/

/*
 * Switches Context from Running to Next (Selected) Task
 *
 * This function is marked as Internal because Assembly functions which located
 * at different files use this variable for context switching.
 */
INTERNAL void SwitchContext(void)
{
    /* TODO allow context switch if scheduler suspended */
    
    /* TODO Check for stack overflow */
    
	currentTCB = nextTCB;
}

/*
 *  A task must not exit or return to its caller. If so break all execution. 
 */
PRIVATE void ErrorOnTaskExit(void)
{
	Drv_CPUCore_Halt();
}

/***************************** PUBLIC FUNCTIONS *******************************/
/*
 * Initializes actual CPU and its components/peripherals.
 *
 * @param none
 * @return none
 */
void Drv_CPUCore_Init(void)
{
	/*
	 * TODO Implement Core Initialization.
	 *
	 * Currently Core initialization done by System_Init function before
	 * main() function. We can move it here for coding consistency.
	 */
}

/*
 * Halts all system. 
 */
void Drv_CPUCore_Halt(void)
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
void Drv_CPUCore_CSStart(reg32_t* initialTCB)
{
	/* For the first time let's assign current and next task using initial TCB */
    currentTCB = initialTCB;
	nextTCB = initialTCB;
    
	/* Disable interrupts to avoid any interruption during Context Switching Initialization */
	__disable_irq();
    
    /*
     * Set priorities for Kernel Interrupts using System Handlers Priority (SHP)
     *  SHB Registers are byte accessible. 
     *  - PENDSV    : SHB[10]
     */
    /* TODO should we move to more common area than this function */
	SCB->SHP[SCB_SHP_PENDSV_INDEX] = KERNEL_INTERRUPT_PRIORITY;
    
    /* 
	 * Everything for CS is ready now let's trigger Context Switching
	 */
	StartContextSwitching();
}

/*
 * Switches running task to provided new TCB
 *
 * @param newTCB to be switched TCB
 * @param none
 * 
 */
void Drv_CPUCore_CSYieldTo(reg32_t* newTCB)
{
	/* Save TCB for task switching */
    nextTCB = newTCB;
    
	/* Set a PendSV to request a context switch. */
    SCB->ICSR = (reg32_t)SCB_ICSR_PENDSVSET_Msk;
	
	/*     
     * Barriers are normally not required but do ensure the code is completely
	 * within the specified behavior for the architecture. (Note From FreeRTOS)
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
PUBLIC reg32_t* Drv_CPUCore_CSInitializeTaskStack(uint8_t* stack, uint32_t stackSize, Drv_CPUCore_TaskStartPoint taskStartPoint)
{

	reg32_t* topOfStack = (reg32_t*)stack;
	uint32_t stackDepth = stackSize / sizeof(int);
	TaskStackMap* stackMap;

	/* Calculate the top of user stack address which aligned */
	topOfStack = topOfStack + (stackDepth - 1);
	/* Align Stack Address with 8 */
	topOfStack = (reg32_t*)(((uintptr_t)topOfStack) & (~(0x0007)));

	/* Map Stack Address */
	stackMap = (TaskStackMap*)topOfStack;

	/*
	 * Stack uses memory in descending order so after mapping the stack address
	 * with the the top of stack in previous line, we need to decrease this
	 * pointer to access stack content Because C structs work in ascending order
	 */
	stackMap--;

	/*
	 * Stack Initialization
	 */

	/* Set a Program Status Value for initial state */
	stackMap->PSR = TASK_INITIAL_PSR;

	/* Set Program Counter using User Task Start Point (Function) */
	stackMap->PC = ((uintptr_t)taskStartPoint) & TASK_START_ADDRESS_MASK;

	/* Set link register to handle case of fail */
	stackMap->LR = (uintptr_t)ErrorOnTaskExit;

	/* We do not pass argument so R0 must be zero. */
	stackMap->R0 = (uintptr_t)NULL;

	/* Return actual stack address for execution start */
	return (reg32_t*)stackMap;
}
