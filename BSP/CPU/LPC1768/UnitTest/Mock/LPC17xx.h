/*******************************************************************************
 *
 * @file LPC17xx.h
 *
 * @author Murat Cakmak
 *
 * @brief Mock Header for LPC17xx.
 * 			Whlie Real LPC17xx includes additional headers (e.g core_cm3.h),
 * 			we will collect (or mock)  all other header objects in this file.
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

#ifndef __LPC17XX_H
#define __LPC17XX_H

/********************************* INCLUDES ***********************************/

#include "stdint.h"

/***************************** MACRO DEFINITIONS ******************************/

#define _BIT(n)	(1<<n)
#define _SBF(f,v) (v<<f)

#define MOCK_REG_DEF(__Type, __Name) \
		__attribute__((unused)) static __Type REG##__Name; \
		__attribute__((unused)) static __Type* __Name = &REG##__Name;


#define SCB_ICSR_PENDSVSET_Pos             28U                                            /*!< SCB ICSR: PENDSVSET Position */
#define SCB_ICSR_PENDSVSET_Msk             (1UL << SCB_ICSR_PENDSVSET_Pos)                /*!< SCB ICSR: PENDSVSET Mask */

/*
 * splint (Static Code Analysis Tool) gives error if a object is not used but
 * we may not need to use some object in scope of Unit Testing.
 * To suppress this error, we will use directive.
 */
#define SPLINT_SUPPRESS_UNUSED_ERROR 		/*@unused@*/

/***************************** TYPE DEFINITIONS *******************************/

typedef struct
{
	uint32_t CPUID;                  /*!< Offset: 0x000 (R/ )  CPUID Base Register */
    uint32_t ICSR;                   /*!< Offset: 0x004 (R/W)  Interrupt Control and State Register */
    uint32_t VTOR;                   /*!< Offset: 0x008 (R/W)  Vector Table Offset Register */
    uint32_t AIRCR;                  /*!< Offset: 0x00C (R/W)  Application Interrupt and Reset Control Register */
    uint32_t SCR;                    /*!< Offset: 0x010 (R/W)  System Control Register */
    uint32_t CCR;                    /*!< Offset: 0x014 (R/W)  Configuration Control Register */
    uint8_t  SHP[12U];               /*!< Offset: 0x018 (R/W)  System Handlers Priority Registers (4-7, 8-11, 12-15) */
    uint32_t SHCSR;                  /*!< Offset: 0x024 (R/W)  System Handler Control and State Register */
    uint32_t CFSR;                   /*!< Offset: 0x028 (R/W)  Configurable Fault Status Register */
    uint32_t HFSR;                   /*!< Offset: 0x02C (R/W)  HardFault Status Register */
    uint32_t DFSR;                   /*!< Offset: 0x030 (R/W)  Debug Fault Status Register */
    uint32_t MMFAR;                  /*!< Offset: 0x034 (R/W)  MemManage Fault Address Register */
    uint32_t BFAR;                   /*!< Offset: 0x038 (R/W)  BusFault Address Register */
    uint32_t AFSR;                   /*!< Offset: 0x03C (R/W)  Auxiliary Fault Status Register */
    uint32_t PFR[2U];                /*!< Offset: 0x040 (R/ )  Processor Feature Register */
    uint32_t DFR;                    /*!< Offset: 0x048 (R/ )  Debug Feature Register */
    uint32_t ADR;                    /*!< Offset: 0x04C (R/ )  Auxiliary Feature Register */
    uint32_t MMFR[4U];               /*!< Offset: 0x050 (R/ )  Memory Model Feature Register */
    uint32_t ISAR[5U];               /*!< Offset: 0x060 (R/ )  Instruction Set Attributes Register */
    uint32_t RESERVED0[5U];
    uint32_t CPACR;                  /*!< Offset: 0x088 (R/W)  Coprocessor Access Control Register */
} SCB_Type;

typedef struct
{
    uint32_t PINSEL0;
    uint32_t PINSEL1;
    uint32_t PINSEL2;
    uint32_t PINSEL3;
    uint32_t PINSEL4;
    uint32_t PINSEL5;
    uint32_t PINSEL6;
    uint32_t PINSEL7;
    uint32_t PINSEL8;
    uint32_t PINSEL9;
    uint32_t PINSEL10;
    uint32_t RESERVED0[5];
    uint32_t PINMODE0;
    uint32_t PINMODE1;
    uint32_t PINMODE2;
    uint32_t PINMODE3;
    uint32_t PINMODE4;
    uint32_t PINMODE5;
    uint32_t PINMODE6;
    uint32_t PINMODE7;
    uint32_t PINMODE8;
    uint32_t PINMODE9;
    uint32_t PINMODE_OD0;
    uint32_t PINMODE_OD1;
    uint32_t PINMODE_OD2;
    uint32_t PINMODE_OD3;
    uint32_t PINMODE_OD4;
    uint32_t I2CPADCFG;
} LPC_PINCON_TypeDef;

/*
 * [IMP]
 * This structure is not same with the original version in the LPC17xx.h file.
 * This is because static analysis tool (splint) can not parse some nested
 * types (combinations of unions and structs).
 * Let's use just mandatory fields.
 */
typedef struct
{
	uint32_t FIODIR;
	uint32_t RESERVED0[3];
	uint32_t FIOMASK;
	uint32_t FIOPIN;
	uint32_t FIOSET;
	uint32_t FIOCLR;
} LPC_GPIO_TypeDef;

