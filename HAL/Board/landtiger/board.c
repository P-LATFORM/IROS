/*******************************************************************************
 *
 * @file board.c
 *
 * @author Murat Cakmak
 *
 * @brief Board specific hardware abstraction implementation
 *
 * @see http://https://github.com/ir-os/IROS
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

/********************************* INCLUDES ***********************************/

#include "postypes.h"

/***************************** MACRO DEFINITIONS ******************************/

/***************************** TYPE DEFINITIONS *******************************/

/**************************** FUNCTION PROTOTYPES *****************************/

/******************************** VARIABLES ***********************************/
static int board_module_initialized = 0;

/**************************** INTERNAL FUNCTIONS ******************************/

/***************************** PUBLIC FUNCTIONS *******************************/

int board_init(void)
{
	board_module_initialized = 1;

	return 0;
}
