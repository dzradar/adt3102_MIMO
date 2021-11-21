//--------------------------------------------------------------------
//Copyright(c)2020,Andar Technologise Inc.
//All Rights Reserved
//Confidential Property of Andar Technologies Inc.
//
//Module Description:
//  .include some commonly used function modules
//  .
//  
//  
//  
//
//Created by :wuhao
//$Revision: 1.0
//$Data: 2020/11/24
//--------------------------------------------------------------------
//
//All include header files
#include "CMSDK_CM3.h"
#include "adt3102_type_define.h"
#include "adt3102_uart.h"
#include "adt3102_pmu.h"
#include "adt3102_timer.h"
#include "adt3102_system.h"
#include "uart_ctype_map.h"
#include "gpio_ctype_map.h"
#include "pmu_ctype_map.h"
#include "rfc_ctype_map.h"
#include "stdio.h"
#include "adt3102.h"
#include "adt3102_rtc.h"
#include "adt3102_gpio.h"
#include "adt3102_tx.h"
#include "adt3102_rx.h"
#include "adt3102_system.h"
//
//------Global variable declaration
volatile int32 g_delayUsFlag;
#ifndef  __NO_SYSTEM_INIT
void SystemInit()
{
    //Init.
    for(volatile int i=0;i<10000;i++){}
}
#endif
/****************************************************************************************
*
*Function name : setInterrupt
*Description   : switching interrupt 
*Paramater	   :
*        @interrutNum : interrupt number
*        @enableOrDisenable : on or off total interrupt 1:on 0:off
*        @
NMI: Watchdog
0  : Uart0 tx/rx                9  : spi1
1  : Uart1 tx/rx                10 : RTC
2  : Uart3 tx/rx                11 : IIC0
3  : Timer0                     12 : IIC1
4  : Timer1                     13 : FFT
5  : Dualtimer2                 14 : reserve
6  : Uart0/1/2 overflow         15 : gpio
7  : pmu interrupt              16 : dma_int0
8  : spi0                       17 : dma_int1       18 : TRNG
*        @
*Return        : None
****************************************************************************************/
void setInterrupt(int32 interruptNum, int32 enableOrDisenable)
{
    if (enableOrDisenable == 0)
        soc_NVIC[soc_CLRENA] = 1<<interruptNum;  //clear interrupt.
    else
        soc_NVIC[soc_SETENA] = 1<<interruptNum;  //enable interrupt.
}

/****************************************************************************************
*
*Function name : MY_NVIC_SetVectorTable
*Description   : Used to indicate that the vector table is in the code area or in the RAM area
*Paramater	   :
*        @NVIC_VectTab :The defalut of ADT3102 is 0x00000000;
*        @Offset   : vector table offset ,The defalut of ADT3102 is 0x8000
*        @:
*Return        : None
****************************************************************************************/
void MY_NVIC_SetVectorTable(uint32 NVIC_VectTab, uint32 Offset)	 
{ 	   	 
    //set NVIC's vector table offset register
	SCB->VTOR = NVIC_VectTab|(Offset & (uint32)0x1FFFFF80);

}

/****************************************************************************************
*
*Function name : delayUs
*Description   : The delay realized by dualtimer, the accuracy is in "US"
*Paramater	   :
*        @delayUs : the time you want to delay,in us
*        @highClock50M : high clock setting,50M or 125M
*        @:
*
*function call example:  delayUs(1000,HIGH_CLOCK);
*Return        : None
****************************************************************************************/
void delayUs(uint32 delayus, int32 highClock50M)
{
    g_delayUsFlag = 0;
    if (highClock50M == 1)
    {
        dualtimerInit(delayus*50-1,clkDIV1);
    }else
    {
        dualtimerInit(delayus*125-1,clkDIV1);
    }
    while(g_delayUsFlag != 1);
}

/****************************************************************************************
*
*Function name : system_reset
*Description   : system reset 
*Paramater	   :
*        @:
*
*Return        : None
****************************************************************************************/
void system_reset(void)
{
  int32 regVal=0;
  // read 0xE000ED0C, and then change key to 0x05fa, and write bit [2] = 1. 
  regVal = soc_AIRCR[0x0];
  regVal = (0x0000ffff & regVal) | 0x05fa0004;
  soc_AIRCR[0x0] = regVal;
}


/****************************************************************************************
*Function name : 
*Description   : 
*Paramater	   :
*        @ :  
*        @ :   
*Paramater	selection:		

*Power consumption : 
*Return        :
****************************************************************************************/
void switch125MHclk(void)
{
    delayUs(1000, HIGH_CLOCK);
    switch_to_plli();
    delayUs(1000, HIGH_CLOCK_125M);
}

