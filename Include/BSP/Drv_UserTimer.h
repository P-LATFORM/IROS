/*******************************************************************************
 *
 * @file Drv_UserTimer.h
 *
 * @author Murat Cakmak
 *
 * @brief User Timer Driver Interface
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
 *******************************************************************************/
#ifndef __DRV_USERTIMER_H
#define __DRV_USERTIMER_H

/********************************* INCLUDES ***********************************/

#include "postypes.h"

/***************************** MACRO DEFINITIONS ******************************/
#define DRV_TIMER_INVALID_HANDLE        (-1)

/***************************** TYPE DEFINITIONS *******************************/
typedef int32_t Drv_TimerHandle;
typedef void (*Drv_TimerCallback)(void);

/*************************** FUNCTION DEFINITIONS *****************************/
void Drv_Timer_Init(void);
Drv_TimerHandle Drv_Timer_Create(Drv_TimerCallback userTimerCB);
void Drv_Timer_Remove(Drv_TimerHandle timer);
void Drv_Timer_Start(Drv_TimerHandle timer, uint32_t timeout);
void Drv_Timer_Stop(Drv_TimerHandle timer, uint32_t timeout);
void Drv_Timer_DelayUs(uint32_t microseconds);
void Drv_Timer_DelayMs(uint32_t milliseconds);

#endif	/* __DRV_USERTIMER_H */
