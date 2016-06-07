/*******************************************************************************
 *
 * @file Drv_Timer.c
 *
 * @author Murat Cakmak
 *
 * @brief Timer Driver Implementation.
 *
 *			This timer provides one shot timer so Timer is not resetted on
 *			match and just interrupt created. Timer is also disabled on match.
 *
 *			Timer resolution is 1 microsecond so clock dividers and prescale
 *			values are set according to this resolution.
 *
 *        TODO
 *			- Timer Power should be closed when device enter sleep (Low Power)
 *			- IMP : Check for Cost of Timer Enable/Disable for each time.
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
#include "Drv_Timer.h"

#include "LPC17xx.h"
#include "lpc17xx_clkpwr.h"
#include "lpc17xx_timer.h"

#include "Debug.h"
#include "postypes.h"

#include "DRVConfig.h"

/***************************** MACRO DEFINITIONS ******************************/
/*
 * Number of HW Timers in used CPU.
 *  LPC17xx has 4 HW Timer.
 */
#define NUM_OF_HW_TIMERS					(4)

/*
 * If upper layers do not specify HW Timer count, use all HW Timers
 */
#ifndef DRV_CONFIG_NUM_OF_USED_HW_TIMERS
#define DRV_CONFIG_NUM_OF_USED_HW_TIMERS	(4)
#endif	/* DRV_CONFIG_NUM_OF_USED_HW_TIMERS */

/*
 * Check External Configurations
 */
#if DRV_CONFIG_NUM_OF_USED_HW_TIMERS > NUM_OF_HW_TIMERS
#error "Unsupported HW Timer Number. LPC17xx has just 4 HW Timer!"
#endif

/* Just a wrapper for to be used timer number */
#define NUM_OF_TIMERS						DRV_CONFIG_NUM_OF_USED_HW_TIMERS

/* Local Wrapper for global debugging mode */
#define TIMER_DEBUG_MODE					ENABLE_DEBUG_ASSERT

#if TIMER_DEBUG_MODE
	/* Static checks for debugging mode  */

	/*
	 * When a HW Timer initialized and assigned to client using a handle,
	 * we set this value into the timer object. In this way, we can check both
	 * whether timer is valid (initialized) or not.
	 */
	#define TIMER_VALIDATION_KEY			(0x18273645)

	/* Sets validation key to mark timer as initialized. */
	#define TIMER_SET_VALIDATION_KEY(timer)	\
				{ timer->validationKey =  TIMER_VALIDATION_KEY; }

	/* Checks timer is valid. Checks timer objects includes validation key */
	#define TIMER_HANDLE_IS_VALID(timer) \
				(timer->validationKey == TIMER_VALIDATION_KEY)

#else	/* TIMER_DEBUG_MODE */

	/* Empty definitions to remove static check for release versions */
	#define TIMER_SET_VALIDATION_KEY(timer)
	#define TIMER_HANDLE_IS_VALID(timer)

#endif	/* TIMER_DEBUG_MODE */

/* Mask to clear all interrupt pendings. */
#define TIMER_CLEAR_ALL_INT_PENDINGS_MASK \
				TIM_IR_CLR(TIM_MR0_INT) | /* Match channel 0   */ \
				TIM_IR_CLR(TIM_MR1_INT) | /* Match channel 1   */ \
				TIM_IR_CLR(TIM_MR2_INT) | /* Match channel 2   */ \
				TIM_IR_CLR(TIM_MR3_INT) | /* Match channel 3   */ \
				TIM_IR_CLR(TIM_CR0_INT) | /* Capture channel 0 */ \
				TIM_IR_CLR(TIM_CR1_INT)   /* Capture channel 1 */
/*
 * Clock Division Value for Timer Clock
 * 		Timer_Clock = CPU_Clock / TIMER_CLK_DIV
 */
#define TIMER_CLK_DIV						(4)

/*
 * Timer resolution for Milliseconds
 * 	1ms = 1 sec / 1000
 */
#define TIMER_RESOLUTION_MS					(1000)

/*
 * Timer resolution for Microseconds
 * 	1us = 1 sec / 1000000
 */
