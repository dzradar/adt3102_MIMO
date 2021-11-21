#ifndef __TRNG__H
#define __TRNG__H
#include "adt3102_type_define.h"
/****************************************************************************************
*
*Function name : trngInit
*Description   : trng Init 
*Paramater	   :
*        @ :
*        @ :
        uint32 trngTestBuff[5];
        trngInit(trngTestBuff);
        for(int32 i=0; i<6; i++)
        {
            printf("%x\r\n", trngTestBuff[i]);
        }
*Return        : None
****************************************************************************************/
void trngInit(uint32 *trngBuff);





#endif

