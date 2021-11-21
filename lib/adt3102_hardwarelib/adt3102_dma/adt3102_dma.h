#ifndef __DMA__H
#define __DMA__H
#include "adt3102_type_define.h"
/***Macro Definition*********************************************************************
DMAx : DMA ID select
****************************************************************************************/
#define DMA0 0
#define DMA1 1
#define DMA2 2
#define DMA3 3
#define DMA4 4
/***Macro Definition*********************************************************************
PERI_SELECT_x : dma peri select:
****************************************************************************************/
#define PERI_NONE          0
#define PERI_SELECT_SPI0TX 4
#define PERI_SELECT_SPI0RX 5
#define PERI_SELECT_SPI1TX 6
#define PERI_SELECT_SPI1RX 7
#define PERI_SELECT_IIC0TX 8
#define PERI_SELECT_IIC0RX 9
#define PERI_SELECT_IIC1TX 10
#define PERI_SELECT_IIC1RX 11
#define PERI_SELECT_ADC0   12
#define PERI_SELECT_ADC1   13
/***Macro Definition*********************************************************************
PERITOMEM : mem to peri or peri to mem
MEMTOMEM : mem to mem
****************************************************************************************/
#define PERITOMEM  0
#define MEMTOMEM   1
/****************************************************************************************
*Function name : dma0Config
*Description   : dma0 setting
*Paramater	   :
*        @srcAddre : dma 0 source address
*        @dstAddre : dma 0 destination address
*        @srcStep  : dma 0 source address increment steps
*        @dstStep  : dma 0 destination address increment steps
*        @transLen : dma 0 count need to transfer
*        @periSelect  : dma 0 peri select:
*        @  :
*Return        : None
****************************************************************************************/
void dmaConfig(
     int32 dmaId,
     uint32 srcAddre, 
     uint32 dstAddre, 
     uint16 srcStep, 
     uint16 dstStep, 
     uint16 transLen, 
     int32 periSelect,
     int32 memTomemOrperi
);
/****************************************************************************************
*Function name : dma0disable dma1Disable dma2Disable dma3Disable dma4Disable
*Description   : dma0 or dma1 or dma2 or dma3 or dma4 disable
*Paramater	   : None
*        @  : 
*        @  : 
*Return        : None
****************************************************************************************/    
void dmaDisable(int32 dmaId);


#endif




