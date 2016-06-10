/********************************* INCLUDES ***********************************/

#include "Drv_GPIO.h"
#include "Drv_UserTimer.h"

#include "Kernel.h"
#include "Board.h"

#include "OSConfig.h"

/***************************** MACRO DEFINITIONS ******************************/

/***************************** TYPE DEFINITIONS *******************************/

/**************************** FUNCTION PROTOTYPES *****************************/

/******************************** VARIABLES ***********************************/

/**************************** PRIVATE FUNCTIONS ******************************/

void myDelay(int delay)
{
    int i;
    while (i++ < delay);
}
#define VALL 10000000

OS_USER_TASK_START_POINT(MyTask1Func)
{
    static int flag = 0;
    int cnt = 0;
    
    while (1)
    {    
        flag = !flag;
        
        if (flag)
        {
            Board_LedOn(0);
        }
        else
        {
            Board_LedOff(0);
        }
        
        myDelay(VALL);
        cnt++;
#if OS_SCHEDULER == OS_SCHEDULER_COOPARATIVE
        OS_Yield();
#endif /* OS_SCHEDULER == OS_SCHEDULER_COOPARATIVE */
    }
}

OS_USER_TASK_START_POINT(MyTask2Func)
{
    static int flag = 0;
    int cnt = 0;
    
    while (1)
    {    
        flag = !flag;
        
        if (flag)
        {
            Board_LedOn(1);
        }
        else
        {
            Board_LedOff(1);
        }
        
        myDelay(VALL);
        cnt++;

#if OS_SCHEDULER == OS_SCHEDULER_COOPARATIVE
        OS_Yield();
#endif /* OS_SCHEDULER == OS_SCHEDULER_COOPARATIVE */
    }
}
/***************************** PUBLIC FUNCTIONS *******************************/

void OS_InitializeUserSpace(void)
{
}
