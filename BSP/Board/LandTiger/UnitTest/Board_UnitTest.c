/*******************************************************************************
 *
 * @file Board_UnitTest.c
 *
 * @author Murat Cakmak
 *
 * @brief Unit test file for board module
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

/********************************* INCLUDES ***********************************/
/* Include board source file for WHITE-BOX unit testing */
#include "../Board.c"

#include "Mock/mockCPU.c"

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
}

void test_AfterBoardInit(void)
{
	Board_Init();
}
