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
void Ham_range_filter(uint32 src, uint32 ham_win)
{
    int16 iread, qread;
    float hw;
	for(int i=0; i< RVA_HISTORY_SIZE_U16; i++)
    {
        for(int j=0; j< RVA_MAX_NSAMPLES_U16; j++)
        {
            iread = *((int16 *)(src + i*RVA_MAX_NSAMPLES_U16*4 + 4*j ));
            qread = *((int16 *)(src + i*RVA_MAX_NSAMPLES_U16*4 + 4*j + 2 ));
            hw = *((float *)(ham_win + 4*j ));
            *((int16 *)(src + i*RVA_MAX_NSAMPLES_U16*4 + 4*j )) = (int16)(iread * hw) ;
            *((int16 *)(src + i*RVA_MAX_NSAMPLES_U16*4 + 4*j + 2 )) = (int16)(qread * hw) ;
        }
    }
}
