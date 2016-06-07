/*******************************************************************************
 *
 * @file lpc17xx_clkpwr.h
 *
 * @author Murat Cakmak
 *
 * @brief Mock Definitions for lpc17xx_clkpwr.h
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

#ifndef LPC17XX_CLKPWR_H_
#define LPC17XX_CLKPWR_H_

/********************************* INCLUDES ***********************************/

#include "LPC17xx.h"

/***************************** MACRO DEFINITIONS ******************************/

/** Power Control for Peripherals bit mask */
#define CLKPWR_PCONP_BITMASK				0xEFEFF7DE

/** Timer/Counter 0 power/clock control bit */
#define	 CLKPWR_PCONP_PCTIM0				((uint32_t)(1<<1))
/* Timer/Counter 1 power/clock control bit */
#define	 CLKPWR_PCONP_PCTIM1				((uint32_t)(1<<2))
/** Timer 2 power/clock control bit */
#define	 CLKPWR_PCONP_PCTIM2				((uint32_t)(1<<22))
/** Timer 3 power/clock control bit */
#define	 CLKPWR_PCONP_PCTIM3				((uint32_t)(1<<23))

#define CLKPWR_PCLKSEL_BITMASK(p)			_SBF(p,0x03)

#define CLKPWR_PCLKSEL_SET(p,n)				_SBF(p,n)

/** Peripheral clock divider bit position for TIMER0 */
#define	CLKPWR_PCLKSEL_TIMER0  				((uint32_t)(2))

#define	CLKPWR_PCLKSEL_CCLK_DIV_4  			((uint32_t)(0))

#define TIM_CTCR_MODE_MASK  				0x3

#endif /* #ifndef LPC17XX_CLKPWR_H_ */
