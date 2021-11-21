//--------------------------------------------------------------------
//Copyright(c)2020,Andar Technologise Inc.
//All Rights Reserved
//Confidential Property of Andar Technologies Inc.
//
//Module Description:
//  .
/*
memory                   address                size
CODE RAM          0x0000_8000-0x0000_ffff       32KB     coding memory   
DATA RAM          0x2000_0000-0x2000_7fff       32KB     stack and heap
DATA RAM          0x2000_8000-0x2001_7fff       64KB     adc0 ,fft data
DATA RAM          0x2001_8000-0x2002_7fff       64KB     adc1 ,fft data

*/
//Created by :wuhao
//$Revision: 1.0
//$Data: 2020/11/9
//--------------------------------------------------------------------
// 
//All include header files
#include "adt3102_type_define.h"
#include "adt3102.h"
#include "adt3102_adc.h"
#include "adt3102_memscheduling.h"
#include "adt3102_dma.h"
#include "dsp_ctype_map.h"
#include "rfc_ctype_map.h"
#include "dma_ctype_map.h"
//------Global variable declaration
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
     uint16 transLen)
{
    volatile uint32 regVal;
    dmaConfig(DMA4, srcAddre, dstAddre, srcStep, dstStep, transLen, 0, MEMTOMEM);
    while(1)
    {
        regVal   = DMA->dma_4_tcnt;
        if(regVal >= transLen) {
            break;
        }
    }    
    dmaDisable(DMA4);
}


















