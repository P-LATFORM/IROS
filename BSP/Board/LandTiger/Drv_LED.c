/*******************************************************************************
*
* @file LED.c
*
* @author Murat Cakmak
*
* @brief LED Implementation for Actual Board (LandTiger)
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
******************************************************************************/

#include "BSPConfig.h"

#if BOARD_ENABLE_LED_INTERFACE

/********************************* INCLUDES ***********************************/
#include "Drv_GPIO.h"

#include "postypes.h"

/***************************** MACRO DEFINITIONS ******************************/
#define BOARD_LED_COUNT			8

/***************************** TYPE DEFINITIONS *******************************/

/**************************** FUNCTION PROTOTYPES *****************************/

/******************************** VARIABLES ***********************************/

/**************************** PRIVATE FUNCTIONS *******************************/

/***************************** PUBLIC FUNCTIONS *******************************/
void Board_LedInit(void)
{
	int pin;
    /*
     * Configure LCD Pins
     * P2.0 ... P2.7
     * 
     */    
	for (pin = 0; pin < BOARD_LED_COUNT; pin++)
	{
		Drv_GPIO_ConfigurePin(2, pin, 0, 0);
	}

	/* LCD cannot be work with LCD at same time.So disable LCD Control pins */   
    /* P0.19 ... P0.25 */
    for (pin = 19; pin <= 25; pin++)
    {
        Drv_GPIO_ConfigurePin(0, pin, 0, 0);
        Drv_GPIO_WritePin(0, pin, DRV_GPIO_PINSTATE_HIGH);
    }
}

void Board_LedOn(uint32_t ledNo)
{
    if (ledNo >= BOARD_LED_COUNT) return;
    
    Drv_GPIO_WritePin(2, ledNo, DRV_GPIO_PINSTATE_HIGH);
}

void Board_LedOff(uint32_t ledNo)
{
    if (ledNo >= BOARD_LED_COUNT) return;
    
    Drv_GPIO_WritePin(2, ledNo, DRV_GPIO_PINSTATE_LOW);
}

#endif /* BOARD_ENABLE_LED_INTERFACE */
