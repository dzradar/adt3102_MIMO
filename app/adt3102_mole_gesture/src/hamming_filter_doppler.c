#include <arm_math.h>
#include <math.h>
#include "peak_single_mimo.h"




void Ham_doppler_filter(uint32 src, uint32 ham_win)
{
    int16 iread, qread;
    float hw;
	for(int i=0; i< RVA_HISTORY_SIZE_U16; i++)
    {
        hw = *((float *)(ham_win + 4*i ));
        for(int j=0; j< RVA_MAX_NSAMPLES_U16; j++)
        {
            iread = *((int16 *)(src + i*RVA_MAX_NSAMPLES_U16*4 + 4*j ));
            qread = *((int16 *)(src + i*RVA_MAX_NSAMPLES_U16*4 + 4*j + 2 ));
            
            *((int16 *)(src + i*RVA_MAX_NSAMPLES_U16*4 + 4*j )) = (int16)(iread * hw) ;
            *((int16 *)(src + i*RVA_MAX_NSAMPLES_U16*4 + 4*j + 2 )) = (int16)(qread * hw) ;
        }
    }
}
