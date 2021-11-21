//--------------------------------------------------------------------
//Copyright(c)2020,Andar Technologise Inc.
//All Rights Reserved
//Confidential Property of Andar Technologies Inc.
//
//Module Description:
//  .watchdog config
//  .
//  .
//  .
//  .

//Created by :wuhao
//$Revision: 1.0
//$Data: 2020/11/9
//--------------------------------------------------------------------
//
//All include header files
#include "adt3102_type_define.h"
#include "watchdog_ctype_map.h"
#include "adt3102_gpio.h"
#include "adt3102_watchdog.h"
//
//------Global variable declaration

/****************************************************************************************
*
*Function name : watchdogInit
*Description   : watchdog config
*Paramater	   :
*        @ :
*        @ :
*Return        : None
****************************************************************************************/
void watchdogInit(uint32 reload)
{   
    /*RW
    [1]: Enable watchdog reset output, acts as a mask for the reset output.
         Set HIGH to enable the reset, or LOW to disable the reset
    [0]: Enable the interrupt event, Set HIGH to enable the counter and the interrupt,
         or LOW to disable the counter and interrupt.    
    *///we set it to the reset function
    WATCHDOG->WDOGCONTROL=1<<1;
    WATCHDOG->WDOGLOAD=reload;//ÖØ×°Öµ
}

/****************************************************************************************
*
*Function name : watchdogFeed
*Description   : watchdog 
*Paramater	   :
*        @ :
*        @ :
*Return        : None
****************************************************************************************/
void watchdogFeed(uint32 reload)
{   
    /*RW
    When this register is written to, the count is immediately restarted the new value
    The minimum valid value for WDOGLAOD is 1
    */
    WATCHDOG->WDOGLOAD=reload;
}

/****************************************************************************************
*
*Function name : watchdogLock
*Description   : watchdog 
*Paramater	   :
*        @ :
*        @ :
*Return        : None
****************************************************************************************/
void watchdogLock(uint32 lock)
{   
    /*RW
    0: Indicates that write access is enable, not locked
    1: Indicates that write access is disenable, locked
    */
    WATCHDOG->WDOGLOCK = lock;
}