typedef struct
{
  uint32_t IR;
  uint32_t TCR;
  uint32_t TC;
  uint32_t PR;
  uint32_t PC;
  uint32_t MCR;
  uint32_t MR0;
  uint32_t MR1;
  uint32_t MR2;
  uint32_t MR3;
  uint32_t CCR;
  uint32_t CR0;
  uint32_t CR1;
  uint32_t RESERVED0[2];
  uint32_t EMR;
  uint32_t RESERVED1[12];
  uint32_t CTCR;
} LPC_TIM_TypeDef;

typedef struct
{
       uint32_t FLASHCFG;               /* Flash Accelerator Module           */
       uint32_t RESERVED0[31];
       uint32_t PLL0CON;                /* Clocking and Power Control         */
       uint32_t PLL0CFG;
       uint32_t PLL0STAT;
       uint32_t PLL0FEED;
       uint32_t RESERVED1[4];
       uint32_t PLL1CON;
       uint32_t PLL1CFG;
       uint32_t PLL1STAT;
       uint32_t PLL1FEED;
       uint32_t RESERVED2[4];
       uint32_t PCON;
       uint32_t PCONP;
       uint32_t RESERVED3[15];
       uint32_t CCLKCFG;
       uint32_t USBCLKCFG;
       uint32_t CLKSRCSEL;
       uint32_t	CANSLEEPCLR;
       uint32_t	CANWAKEFLAGS;
       uint32_t RESERVED4[10];
       uint32_t EXTINT;                 /* External Interrupts                */
       uint32_t RESERVED5;
       uint32_t EXTMODE;
       uint32_t EXTPOLAR;
       uint32_t RESERVED6[12];
       uint32_t RSID;                   /* Reset                              */
       uint32_t RESERVED7[7];
       uint32_t SCS;                    /* Syscon Miscellaneous Registers     */
       uint32_t IRCTRIM;                /* Clock Dividers                     */
       uint32_t PCLKSEL0;
       uint32_t PCLKSEL1;
       uint32_t RESERVED8[4];
       uint32_t USBIntSt;               /* USB Device/OTG Interrupt Register  */
       uint32_t DMAREQSEL;
       uint32_t CLKOUTCFG;              /* Clock Output Configuration         */
 } LPC_SC_TypeDef;

/*
* Mock objects and flags
*
* 	We collect all test specific object in that container
*/
typedef struct
{
	struct
	{
		uint32_t memory_barrier : 1;			/* Memory Barreer is applied */
		uint32_t interrupt_disabled : 1;	/* Flag to see whether Interrupts are disabled or not */
		uint32_t endless_while_loop : 1;	/* Flag to see we entered into endless while loop */
		uint32_t svc_handler_call : 1;		/* Flag to see whether SVC Handler is called or not */
	} flags;

} LPC17xxMockObjects;
/**************************** FUNCTION PROTOTYPES *****************************/

extern void POS_PendSV_Handler(void);

/******************************** VARIABLES ***********************************/

/*
 * Register Definitions
 */
MOCK_REG_DEF(SCB_Type, SCB);
MOCK_REG_DEF(LPC_PINCON_TypeDef, LPC_PINCON);
MOCK_REG_DEF(LPC_GPIO_TypeDef, LPC_GPIO0);
MOCK_REG_DEF(LPC_TIM_TypeDef, LPC_TIM0);
MOCK_REG_DEF(LPC_SC_TypeDef, LPC_SC);

/*
 * Mock objects and flags
 */
static LPC17xxMockObjects lpcMockObjects;

/********************************** FUNCTIONS *********************************/

/*
 * Resets all LPC17xx objects including register mocks and test objects.
 */
SPLINT_SUPPRESS_UNUSED_ERROR
static INLINE void ResetRegistersAndObjects(void)
{
	memset(SCB, 0, sizeof(SCB_Type));
	memset(LPC_PINCON, 0, sizeof(LPC_PINCON_TypeDef));
	memset(LPC_GPIO0, 0, sizeof(LPC_GPIO_TypeDef));
	memset(LPC_TIM0, 0, sizeof(LPC_TIM_TypeDef));
	memset(LPC_SC, 0, sizeof(LPC_SC_TypeDef));

	memset(&lpcMockObjects, 0, sizeof(lpcMockObjects));
}

/*
 * Mock Implementation for DMB (Data Memory Barrier)
 */
SPLINT_SUPPRESS_UNUSED_ERROR
static INLINE void __DMB(void)
{
	/* Set flag to inform */
	lpcMockObjects.flags.memory_barrier = 1;

	/* DMB Instruction can be called from different areas so to differentiate
	 * callers and simulate required behavior, we can check some indicators.
	 *
	 */
	if (SCB->ICSR == (reg32_t)SCB_ICSR_PENDSVSET_Msk)
	{
		/* If PendSV Interrupt is set, call PendSV ISR */
		POS_PendSV_Handler();
	}
}

/*
 * Mock Implementation for Global Interrupt Disable
 */
SPLINT_SUPPRESS_UNUSED_ERROR
static INLINE void __disable_irq(void)
{
	/* Just set flag to inform tester */
	lpcMockObjects.flags.interrupt_disabled = 1;
}

/*
 * Mock Implementation for Global Interrupt Enable
 */
SPLINT_SUPPRESS_UNUSED_ERROR
static INLINE void __enable_irq(void)
{
	/* Just set flag to inform tester */
	lpcMockObjects.flags.interrupt_disabled = 0;
}

#define NVIC_SetPriority(...)
#define NVIC_EnableIRQ(...)

#endif		/* __LPC17XX_H */
