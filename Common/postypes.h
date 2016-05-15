/*******************************************************************************
 *
 *  @file postypes.h
 *
 *  @author Murat Cakmak
 *
 *  @brief P-OS Specific Type Definitions which extend stdtypes
 *
 *  @see http://https://github.com/ir-os/IROS
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
#define RESULT_FAIL			(-1)
#endif /* RESULT_FAIL */

#ifndef INLINE
#define INLINE __inline__
#endif	/* INLINE */

/***************************** TYPE DEFINITIONS *******************************/

/*************************** FUNCTION DEFINITIONS *****************************/

#endif	/* __POS_TYPES_H */
