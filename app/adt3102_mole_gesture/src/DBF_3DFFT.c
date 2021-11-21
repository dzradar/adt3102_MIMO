
#include <arm_math.h>
#include <math.h>
#include "peak_single_mimo.h"

  
void DBF_3DFFT(uint32 rv1, uint32 rv2, uint32 * rv_list, uint32 *target_list)
{

    int i, idx_r, idx_fd;
    int16 rdata_ch1, rdata_ch2, idata_ch1, idata_ch2;
    float  tmp1, tmp2, tmp3;
    float dphi;
    int tar_num;
    int ntar;
    ntar = 0;
    
    //target_list->Ntargets  = 0;
    *((int32 *)(target_list+3)) = 0;
    for (i = 0; i < MAX_TARGET_NUM; i++)
    {
        *((float *)(target_list+4+ i*4))   = 0;
        *((float *)(target_list+4+ i*4+1))   = 0;
        *((float *)(target_list+4+ i*4+2))   = 0;
        *((float *)(target_list+4+ i*4+3))   = 0;
    }
  
    //tar_num = rv_list.Ntargets_u16;
    tar_num = *((int32 *)(rv_list));
  

    for (i = 0; i < tar_num; i++)
    {
        //idx_r  = rv_list.r_idx[i];
        idx_r = *((uint16 *)rv_list+1+6*i+6);
        //idx_fd = rv_list.v_idx[i];
        idx_fd = *((uint16 *)rv_list+1+6*i+5);
		// this rv is not shifted.
        if (idx_fd < 16)
            idx_fd = idx_fd+16;
        else 
            idx_fd = idx_fd-16;
        rdata_ch1 = (*((int16 *)rv1 + (idx_r * RVA_HISTORY_SIZE_U16 + idx_fd)*2 )) ; //rv1[idx_r * RVA_HISTORY_SIZE_U16 + idx_fd];
        idata_ch1 = (*((int16 *)rv1 + (idx_r * RVA_HISTORY_SIZE_U16 + idx_fd)*2 + 1)) ; //rv1[idx_r * RVA_HISTORY_SIZE_U16 + idx_fd + 1];      
        rdata_ch2 = (*((int16 *)rv2 + (idx_r * RVA_HISTORY_SIZE_U16 + idx_fd)*2 )) ; //rv2[idx_r * RVA_HISTORY_SIZE_U16 + idx_fd];
        idata_ch2 = (*((int16 *)rv2 + (idx_r * RVA_HISTORY_SIZE_U16 + idx_fd)*2 + 1)) ; // rv2[idx_r * RVA_HISTORY_SIZE_U16 + idx_fd + 1];   
       /*
		//Incoherent accumulation
    int32 l = 0;
    for(int32 i =0; i<fftLen*rngMax*2; i=i+2)
    {
        int16 tmp_r = (*( ((int16 *)srcAddr) + i ));
        int16 tmp_i = (*( (int16 *)srcAddr + i+1 ));
        *( ((int32 *)dstAddr) + l )=tmp_r*tmp_r+tmp_i*tmp_i;
        l++;
    }*/       
        tmp1   = rdata_ch1 * rdata_ch2 + idata_ch1 * idata_ch2;
        tmp2   = rdata_ch1 * idata_ch2 - idata_ch1 * rdata_ch2;
        dphi = atan2(tmp2, tmp1);
        
        tmp3 = asin(dphi/3.1415f);
        tmp1 = (*((float *)(rv_list+1+3*i))) *RANGE_STEP/100.0f; 
        *((float *)(target_list+4+ ntar*4))        = tmp1;
        *((float *)(target_list+4+ ntar*4+1))       = (*((float *)(rv_list+1+3*i+1)));
        *((float *)(target_list+4+ ntar*4+2))        = tmp1 * sin(tmp3);
        *((float *)(target_list+4+ ntar*4+3))        = tmp1 * cos(tmp3);
        ntar = ntar + 1;
        if (ntar >= MAX_TARGET_NUM)
        {
            break;
        }
    }//for (i = 0; i < tar_num; i++)
    //target_list->Ntargets = ntar;
    *((int32 *)(target_list+3)) = ntar;
  
}
