#ifndef __MEM_SCHDULING_H
#define __MEM_SCHDULING_H
#include "adt3102_type_define.h"
/****************************************************************************************
*Function name : memScheduling
*Description   : adt3102 memory scheduling
*Paramater	   :
*        @srcAddre: memory source address
*        @dstAddre: memory destination address
*        @srcStep:  memory source address increment steps
*        @dstStep:  memory destination address increment steps
*        @transLen: memory count need to transfer
*Return        : None
*function example : 
  uint32 testArray[5] = {0x01, 0x02, 0x03, 0x04, 0x05};
  uint32 testArray1[5];
  memScheduling((uint32)&testArray, (uint32)&testArray1, 4, 4, 5);
****************************************************************************************/
void memScheduling(
     uint32 srcAddre, 
     uint32 dstAddre, 
     uint16 srcStep, 
     uint16 dstStep, 
     uint16 transLen);


#endif


