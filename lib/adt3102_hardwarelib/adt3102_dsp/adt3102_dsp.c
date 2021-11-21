//--------------------------------------------------------------------
//Copyright(c)2020,Andar Technologise Inc.
//All Rights Reserved
//Confidential Property of Andar Technologies Inc.
//
//Module Description:
//  .range fft
//  .velocity fft
//  .cfar(Sum Sort)
//  .
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
#include "math.h"
#include "adt3102_type_define.h"
#include "adt3102.h"
#include "stdio.h"
#include "adt3102_dsp.h"
#include "adt3102_dma.h"
#include "adt3102_gpio.h"
#include "dsp_ctype_map.h"
#include "dma_ctype_map.h"
//
//------Global variable declaration
volatile int32 v_g_fftFlag;


/****************************************************************
Function name : fftProcess

*Description   : ADT301 ASIC FFT
     |   1   2 3 4 5..........................................fftLen|
      srcAddr
*Paramater      :
*        @fftLen: fft length 32,64,128,256,512
*        @srcAddr:
*        @srcStep:
*        @dstStep:
*Return        : None
****************************************************************/
void fftSingle(int32 fftLen, uint32 srcAddr, uint32 dstAddr, uint16 srcStep, uint16 dstStep)
{
    uint32 regVal;
    DSP->dsp_fft_ctrl = dsp_fft_ctrl_fft_exp_bp_bit | dsp_fft_ctrl_fft_en_bit | fftLen;
    DSP->dsp_int_clear = dsp_int_clear_fft_start_bit;
    v_g_fftFlag = 1;
    dmaConfig(DMA0, srcAddr, FFT_BUF_HF1, srcStep, 4, fftLen/2, PERI_NONE, MEMTOMEM);  
    while(1)
    {
        regVal = DMA->dma_0_tcnt;
        if(regVal >= fftLen/2)
        {
            //printf("The adc_mem TO first_fft_mem OK \r\n");
            break;
        }
    }
    dmaDisable(DMA0);
    dmaConfig(DMA0, srcAddr+fftLen/2*srcStep, FFT_BUF_HF2, srcStep, 4, fftLen/2, PERI_NONE, MEMTOMEM);  
    while(1)
    {
        regVal = DMA->dma_0_tcnt;
        if(regVal >= fftLen/2)
        {
            //printf("The adc_mem TO first_fft_mem OK \r\n");
            break;
        }
    }
//    //software write fft
//    v_g_fftFlag = 1;
//    for(int32 i=0; i<fftLen/2;i++)
//    {
//        *(int32 *)(FFT_BUF_HF1+i*4)=*(int32 *)(srcAddr+srcStep*i);
//        //*(int32 *)(FFT_BUF_HF2+i*4)=*(int32 *)(srcAddr+fftLen*2+srcStep*i);
//    }    
//    //has to finish 1st half !
//    for(int32 i=0; i<fftLen/2;i++)
//    {
//        *(int32 *)(FFT_BUF_HF2+i*4)=*(int32 *)(srcAddr+fftLen/2*srcStep+srcStep*i);
//    }
    while( 1 == v_g_fftFlag); // printf("1D fft done ! \r\n");
    dmaDisable(DMA0);
    dmaConfig(DMA0, FFT_BUF_HF1, dstAddr, 4, dstStep, fftLen/2, PERI_NONE, MEMTOMEM);
    while(1)
    {
        regVal = DMA->dma_0_tcnt;
        if(regVal >= fftLen/2)
        {
            //printf("The adc_mem TO first_fft_mem OK \r\n");
            break;
        }
    }
    dmaDisable(DMA0);
    dmaConfig(DMA0, FFT_BUF_HF2, dstAddr+fftLen/2*dstStep, 4, dstStep, fftLen/2, PERI_NONE, MEMTOMEM);
    while(1)
    {
        regVal = DMA->dma_0_tcnt;
        if(regVal >= fftLen/2)
        {
            //printf("The adc_mem TO first_fft_mem OK \r\n");
            break;
        }
    }
    dmaDisable(DMA0);
//    //software write fft
//    for(int32 i=0; i<fftLen/2;i++)
//    {
//        *(int32 *)(dstAddr+dstStep*i)=*(int32 *)(FFT_BUF_HF1+i*4);
//        //*(int32 *)(dstAddr+fftLen*2+dstStep*i)=*(int32 *)(FFT_BUF_HF2+i*4);
//    }    
//    for(int32 i=0; i<fftLen/2;i++)
//    {
//        *(int32 *)(dstAddr+fftLen/2*dstStep+dstStep*i)=*(int32 *)(FFT_BUF_HF2+i*4);
//    }
    v_g_fftFlag = 1;
}