#define TIMER_RESOLUTION_US					(1000000)

/***************************** TYPE DEFINITIONS *******************************/

/*
 * Structure to keep HW Timer specific information.
 *  There is no specific relation between HW Timer Register addresses and
 *  values so to linearize them for constant time access, we collect HW Timer
 *  specific informations in an array of this structure.
 *
 *  This value keeps HW Values so should be used for Constant
 *  (like register addresses) information.
 */
typedef struct
{
	/* Address of HW Timer Registers */
    LPC_TIM_TypeDef* LPC_TIM;
	/* Value (Mask) for Peripheral Control Block to power up of HW Timer */
    uint32_t PCONP_Value;
} HWTimerInfo;

/*
 * Timer Object to provide SW Timer functionality.
 */
typedef struct
{
	/* Let's add static analysis items in debug mode */
#if TIMER_DEBUG_MODE
	/* Validation Key to mark Timer as initialized and valid. */
	uint32_t validationKey;
#endif /* TIMER_DEBUG_MODE */

	/* Client callback function to inform client about Timer Timeout */
	DrvTimerCallback callback;
	/* Reference to HW Objects (e.g. Registers) */
	const HWTimerInfo* hwTimerInfo;
} Timer;

/**************************** FUNCTION PROTOTYPES *****************************/
PRIVATE void TIMER_IRQHandler(TimerNo timerNo);

/******************************** VARIABLES ***********************************/

/*
 * Information about Timer Hardwares.
 *  This container collects constant information about Timer Hardwares like
 *  Timer Register addresses or constant masks.
 */
PRIVATE const HWTimerInfo HWTimers[] =
{
	/* HW Timer 0 */
    { LPC_TIM0, CLKPWR_PCONP_PCTIM0 },

	/* HW Timer 1 */
#if NUM_OF_TIMERS > 1
    { LPC_TIM1, CLKPWR_PCONP_PCTIM1 },
#endif	/* #if NUM_OF_TIMERS > 1 */

	/* HW Timer 2 */
#if NUM_OF_TIMERS > 2
    { LPC_TIM2, CLKPWR_PCONP_PCTIM2 },
#endif	/* #if NUM_OF_TIMERS > 2 */

	/* HW Timer 3 */
#if NUM_OF_TIMERS > 3
    { LPC_TIM3, CLKPWR_PCONP_PCTIM3 }
#endif	/* #if NUM_OF_TIMERS > 3 */
};

/*
 * All timer objects
 */
PRIVATE Timer timers[NUM_OF_TIMERS];

/*
 * Custom (Emprically Defined) IRQ Priorities for HW Timers
 * 		Cortex M3 NVIC allows priorities between 0~31
 *  	max = 0, min = 31
 */
PRIVATE const uint32_t timerIRQPriorities[] =
{
	3,		/* High Priority */
	9,		/* Midd Priority */
	15		/* Low Priority  */
};
/**************************** PRIVATE FUNCTIONS *******************************/

/*
 * ISR Function for TIMER 0 Interrupt
 */
INTERNAL void POS_TIMER0_IRQHandler(void)
{
	TIMER_IRQHandler(0);
}

/*
 * ISR Function for TIMER 1 Interrupt
 */
INTERNAL void POS_TIMER1_IRQHandler(void)
{
	/* Avoid function call cost in case of Timer is not used */
#if NUM_OF_TIMERS > 1
	TIMER_IRQHandler(1);
#endif /* #if NUM_OF_TIMERS > 1 */
}

/*
 * ISR Function for TIMER 2 Interrupt
 */
INTERNAL void POS_TIMER2_IRQHandler(void)
{
	/* Avoid function call cost in case of Timer is not used */
#if NUM_OF_TIMERS > 2
	TIMER_IRQHandler(2);
#endif /* #if NUM_OF_TIMERS > 2 */
}

/*
 * ISR Function for TIMER 3 Interrupt
 */
INTERNAL void POS_TIMER3_IRQHandler(void)
{
	/* Avoid function call cost in case of Timer is not used */
#if NUM_OF_TIMERS > 3
	TIMER_IRQHandler(3);
#endif /* #if NUM_OF_TIMERS > 3 */
}

