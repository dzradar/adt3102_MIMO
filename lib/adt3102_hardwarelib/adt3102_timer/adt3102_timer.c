//--------------------------------------------------------------------
//Copyright(c)2020,Andar Technologise Inc.
//All Rights Reserved
//Confidential Property of Andar Technologies Inc.
//
//Module Description:
//  .timer initial
//  .dualtimer initial
//  
//Created by :wuhao
//$Revision: 1.0
//$Data: 2020/11/24
//--------------------------------------------------------------------
#include "adt3102_timer.h"
#include "adt3102_type_define.h"
#include "timer_ctype_map.h"
#include "stdio.h"
//
//------Global variable declaration


/****************************************************************************************
*
*Function name : Timer0_Init, Timer1_Init
*Description   : Timer init, 
                 50:50M high clock,if switch 125M high clock, we have to change  50 to 125			
                 excute callback function when timming is up
*Paramater	   :
*        @time    set the time of the timer,the unit is us


*callback function :void Irq3_Handler()
*Return        : None
****************************************************************************************/

#ifdef TIMER_CALLBACK

timerCallback timeRoutine;
void timerCallbackTest(int32 *a )
{
    printf("aaa%d\r\n",*a);
}

static void *arg;

void timer0Handler(void)//Timer0
{ 	
	Timer0->INTSTATUS=1;// clear timer interrupt
    if(timeRoutine)
    {
        timeRoutine(arg);
    }
}

timerCallback timer0Start(float time, timerCallback timerFunc,void *data,int32 highClock50M)
{
    uint32 reload;
    //50M   reload (us)
    reload = highClock50M ? (time*50) : (time*125);
    Timer0->RELOAD=reload;
	Timer0->CTRL=0x09;
    timeRoutine = timerFunc;
    arg = data;
    return timeRoutine;
}
#endif


/****************************************************************************************
*
*Function name : Timer0_Init, Timer1_Init
*Description   : Timer initialization, 50:50M high clock,if switch 125M high clock, we have to change
                 50 to 125
*Paramater		 :
*        @time   : set the time of the timer,the unit is us
*        @Timerx : include Timer0,Timer1

*Return        : None
****************************************************************************************/
void timerInit(TimerTypeDef *Timerx, float time, int32 highClock50M)
{
    uint32 reload;
    //50M   reload (us)
    reload = highClock50M ? (time*50) : (time*125);
    Timerx->RELOAD=reload;
	Timerx->CTRL=0x09;
}

/****************************************************************************************
*
*Function name : timer0Off
*Description   : cloes timer,clear current value
*Paramater		 : None


*Return        : None
****************************************************************************************/
void timerOff(TimerTypeDef *Timerx)
{
    Timerx->RELOAD=0;
	Timerx->CTRL=0x00;
    Timerx->VALUE=0;
}
/****************************************************************************************
*
*Function name : timerInclock
*Description   : Timer external clock input for GPIO
*Paramater		 :
*        @Timerx : include Timer0,Timer1

*Return        : None
****************************************************************************************/
void timerInclock(TimerTypeDef *Timerx ,float time, int32 highClock50M)
{
    uint32 reload;
    //Timer0 External clock input GPIO is GPIO2
    //50M   reload (us)
    reload = highClock50M ? (time*50) : (time*125);
    Timerx->CTRL=0x0D;//select external input as high clock
    Timerx->RELOAD = reload;
}
/****************************************************************************************
*
*Function name : timerInclock
*Description   : Timer external clock enable for GPIO
*Paramater		 :
*        @Timerx : include Timer0,Timer1

*Return        : None
****************************************************************************************/
void timerInenable(TimerTypeDef *Timerx ,float time,int32 highClock50M)
{
    uint32 reload;
    //select external GPIO as Timer enable,high level active
    //50M   reload (us)
    reload = highClock50M ? (time*50) : (time*125);
    Timerx->CTRL=0x0b;
    Timerx->RELOAD=reload;
}
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
void dualtimerInit(uint32 reloadValue, int32 clkDiv)
{
    /*RW
    [7]:timer enable
    [6]:timer mode 
        0:timer is in free_runing mode
        1:timer is in periodic mode
    [5]:intterrupt enable 1:enable
    [3:2]:timerper 00: 0 stages of perscale,clock is divided by 1
                   01: 4 stages of perscale,clock is divided by 16
                   10: 8 stages of perscale,clock is divided by 256
                   11: not used
    [1]:tiemr size ,0:16-bit counter
                    1:32-bit counter
    [0]:0 wrapping mode
        1 One-shot mode
    */
    Dualtimer->TIMER2CONTROL=1<<7|1<<6|1<<5|(clkDiv<<2)|1<<1|1<<0;
    Dualtimer->TIMER2LOAD=reloadValue;
    
}