void absAdt3102(uint16 fftLen ,uint16 rngMax ,uint32 srcAddr,uint32 dstAddr)
{
    //Incoherent accumulation
    int32 l = 0;
    for(int32 i =0; i<fftLen*rngMax*2; i=i+2)
    {
        int16 tmp_r = (*( ((int16 *)srcAddr) + i ));
        int16 tmp_i = (*( (int16 *)srcAddr + i+1 ));
        *( ((int32 *)dstAddr) + l )=tmp_r*tmp_r+tmp_i*tmp_i;
        l++;
    }
}
/****************************************************************
Function name : fftProcess

*Description   : ADT301 ASIC FFT
srcAddr |11 12 13 14 15..........................................1(fftLen) |
        |21 22 23 24 25..........................................2(fftLen) |
        |31 32 33 34 35..........................................3(fftLen) |
        |..................................................................|
        |..................................................................|
        |..................................................................|
        |..................................................................|
        |..................................................................|
        |(countFft)1 (countFft)2 (countFft)3.............(countFft)(fftLen)|

*Paramater      :
*        @fftLen:
*        @srcAddr:
*        @srcStep:
*        @dstStep:
*Return        : None
****************************************************************/
void fftProcess(int32 fftLen, int32 fftChirpNum, uint32 srcAddr, uint32 dstAddr, uint16 srcStep, uint16 dstStep)
{
    //DSP->dsp_fft_ctrl = dsp_fft_ctrl_fft_exp_bp_bit | fftLen;
    for(int32 fftFrameCount=0; fftFrameCount<fftChirpNum; fftFrameCount++)
    {
        fftSingle(fftLen, srcAddr+fftFrameCount*fftLen*4, dstAddr+fftFrameCount*fftLen*4, srcStep, dstStep);
    }  
    DSP->dsp_fft_ctrl &= ~dsp_fft_ctrl_fft_en_bit;
}
/****************************************************************************************
*Function name : 
*Description   : 
*Paramater	   :
*        @ :  
*        @ :   
*Paramater	selection:		

*Power consumption : 
*Return        :
****************************************************************************************/
void velocityFftProcess(int32 dopFftLen, int32 rngMax, uint32 srcAddr, uint32 dstAddr, uint16 srcStep, uint16 dstStep)
{
    //DSP->dsp_fft_ctrl = dsp_fft_ctrl_fft_exp_bp_bit | fftLen;
    for(int32 fftFrameCount=0; fftFrameCount<rngMax; fftFrameCount++)
    {
        fftSingle(dopFftLen, srcAddr+fftFrameCount*4, dstAddr+fftFrameCount*dopFftLen*4, srcStep, dstStep);
        //fftSingle(dopFftLen, srcAddr+fftFrameCount*4, dstAddr+fftFrameCount*4, srcStep, dstStep);
    }  
    DSP->dsp_fft_ctrl &= ~dsp_fft_ctrl_fft_en_bit;
}
/*********************************************************
Function name: dopplerSwap
Description:   serach for single peak
Paramater:     dopFftLen : doppler fft length
               rngMax    : max range
			   absMergeArray : abs array, size[RANGE_MAX][CHIRP_NUM]
Return:        none
*********************************************************/
void dopplerSwap(int32 dopFftLen, int32 rngMax, uint32 absMergeArray[][CHIRP_NUM])
{
    uint32 tempdata2Dfft1,tempdata2Dfft2;
	for(uint32 count1 =0; count1< rngMax; count1++)
    {
        for(unsigned char count2 =0; count2< dopFftLen/2; count2++)
        {
            //swap velocity.After swap, absMergeArray[][dopFftLen/2] is static 
            tempdata2Dfft1  = absMergeArray[count1][count2];
            tempdata2Dfft2  = absMergeArray[count1][count2+dopFftLen/2];
            absMergeArray[count1][count2] = tempdata2Dfft2;
            absMergeArray[count1][count2+dopFftLen/2] = tempdata2Dfft1 ;
        }
    }
}
