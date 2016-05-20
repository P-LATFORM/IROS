/*******************************************************************************
 *
 * @file postypes.h
 *
 * @author Murat Cakmak
 *
 * @brief P-OS Specific Type Definitions which extend standart types
 *
 * @see https://github.com/P-LATFORM/P-OS/wiki
 *
 ******************************************************************************
 *
 *  Copyright (2016), P-OS
 *
 *   This software may be modified and distributed under the terms of the
 *   'MIT License'.
 *
 *   See the LICENSE file for details.
 *
 ******************************************************************************/

#ifndef __POS_TYPES_H
#define __POS_TYPES_H

/********************************* INCLUDES ***********************************/
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

/***************************** MACRO DEFINITIONS ******************************/

#ifndef PUBLIC
#define PUBLIC
#endif /* PUBLIC */

#ifndef INTERNAL
#define INTERNAL
#endif /* INTERNAL */

#ifndef PRIVATE
#define PRIVATE static
#endif /* PRIVATE */

#ifndef RESULT_SUCCESS
#define RESULT_SUCCESS			(0)
#endif /* RESULT_SUCCESS */

#ifndef RESULT_FAIL
#define RESULT_FAIL				(-1)
#endif /* RESULT_FAIL */

#ifndef INLINE
	#if defined(WIN32)
		#define INLINE __inline
	#elif defined(__ARMCC_VERSION)
		#define INLINE __inline
    #else
        #define INLINE __inline__
	#endif
#endif	/* INLINE */

#ifndef ENDLESS_WHILE_LOOP
#define ENDLESS_WHILE_LOOP while (1)
#endif

#ifndef SUPER_LOOP
#define SUPER_LOOP ENDLESS_WHILE_LOOP
#endif
/***************************** TYPE DEFINITIONS *******************************/

/*************************** FUNCTION DEFINITIONS *****************************/

#endif	/* __POS_TYPES_H */
