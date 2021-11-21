
#include <arm_math.h>
#include <math.h>
#include "peak_single_mimo.h"



float coeff_dop_dvi[RVA_HISTORY_SIZE_U16] = {     
    1.0000,
    0.9952,
    0.9808,
    0.9569,
    0.9239,
    0.8819,
    0.8315,
    0.7730,
    0.7071,
    0.6344,
    0.5556,
    0.4714,
    0.3827,
    0.2903,
    0.1951,
    0.0980,
   -0.0000,
    0.0980,
    0.1951,
    0.2903,
    0.3827,
    0.4714,
    0.5556,
    0.6344,
    0.7071,
    0.7730,
    0.8315,
    0.8819,
    0.9239,
    0.9569,
    0.9808,
    0.9952};
  

float coeff_dop_dvq[RVA_HISTORY_SIZE_U16] = {        
    0,
   -0.0980,
   -0.1951,
   -0.2903,
   -0.3827,
   -0.4714,
   -0.5556,
   -0.6344,
   -0.7071,
   -0.7730,
   -0.8315,
   -0.8819,
   -0.9239,
   -0.9569,
   -0.9808,
   -0.9952,
    1.0000,
    0.9952,
    0.9808,
    0.9569,
    0.9239,
    0.8819,
    0.8315,
    0.7730,
    0.7071,
    0.6344,
    0.5556,
    0.4714,
    0.3827,
    0.2903,
    0.1951,
    0.0980
  
};


void ch_phase_compensation(uint32 rv1, uint32 rv2, uint32 rv3, uint32 rv4, float * r_cali, float * i_cali)
{

    int i, j;
    float rdata, idata;
	float tmpr,tmpi;
	//float r_cali[4], i_cali[4];
    //float dphi;
    //int idx;
    float dvi, dvq;
    /*
    for (i = 0; i< 4; i++)
    {
        r_cali[i] = cos(cali_phase[i]);
        i_cali[i] = sin(cali_phase[i]);
    }*/
	// loop for all points 
	// compensate channel phase 
    

    
    for (i = 0; i < RVA_MAX_NSAMPLES_U16*RVA_HISTORY_SIZE_U16*2; i=i+2)
    {
        // rx2
        rdata = (*( (int16 *)rv2 + i ));
        idata = (*( (int16 *)rv2 + i +1));
		
		tmpr = rdata * r_cali[1] - idata * i_cali[1];
		tmpi = rdata * i_cali[1] + idata * r_cali[1];
		(*( (int16 *)rv2 + i )) = (int16)tmpr;
		(*( (int16 *)rv2 + i +1)) = (int16)tmpi;

        // rx3
        rdata = (*( (int16 *)rv3 + i ));
        idata = (*( (int16 *)rv3 + i +1));
		
		tmpr = rdata * r_cali[2] - idata * i_cali[2];
		tmpi = rdata * i_cali[2] + idata * r_cali[2];
		(*( (int16 *)rv3 + i )) = (int16)tmpr;
		(*( (int16 *)rv3 + i +1)) = (int16)tmpi;
        
        // rx4
        rdata = (*( (int16 *)rv4 + i ));
        idata = (*( (int16 *)rv4 + i +1));
		
		tmpr = rdata * r_cali[3] - idata * i_cali[3];
		tmpi = rdata * i_cali[3] + idata * r_cali[3];
		(*( (int16 *)rv4 + i )) = (int16)tmpr;
		(*( (int16 *)rv4 + i +1)) = (int16)tmpi;
    }

    
    // Rx2 and Rx4 need doppler compensation
    
    for(i=0; i < RVA_MAX_NSAMPLES_U16; i ++)
    {
        for(j=0; j < RVA_HISTORY_SIZE_U16; j++)
        {    
/*            
            if(j<16)
            {
                idx = j;
            }
            else
            {
                idx = j - RVA_HISTORY_SIZE_U16;
            }
            dphi = 3.1416*idx/32;
            dvi = cos(-dphi);
            dvq = sin(-dphi);
            */
            
            //(*( (float *)(DOP_COMP_COEFF + j*8 ))) = dvi;
            //(*( (float *)(DOP_COMP_COEFF + j*8 +4 ))) = dvq;
            dvi = coeff_dop_dvi[j];
            dvq = coeff_dop_dvq[j];
                   
            // Rx2
            rdata = (*( (int16 *)rv2 + (i*RVA_HISTORY_SIZE_U16+j)*2));
            idata = (*( (int16 *)rv2 + (i*RVA_HISTORY_SIZE_U16+j)*2 + 1));

            tmpr = rdata * dvi - idata * dvq;
            tmpi = rdata * dvq + idata * dvi;            
            
            (*( (int16 *)rv2 + (i*RVA_HISTORY_SIZE_U16+j)*2)) = (int16)tmpr;
            (*( (int16 *)rv2 + (i*RVA_HISTORY_SIZE_U16+j)*2 + 1))= (int16)tmpi;
            
            // Rx4
            rdata = (*( (int16 *)rv4 + (i*RVA_HISTORY_SIZE_U16+j)*2));
            idata = (*( (int16 *)rv4 + (i*RVA_HISTORY_SIZE_U16+j)*2 + 1));

            tmpr = rdata * dvi - idata * dvq;
            tmpi = rdata * dvq + idata * dvi;            
            
            (*( (int16 *)rv4 + (i*RVA_HISTORY_SIZE_U16+j)*2)) = (int16)tmpr;
            (*( (int16 *)rv4 + (i*RVA_HISTORY_SIZE_U16+j)*2 + 1))= (int16)tmpi;
        }
    }
}
