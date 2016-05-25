/*******************************************************************************
 *
 * @file Timer.c
 *
 * @author Murat Cakmak
 *
 * @brief Timer Driver Implementation
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
#include "Timer.h"

#include "BSP_Config.h"

#include "LPC17xx.h"
#include "lpc17xx_clkpwr.h"
#include "lpc17xx_timer.h"

#include "Debug.h"
#include "postypes.h"

/***************************** MACRO DEFINITIONS ******************************/

/***************************** TYPE DEFINITIONS *******************************/
typedef struct
{
    TimerCallback callback;
    int32_t timeout;
} TimerInfo;
/**************************** FUNCTION PROTOTYPES *****************************/

/******************************** VARIABLES ***********************************/
PRIVATE TimerInfo timers[CPU_TIMER_MAX_TIMER_COUNT];


extern uint32_t SystemCoreClock;
/**************************** PRIVATE FUNCTIONS *******************************/
void TIMER0_IRQHandler(void)
{
	int32_t i;

 	if ((LPC_TIM0->IR) & TIM_IR_CLR(TIM_MR0_INT))
	{
		for(i = (CPU_TIMER_MAX_TIMER_COUNT - 1); i >= 0; i--)	//look all timers
		{
			if(timers[i].timeout > 0)
            {
                timers[i].timeout--;
                if(timers[i].timeout <= 0)
                {				 	
                    timers[i].callback();   /*call callBack function */
                }
            }
		}  
	}

    LPC_TIM0->IR |= TIM_IR_CLR(TIM_MR0_INT);
}

PRIVATE INLINE void InitializeTimerHW(void)
{
    /* Enable Timer Block Power */    
    LPC_SC->PCONP |= CLKPWR_PCONP_PCTIM0 & CLKPWR_PCONP_BITMASK;
    
    /* Set Clock Divi Val */
	LPC_SC->PCLKSEL0 &= (~(CLKPWR_PCLKSEL_BITMASK(CLKPWR_PCLKSEL_TIMER0)));  /* Clear register */
    LPC_SC->PCLKSEL0 |= (CLKPWR_PCLKSEL_SET(CLKPWR_PCLKSEL_TIMER0, CLKPWR_PCLKSEL_CCLK_DIV_4)); /* Set two selected bit */
	
    /* TODO add comments about following registers */
	LPC_TIM0->CCR &= ~TIM_CTCR_MODE_MASK;
	LPC_TIM0->CCR |= TIM_TIMER_MODE;
	LPC_TIM0->TC =0;
	LPC_TIM0->PC =0;
	LPC_TIM0->PR =0;
	LPC_TIM0->TCR |= (1<<1); //Reset Counter
	LPC_TIM0->TCR &= ~(1<<1); //release reset
     
    /* Set Prescale (PR should be set by PrescaleValue - 1) */
    /* Divide SystemCoreClock By 4 due to previous set (CLKPWR_PCLKSEL_CCLK_DIV_4) */
    LPC_TIM0->PR = ((SystemCoreClock) / (4 * 1000000)) - 1;
        
	// Clear interrupt pending
	LPC_TIM0->IR = 0xFFFFFFFF;    
}

PRIVATE INLINE void InitializeTimerMatch(void)
{
    //#warning "MATCH VALUE"
    /* 10 ms */
    LPC_TIM0->MR0 = 10 * 1000;
    
    /* Clear Register */
    LPC_TIM0->MCR &=~TIM_MCR_CHANNEL_MASKBIT(0);
	/* Int and reset on Match */
    LPC_TIM0->MCR |= TIM_INT_ON_MATCH(0) | TIM_RESET_ON_MATCH(0);
    
    LPC_TIM0->EMR 	&= ~(uint32_t)(TIM_EM_MASK(0)); /* Clear Reg */
    
    /* Match output type */
	LPC_TIM0->EMR   |= TIM_EM_SET(0, TIM_EXTMATCH_NOTHING);
}

/***************************** PUBLIC FUNCTIONS *******************************/
void Timer_Init(void)
{
    InitializeTimerHW();
    InitializeTimerMatch();
    
    NVIC_SetPriority(TIMER0_IRQn, ((0x01<<3) | 0x01)); /* preemption = 1, sub-priority = 1 */
    NVIC_EnableIRQ(TIMER0_IRQn); // Enable interrupt 
	
    /* Enable Timer */
    LPC_TIM0->TCR |= TIM_ENABLE;
		
}
TimerHandle Timer_Create(TimerCallback userTimerCB)
{
    int32_t i;
    
    if (userTimerCB == NULL) return TIMER_INVALID_HANDLE;

	// if there is not any free slot we will return -1 (FAILURE)
	for(i = (CPU_TIMER_MAX_TIMER_COUNT - 1); i >= 0; i--)
	{				   
		if(timers[i].callback == NULL) // is there free
		{	  
	  		timers[i].callback = userTimerCB;
            timers[i].timeout = 0;
			break;
		}
	}

	return(i);
}
void Timer_Remove(TimerHandle timer);
void Timer_Start(TimerHandle timer, uint32_t timeout)
{
    if(timer >= CPU_TIMER_MAX_TIMER_COUNT)
	{
		return;
	}
	
    timers[timer].timeout = (int32_t)(timeout / 10);
}
void Timer_Stop(TimerHandle timer, uint32_t timeout);
void Timer_DelayUs(uint32_t microseconds);
void Timer_DelayMs(uint32_t milliseconds);
