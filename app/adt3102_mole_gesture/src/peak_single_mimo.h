//--------------------------------------------------------------------
//Copyright(c)2020,Andar Technologise Inc.
//All Rights Reserved
//Confidential Property of Andar Technologies Inc.
//
//Module Description:
//---------------------------------------------------------------
// To search for single max peak in 2D FFT table for mimo
//  
//
//Created by :jiangqi
//$Revision: 1.5
//$Data: 2021/6/9
//--------------------------------------------------------------------
#ifndef __PEAK_SINGLE_MIMO_H__  
#define __PEAK_SINGLE_MIMO_H__ 

#include "adt3102.h"

#define K_threshold_u16 8
#define cfar_window_size_u16 16
#define RVA_MAX_NSAMPLES_U16 128
#define RVA_HISTORY_SIZE_U16 32
#define MAX_TARGET_NUM 128
#define RVA_RV_LIST_MAX_U16 128
#define N_fft 128*32


typedef struct
{
    uint8  valid;
    int32  rngIdx;
    int32  dopIdx;
    float  powerSignal;
} OBJ_PEAKSINGLE_TypeDef;

//peak detection target list
typedef struct
{
    uint32           Ntargets_u16;            
    
    float         range[MAX_TARGET_NUM];      
    float         doppler[MAX_TARGET_NUM]; 
    float         snr[MAX_TARGET_NUM];
    int           r_idx[MAX_TARGET_NUM];
    int           v_idx[MAX_TARGET_NUM];
}RV_List;


typedef struct
{
    uint16       magicword[4];
    uint32       framenumber;
    int32           Ntargets;                 
    float         r_flt32[MAX_TARGET_NUM];      
    float         v_flt32[MAX_TARGET_NUM];   
    //float         alpha_flt32[MAX_TARGET_NUM]; 
    float         x_flt32[MAX_TARGET_NUM];
    float         y_flt32[MAX_TARGET_NUM];
    float         z_flt32[MAX_TARGET_NUM];
    float         snr_flt32[MAX_TARGET_NUM];

}Raw_Target_List;


void gen_hamm_filter(uint32 src, int16 len);
void Ham_range_filter(uint32 src, uint32 ham_win);
void Ham_doppler_filter(uint32 src, uint32 ham_win);
void background_removal(int32 srcAddr, int32 bgAddr, uint16 nFftRange,uint16 rangeMax, uint16 nFftVel,uint16 nLog2FftVel, float iqcoef, uint8 update_flag);
//void ch_phase_compensation_lut(float (*rv2)[RVA_HISTORY_SIZE_U16], float (*rv3)[RVA_HISTORY_SIZE_U16]);
void DBF_3DFFT(uint32 rv1, uint32 rv2, uint32 * rv_list, uint32 *target_list);
void DBF_3DFFT_MIMO(uint32 rv1, uint32 rv2, uint32 rv3, uint32 rv4, uint32 * rv_list, uint32 *target_list);
void noise_estimation_cfar (uint32 * tmp_rv_pflt32, uint32 * cfar_window_flt32, uint32 * rva_range_average_noise_pflt32, int v_u16);
void rva_detect_peaks(uint32 * rva_2d_rv_image_pfloat, uint32 * noise_v0, uint32 * noise_v1, uint32 * rva_rv_list);
void ch_phase_compensation(uint32 rv1, uint32 rv2, uint32 rv3, uint32 rv4, float * r_cali, float * i_cali);
void DBF_3DFFT_MIMO_3D(uint32 rv1, uint32 rv2, uint32 rv3, uint32 rv4, uint32 * rv_list, uint32 *target_list);

/*********************************************************
Function name: dopplerSwapMimo
Description:   serach for single peak
Paramater:     dopFftLen : doppler fft length
               rngMax    : max range
			   absMergeArray : abs array, size[RANGE_MAX][CHIRP_NUM/2]
Return:        none
*********************************************************/
void dopplerSwapMimo(int32 dopFftLen, int32 rngMax, uint32 absMergeArray[][CHIRP_NUM/2]);

/*********************************************************
Function name: peakSingleMimo
Description:   serach for single peak for mimo
Paramater:     dopFftLen : doppler fft length
               rngMin    : min range
               rngMax    : max range
               dopMin    : min doppler speed
			   absMergeArray : abs array, size[RANGE_MAX][CHIRP_NUM/2]
               powerThdAdj  : power threshold in dBFS, has to be negitive
			   nearest : 1-find the nearest object ; 0-find the highest snr object
               distComp : 1-compensate for distance ; 0-do not compensate
			   obj     : object found
Return:        void
*********************************************************/
void peakSingleMimo(int32 dopFftLen, int32 dopMin, int32 rngMin, int32 rngMax, uint32 absMergeArray[][CHIRP_NUM/2], int32 powerThd, uint32 nearest, uint32 distComp, OBJ_PEAKSINGLE_TypeDef *obj);
#endif

