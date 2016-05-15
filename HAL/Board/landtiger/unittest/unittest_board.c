/*******************************************************************************
 *
 * @file unittest_board.c
 *
 * @author Murat Cakmak
 *
 * @brief Unit test file for board module
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
/* Include board source file for WHITE-BOX unit testing */
#include "../board.c"

#include "unity.h"
/***************************** MACRO DEFINITIONS ******************************/

/***************************** TYPE DEFINITIONS *******************************/

/**************************** FUNCTION PROTOTYPES *****************************/

/******************************** VARIABLES ***********************************/

/**************************** INTERNAL FUNCTIONS ******************************/
/**
 * @brief Constructor Method for each test case
 *
 */
void setUp(void)
{
}

/**
 * @brief Constructor Method for each test case
 *
 */
void tearDown(void)
{

}

/***************************** TEST FUNCTIONS *******************************/
void test_BeforeBoardInit(void)
{
	//All of these should pass
	TEST_ASSERT_EQUAL(0, board_module_initialized);
}


void test_AfterBoardInit(void)
{
	board_init();

	//All of these should pass
	TEST_ASSERT_EQUAL(1, board_module_initialized);
}
