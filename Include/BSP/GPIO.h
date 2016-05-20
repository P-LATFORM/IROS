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
#ifndef __GPIO_H
#define __GPIO_H

/********************************* INCLUDES ***********************************/
#include "postypes.h"
/***************************** MACRO DEFINITIONS ******************************/

/***************************** TYPE DEFINITIONS *******************************/
/* GPIO Pin States */
typedef enum
{
	GPIO_PINSTATE_LOW = 0,
	GPIO_PINSTATE_HIGH = 1
} GPIOPinState;

/*************************** FUNCTION DEFINITIONS *****************************/
void GPIO_Init(void);
void GPIO_ConfigurePin(uint32_t port, uint32_t pin, uint32_t functionNo, uint32_t driveMode);
void GPIO_WritePin(uint32_t port, uint32_t pin, GPIOPinState state);
GPIOPinState GPIO_ReadPin(uint32_t port, uint32_t pin);

#endif	/* __GPIO_H */
