#include <arm_math.h>
#include <math.h>
#include "peak_single_mimo.h"



/*********************************************************
Function name: dopplerSwap
Description:   serach for single peak
Paramater:     dopFftLen : doppler fft length
               rngMax    : max range
			   absMergeArray : abs array, size[RANGE_MAX][CHIRP_NUM]
Return:        none
*********************************************************/
void dopplerSwapMimo(int32 dopFftLen, int32 rngMax, uint32 absMergeArray[][CHIRP_NUM/2])
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
