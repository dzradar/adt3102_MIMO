#ifndef __DSP_H
#define __DSP_H
#include "adt3102_type_define.h"
#include "adt3102.h" 
/****************************************************************
Function name : fftProcess

*Description   : ADT301 ASIC FFT
     |   1   2 3 4 5..........................................fftLen|
      srcAddr
*Paramater      :
*        @fftLen: fft points
*        @srcAddr:fft source data first address
*        @srcStep:fft source data step
*        @dstStep:fft destination data step
*Return        : None
****************************************************************/
void fftSingle(int32 fftLen, uint32 srcAddr, uint32 dstAddr, uint16 srcStep, uint16 dstStep);
void absAdt3102(uint16 fftLen ,uint16 rngMax ,uint32 srcAddr,uint32 dstAddr);
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
void fftProcess(int32 fftLen, int32 fftChirpNum, uint32 srcAddr, uint32 dstAddr, uint16 srcStep, uint16 dstStep);
/****************************************************************************************
*Function name : velocityFftProcess
*Description   : 
*Paramater	   :
*        @dopFftLen :doppler fft points  
*        @rngMax    :maximum on range dimension   
*        @srcAddr   :doppler fft source data first address
*        @dstAddr   :doppler fft destination data first address
*        @srcStep   :doppler fft source data step
*        @dstStep   :doppler fft destination data step
*Paramater	selection:		

*Power consumption : 
*Return        :
****************************************************************************************/
void velocityFftProcess(int32 dopFftLen, int32 rngMax, uint32 srcAddr, uint32 dstAddr, uint16 srcStep, uint16 dstStep);
void dopplerSwap(int32 dopFftLen, int32 rngMax, uint32 absMergeArray[][CHIRP_NUM]);


#endif




