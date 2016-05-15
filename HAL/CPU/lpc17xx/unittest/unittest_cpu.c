/*******************************************************************************
 *
 * @file unittest_cpu.c
 *
 * @author Murat Cakmak
 *
 * @brief Unit test file for cpu module
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
/* Include cpu source file for WHITE-BOX unit testing */
#include "../cpu.c"

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
void test_BeforeCPUInit(void)
{
	//All of these should pass
	TEST_ASSERT_EQUAL(0, cpu_module_initialized);
}


void test_AfterCPUInit(void)
{
	cpu_init();

	//All of these should pass
	TEST_ASSERT_EQUAL(1, cpu_module_initialized);
}
