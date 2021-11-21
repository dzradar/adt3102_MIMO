#ifndef __SAGC__H
#define __SAGC__H
#include "adt3102_type_define.h"
/****************************************************************************************
*
*Function name : sagc
*Description   : software if agc using only one channel input
*Paramater	   :
*        @highGainEn : Set to 1 to make highest tiaGain 3, otherwise 2.
*        @srcAddr : If data address. Data arrangement is 16 bit I in lower address and 16bit Q in higher. 
*        @length: Samples to calculate input amplitute.  
*        @avgTarget: target output average amplitute for agc. Sinwave ideal average is 4096/pi=1303.
*        @pTiaGain,pPgaGain : pointer to tiaGain and pgaGain
*function call example: sAgc(RNG_FFT_CH1_ADDR, NFFT_RNG*8,1000,&tiaGain,&vgaGain);
*Return        : None
****************************************************************************************/
void sAgc(int16 highGainEn, int32 srcAddr, int16 length, int16 avgTarget, uint32 *pTiaGain, uint32 *pPgaGain);

#endif



