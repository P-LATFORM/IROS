#include "Kernel.h"
#include "Board.h"
#include "GPIO.h"
#include "Timer.h"

#include "Timer.h"

OSTaskHandle handle1, handle2;
TimerHandle  timer1, timer2;

void isr1(void)
{   
    Timer_Start(timer1, 1000);
    OS_SetTaskState(handle1, OSTaskState_Ready);
}

void isr2(void)
{     
    Timer_Start(timer2, 1000);
    OS_SetTaskState(handle2, OSTaskState_Ready);
}

OS_USER_APPLICATION(app1)
{
    static int flag = 0;
    
    flag = !flag;
    if (flag)
    {
        Board_LedOn(0);
        Board_LedOn(3);
    }
    else
    {
        Board_LedOff(0);
        Board_LedOff(3);
    }
}

OS_USER_APPLICATION(app2)
{
    static int flag = 0;
    
    flag = !flag;
    if (!flag)
    {
        Board_LedOn(1);
        Board_LedOn(2);
    }
    else
    {
        Board_LedOff(1);
        Board_LedOff(2);
    }
}

void OS_InitializeUserSpace(void)
{
    Timer_Init();
    
    timer1 = Timer_Create(isr1);
    timer2 = Timer_Create(isr2);
    
    Timer_Start(timer1, 1000);
    Timer_Start(timer2, 1000);
    
	handle1 = OS_CreateTask(app1);
	handle2 = OS_CreateTask(app2);    
}
