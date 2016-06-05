/*******************************************************************************
 *
 * @file GPIO.h
 *
 * @author Murat Cakmak
 *
 * @brief GPIO Interface
 *
 * @see https://github.com/P-LATFORM/P-OS/wiki
 *
 ******************************************************************************
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 Platform
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
#ifndef __BOARD_CONFIG_H
#define __BOARD_CONFIG_H

/********************************* INCLUDES ***********************************/
#include "postypes.h"
/***************************** MACRO DEFINITIONS ******************************/

/***************************** TYPE DEFINITIONS *******************************/

/* BOARD LED INTERFACE */
#define BOARD_ENABLE_LED_INTERFACE 1

/* BOARD LCD INTERFACE */
#define BOARD_ENABLE_LCD_INTERFACE 0

#if (BOARD_ENABLE_LED_INTERFACE && BOARD_ENABLE_LCD_INTERFACE)
    #error "LED and LCD interfaces can not be used at same time!"
#endif

#define CPU_TIMER_MAX_TIMER_COUNT       (30)


#endif	/* __BOARD_CONFIG_H */
