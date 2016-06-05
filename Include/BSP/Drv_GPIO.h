/*******************************************************************************
 *
 * @file Drv_GPIO.h
 *
 * @author Murat Cakmak
 *
 * @brief GPIO Driver Interface
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
#ifndef __DRV_GPIO_H
#define __DRV_GPIO_H

/********************************* INCLUDES ***********************************/
#include "postypes.h"
/***************************** MACRO DEFINITIONS ******************************/

/***************************** TYPE DEFINITIONS *******************************/
/* GPIO Pin States */
typedef enum
{
	DRV_GPIO_PINSTATE_LOW = 0,
	DRV_GPIO_PINSTATE_HIGH = 1
} Drv_GPIO_PinState;

/*************************** FUNCTION DEFINITIONS *****************************/
void Drv_GPIO_Init(void);
void Drv_GPIO_ConfigurePin(uint32_t port, uint32_t pin, uint32_t functionNo, uint32_t driveMode);
void Drv_GPIO_WritePin(uint32_t port, uint32_t pin, Drv_GPIO_PinState state);
Drv_GPIO_PinState Drv_GPIO_ReadPin(uint32_t port, uint32_t pin);

#endif	/* __DRV_GPIO_H */
