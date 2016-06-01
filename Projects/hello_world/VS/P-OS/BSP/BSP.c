#include "Board.h"
#include "CPU.h"
#include "Timer.h"

#if !defined(__GNUC__)


void Board_Init(void) {}
void Board_LedInit(void) {}
void Board_LedOn(uint32_t ledNo) {}
void Board_LedOff(uint32_t ledNo) {}
void Timer_Init(void) { }
TimerHandle Timer_Create(TimerCallback userTimerCB) { return 0;  }
void Timer_Start(TimerHandle timer, uint32_t timeout) { }

#endif