/****************************************************************************************
*Function name : 
*Description   : 
*Paramater	   :
*        @ :  
*        @ :   
*Paramater	selection:		

*Power consumption : 
*Return        :
****************************************************************************************/
float getRangeRes(uint32 fm, uint32 t0, uint16 downSample, uint16 fftPoint)
{
    float rangeResolution;
    rangeResolution = 300000000.0*(t0/1000000.0)*(125.0/15.0/(downSample+1)*1000000.0)/2.0/(fm*1000000.0)/fftPoint;
    return rangeResolution;
}

void adt3102DigiInit(void)
{
    // add some settings for bootloader.
    setInterrupt(INT_UART1, DISABLEINT);  //disable uart1.
    gpioDirection(10,2);  //high impedence. 
    gpioDirection(11,2);  //high impedence.    

// if not in cascade appliaction, use ds_start to start fmcw and adc sampling.
// if in cascade appliaction, and as a master, use sync_out to start fmcw and adc sampling.   
// if in cascade appliaction, and as a slave,  wait external sync_in to start fmcw and adc sampling.  
#ifdef CASCADE_MASTER    
    GPIO->gpio_p02_config = 0x33;   //as sync_in.
    GPIO->gpio_p09_config = 0x3;   //as sync_out.  
    RFC->fmcw_fmcw_sync = fmcw_fmcw_sync_adc_start_from_syncin_bit | fmcw_fmcw_sync_en_ckadc_from_syncin_bit | fmcw_fmcw_sync_negedge_sel_bit;
#endif
#ifdef CASCADE_SLAVE    
    GPIO->gpio_p02_config = 0x33;   //as sync_in. 
    RFC->fmcw_fmcw_sync = fmcw_fmcw_sync_adc_start_from_syncin_bit | fmcw_fmcw_sync_en_ckadc_from_syncin_bit;
#endif    
    
    //switch_to_50m();
    mcuSleepConfig(0);
	rtcInit(1, 1000, 32, RCSOR); 
	rc32kCalibration();
    // when VDD_RF is on, reset phase shifter. 
    RFC->rfc_nload_reg =  rfc_nload_reg_ps_rstn_inv_bit; 
}

/****************************************************************************************
*Function name : adt3102 Rf setting
*Description   :
****************************************************************************************/
void adt3102RfInit(int32 rx0En,int32 rx1En,int32 tx0En,int32 tx1En,uint32 phaseShftLut0,uint32 phaseShftLut1,uint32 chirpNumber)
{
    plliDacSetting();
    plliConfig(ENABLE);
    pllnDacSetting();
    pllnConfig(ENABLE);
    plliLock();

    pllnLock(FSTART, FM);
    fmcwConfig(FSTART, FM, CHIRP_T0, CHIRP_T1, CHIRP_T2, chirpNumber);
    //fmcwConfig_dualSaw(FSTART, 2500, 64, 8, 800, 32, 4, CHIRP_T2, chirpNumber);  
    //fmcwConfig_dualSaw(FSTART, FM, CHIRP_T0, CHIRP_T1, 0, 0, 0, CHIRP_T2, chirpNumber);
    //manuPllnCapArray(0xB);//76G:B  77G:A,B
                
	rfPhaseShiftLUT(phaseShftLut0, phaseShftLut1);
    RFC->rf_spare_reg = 0x01;
    
    //set fmcw pll to doubler
    fmcwModeSel(MODE_INTER);
      
    //enable 20G and doubler
    rfbuff20G(rx0En | rx1En | tx0En | tx1En, rx0En | rx1En | tx0En | tx1En);
    //enable buffer1to4
    rfBuff77GEnable(1, tx0En, tx1En, rx0En, rx1En);
    //enable bias for 20G(fmcw buffer), doubler and buffer1to4
    bias20GTo80G(8, 8, 8, 8, 8, 8, 8, 8);
}

extern int32 g_rx0En;
extern int32 g_rx1En;
extern int32 g_tx0En;
extern int32 g_tx1En;
extern uint32 g_tiaGain0;
extern uint32 g_tiaGain1;
extern uint32 g_pgaGain0;
extern uint32 g_pgaGain1;
extern uint32 g_tiaHpf;
extern uint32 g_rcHpf;
extern uint32 g_pgaHpf;
extern uint32 g_pgaLpf;
extern uint32 g_tiaHpfEn;
extern uint32 g_rcHpfEn;
extern uint32 g_pgaHpfEn;
extern uint32 g_PaGain0;
extern uint32 g_PaGain1;
extern uint32 g_phaseShftLut0;
extern uint32 g_phaseShftLut1;

