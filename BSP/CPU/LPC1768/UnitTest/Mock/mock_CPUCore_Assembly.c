/*******************************************************************************
 *
 * @file mockCPUCore_Assembly.c
 *
 * @author Murat Cakmak
 *
 * @brief Mock Implementation for Assembly functions of CPU
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
#include "LPC17xx.h"

/***************************** MACRO DEFINITIONS ******************************/

/***************************** TYPE DEFINITIONS *******************************/

/**************************** FUNCTION PROTOTYPES *****************************/
extern void SwitchContext(void);

/******************************** VARIABLES ***********************************/

/********************************** FUNCTIONS *********************************/
/*
 *  Mock Implementation of PendSV ISR
 */
void POS_PendSV_Handler(void)
{
	/* PendSV ISR normally calls SwitchContext() function to get next TCB */
	SwitchContext();
}

/*
 * Mock Implementation of SVC ISR
 */
void POS_SVC_Handler(void)
{
	/* Just set flag to inform about SVC Call */
	lpcMockObjects.flags.svc_handler_call = 1;
}

/*
 * Mock Implementation of StartContextSwitching
 */
void StartContextSwitching(void)
{
	/* Original function enables global interrupts before SVC ISR */
	__enable_irq();

	/* Jump to SVC. SVC ISR starts context switching */
	POS_SVC_Handler();
}
