/********************************* INCLUDES ***********************************/

#include "Drv_GPIO.h"
#include "Drv_UserTimer.h"

#include "Kernel.h"

#include "OSConfig.h"
#include "BSPConfig.h"

#include "postypes.h"

/***************************** MACRO DEFINITIONS ******************************/

#define TASK_DELAY_IN_MS            (1000)

/***************************** TYPE DEFINITIONS *******************************/

/**************************** FUNCTION PROTOTYPES *****************************/
extern void Board_LedInit(void);
extern void Board_LedOn(uint32_t ledNo);
extern void Board_LedOff(uint32_t ledNo);

/******************************** VARIABLES ***********************************/

/**************************** PRIVATE FUNCTIONS ******************************/
#if defined(PSOC_CREATOR_PROJECT)

extern void  CyDelay(uint32_t milliseconds);

#define Delay(delayInMs)           CyDelay(delayInMs)

#elif defined(UVISION_PROJECT)

static void delayMs(uint32_t delayInMs)
{
	#define MILISECONDS                 (2500000 / 100 - 1)

	uint32_t counterReset = 0;

	counterReset = (delayInMs * MILISECONDS);
	while (counterReset--);
}

#define Delay(delayInMs)           delayMs(delayInMs)

#else

#define Delay(delayInMs)

#endif


OS_USER_TASK_START_POINT(MyTask1Func)
{
    (void)args;
    
    while (1)
    {
        Delay(TASK_DELAY_IN_MS / 2);

        Board_LedOn(0);
        
        Delay(TASK_DELAY_IN_MS / 2);

#if OS_SCHEDULER == OS_SCHEDULER_COOPARATIVE
        OS_Yield();
#endif /* OS_SCHEDULER == OS_SCHEDULER_COOPARATIVE */
    }
}

OS_USER_TASK_START_POINT(MyTask2Func)
{
    (void)args;
    
    while (1)
    {
        Board_LedOff(0);
        
        Delay(TASK_DELAY_IN_MS);

#if OS_SCHEDULER == OS_SCHEDULER_COOPARATIVE
        OS_Yield();
#endif /* OS_SCHEDULER == OS_SCHEDULER_COOPARATIVE */
    }
}

PRIVATE ALWAYS_INLINE void InitializeBoard(void)
{
#if BOARD_ENABLE_LED_INTERFACE
	Board_LedInit();
#endif
}
/***************************** PUBLIC FUNCTIONS *******************************/

void OS_InitializeUserSpace(void)
{
    /* Board initialization should be under user space responsiblity */
    InitializeBoard();
}
