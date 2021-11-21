//--------------------------------------------------------------------
//Copyright(c)2020,Andar Technologise Inc.
//All Rights Reserved
//Confidential Property of Andar Technologies Inc.
//
//Module Description:
//  .DMA config
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
#include "adt3102_dma.h"
#include "dma_ctype_map.h"
//
//------Global variable declaration


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
)
{
    uint32 regVal = 0;
    switch (dmaId)
    {
        case 0 : 
            DMA->dma_0_src_addr = srcAddre; 
            DMA->dma_0_dst_addr = dstAddre; 
            DMA->dma_0_addr_step = (dstStep << dma_0_addr_step_0_dst_addr_step_shift)+srcStep; //[31:16] destiantion step. [15:0] source step.
        
            DMA->dma_0_config = 0;
            regVal = dma_0_config_0_en_bit |  dma_0_config_0_pri_bit | transLen;
            regVal |= memTomemOrperi << 23;
            regVal |= ( 0x3 << dma_0_config_0_dst_size_shift ) | (0x3 << dma_0_config_0_src_size_shift);
            regVal |= periSelect << dma_0_config_0_peri_sel_shift;
            DMA->dma_0_config = regVal;
            break;
        case 1 :
            DMA->dma_1_src_addr = srcAddre; // address to read windowed data for antenna 1
            DMA->dma_1_dst_addr = dstAddre; // address to store windowed data for antenna 1. Total space is 512*32*4 = 0x10000.
            DMA->dma_1_addr_step = (dstStep << dma_0_addr_step_0_dst_addr_step_shift)+srcStep; //[31:16] destiantion step. [15:0] source step.
           
            DMA->dma_1_config = 0;
            regVal = dma_1_config_1_en_bit |  dma_1_config_1_pri_bit | transLen;
            regVal |= memTomemOrperi << 23;
            regVal |= ( 0x3 << dma_1_config_1_dst_size_shift ) | (0x3 << dma_1_config_1_src_size_shift);
            regVal |= periSelect << dma_1_config_1_peri_sel_shift;
            DMA->dma_1_config = regVal;
            break;
        case 2 :
            DMA->dma_2_src_addr = srcAddre; // address to read windowed data for antenna 1
            DMA->dma_2_dst_addr = dstAddre; // address to store windowed data for antenna 1. Total space is 512*32*4 = 0x10000.
            DMA->dma_2_addr_step = (dstStep << dma_0_addr_step_0_dst_addr_step_shift)+srcStep; //[31:16] destiantion step. [15:0] source step.
        
            DMA->dma_2_config = 0;
            regVal = dma_2_config_2_en_bit |  dma_2_config_2_pri_bit | transLen;
            regVal |= memTomemOrperi << 23;
            regVal |= ( 0x3 << dma_2_config_2_dst_size_shift ) | (0x3 << dma_2_config_2_src_size_shift);
            regVal |= periSelect << dma_2_config_2_peri_sel_shift;
            DMA->dma_2_config = regVal;
            break;
        case 3 :
            DMA->dma_3_src_addr = srcAddre; // address to read windowed data for antenna 1
            DMA->dma_3_dst_addr = dstAddre; // address to store windowed data for antenna 1. Total space is 512*32*4 = 0x10000.
            DMA->dma_3_addr_step = (dstStep << dma_0_addr_step_0_dst_addr_step_shift)+srcStep; //[31:16] destiantion step. [15:0] source step.
        
            DMA->dma_3_config = 0;
            regVal = dma_3_config_3_en_bit |  dma_3_config_3_pri_bit | transLen;
            regVal |= memTomemOrperi << 23;
            regVal |= ( 0x3 << dma_3_config_3_dst_size_shift ) | (0x3 << dma_3_config_3_src_size_shift);
            regVal |= periSelect << dma_3_config_3_peri_sel_shift;
            DMA->dma_3_config = regVal;
            break;
        case 4 :
            DMA->dma_4_src_addr = srcAddre; // address to read windowed data for antenna 1
            DMA->dma_4_dst_addr = dstAddre; // address to store windowed data for antenna 1. Total space is 512*32*4 = 0x10000.
            DMA->dma_4_addr_step = (dstStep << dma_0_addr_step_0_dst_addr_step_shift)+srcStep; //[31:16] destiantion step. [15:0] source step.
            
            DMA->dma_4_config = 0;
            regVal = dma_4_config_4_en_bit |  dma_4_config_4_pri_bit | transLen;
            regVal |= memTomemOrperi << 23;
            regVal |= ( 0x3 << dma_4_config_4_dst_size_shift ) | (0x3 << dma_4_config_4_src_size_shift);
            regVal |= periSelect << dma_4_config_4_peri_sel_shift;
            DMA->dma_4_config = regVal;
            break;
        default:
            break;
    }
}
/****************************************************************************************
*Function name : dma0disable dma1Disable dma2Disable dma3Disable dma4Disable
*Description   : dma0 or dma1 or dma2 or dma3 or dma4 disable
*Paramater	   : None
*        @  : 
*        @  : 
*Return        : None
****************************************************************************************/
void dmaDisable(int32 dmaId)
{
    switch (dmaId)
    {
        case 0 : 
            DMA->dma_0_config &= ~dma_0_config_0_en_bit;
            break;
        case 1 :
            DMA->dma_1_config &= ~dma_1_config_1_en_bit;
            break;
        case 2 :
            DMA->dma_2_config &= ~dma_2_config_2_en_bit;
            break;
        case 3 :
            DMA->dma_3_config &= ~dma_3_config_3_en_bit;
            break;
        case 4 :
            DMA->dma_4_config &= ~dma_4_config_4_en_bit;
            break;
        default:
            break;
    }
}


















