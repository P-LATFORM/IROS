/*******************************************************************************
 *
 * @file lpc17xx_timer.h
 *
 * @author Murat Cakmak
 *
 * @brief Mock Definitions for lpc17xx_timer.h
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

#ifndef __LPC17XX_TIMER_H_
#define __LPC17XX_TIMER_H_

/********************************* INCLUDES ***********************************/
#include "LPC17xx.h"

/***************************** MACRO DEFINITIONS ******************************/

#define TIM_INT_ON_MATCH(n)      		(_BIT((n * 3)))

/** Bit location for reset on MRx match, n = 0 to 3 */
#define TIM_RESET_ON_MATCH(n)    		(_BIT(((n * 3) + 1)))

/** Bit location for stop on MRx match, n = 0 to 3 */
#define TIM_STOP_ON_MATCH(n)     		(_BIT(((n * 3) + 2)))

/** Timer Match control bit mask for specific channel*/
#define	TIM_MCR_CHANNEL_MASKBIT(n)		((uint32_t)(7<<(n*3)))

#define TIM_EM_MASK(n) 					(_SBF((((uint32_t)n << 1) + 4), 0x03))

#define TIM_EM_SET(n,s) 				(_SBF((((uint32_t)n << 1) + 4), (s & 0x03)))

/** Timer/counter enable bit */
#define TIM_ENABLE						((uint32_t)(1<<0))

#define TIM_IR_CLR(n) 					_BIT(n)

/** Timer/counter enable bit */
#define TIM_ENABLE						((uint32_t)(1<<0))
/** Timer/counter reset bit */
#define TIM_RESET						((uint32_t)(1<<1))
/** Timer control bit mask */
#define TIM_TCR_MASKBIT					((uint32_t)(3))

/***************************** TYPE DEFINITIONS *******************************/

typedef enum
{
	TIM_MR0_INT =0, /*!< interrupt for Match channel 0*/
	TIM_MR1_INT =1, /*!< interrupt for Match channel 1*/
	TIM_MR2_INT =2, /*!< interrupt for Match channel 2*/
	TIM_MR3_INT =3, /*!< interrupt for Match channel 3*/
	TIM_CR0_INT =4, /*!< interrupt for Capture channel 0*/
	TIM_CR1_INT =5, /*!< interrupt for Capture channel 1*/
}TIM_INT_TYPE;

typedef enum
{
	TIM_TIMER_MODE = 0,				/*!< Timer mode */
	TIM_COUNTER_RISING_MODE,		/*!< Counter rising mode */
	TIM_COUNTER_FALLING_MODE,		/*!< Counter falling mode */
	TIM_COUNTER_ANY_MODE			/*!< Counter on both edges */
} TIM_MODE_OPT;

typedef enum
{
	TIM_EXTMATCH_NOTHING = 0,		/*!< Do nothing for external output pin if match */
	TIM_EXTMATCH_LOW,				/*!< Force external output pin to low if match */
	TIM_EXTMATCH_HIGH,				/*!< Force external output pin to high if match */
	TIM_EXTMATCH_TOGGLE				/*!< Toggle external output pin if match */
}TIM_EXTMATCH_OPT;

#endif /* __LPC17XX_TIMER_H_ */
