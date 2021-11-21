#ifndef __Timer_H
#define __Timer_H
#include "adt3102_type_define.h"
#include "timer_ctype_map.h"

#define clkDIV1   00
#define clkDIV16  01
#define clkDIV256 10


/****************************************************************************************
*
*Function name : Timer0_Init, Timer1_Init
*Description   : Timer initialization, 50:50M high clock,if switch 125M high clock, we have to change
                 50 to 125
*Paramater		 :
*        @time   : set the time of the timer,the unit is us
*        @Timerx : include Timer0,Timer1
*        @highClock50M : select high clock, include 50M and 125M

*Return        : None
****************************************************************************************/
typedef void (*timerCallback)(void *arg);
extern timerCallback timer0Start(float time, timerCallback timerFunc,void *data,int32 highClock50M);
void timerInit(TimerTypeDef *Timerx, float time,int32 highClock50M);
/****************************************************************************************
*
*Function name : timer0Off
*Description   : cloes timer,clear current value
*Paramater		 : None


*Return        : None
****************************************************************************************/
void timerOff(TimerTypeDef *Timerx);
/****************************************************************************************
*
*Function name : timerInclock
*Description   : Timer external clock input for GPIO
*Paramater		 :
*        @time   : set the time of the timer,the unit is us
*        @Timerx : include Timer0,Timer1
*        @highClock50M : select high clock, include 50M and 125M
*Return        : None
****************************************************************************************/
void timerInclock(TimerTypeDef *Timerx ,float time, int32 highClock50M);
/****************************************************************************************
*
*Function name : timerInclock
*Description   : Timer external clock enable for GPIO
*Paramater		 :
*        @time   : set the time of the timer,the unit is us
*        @Timerx : include Timer0,Timer1
*        @highClock50M : select high clock, include 50M and 125M
*Return        : None
****************************************************************************************/
void timerInenable(TimerTypeDef *Timerx ,float time,int32 highClock50M);
void timerCallbackTest(int32 *a);
/****************************************************************************************
*
*Function name : dualtimerInit
*Description   : 
                

*Paramater		 :
*        @reloadValue: timer reload 
*        @clkDiv :clock is divided by clkDiv
                 clkDIV1   :divided by 1
                 clkDIV16  :divided by 16 
                 clkDIV256 :divided by 256
*Return        : None
****************************************************************************************/
void dualtimerInit(uint32 reloadValue, int32 clkDiv);

#endif




