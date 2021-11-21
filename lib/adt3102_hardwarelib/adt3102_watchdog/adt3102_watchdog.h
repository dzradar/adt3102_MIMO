#ifndef __WATCHDOG__H
#define __WATCHDOG__H
#include "adt3102_type_define.h"

/****************************************************************************************
*
*Function name : watchdogInit
*Description   : watchdog config
*Paramater	   :
*        @ :
*        @ :
*Return        : None
****************************************************************************************/
void watchdogInit(uint32 reload);
/****************************************************************************************
*
*Function name : watchdogFeed
*Description   : watchdog 
*Paramater	   :
*        @ :
*        @ :
*Return        : None
****************************************************************************************/
void watchdogFeed(uint32 reload);
/****************************************************************************************
*
*Function name : watchdogLock
*Description   : watchdog 
*Paramater	   :
*        @ :
*        @ :
*Return        : None
****************************************************************************************/
void watchdogLock(uint32 lock);






#endif



