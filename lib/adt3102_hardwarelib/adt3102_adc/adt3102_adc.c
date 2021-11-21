//--------------------------------------------------------------------
//Copyright(c)2020,Andar Technologise Inc.
//All Rights Reserved
//Confidential Property of Andar Technologies Inc.
//
//Module Description:
//  .adc config
//  .adc sampling data with dma
//  .siggnen
//  .
//  .
//  .
//  .
//  .

//Created by :wuhao
//$Revision: 1.0
//$Data: 2020/11/9
//--------------------------------------------------------------------
//
//All include header files
#include "adt3102_type_define.h"
#include "adt3102.h"
#include "math.h"
#include "adt3102_system.h"
#include "adt3102_adc.h"
#include "adt3102_dma.h"
#include "dsp_ctype_map.h"
#include "rfc_ctype_map.h"
#include "dma_ctype_map.h"
#include "pmu_ctype_map.h"
//
//------Global variable declaration

/****************************************************************************************
*Function name : adt3102 adc setting
*Description   : 
*Parameters    :
rx0En,rx1En : RX channel enable
wnSel       : 1:windowing cof select 0 hanning 1 hamming
****************************************************************************************/
void adt3102AdcSetting(int32 rx0En,int32 rx1En,int32 wnSel,int32 adcClk,uint32 chirpNumber)
{  
    int32 biasRx0=rx0En*8;
    int32 biasRx1=rx1En*8;  
    adcEnable(rx0En, rx1En);
    adcBiasSetting(biasRx0, biasRx0, biasRx0, biasRx0, biasRx0, biasRx0, biasRx1, biasRx1, biasRx1, biasRx1, biasRx1, biasRx1);
    adcClkSwitch(adcClk);
    adcSampleInit(rx0En, rx1En);
    adcWnpara(wnSel, WN_BYPASS, SAMPLE_POINT);
    //set 1/2 down sample.
    //50M Hclk, adc read buffer rate:50M/4.5/2(adc0 and adc1)=5.555M>8.333/2M
    adcFirDownsample(0, DOWN_SAMPLE);
    dataSampleConfig(INTERVALNUM, SAMPLE_END_NUM, SAMPLE_POINT, DOWN_SAMPLE, chirpNumber);
}

/****************************************************************************************
*Function name : siggen
*Description   : Generate adc data with object information
*Paramater	   :
*        @step     :siggen_step[31:16] :step
*        @chgStep  :siggen_step[15:0] :chg_stp
*        @comp     :siggen_ctrl[17] :comp
*        @siggenEn :siggen_ctrl[16] :en
*        @ang      :siggen_ctrl[15:0]:ang
*        @         :
*
*Return        : None
****************************************************************************************/
void siggen(uint16 step, uint16 chgStep, uint16 comp, uint16 siggenEn, uint16 ang)
{
    /*RW
    [31:16] step:Phase step for 256x sinewave table. [15:8] ingeter; [7:0] fraction.
                                        For example, [15:8] = 1, [7:0] = 0 will generate a waveform whose frequence is 1/256 of Fadc.

    [15:0] chg_stp:Phase change step. Phase step is increased by this value every sample.
                   Accumulated phase step is reset to original phase step on a adc start.
                   Set to 0 if want constant phase step.
                   Actual phase_step in clock n is phase_step[15:0]*2^16 + phase_chg_step[15:0]*n.
    */
    DSP->siggen_step = (step << siggen_step_stp_shift)|(chgStep);
    /*RW
    [17] comp:Data from signal generater is 2's complement, otherwise it's unsigned.
    [16] en:Enable signal generater
    [15:0] ang:Object angler [15:8] ingeter; [7:0] fraction.
           Phase advance of antennant1 comparing with antennant0.
           [15:8] = 32 means 45 (32/256*360) degree advance of ant1.
    */
    DSP->siggen_ctrl = (comp << 17)|(siggenEn << 16)|ang;
}
/****************************************************************************************
*Function name : gpadcEnable
*Description   : adout power
*Paramater	   :
*        @
*        @
*
*Return        : None
****************************************************************************************/
void gpadcEnable(uint16 enableSigInput)
{
    //RO[7:0] gpadc_data:General purpose ADC data
    //RFC->rf_gpadc_data_reg =
    //RW[7:0] gpadc_reg1:reg gpadc_reg2:reg gpadc_reg3:reg
    RFC->rf_gpadc_reg_reg = (1<<(7+enableSigInput))|1<<0|1<<5|1<<2;
}

