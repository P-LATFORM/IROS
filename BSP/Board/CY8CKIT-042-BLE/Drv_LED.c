/*******************************************************************************
 *
 * @file Drv_LED.c
 *
 * @author Murat Cakmak
 *
 * @brief LED Implementation to manage RDG LED on board.
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

#include "BSPConfig.h"

#if BOARD_ENABLE_LED_INTERFACE
/********************************* INCLUDES ***********************************/

#include "Board.h"

/* Header which includes all existing Cypress Headers */
#include <project.h>

#include "Drv_CPUCore.h"
#include "BSPConfig.h"
#include "postypes.h"

/***************************** MACRO DEFINITIONS ******************************/

/***************************** TYPE DEFINITIONS *******************************/

/**************************** FUNCTION PROTOTYPES *****************************/

/******************************** VARIABLES ***********************************/

/**************************** PRIVATE FUNCTIONS ******************************/

/***************************** PUBLIC FUNCTIONS *******************************/

void Board_LedInit(void)
{

}

void Board_LedOn(uint32_t ledNo)
{
    switch (ledNo)
    {
        case 0: LED_RED_Write(0); break;
        default: break;
    }
}

void Board_LedOff(uint32_t ledNo)
{
    switch (ledNo)
    {
        case 0: LED_RED_Write(1); break;
        default: break;
    }
}

#endif	/* BOARD_ENABLE_LED_INTERFACE */
