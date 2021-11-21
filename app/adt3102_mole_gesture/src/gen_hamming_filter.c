#include <arm_math.h>
#include <math.h>
#include "peak_single_mimo.h"



void gen_hamm_filter(uint32 src, int16 len)
{
    int16 iread, qread;
    float hw;
    float pi=3.1416;
    float N = len-1;
    float w;
	for(int n=0; n< len; n++)
    {
        w = 0.35875 - 0.48829 * cos(2*pi*n/N) + 0.14128* cos(2*pi*n/N) - 0.01168* cos(2*pi*n/N);
        *((float *)(src + n*4 )) = w ;
    }
}