/*
 * Comman ISR Function for all Timer Interrupts.
 *  While all ISR functions do same things on different HW Timer registers,
 *  we can collect in single function and pass timer number to get required
 *  Register
 *
 * @param timerNo Number of Timer interrupt source
 */
PRIVATE void TIMER_IRQHandler(TimerNo timerNo)
{
    /* Get HW TIMER Pointer */
    LPC_TIM_TypeDef* LPC_TIM = HWTimers[timerNo].LPC_TIM;

	if ((LPC_TIM->IR) & TIM_IR_CLR(TIM_MR0_INT))
	{
		/* Inform external (client) module if interrupt source is true*/
		timers[timerNo].callback();

		/* Clear Interrupt Pending Flag */
		LPC_TIM->IR = (uint32_t)TIM_IR_CLR(TIM_MR0_INT);
	}
}

/*
 * Set Timer Match Value to fire an Interrupt.
 */
PRIVATE ALWAYS_INLINE void StartTimer(LPC_TIM_TypeDef* LPC_TIM, uint32_t timeoutInUs)
{
	/*
	 * Our timer resolution is 1 uS so we can set tiemout value (in uS) directly
	 * as match value
	 */
	LPC_TIM->MR0 = timeoutInUs;

	/* Reset Timer and Prescale counters */
	LPC_TIM->TC = 0;
	LPC_TIM->PC = 0;

	/* Enable Timer */
    LPC_TIM->TCR |= TIM_ENABLE;
}

/*
 * Initializes selected HW Timer.
 *
 * @param hwTimerInfo object which keeps hw specific information
 */
PRIVATE ALWAYS_INLINE void InitializeHWTimer(const HWTimerInfo* hwTimerInfo)
{
    /* Get HW Timer Register Address */
    LPC_TIM_TypeDef* LPC_TIM = hwTimerInfo->LPC_TIM;

	/* Enable Power of Timer Block */
    LPC_SC->PCONP |= hwTimerInfo->PCONP_Value & CLKPWR_PCONP_BITMASK;

    /*
     * If clock division (CLKPWR_PCLKSEL_CCLK_DIV_4) is selected 4, PCLKSEL register
     * should be set to zero.
     * And default values PCLKSEL registers are zero so while default value means
     * CLK_DIV_4 we do not need to set PCLK_SET again and again. So, closing this code
     * block for CLK_DIV_4
     */
#if TIMER_CLK_DIV != 4
    /* LPC_SC->PCLKSEL<Timer> &= (~(CLKPWR_PCLKSEL_BITMASK(CLKPWR_PCLKSEL_TIMER<Timer>))); */
    /* LPC_SC->PCLKSEL<Timer> |= (CLKPWR_PCLKSEL_SET(CLKPWR_PCLKSEL_TIMER<Timer>, CLKPWR_PCLKSEL_CCLK_DIV_<X>)); */
#endif

    /* Clear capture mode to use HW in Timer Mode*/
	LPC_TIM->CCR &= ~TIM_CTCR_MODE_MASK;
	LPC_TIM->CCR |= TIM_TIMER_MODE;

	/* Clear Prescale Register */
	LPC_TIM->PR =0;

	/* Reset counters and before */
	LPC_TIM->TCR |= TIM_RESET;
	LPC_TIM->TCR &= ~(TIM_RESET);	/* Need to release Reset */

    /*
	 * Set Prescale Value (PR)
	 * When the Prescale Counter (below) is equal to this value, the next clock
	 * increments the TC and clears the PC.
	 *
	 * PR should be set to PrescaleValue - 1
	 */
    LPC_TIM->PR = ((SystemCoreClock) / (TIMER_CLK_DIV * TIMER_RESOLUTION_US)) - 1;

	/* Clear all interrupt pendings */
	LPC_TIM->IR = (uint32_t)TIMER_CLEAR_ALL_INT_PENDINGS_MASK;

    /*
	 * Set match register.
	 *  - Just interrupt on match
	 *  - Stop on match. This is because we are one shot timer.
	 *  - No reset on match
	 */
	LPC_TIM->MCR &=~TIM_MCR_CHANNEL_MASKBIT(0);  /* Clear first */
    LPC_TIM->MCR |= TIM_INT_ON_MATCH(0) | TIM_STOP_ON_MATCH(0);
}

