/*******************************************************************************
 *
 * @file DrvCPUCoreAssembly.c
 *
 * @author Murat Cakmak
 *
 * @brief CPU Core specific (Cortex-M3) driver implementation for assembly
 * 		  functions.
 *
 *        Includes two type of assembly code of same implementations
 *        	- ARMCC Assembly
 *        	- GCC (GNU C) Assembly
 *
 *
 *		  [ IMPORTANT NOTE! ]
 *        In that implementation we used FreeRTOS Context Switching
 *        implementation (except small modifications) and according to FreeRTOS
 *        license, P-OS guarantees that P-OS and FreeRTOS performances will not
 *        be compared.
 *        TODO : Implement own Context Switching according to P-OS Requirements.
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

#include "Drv_CPUCore_Internal.h"
#include "postypes.h"

/***************************** MACRO DEFINITIONS ******************************/

/***************************** TYPE DEFINITIONS *******************************/

/**************************** FUNCTION PROTOTYPES *****************************/

/******************************** VARIABLES ***********************************/

/***************************** PRIVATE FUNCTIONS ******************************/

/***************************** PUBLIC FUNCTIONS *******************************/

#if defined(__ARMCC_VERSION) /* ARMCC Assembly Area */

#error "ARMCC support not defined yet!"

#else /* GNU C - GCC Assembly Area */

/*
 * TODO : [IMP] Until we use assembly code, we will not test Assembly modules.
 * So compile assembly blocks only for "Project Builds"
 */
#if !defined(UNIT_TEST)

/*
 * Note: By default GNU ARM compiler stores and restore a Frame Pointer
 * using "r7" and do stack alignment when entering into and exiting functions.
 * To avoid such optimizations we need to declare our handlers as "naked".
 *
 * REF : http://www.thewireframecommunity.com/writing-a-basic-multitasking-os-for-arm-cortex-m3-processor
 *
 */
ASSEMBLY_FUNCTION void POS_PendSV_Handler(void)  	__attribute__ ((naked));
ASSEMBLY_FUNCTION void POS_SVC_Handler(void) 		__attribute__ ((naked));

/*
 * ISR for PendSV Exception
 *
 *  We use PendSV Interrupts for Context Switching
 *
 * Implementation copied from FreeRTOS
 */
ASSEMBLY_FUNCTION void POS_PendSV_Handler(void)
{
	__asm volatile
	(
			"	mrs r0, psp							\n"
			"										\n"
			"	ldr	r3, currentTCBConst2				\n" /* Get the location of the current TCB. */
			"	ldr	r2, [r3]						\n"
			"										\n"
			"	sub r0, r0, #32						\n" /* Make space for the remaining low registers. */
			"	str r0, [r2]						\n" /* Save the new top of stack. */
			"	stmia r0!, {r4-r7}					\n" /* Store the low registers that are not saved automatically. */
			" 	mov r4, r8							\n" /* Store the high registers. */
			" 	mov r5, r9							\n"
			" 	mov r6, r10							\n"
			" 	mov r7, r11							\n"
			" 	stmia r0!, {r4-r7}              	\n"
			"										\n"
			"	push {r3, r14}						\n"
			"	cpsid i								\n"
			"	bl SwitchContext					\n"
			"	cpsie i								\n"
			"	pop {r2, r3}						\n" /* lr goes in r3. r2 now holds tcb pointer. */
			"										\n"
			"	ldr r1, [r2]						\n"
			"	ldr r0, [r1]						\n" /* The first item in pxCurrentTCB is the task top of stack. */
			"	add r0, r0, #16						\n" /* Move to the high registers. */
			"	ldmia r0!, {r4-r7}					\n" /* Pop the high registers. */
			" 	mov r8, r4							\n"
			" 	mov r9, r5							\n"
			" 	mov r10, r6							\n"
			" 	mov r11, r7							\n"
			"										\n"
			"	msr psp, r0							\n" /* Remember the new top of stack for the task. */
			"										\n"
			"	sub r0, r0, #32						\n" /* Go back for the low registers that are not automatically restored. */
			" 	ldmia r0!, {r4-r7}              	\n" /* Pop low registers.  */
			"										\n"
			"	bx r3								\n"
			"										\n"
			"	.align 2							\n"
			"currentTCBConst2: .word currentTCB		  "
	);
}

/*
 * ISR for SVC Exception
 *
 *  We use SVC Interrupt to start Context Switching
 *
 * Implementation copied from FreeRTOS
 */
ASSEMBLY_FUNCTION void POS_SVC_Handler(void)
{
	/*
	 * This function is no longer used, but retained for backward compatibility.
	 */
}

/*
 * Starts context switching by calling SVC interrupt
 *
 * @param none
 *
 * @return This function does not return, jumps to SVC ISR and SVC ISR continues
 *         with the Kernel (and user) defined tasks.
 *
 */
ASSEMBLY_FUNCTION void StartContextSwitching(void)
{
	/* The MSP stack is not reset as, unlike on M3/4 parts, there is no vector
	table offset register that can be used to locate the initial stack value.
	Not all M0 parts have the application vector table at address 0. */
	__asm volatile(
		"	ldr	r2, currentTCBConst			\n" /* Obtain location of pxCurrentTCB. */
		"	ldr r3, [r2]					\n"
		"	ldr r0, [r3]					\n" /* The first item in pxCurrentTCB is the task top of stack. */
		"	add r0, #32						\n" /* Discard everything up to r0. */
		"	msr psp, r0						\n" /* This is now the new top of stack to use in the task. */
		"	movs r0, #2						\n" /* Switch to the psp stack. */
		"	msr CONTROL, r0					\n"
		"	pop {r0-r5}						\n" /* Pop the registers that are saved automatically. */
		"	mov lr, r5						\n" /* lr is now in r5. */
		"	cpsie i							\n" /* The first task has its context and interrupts can be enabled. */
		"	pop {pc}						\n" /* Finally, pop the PC to jump to the user defined task code. */
		"									\n"
		"	.align 2						\n"
		"currentTCBConst: .word currentTCB	  "
				  );
}

#endif /* #if !defined(UNITY) */

#endif /* #if ARMCC | GCC */