/****************************************************************************************
*Function name : adcBiasSetting
*Description   :
*register*RW*rf_bias2_dinb_reg
[31:28]	4'h0	bias2_din15:NC
[27:24]	4'h0	bias2_din14:NC
[23:20]	4'h0	bias2_din13:NC
[19:16]	4'h0	bias2_din12:NC
[15:12]	4'h8	bias2_din11:ADC1 bias3
[11:8]	4'h8	bias2_din10:ADC1 bias2
[7:4]	  4'h8	bias2_din9: ADC1 bias1
[3:0]	  4'h8	bias2_din8: ADC1 bias0
*register*RW*rf_bias2_dina_reg
[31:28]	4'h8	bias2_din7: ADC0 bias3
[27:24]	4'h8	bias2_din6: ADC0 bias2
[23:20]	4'h8	bias2_din5: ADC0 bias1
[19:16]	4'h8	bias2_din4: ADC0 bias0
*Paramater	   :
*        @
*        @
*
*Return        : None
****************************************************************************************/
void adcBiasSetting(int32 adc0Bias0, int32 adc0Bias1, int32 adc0Bias2, int32 adc0Bias3,
                    int32 adc0Bias4, int32 adc0Bias5, int32 adc1Bias0, int32 adc1Bias1,
                    int32 adc1Bias2, int32 adc1Bias3, int32 adc1Bias4, int32 adc1Bias5)
{
    uint32 regVel = 0;
    regVel = RFC->rf_bias2_dina_reg;
    regVel &= ~(rf_bias2_dina_reg_bias2_din4_mask<<rf_bias2_dina_reg_bias2_din4_shift);
    regVel |= adc0Bias0<<rf_bias2_dina_reg_bias2_din4_shift;

    regVel &= ~(rf_bias2_dina_reg_bias2_din5_mask<<rf_bias2_dina_reg_bias2_din5_shift);
    regVel |= adc0Bias1<<rf_bias2_dina_reg_bias2_din5_shift;

    regVel &= ~(rf_bias2_dina_reg_bias2_din6_mask<<rf_bias2_dina_reg_bias2_din6_shift);
    regVel |= adc0Bias2<<rf_bias2_dina_reg_bias2_din6_shift;

    regVel &= ~((uint32)rf_bias2_dina_reg_bias2_din7_mask<<rf_bias2_dina_reg_bias2_din7_shift);
    regVel |= adc0Bias3<<rf_bias2_dina_reg_bias2_din7_shift;
    RFC->rf_bias2_dina_reg = regVel;

    regVel = 0;
    RFC->rf_bias2_dinb_reg = 0;
    regVel |= adc1Bias0<<rf_bias2_dinb_reg_bias2_din8_shift;
    regVel |= adc1Bias1<<rf_bias2_dinb_reg_bias2_din9_shift;
    regVel |= adc1Bias2<<rf_bias2_dinb_reg_bias2_din10_shift;
    regVel |= adc1Bias3<<rf_bias2_dinb_reg_bias2_din11_shift;
	// din12-din15 are NC for adt3102, from Yinwan. 
    //regVel |= adc1Bias4<<rf_bias2_dinb_reg_bias2_din12_shift;
    //regVel |= adc1Bias5<<rf_bias2_dinb_reg_bias2_din13_shift;
    //regVel |= adc0Bias4<<rf_bias2_dinb_reg_bias2_din14_shift;
    //regVel |= adc0Bias5<<rf_bias2_dinb_reg_bias2_din15_shift;
    RFC->rf_bias2_dinb_reg = regVel;
}
/****************************************************************************************
*Function name : baseBandBiasSetting
*Description   : baseBand Bias Setting, enable signal for ztc bias for BB 
*Paramater	   :
*        @bbBiasEn :enable signal for ztc bias for BB   
*        @ :   
*Paramater	selection:		

*Power consumption : 
*Return        :
****************************************************************************************/
void baseBandBiasSetting(int32 bbBiasEn)
{
    if (bbBiasEn == 1)
    {
        RFC->rf_bias2_regs_reg |= 1<<8;
    } else {

        RFC->rf_bias2_regs_reg &= ~(1<<8);
    }
}
/****************************************************************************************
*Function name : adcEnable
*Description   : enable adc, and enable clk output to adc
*Paramater	   :
*        @adc0Enable :adc0 enable
*        @adc1Enable :adc1 enable
*
*Return        : None
****************************************************************************************/
void adcEnable(int32 adc0Enable,int32 adc1Enable)
{
    /*RW
        [18]  adc0_atb_en
        [17]  adc0_ref_en
        [16]  adc0_sar_en
        [15:8]adc0_reg2:bbadc_reg
        [7:0] adc0_reg1:bbadc_config
    */
    RFC->rf_adc0_regs_reg = adc0Enable ? (1<<16|1<<17|0x1A1A) : 0x1A1A;
    //RFC->rf_adc0_regs_reg = adc0Enable ? (1<<16|1<<17|0x1A1A) : 1<<16|0x1A1A; //to fix power ctrl issue
    RFC->rf_adc1_regs_reg = adc1Enable ? (1<<16|1<<17) : 0x00;
    /*RO
        [23:12]adc0_qdata: IF ADC0 Q channel data
        [11: 0]adc0_idata: IF ADC0 I channel data
    */
    //RFC->rf_adc0_data_reg =
    //RFC->rf_adc1_data_reg =
	/*RO
		[0]	1'h0	plli_en_ckadc: 1: Enable clk output to ADC 
    */
    RFC->rf_atb_cfg_reg = 0x030;  //bbadc_config<3:0>
    RFC->rf_spare_reg = 0x031;  //bbadc_config<9:4> and SW_PLLINT: enable pll output to 20G buffer.
//    RFC->rfc_plli_ctrl_reg |= 1<<0;
//    PMU->pmu_pmu_ctrl_reg |= pmu_pmu_ctrl_reg_manu_ctrl_en_bit;
//    delayUs(100, HIGH_CLOCK);
//    PMU->pmu_pmu_ctrl_reg &= ~pmu_pmu_ctrl_reg_manu_ctrl_en_bit;
}
/****************************************************************************************
*Function name : adcClkSwitch
*Description   : adc clock switch
*Paramater	   :
*        @adcClk :  
*        @ :   
*Paramater	selection:	ADC_8P3M  ADC_16P6M	 

*Power consumption : 
*Return        :
****************************************************************************************/
void adcClkSwitch(int32 adcClk)
{
    if (adcClk == ADC_8P3M)
    {
        RFC->rf_plli_rega_reg |= 1<<9;
    }
    else
    {
        RFC->rf_plli_rega_reg &= ~(1<<9);
    }
}
/****************************************************************************************
*Function name : adcSampleInit
*Description   : adc sample init, ch0 enable and ch1 enable  
*Paramater	   : 
*        @ch0 : 
*        @ch1 : 
*Return        : None
****************************************************************************************/
void adcSampleInit(int32 ch0, int32 ch1)
{
    uint32 regVal;
    /*RW
    [4] adc_signed_flag:
        1'b1: means the ADC output data is signed
        1'b0: means the ADC output data is unsigned
    [3] ch1 q adc enable
    [2] ch1 i adc enable
    [1] ch0 q adc enable
    [0] ch0 i adc enable
    */
    regVal = DSP->ds_adc_enable;
    if (ch0 == 1)
    {
        regVal |=  ds_adc_enable_ch0_i_en_bit | ds_adc_enable_ch0_q_en_bit;
    }
    else
    {
        regVal &=  ~(ds_adc_enable_ch0_i_en_bit | ds_adc_enable_ch0_q_en_bit);
    }
    if (ch1 == 1)
    {
        regVal |=  ds_adc_enable_ch1_i_en_bit | ds_adc_enable_ch1_q_en_bit;
    }
    else
    {
        regVal &=  ~(ds_adc_enable_ch1_i_en_bit | ds_adc_enable_ch1_q_en_bit);
    }
    DSP->ds_adc_enable =regVal;
    /*RW
    [21:11] ch0_q_data_offset
    [10:0]  ch0_i_data_offset
    *///the register keeps the default
    //DSP->ds_adc_data_ch0_offset =
    //DSP->ds_adc_data_ch1_offset =

    /*RW
    [7] ch_swap:Set to 1 to swap channel 0 and channel 1 data.
    [6] offset_auto:Set to 1 to remove offset automatically, otherwise use register offsets.
    [5:4] iq_swap:Set to 1 to swap adc i/q data. [4]: channel0 i/q swap. [5]: channel1 i/q swap
    [3:0] dat_invt:Set to 1 to invert adc data. [0]: ch0_i. [1]: ch0_q. [2]:ch1_i. [3]:ch1_q
    */
    //dc_remove & iq_swap for ch1
    DSP->ds_adc_format_ctrl = ds_adc_format_ctrl_offset_auto_bit | (2 << ds_adc_format_ctrl_iq_swap_shift);
    //DSP->ds_adc_format_ctrl = (2 << ds_adc_format_ctrl_iq_swap_shift);
}

