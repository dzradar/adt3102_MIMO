
#include <arm_math.h>
#include <math.h>
//#include <marc.h>
#include "peak_single_mimo.h"

  
void DBF_3DFFT_MIMO(uint32 rv1, uint32 rv2, uint32 rv3, uint32 rv4, uint32 * rv_list, uint32 *target_list)
{

    int i, idx_r, idx_fd;
    int16 rdata_ch1, rdata_ch2, idata_ch1, idata_ch2;
    float  tmp1, tmp2, tmp3;
    float dphi1, dphi2, dphi3;    
    float theta, phi;
    int tar_num;
    int ntar;
    float fRange;
    
    ntar = 0;
  //arm_cfft_radix2_instance_f32 scfft2;
  //arm_cfft_radix2_init_f32(&scfft2, 32, 0, 1);
  //arm_fill_f32(0, &data_out[0][0], FFT_3D * N_fft_half * 2);
    
    //target_list->Ntargets  = 0;
    *((int32 *)(target_list+3)) = 0;
    for (i = 0; i < MAX_TARGET_NUM; i++)
    {
        *((float *)(target_list+4+ i*6))   = 0;
        *((float *)(target_list+4+ i*6+1))   = 0;
        *((float *)(target_list+4+ i*6+2))   = 0;
        *((float *)(target_list+4+ i*6+3))   = 0;
        *((float *)(target_list+4+ i*6+4))   = 0;
        *((float *)(target_list+4+ i*6+5))   = 0;
    }
  
    //tar_num = rv_list.Ntargets_u16;
    tar_num = *((int32 *)(rv_list));
  

    for (i = 0; i < tar_num; i++)
    {
        //idx_r  = rv_list.r_idx[i];
        idx_r = *((uint16 *)rv_list+2+8*i+7);
        //idx_fd = rv_list.v_idx[i];
        idx_fd = *((uint16 *)rv_list+2+8*i+6);
		// this rv is not shifted.
        if (idx_fd < 16)
            idx_fd = idx_fd+16;
        else 
            idx_fd = idx_fd-16;
        
        // Rx1 Rx2
        
        rdata_ch1 = (*((int16 *)rv1 + (idx_r * RVA_HISTORY_SIZE_U16 + idx_fd)*2 )) ; //rv1[idx_r * RVA_HISTORY_SIZE_U16 + idx_fd];
        idata_ch1 = (*((int16 *)rv1 + (idx_r * RVA_HISTORY_SIZE_U16 + idx_fd)*2 + 1)) ; //rv1[idx_r * RVA_HISTORY_SIZE_U16 + idx_fd + 1];      
        rdata_ch2 = (*((int16 *)rv2 + (idx_r * RVA_HISTORY_SIZE_U16 + idx_fd)*2 )) ; //rv2[idx_r * RVA_HISTORY_SIZE_U16 + idx_fd];
        idata_ch2 = (*((int16 *)rv2 + (idx_r * RVA_HISTORY_SIZE_U16 + idx_fd)*2 + 1)) ; // rv2[idx_r * RVA_HISTORY_SIZE_U16 + idx_fd + 1];   
  
        tmp1   = rdata_ch1 * rdata_ch2 + idata_ch1 * idata_ch2;
        tmp2   = rdata_ch1 * idata_ch2 - idata_ch1 * rdata_ch2;
        dphi1 = atan2(tmp2, tmp1);
        
        // Rx2 Rx3 
        
        rdata_ch1 = (*((int16 *)rv2 + (idx_r * RVA_HISTORY_SIZE_U16 + idx_fd)*2 )) ; //rv1[idx_r * RVA_HISTORY_SIZE_U16 + idx_fd];
        idata_ch1 = (*((int16 *)rv2 + (idx_r * RVA_HISTORY_SIZE_U16 + idx_fd)*2 + 1)) ; //rv1[idx_r * RVA_HISTORY_SIZE_U16 + idx_fd + 1];      
        rdata_ch2 = (*((int16 *)rv3 + (idx_r * RVA_HISTORY_SIZE_U16 + idx_fd)*2 )) ; //rv2[idx_r * RVA_HISTORY_SIZE_U16 + idx_fd];
        idata_ch2 = (*((int16 *)rv3 + (idx_r * RVA_HISTORY_SIZE_U16 + idx_fd)*2 + 1)) ; // rv2[idx_r * RVA_HISTORY_SIZE_U16 + idx_fd + 1];   
  
        tmp1   = rdata_ch1 * rdata_ch2 + idata_ch1 * idata_ch2;
        tmp2   = rdata_ch1 * idata_ch2 - idata_ch1 * rdata_ch2;
        dphi2 = atan2(tmp2, tmp1);
        
        // Rx3 Rx4        
        
        rdata_ch1 = (*((int16 *)rv3 + (idx_r * RVA_HISTORY_SIZE_U16 + idx_fd)*2 )) ; //rv1[idx_r * RVA_HISTORY_SIZE_U16 + idx_fd];
        idata_ch1 = (*((int16 *)rv3 + (idx_r * RVA_HISTORY_SIZE_U16 + idx_fd)*2 + 1)) ; //rv1[idx_r * RVA_HISTORY_SIZE_U16 + idx_fd + 1];      
        rdata_ch2 = (*((int16 *)rv4 + (idx_r * RVA_HISTORY_SIZE_U16 + idx_fd)*2 )) ; //rv2[idx_r * RVA_HISTORY_SIZE_U16 + idx_fd];
        idata_ch2 = (*((int16 *)rv4 + (idx_r * RVA_HISTORY_SIZE_U16 + idx_fd)*2 + 1)) ; // rv2[idx_r * RVA_HISTORY_SIZE_U16 + idx_fd + 1];   
  
        tmp1   = rdata_ch1 * rdata_ch2 + idata_ch1 * idata_ch2;
        tmp2   = rdata_ch1 * idata_ch2 - idata_ch1 * rdata_ch2;
        dphi3 = atan2(tmp2, tmp1);
               

        tmp1 = asin(dphi1/3.1415f);
        tmp2 = asin(dphi2/3.1415f);
        tmp3 = asin(dphi3/3.1415f);
        // mean       
        if (( fabs(tmp1 - tmp2) > 0.2 || ( fabs(tmp1 - tmp3) > 0.2) || ( fabs(tmp3 - tmp2) > 0.2) ))
            continue;  

        
        phi = asin((dphi1 + dphi2 + dphi3)/3.0f/3.1415f);
        //tmp3 = asin((dphi1 + dphi2)/2/3.1415f);
        //tmp3 = asin(dphi1/3.1415f);
        theta = 0;

        
        fRange = (*((float *)(rv_list+1+4*i))) *RANGE_STEP/100.0f; 
        *((float *)(target_list+4+ ntar*6))        = fRange;
        *((float *)(target_list+4+ ntar*6+1))       = (*((float *)(rv_list+1+4*i+1)));
        *((float *)(target_list+4+ ntar*6+2))        = fRange * cos(theta) * sin(phi);  //x
        *((float *)(target_list+4+ ntar*6+3))        = fRange * cos(theta) * cos(phi);  //y
        *((float *)(target_list+4+ ntar*6+4))        = fRange * sin(theta);  //z
        *((float *)(target_list+4+ ntar*6+5))        = (*((float *)(rv_list+1+4*i+2)));  //snr
        
        ntar = ntar + 1;
        if (ntar >= MAX_TARGET_NUM)
        {
            break;
        }
    }//for (i = 0; i < tar_num; i++)
    //target_list->Ntargets = ntar;
    *((int32 *)(target_list+3)) = ntar;
  
}
