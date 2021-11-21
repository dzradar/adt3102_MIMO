#ifndef __ADC_H
#define __ADC_H
#include "adt3102_type_define.h"
/***Macro Definition*********************************************************************
ADC0_FIFO : adc0 sample source data fifo address 
ADC1_FIFO : adc1 sample source data fifo address 
****************************************************************************************/
#define ADC0_FIFO  0x40023000
#define ADC1_FIFO  0x40023004
/***Macro Definition*********************************************************************
ADC_8P3M : config adc sample rate 8.3M 
ADC_16P6M : config adc sample rate 16.6M 
****************************************************************************************/
#define ADC_8P3M   1
#define ADC_16P6M  0
/***Macro Definition*********************************************************************
ADC0SEL : select adc0 channel sample 
ADC1SEL : select adc1 channel sample
ADC01SEL : select adc0 and adc1 channel sample
****************************************************************************************/
#define ADC0SEL    0
#define ADC1SEL    1
#define ADC01SEL   2

/***Macro Definition*********************************************************************
GPADC select
****************************************************************************************/
#define GPADC_AIO1    1     //temperature Compensation
#define GPADC_IN2     2     //VBE
#define GPADC_IN3     3   
#define GPADC_IN4     4     
#define GPADC_IN5     5     //powerdector 
#define GPADC_IN6     6     //VBG
#define BG_ATB_OUT    7   
#define VGA_ATB_OUT   8   
#define GPADC_IN18_1  9       
#define GPADC_IN18_2  10    //vtune    
#define GPADC_IN18_3  11  
#define ADC_ATB_OUT   12  

/***Macro Definition*********************************************************************
FirDownsample : config fir filter coefficients
****************************************************************************************/
typedef struct
{
    uint32 firReg0  ; 
    uint32 firReg1  ; 
    uint32 firReg2  ; 
    uint32 firReg3  ; 
    uint32 firReg4  ; 
    uint32 firReg5  ; 
    uint32 firReg6  ; 
    uint32 firReg7  ; 
    uint32 firReg8  ; 
    uint32 firReg9  ; 
    uint32 firReg10 ; 
    uint32 firReg11 ; 
    uint32 firReg12 ; 
    uint32 firReg13 ; 
    uint32 firReg14 ; 
    uint32 firReg15 ;
    uint32 filter_coefficient;   
    
}FirDownsample;
/****************************************************************************************
*Function name : adt3102 adc setting
*Description   : 
*Parameters    :
rx0En,rx1En : RX channel enable
wnSel       : 1:windowing cof select 0 hanning 1 hamming
****************************************************************************************/
void adt3102AdcSetting(int32 rx0En,int32 rx1En,int32 wnSel,int32 adcClk,uint32 chirpNumber);
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
void siggen(uint16 step, uint16 chgStep, uint16 comp, uint16 siggenEn, uint16 ang);
/****************************************************************************************
*Function name : adcEnable
*Description   : enable adc, and enable clk output to adc
*Paramater	   :
*        @adc0Enable :adc0 enable
*        @adc1Enable :adc1 enable
*
*Return        : None
****************************************************************************************/
void adcEnable(int32 adc0Enable,int32 adc1Enable);
/****************************************************************************************
*Function name : baseBandBiasSetting
*Description   : baseBand Bias Setting, enable signal for ztc bias for BB 
*Paramater	   :
*        @bbbiasEn :enable signal for ztc bias for BB   
*        @ :   
*Paramater	selection:		

*Power consumption : 
*Return        :
****************************************************************************************/
void baseBandBiasSetting(int32 bbbiasEn);
/****************************************************************************************
*Function name : adcSampleInit
*Description   : adc sample init, ch0 enable and ch1 enable  
*Paramater	   : 
*        @ch0 : 
*        @ch1 : 
*Return        : None
****************************************************************************************/
void adcSampleInit(int32 ch0, int32 ch1);
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
void adcWnpara(int32 cofSel, int32 bypass, int32 wnLen);
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
void adcFirDownsample(int32 firBypass, uint16 downSample);
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
void dataSampleConfig(uint16 intervalNum,uint16 endNum,uint16 rangeFftPoint,uint16 downSample, uint16 chirpNum);
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
    uint32 ch1dstAddr);
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
void adcFmcwStart(void);
void gpadcEnable(uint16 enableSigInput);
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
					 int32 adc1Bias2, int32 adc1Bias3, int32 adc1Bias4, int32 adc1Bias5);
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
void adcClkSwitch(int32 adcClk);    

uint32 getIntervalNum(uint32 t0,uint32 t1,uint32 t2);


#endif