/****************************************************************************************
*Function name : adt3102 analog setting
*Description   : 


****************************************************************************************/
void adt3102Analog(void)
{ 
    int32 biasRx0=g_rx0En*8;
    int32 biasRx1=g_rx1En*8;
    //enable pa1 and pa0
    txPaEnable(g_tx0En, g_tx1En);
    txPaBias(g_PaGain0, g_PaGain1);
    
    rxLnaEnable(g_rx0En, g_rx1En);
    mixerEnable(g_rx0En, g_rx1En);
    lnaRegularBias(g_rx0En, g_rx1En);
    lnaDacBias(biasRx0, biasRx0, biasRx0, biasRx1, biasRx1, biasRx1);
    
    //bias2_en: enable signal for ztc bias for BB
    baseBandBiasSetting(ENABLE);
    
    //turn on tia pga    
    rfTiaEnable(g_rx0En, g_rx1En);
    rfTiaGainSetting(g_tiaGain0, g_tiaGain0 ,g_tiaGain1 ,g_tiaGain1);
    rfTiaDacBias(biasRx0, biasRx1);
    //pga config
    rfVgaEnable(g_rx0En, g_rx1En);
    rfVgaGainSetting(g_pgaGain0, g_pgaGain0,g_pgaGain1 ,g_pgaGain1);
    rfVgaDacBias(biasRx0, biasRx1);
    rfPgaGainCalEnable(0, 0, 0, 0);
    rfPgaGainCalSetting(PGA_CAL_P1, PGA_CAL_P1 ,PGA_CAL_P1 ,PGA_CAL_P1);
    //filter config
    tiaHpfConfig(g_tiaHpfEn, g_tiaHpfEn, g_tiaHpf, g_tiaHpf);
    rcHpfConfig(g_rcHpfEn,g_rcHpfEn,g_rcHpf,g_rcHpf);
    pgaLpfConfig(g_pgaLpf,g_pgaLpf);
    pgaHpfConfig(g_pgaHpfEn, g_pgaHpfEn, g_pgaHpf, g_pgaHpf);
    rfPhaseShiftLUT(g_phaseShftLut0, g_phaseShftLut0);
    
    #if DEBUGMODE
        //enable vga en_buffer
        RFC->rfc_vga0_reg |= 1<<9;
        //en_buffer0 contorl if signal output gpio23, 24, 25, 26. 
        //en_buffer1 contorl if signal output gpio27, 28, 29, 30.  
        //when we test low power consumption, we need to connect p29 with vddrf enable.
        //Normally, p29 is high resistance, vga1 en_buffer will lower the p29 level.
        RFC->rfc_vga1_reg |= 1<<9;
    #endif 
    
    //???gpio mux to output bist if signal
    GPIO->gpio_p24_config = 0x16;
    GPIO->gpio_p25_config = 0x16;
    
}



void ledAlloff(void)
{
    GPIO->gpio_out_clear=0x03FFFC;
}
void ledAllon(void)
{
	GPIO->gpio_out_set	=0x03FFFC;
}
void ledInit(void)
{
	gpioDirection(2,0);
	gpioDirection(3,0);
	gpioDirection(4,0);
	gpioDirection(5,0);
	gpioDirection(6,0);
	gpioDirection(7,0);
	gpioDirection(8,0);
	gpioDirection(9,0);
	gpioDirection(10,0);
	gpioDirection(11,0);
	gpioDirection(12,0);
	gpioDirection(13,0);
	gpioDirection(14,0);
	gpioDirection(15,0);
	gpioDirection(16,0);
	gpioDirection(17,0);
	
}
void ledOn(uint32 actionType)
{
    GPIO->gpio_out_set=actionType&0x03FFFC;
}
void ledDisplay(uint32 actionType)
{		
	switch(actionType)
		{
			case 0:
				break;
			case 1:
				ledOn(LEDIN);
				break;
			case 2:
				ledOn(LEDOUT);
				break;
			case 3:
				ledOn(LEDLEFT);
				break;
			case 4:
				ledOn(LEDRIGHT);
				break;
			case 5:
				ledOn(LEDUP);
				break;
			case 6:
				ledOn(LEDDOWN);
				break;
			default:
				break;
		}

	}




