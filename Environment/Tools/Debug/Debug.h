/*******************************************************************************
 *
 * @file Debug.h
 *
 * @author Murat Cakmak
 *
 * @brief Debugging Interface
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
#ifndef __DEBUG_H
#define __DEBUG_H

/********************************* INCLUDES ***********************************/
#include "ProjectConfig.h"

/***************************** MACRO DEFINITIONS ******************************/

/* TODO : Define a output for that macro */
#define DEBUG_MESSAGE(message)

#if ENABLE_DEBUG_ASSERT

	#ifdef WIN32

		#include <assert.h>

		/* Breaks execution in case of fail */
		#define DEBUG_ASSERT(condition) { assert(condition); }

		/* Breaks execution and print message in case of fail */
		#define DEBUG_ASSERT_MESSAGE(condition, message) { DEBUG_MESSAGE(message); assert(condition); }

	#else /* WIN32 */

		#define DEBUG_ASSERT(condition)
		#define DEBUG_ASSERT_MESSAGE(condition, message)

	#endif /* WIN32 */

#else
	#define DEBUG_ASSERT(condition)
	#define DEBUG_ASSERT_MESSAGE(condition, message)
#endif	/* ENABLE_DEBUG_ASSERT */
/***************************** TYPE DEFINITIONS *******************************/

/*************************** FUNCTION DEFINITIONS *****************************/

#endif	/* __DEBUG_H */