/***************************** PUBLIC FUNCTIONS *******************************/
/*
 * Creates a SW Timer which matches with a HW Timer.
 *
 *  There is no special note about internal implementation details.
 *  See header files to function description.
 */
PUBLIC TimerHandle Drv_Timer_Create(TimerNo timerNo, DrvTimerPriority priority, DrvTimerCallback timerCallback)
{
	Timer* timer;
	IRQn_Type timerIRQNo;

	/* Internal Checks for debug mode */
	DEBUG_ASSERT_MESSAGE((timerNo < NUM_OF_HW_TIMERS), "Invalid Timer No!");

	/*
	 * Get timer objects to fill client info.
     *  Get here because other internal checks may use timer object
	 */
	timer = &timers[timerNo];

	/* Rest of internal checks */
	DEBUG_ASSERT_MESSAGE(TIMER_HANDLE_IS_VALID(timer) == 0, "Timer is already assigned before!");
	DEBUG_ASSERT_MESSAGE((priority < DRV_TIMER_PRI_NUM), "Invalid Timer Priority!");
	DEBUG_ASSERT_MESSAGE(timerCallback != NULL, "Invalid (NULL) Callback!");

	/* Save Client Callback to call in case of timer timeout */
	timer->callback = timerCallback;
	/* Link HW Info with Timer Objects */
	timer->hwTimerInfo = &HWTimers[timerNo];

	/* Initialize Timer HW Block for selected HW Timer */
    InitializeHWTimer(timer->hwTimerInfo);

	/* Calculate Timer IRQ Num. For LPC17xx all of them are sequential */
	timerIRQNo = (IRQn_Type)(TIMER0_IRQn + timerNo);

	/* Set interrupt priority using client's priority request */
	NVIC_SetPriority(timerIRQNo, timerIRQPriorities[priority]);

	/* We can enable interrupt of specified Timer */
    NVIC_EnableIRQ(timerIRQNo);

	/* We initialized timer so we can mark it as validated (initialized) */
	TIMER_SET_VALIDATION_KEY(timer);

	/*
	 * Return internal reference as an integer number as Timer Handle.
	 * This hidden internal reference simplies to manage internal objects
	 */
	return (TimerHandle)timer;
}

/*
 * Starts a Timer.
 *
 *  There is no special note about internal implementation details.
 *  See header files to function description.
 */
PUBLIC void Drv_Timer_Start(TimerHandle timerHandle, uint32_t timeoutInUs)
{
	/* Get internal timer using timer handle */
	Timer* timer = (Timer*)timerHandle;

	/* Internal Checks for debug mode */
	DEBUG_ASSERT_MESSAGE(TIMER_HANDLE_IS_VALID(timer), "Invalid Timer Handle");

	/* Start specified HW Timer with timeout value */
	StartTimer(timer->hwTimerInfo->LPC_TIM, timeoutInUs);
}

/*
 * Reads elapsed time in a Timer.
 *
 *  Our resolution is 1 microseconds so 1 tick means 1 us so we can directly
 *  return timer tick counter as elapsed time.
 */
PUBLIC uint32_t Drv_Timer_ReadElapsedTimeInUs(TimerHandle timerHandle)
{
	/* Get internal timer using timer handle */
	Timer* timer = (Timer*)timerHandle;
    LPC_TIM_TypeDef* LPC_TIM;

	/* Internal checks for debug mode */
	DEBUG_ASSERT_MESSAGE(TIMER_HANDLE_IS_VALID(timer), "Invalid Timer Handle");

	/* Get HW TIMER Register */
    LPC_TIM = timer->hwTimerInfo->LPC_TIM;

	/* Return just tick count (1 tick = 1 us) as elapsed time */
	return (uint32_t)LPC_TIM->TC;
}
