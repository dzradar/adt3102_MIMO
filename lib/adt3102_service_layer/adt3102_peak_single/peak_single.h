//--------------------------------------------------------------------
//Copyright(c)2020,Andar Technologise Inc.
//All Rights Reserved
//Confidential Property of Andar Technologies Inc.
//
//Module Description:
//---------------------------------------------------------------
// To search for single max peak in 2D FFT table
//  
//
//Created by :jiangqi
//$Revision: 1.5
//$Data: 2020/11/9
//--------------------------------------------------------------------
#ifndef __PEAK_SINGLE_H__  
#define __PEAK_SINGLE_H__ 

#include "adt3102.h"

/*********************************************************
Function name: peakSingle
Description:   serach for single peak
Paramater:     dopFftLen : doppler fft length
               rngMin    : min range
               rngMax    : max range
               dopMin    : min doppler speed
			   absMergeArray : abs array, size[RANGE_MAX][CHIRP_NUM]
               powerThdAdj  : power threshold in dBFS, has to be negitive
			   nearest : 1-find the nearest object ; 0-find the highest snr object
               distComp : 1-compensate for distance ; 0-do not compensate
			   obj     : object found
Return:        void
*********************************************************/
typedef struct
{
    uint8  valid;
    int32  rngIdx;
    int32  dopIdx;
    int32  powerSignal;
    float  snrSignal;
} OBJ_PEAKSINGLE_TypeDef;

void peakSingle(int32 dopFftLen, int32 dopMin, int32 rngMin, int32 rngMax, uint32 absMergeArray[][CHIRP_NUM], int32 powerThd, uint32 nearest, uint32 distComp, OBJ_PEAKSINGLE_TypeDef *obj);
#endif

