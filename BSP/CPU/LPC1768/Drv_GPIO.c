/*******************************************************************************
*
* @file Drv_GPIO.c
*
* @author Murat Cakmak
*
* @brief General Purpose Input/Output Driver Implementation
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

/********************************* INCLUDES ***********************************/
#include "Drv_GPIO.h"

#include "LPC17xx.h"

#include "Debug.h"
#include "postypes.h"

/***************************** MACRO DEFINITIONS ******************************/

/***************************** TYPE DEFINITIONS *******************************/

/**************************** FUNCTION PROTOTYPES *****************************/

/******************************** VARIABLES ***********************************/

/**************************** PRIVATE FUNCTIONS *******************************/
/**
 * Sets pin registers.
 * Code flow for PINSEL and PINMODE registers are same so code flow is merged
 * into single function. Just Client code have to provide Register Address
 * which will be set.
 *
 * @param regPtr to be set Register
 * @param port Port Number of IO
 * @param pin Pin Number of IO
 * @param value new pin value according to selected Register
 *
 * @return none
 */
PRIVATE void SetPinRegister(reg32_t* regPtr, uint32_t port, uint32_t pin, uint32_t val)
{
	/* Function per Pin is represented by two bits so lets find function offset of pin */
	uint32_t pinOffset = pin * 2;
	/*
	 * While function are represented by two bit, 32 bit PINSEL register just
	 * handle 16 pins (32 / 2 function bits). So, we need to multiply port number by two
	 * to get exact PINSEL register.
	 * On the other hand, if pin number is equal or higher than 16, it also exceeds a
	 * pinsel register so we need to handle next PINSEL register so add also
	 * division of pinFunctionOffset by 32.
	 */
	uint32_t pinselRegOffset = port * 2 + (pinOffset / 32);
	/* If pin function offset excees PINSEl Register, take a mod to get exact offset. */
	pinOffset %= 32;

	/* Get exact PINSEL register to control*/
	regPtr = &regPtr[pinselRegOffset];

	/* Set Pin Function */
	*regPtr &= ~(0x3u << pinOffset);
	*regPtr |= val << pinOffset;
}

/**
 * Sets pin Function.
 * Please see LPC17xx datasheet for IO Functions.
 * Implemented as INLINE to avoid function call overhead.
 *
 * @param port Port Number of IO
 * @param pin Pin Number of IO
 * @param function to be set Function
 *
 * @return none
 */
PRIVATE ALWAYS_INLINE void SetPinFunction(uint32_t port, uint32_t pin, uint32_t function)
{
    SetPinRegister(&LPC_PINCON->PINSEL0, port, pin, function);
}

/**
 * Sets pin Drive Mode.
 *
 * Implemented as INLINE to avoid function call overhead.
 *
 * @param port Port Number of IO
 * @param pin Pin Number of IO
 * @param driveMode to be set Drive Mode
 *
 * @return none
 */
PRIVATE ALWAYS_INLINE void SetPinMode(uint32_t port, uint32_t pin, uint32_t driverMode)
{
    SetPinRegister(&LPC_PINCON->PINMODE0, port, pin, driverMode);
}

/***************************** PUBLIC FUNCTIONS *******************************/
/**
 * Initializes GPIO Driver.
 *
 * @param node
 *
 * @return none
 */
void Drv_GPIO_Init(void)
{
}

/**
 * Configures a pin for Function and Drive Mode
 *
 * @param port Port Number of IO
 * @param pin Pin Number of IO
 * @param function to be set Function
 * @param function to be set Drive Mode
 *
 * @return none
 */
void Drv_GPIO_ConfigurePin(uint32_t port, uint32_t pin, uint32_t functionNo, uint32_t driveMode)
{
	SetPinFunction(port, pin, functionNo);
	SetPinMode(port, pin, driveMode);
}

/**
 * Writes state to Output Pin
 *
 * @param port Port Number of IO
 * @param pin Pin Number of IO
 * @param newState new output value(High/Low) IO
 *
 * @return none
 */
void Drv_GPIO_WritePin(uint32_t port, uint32_t pin, Drv_GPIO_PinState newState)
{
	/* Get Mask for Pin */
    uint32_t pinMask = ((uint32_t)1)<<pin;
    /* Get GPIO Register Address */
    LPC_GPIO_TypeDef* regGPIO = &LPC_GPIO0[port];

    /* Set Pin as output */
    regGPIO->FIODIR |= pinMask;

    if (newState == DRV_GPIO_PINSTATE_HIGH)
    {
    	/* Set (1) IO output */
        regGPIO->FIOSET = pinMask;
    }
    else
    {
    	/* Clear (0) IO output */
        regGPIO->FIOCLR = pinMask;
    }
}

/**
 * Reads State of Input Pin
 *
 * @param port Port Number of IO
 * @param pin Pin Number of IO
 *
 * @return returns state (High or Low) of Pin
 */
Drv_GPIO_PinState GPIO_ReadPin(uint32_t port, uint32_t pin)
{
	/* Get Mask for Pin */
    uint32_t pinMask = ((uint32_t)1)<<pin;
    /* Get GPIO Register Address */
    LPC_GPIO_TypeDef* regGPIO = &LPC_GPIO0[port];
    uint32_t pinState;

    /* Set Pin as input */
    regGPIO->FIODIR &= ~pinMask;

    /* Get Pin State */
    pinState = (uint32_t)(((regGPIO->FIOPIN & pinMask) == 0) ? 0 : 1);

    return (Drv_GPIO_PinState)pinState;
}
