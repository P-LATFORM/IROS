/*******************************************************************************
 *
 * @file Scheduler.h
 *
 * @author Murat Cakmak
 *
 * @brief Generic Scheduler Interface for Kernel
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
#ifndef __SCHEDULER_H
#define __SCHEDULER_H

/********************************* INCLUDES ***********************************/
#include "Kernel.h"

#include "postypes.h"

/***************************** MACRO DEFINITIONS ******************************/

/***************************** TYPE DEFINITIONS *******************************/

/*************************** FUNCTION DEFINITIONS *****************************/
void Scheduler_Init(void);
void Scheduler_Start(void);

#endif	/* __SCHEDULER_H */