/****************************************************************************************
*
*Function name : adcWnpara
*Description   : bbadc with window
*Paramater	   :
*			@cofSel : windowing cof select 0 hanning 1 hamming
*			@bypass : bypass:2: set to 1 to bypasse windowing
*			@wnLen  : window len, valid settings: 32,64,128,256,512
*			@
*Return        : None
****************************************************************************************/
void adcWnpara(int32 cofSel, int32 bypass, int32 wnLen)
{
    /*RW
    [11:2] wn_len:3: window len, valid settings: 32,64,128,256,512
    [1]    bypass:2: set to 1 to bypasse windowing
    [0]    cof_sel:1:windowing cof select 0 hanning 1 hamming
    */
    DSP->wn_wn_para = (wnLen << 2)|(bypass << 1)|cofSel;
}

/****************************************************************************************
*Function name : adcFirDownsample
*Description   :
*Paramater	   : from adt3102.h FirDownsample firds
*			@firBypass:fir bypass
*			@
*Return        : None
****************************************************************************************/
FirDownsample firds = {
    FIRREG0 ,//firReg0
    FIRREG1 ,//firReg1
    FIRREG2 ,//firReg2
    FIRREG3 ,//firReg3
    FIRREG4 ,//firReg4
    FIRREG5 ,//firReg5
    FIRREG6 ,//firReg6
    FIRREG7 ,//firReg7
    FIRREG8 ,//firReg8
    FIRREG9 ,//firReg9
    FIRREG10,//firReg10
    FIRREG11,//firReg11
    FIRREG12,//firReg12
    FIRREG13,//firReg13
    FIRREG14,//firReg14
    FIRREG15,//firReg15
    FC
};
/****************************************************************************************
*Function name : adcFirDownsample
*Description   : the function is used to adc fir bypass and dowmsample
*Paramater	   :
*        @firBypass : fir byass
*        @downSample : set adc sample dowmsample  
*Paramater	selection:		

*Power consumption : 
*Return        :
****************************************************************************************/
void adcFirDownsample(int32 firBypass, uint16 downSample)
{
    uint32 regVal;
    regVal = ((downSample) << fir_cof_sample_rate_down_sample_rate_shift) | (firBypass << 17) | firds.filter_coefficient;
    DSP->fir_cof_sample_rate = regVal;
    /*RW
    [7] din_dly2_en:fir input delay 2 cycles to match rf bist sync flag. For rf bist mode only.
    [6] grp_dly_en:
    [5:0] 	grp_dly_num:1:fir filter group dely num
    *///the register keeps the default
    //DSP->fir_grp_dly_select =

    DSP->fir_reg0  = firds.firReg0  ;
    DSP->fir_reg1  = firds.firReg1  ;
    DSP->fir_reg2  = firds.firReg2  ;
    DSP->fir_reg3  = firds.firReg3  ;
    DSP->fir_reg4  = firds.firReg4  ;
    DSP->fir_reg5  = firds.firReg5  ;
    DSP->fir_reg6  = firds.firReg6  ;
    DSP->fir_reg7  = firds.firReg7  ;
    DSP->fir_reg8  = firds.firReg8  ;
    DSP->fir_reg9  = firds.firReg9  ;
    DSP->fir_reg10 = firds.firReg10 ;
    DSP->fir_reg11 = firds.firReg11 ;
    DSP->fir_reg12 = firds.firReg12 ;
    DSP->fir_reg13 = firds.firReg13 ;
    DSP->fir_reg14 = firds.firReg14 ;
    DSP->fir_reg15 = firds.firReg15 ;
}
/****************************************************************************************
*Function name : dataSampleConfig
*Description   : adc sample config, is used to extract data
*Paramater	   : 
*			@intervalNum :pll adc interval num
*			@endNum : the interval_num = start_num + chirp_sample*2 + end_num, which 
					   means head, valid and tail separately.
*			@rangeFftPoint : a chirp include adc point number
*			@downSample : adc downSample 
*			@chirpNum : fmcw chirp number
*Return        : None
****************************************************************************************/
uint32 getIntervalNum(uint32 t0,uint32 t1,uint32 t2)
{
    uint32 t_temp;
    t_temp = t0 + t1 + t2;
    while((t_temp%3) != 0)
    {
       if((t_temp%3) == 1)
       {
            t0--;
       }else if ((t_temp%3) == 2)
       {
            t0++;
       }
       t_temp = t0 + t1 + t2;
    }
    return t_temp*1000/120;
}
void dataSampleConfig(uint16 intervalNum,uint16 endNum,uint16 rangeFftPoint,uint16 downSample, uint16 chirpNum)
{

    // the interval_num = start_num + chirp_sample*2 + end_num, which means head, valid and tail separately.
    DSP->ds_ds_start_num = intervalNum - rangeFftPoint*(downSample+1) - endNum;
    DSP->ds_ds_adc_valid_num = rangeFftPoint*(downSample+1);
    DSP->ds_ds_interval_num = intervalNum;
    DSP->ds_ds_scan_num = chirpNum;
    //DSP->ds_ds_pulse_num =
    /*RW
    [3] dma_err_intr_en:1: dma fifo full intr_enable
    [2:0] adc_dma_select:DMA src selec
    3'h0: means fifo0[31:0] = {ch0_q[N][15:0], ch0_i[N][15:0]} and fifo1[31:0] = {ch1_q[N][15:0], ch1_i[N][15:0]} ;
    3'h1: means fifo0[31:0] = {ch0_i[N][15:0], ch1_i[N][15:0]} and fifo1[31:0] = 0.
    3'h2: means fifo0[31:0] = {ch0_q[N][15:0], ch1_q[N][15:0]} and fifo1[31:0] = 0.
    3'h3: means fifo0[31:0] = {ch0_i[N-1][15:0], ch0_i[N][15:0]} and fifo1[31:0] = 0.
    3'h4: means fifo0[31:0] = {ch0_q[N-1][15:0], ch0_q[N][15:0]} and fifo1[31:0] = 0.
    3'h5: means fifo0[31:0] = {ch1_i[N-1][15:0], ch1_i[N][15:0]} and fifo1[31:0] = 0.
    3'h6: means fifo0[31:0] = {ch1_q[N-1][15:0], ch1_q[N][15:0]} and fifo1[31:0] = 0.
    */
    DSP->ds_adc_dma_src_select = 0;
}
/****************************************************************************************
*Function name : FMCW_ADC_start, FMCW_ADC_start_gesture
*Description   : adc sampling,DMA moves the data to the specified address,where the ADC0
			     source address is ADC0_FIFO,the ADC0 destination address is 0x20008000,
			     where the ADC1 source address is ADC1_FIFO,the ADC1 destination address
			     is 0x20008000+adc0 data space;The sampling time of adc is synchronized with
			     the time of sending chirp wave.

srcAddr |11 12 13 14 15...............................................1(adcSamples) |
        |21 22 23 24 25...............................................2(adcSamples) |
        |31 32 33 34 35...............................................3(adcSamples) |
        |...........................................................................|
        |...........................................................................|
        |...........................................................................|
        |...........................................................................|
        |...........................................................................|
        |(chirpNumber)1 (chirpNumber)2 (chirpNumber)3......(chirpNumber)(adcSamples)|

*Paramater		 :
*        @adcSamples  :ADC sampling points
*        @chirpNumber :Fmcw chirp number
*        @dstAddr     :Destination address
*Return        : None
****************************************************************************************/
void adcSampleStart(
    uint16 adcSel,
    uint32 adcSamples,
    uint32 chirpNumber,
    uint32 ch0srcAddr,
    uint32 ch0dstAddr,
    uint32 ch1srcAddr,
    uint32 ch1dstAddr)
{
    uint32 regVal;
    uint32 regVal1;

    if (adcSel == 0)
    {
        dmaConfig(DMA1, ch0srcAddr, ch0dstAddr, 0, 4, chirpNumber*adcSamples, PERI_SELECT_ADC0, PERITOMEM);
        adcFmcwStart();
        while(1)
        {
            regVal   = DMA->dma_1_tcnt;
            if(regVal >= chirpNumber*adcSamples)
            {
                break;
            }
        }
        dmaDisable(DMA1);
    }
    else if(adcSel == 1)
    {
        dmaConfig(DMA2, ch1srcAddr, ch1dstAddr, 0, 4, chirpNumber*adcSamples, PERI_SELECT_ADC1, PERITOMEM);
        adcFmcwStart();
        while(1)
        {
            regVal   = DMA->dma_2_tcnt;
            if(regVal >= chirpNumber*adcSamples)
            {
                break;
            }
        }
        dmaDisable(DMA2);
    } else if (adcSel == 2)
    {
        dmaConfig(DMA1, ch0srcAddr, ch0dstAddr, 0, 4, chirpNumber*adcSamples, PERI_SELECT_ADC0, PERITOMEM);
        dmaConfig(DMA2, ch1srcAddr, ch1dstAddr, 0, 4, chirpNumber*adcSamples, PERI_SELECT_ADC1, PERITOMEM);
        adcFmcwStart();
        while(1)
        {
            regVal   = DMA->dma_1_tcnt;
            regVal1  = DMA->dma_2_tcnt;
            if((regVal >= chirpNumber*adcSamples)&&(regVal1 >= chirpNumber*adcSamples))
            {
                break;
            }
        }
        dmaDisable(DMA1);
        dmaDisable(DMA2);
    }
		// Wait T1+T2, until the whole chirp finished. by XX. 
		// By doing this, ADC sampling state machine will be finished normally, otherwise, the next frame would be incorrect.  
		if((CHIRP_T1+CHIRP_T2)>0)
        {
            delayUs((CHIRP_T1+CHIRP_T2),HIGH_CLOCK);
        }
#ifdef CASCADE_MASTER         
        RFC->fmcw_fmcw_sync &= ~fmcw_fmcw_sync_sync_out_bit;
#endif        
        
}
/****************************************************************************************
*Function name : adcFmcwStart
*Description   : when FMCW trigger signal as ADC start signal, the funtion is start signal.
*Paramater	   : None
*        @ :  
*        @ :   
*Paramater	selection:	None	

*Power consumption : None
*Return        :None
****************************************************************************************/
void adcFmcwStart(void)
{
// if not in cascade appliaction, use ds_start to start fmcw and adc sampling.
// if in cascade appliaction, and as a master, use sync_out to start fmcw and adc sampling.   
// if in cascade appliaction, and as a slave,  wait external sync_in to start fmcw and adc sampling.     
#ifdef CASCADE_MASTER    
    RFC->fmcw_fmcw_sync |= fmcw_fmcw_sync_sync_out_bit;
#else
    #ifdef CASCADE_SLAVE
        ;
    #else
        DSP->ds_start = 0x01;
    #endif
#endif
}

