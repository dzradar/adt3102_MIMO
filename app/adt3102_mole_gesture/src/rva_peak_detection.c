#include <arm_math.h>
#include <math.h>
#include "peak_single_mimo.h"

void rva_detect_peaks(uint32 * rva_2d_rv_image_pfloat, uint32 * noise_v0, uint32 * noise_v1, uint32 * rva_rv_list)
{
	int r_u16,v_u16;
	int r_max_u16,v_max_u16;
	int subr_s16,subv_s16;
	int rindex,vindex;
	float val_float;
	float cmp_float;
    float cmp_float_v0;
    float cmp_float_v1;
	float local_cmp_float;
	int breakflag_u16;
	//float threshold_factor_float;
	float sum_r_float;
	float sum_v_float;
	float iten_sum_float;
    float tmp_snr;
    float noise_level;


    

    *((int32 *)(rva_rv_list)) = 0;
    for(r_u16 =0 ; r_u16 < RVA_RV_LIST_MAX_U16; r_u16++)
    {
        //rva_rv_list->r_idx[r_u16] = 0;
        //rva_rv_list->v_idx[r_u16] = 0; 
        *((int32 *)(rva_rv_list+1+r_u16*4))=0;
        *((int32 *)(rva_rv_list+1+r_u16*4+1))=0;
        *((int32 *)(rva_rv_list+1+r_u16*4+2))=0;
        *((int32 *)(rva_rv_list+1+r_u16*4+3))=0;
    }


	r_max_u16 = RVA_MAX_NSAMPLES_U16;
	v_max_u16 = RVA_HISTORY_SIZE_U16;
    //    threshold_factor_float = 1.1;


	// detect peaks for range bin 1 to 126

	for (r_u16=5;r_u16<r_max_u16-10;r_u16++) {

		cmp_float_v0 = noise_v0[r_u16]*2;       //static noise
        cmp_float_v1 = noise_v1[r_u16]*20;      //dynamic noise

		for (v_u16=1;v_u16<v_max_u16-1;v_u16++) {

		val_float = rva_2d_rv_image_pfloat[v_u16+r_u16*RVA_HISTORY_SIZE_U16];
                        
        if (v_u16 == RVA_HISTORY_SIZE_U16/2)
        {
            cmp_float = cmp_float_v0;
            noise_level = noise_v0[r_u16];
        }
        else
        {
            cmp_float = cmp_float_v1;
            noise_level = noise_v1[r_u16];
        }
	
			if (val_float > cmp_float) {
			
				// step 1 : find local maximum
				breakflag_u16 = 0;
				for (subr_s16=-1;(subr_s16<=1)&&(0==breakflag_u16);subr_s16++) {
					rindex = r_u16+subr_s16;
					for (subv_s16=-1;(subv_s16<=1)&&(0==breakflag_u16);subv_s16++) {
						vindex = v_u16+subv_s16;
						local_cmp_float = rva_2d_rv_image_pfloat[vindex+rindex*RVA_HISTORY_SIZE_U16];
						if (local_cmp_float>val_float) { 
							breakflag_u16 = 1;
						}
					}
				}

				if ((0==breakflag_u16)&&( *((int32 *)(rva_rv_list)) <RVA_RV_LIST_MAX_U16)) {

					// step 2: find balance point
					sum_v_float = 0;
					sum_r_float = 0;
					iten_sum_float =0;
					for (subr_s16=-1;(subr_s16<=1);subr_s16++) {
						rindex = r_u16+subr_s16;
						for (subv_s16=-1;(subv_s16<=1);subv_s16++) {
							vindex = v_u16+subv_s16;
							local_cmp_float = rva_2d_rv_image_pfloat[vindex+rindex*RVA_HISTORY_SIZE_U16];
							sum_v_float += vindex*local_cmp_float;
							sum_r_float += rindex*local_cmp_float;
							iten_sum_float +=local_cmp_float;
						}
					}
				


					// Step 3: saving peak in raw target list

					sum_r_float/=iten_sum_float;
					sum_v_float/=iten_sum_float;

					//rva_rv_list->range[rva_rv_list->Ntargets_u16] = (float)sum_r_float ;
                    (*((float *)rva_rv_list+1+4*(*((int32 *)(rva_rv_list))))) = (float)sum_r_float ;
					//rva_rv_list->doppler[rva_rv_list->Ntargets_u16] = (float)(sum_v_float-(float)v_max_u16/2);
                    (*((float *)rva_rv_list+1+4*(*((int32 *)(rva_rv_list)))+1)) = (float)(sum_v_float-(float)v_max_u16/2);
                    tmp_snr = 4.3429f*log((float)val_float/noise_level); 
                    (*((float *)rva_rv_list+1+4*(*((int32 *)(rva_rv_list)))+2)) = tmp_snr;   // snr float
 

					//rva_rv_list->r_idx[rva_rv_list->Ntargets_u16] = r_u16;
                    (*((uint16 *)rva_rv_list+2+8*(*((int32 *)(rva_rv_list)))+7)) = r_u16;
					//rva_rv_list->v_idx[rva_rv_list->Ntargets_u16] = v_u16;
                    (*((uint16 *)rva_rv_list+2+8*(*((int32 *)(rva_rv_list)))+6)) = v_u16;

					/*
					increment target counter
					*/
					(*((int32 *)(rva_rv_list)))++;

					
				}
			}
		}
	}


}

