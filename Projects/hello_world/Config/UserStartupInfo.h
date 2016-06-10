/*******************************************************************************
 *
 * @file UserStartupInfo.h
 *
 * @author Murat Cakmak
 *
 * @brief P-OS support static task creation to decrease memory footprint and
 * 		  this file provides information about static user tasks.
 *
 * 		  - This file is a mandatory for static task creation.
 * 		  - Also user must specify startup task using STARTUP_APPLICATIONS(...)
 * 		  	macro
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
#ifndef __USER_STARTUP_INFO_H
#define __USER_STARTUP_INFO_H

/********************************* INCLUDES ***********************************/
#include "Kernel.h"

#include "postypes.h"

/***************************** MACRO DEFINITIONS ******************************/

/***************************** TYPE DEFINITIONS *******************************/

/**************************** FUNCTION PROTOTYPES *****************************/
/*
 * User Task Start Points (Functions)
 */
OS_USER_TASK_START_POINT(MyTask1Func);
OS_USER_TASK_START_POINT(MyTask2Func);

/********************************* VARIABLES **********************************/

/* User Task 1 with 256 stack size */
OS_USER_TASK(MyTask1, MyTask1Func, 256, 15);

/* User Task 1 with 512 stack size */
OS_USER_TASK(MyTask2, MyTask2Func, 512, 15);

/*
 * Startup Application.
 * After bootup and kernel initialization, kernel starts following applications
 *
 */
OS_STARTUP_APPLICATIONS
(
    OS_USER_TASK_PREFIX(MyTask1),
    OS_USER_TASK_PREFIX(MyTask2)
)

#endif	/* __USER_STARTUP_INFO_H */
