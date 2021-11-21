//--------------------------------------------------------------------
//Copyright(c)2020,Andar Technologise Inc.
//All Rights Reserved
//Confidential Property of Andar Technologies Inc.
//
//Module Description:
//  .PLLI
//  .PLLN
//  .LDO(1.2v 1.8v )
//  .Buff20G
//  .Buff77G
//  .Tx PA
//  .Phase shift
//  .

//Created by :wuhao
//$Revision: 1.0
//$Data: 2020/11/9
//--------------------------------------------------------------------
//
//All include header files
#include "adt3102_type_define.h"
#include "adt3102.h"
#include "adt3102_pmu.h"
#include "adt3102_gpio.h"
#include "adt3102_tx.h"
#include "adt3102_rx.h"
#include "adt3102_system.h"
#include "pmu_ctype_map.h"
#include "rfc_ctype_map.h"
#include "gpio_ctype_map.h"
extern uint32 g_phaseShftLut0;
extern uint32 g_phaseShftLut1;

//------Global variable declaration
/****************************************************************************************
*Function name : adt3102 power setting
*Description   : 
****************************************************************************************/
void adt3102PowerSw(int onOroff)
{   
    if(onOroff == 1)
    { 
        baseband18LDO(ENABLE, BB_LDO_1P8, DISABLE, BB_LDO_1P8);
	    delayUs(50,HIGH_CLOCK);	 
        baseband18LDO(ENABLE, BB_LDO_1P8, ENABLE, BB_LDO_1P8);
	    delayUs(50,HIGH_CLOCK);	 
        plliLDOEnable(ENABLE, ENABLE);
	    delayUs(50,HIGH_CLOCK);	  
        pllnVcoConfig(ENABLE, PLLN_VCO_LDO_1P8);
	    delayUs(50,HIGH_CLOCK);	  
        pllnPllConfig(ENABLE, PLLN_PLL_LDO_2P5, ENABLE);
	    delayUs(50,HIGH_CLOCK);	  
        dacEnBias1Pd(ENABLE, POWER_ON);
	    delayUs(50,HIGH_CLOCK);
        gpioOutput(29,1);  //must output 1, ant then set as output. so there is no glitch of P29's high level output.
        gpioDirection(29,0);
        gpioOutput(22,1);  //must output 1, ant then set as output. so there is no glitch of P29's high level output.
        gpioDirection(22,0);        
    }else{
        plliLDOEnable(DISABLE, ENABLE);
        pllnVcoConfig(DISABLE, PLLN_VCO_LDO_1P8);
        pllnPllConfig(DISABLE, PLLN_PLL_LDO_2P5, DISABLE);
        baseband18LDO(DISABLE, BB_LDO_1P8, DISABLE, BB_LDO_1P8);
        dacEnBias1Pd(DISABLE, POWER_DOWN);
    }
}

void switch_to_50m(void)
{
    int32 regVal=0;
    int32 i;

    //-----------------------------------------------
    //--- switch to DCXO.
    //-----------------------------------------------
    regVal = PMU->pmu_clk_high_reg;
    regVal &= ~pmu_clk_high_reg_ctrl_high_clk_sel_mask << pmu_clk_high_reg_ctrl_high_clk_sel_shift;
    regVal |= 0x0 << pmu_clk_high_reg_ctrl_high_clk_sel_shift;
    PMU->pmu_clk_high_reg = regVal;
    delayUs(100, HIGH_CLOCK);
    for(i=0; i<10; i=i+1);
}

void switch_to_plli(void)
{
    int32 regVal=0;
    int32 i;

    // for adt301. turn on PLLI.
    //RFC->rfc_plli_ctrl_reg = 0xffffffff;

    //-----------------------------------------------
    //--- switch to plli.
    //-----------------------------------------------
    regVal = PMU->pmu_clk_high_reg;
    regVal &= ~pmu_clk_high_reg_ctrl_high_clk_sel_mask << pmu_clk_high_reg_ctrl_high_clk_sel_shift;
    regVal |= 0x1 << pmu_clk_high_reg_ctrl_high_clk_sel_shift;
    PMU->pmu_clk_high_reg = regVal;
    for(i=0; i<10; i=i+1);
}

void mcu_idle(void)
{
    soc_SCR[0x0] = 0x00;
    __asm("WFI");
    return;
}

void mcuGotoSleep(void)
{
    int32 regVal=0;

    //--- MUST check RTC's counter, making sure it will not wake up in 3 cycles.
    //

    //--- MUST make sure PMU's state is idle before going to sleep.
    regVal = PMU->pmu_pmu_state;
    regVal = regVal & (pmu_pmu_state_cur_state_mask << pmu_pmu_state_cur_state_shift);
    while( regVal != 0x00 )
    {
        regVal = PMU->pmu_pmu_state;
        regVal = regVal & (pmu_pmu_state_cur_state_mask << pmu_pmu_state_cur_state_shift);
    }

    // disable Manu mode, and configure sleeping registers.
    //PMU->pmu_pmu_ctrl_reg         &= ~pmu_pmu_ctrl_reg_manu_ctrl_en_bit ;
    //delayUs(100, HIGH_CLOCK) ; //10us for ASIC simulation, 100us for REAL CHIP.

    //

    soc_SCR[0x0] = 0x04;
    __asm("WFI");


    //--- Configure some registers after waking up.
    //--- Phase shifter
    //rfPhaseShift(0x1f, 0, 0x1f, 0);
}

// ramOn = 0, sleep with PD_RAM off.
// ramOn = 1, sleep with PD_RAM on, keep RAM powered on.
void mcuSleepConfig(int32 ramOn)
{
    // ramOn = 0, sleep with PD_RAM off.
    // ramOn = 1, sleep with PD_RAM on, keep RAM powered on.

    //--- MUST enable all of clock gating cells. Otherwise, the sleep will fail.
    PMU->pmu_reg_ext_clk_en_reg = 0x0 ;

    //--- disable debugger's wake up function.
    //*(uint32*)0x4000c004 = 0x03;
    //PMU->pmu_int_en_reg &= ~(pmu_int_en_reg_ctrl_debug_wakeup_en_bit|pmu_int_en_reg_ctrl_rtc_wakeup_en_bit|pmu_int_en_reg_ctrl_io_wakeup_en_bit);
    PMU->pmu_int_en_reg = pmu_int_en_reg_ctrl_rtc_wakeup_en_bit | pmu_int_en_reg_ctrl_io_wakeup_en_bit;

    //--- set P29 as output, 1'h1.
    //*(uint32*)0x40020020 = 0x20000000;  // P29 output is high. .
    //*(uint32*)0x400200a4 = 0x01;  //    p29 as external LDO enable, for VDD_RF.
    gpioOutput(29,1);  //must output 1, ant then set as output. so there is no glitch of P29's high level output.
    gpioDirection(29,0);
    gpioOutput(22,1);  //must output 1, ant then set as output. so there is no glitch of P29's high level output.
    gpioDirection(22,0);


    // the time of IO_Wakeup to MCU_running = (powering_up_time+4)*T_32k.  0x2a means (42+4)*32us = 1.5ms appro.
    PMU->pmu_pmu_stage_time       = 0x01002a;

    PMU->pmu_slp_xo50m_cfg        = 0x0 ;
    PMU->pmu_slp_plli_ctrl_reg    = 0x0 ;
    PMU->pmu_slp_ldo_bg_reg_reg   = 0x0 ;

    //0x7:1.3v.   0x0:1.2v,   0x6:1.1v,   0x5:1.0v,    0x4:0.85v,   0x3:0.8v,   0x2:0.75v.   0x1:0.7v,
    PMU->pmu_slp_ldo12n_rega_reg  = 0x0 << pmu_slp_ldo12n_rega_reg_slp_ldo12_lp_vcfg_shift ;//   0x1800; //0x1800 ;   //0x400 is 0.7v.

    if(ramOn) {
        PMU->pmu_slp_power_ctrl_reg   = 0x1f ;
    }
    else {
        PMU->pmu_slp_power_ctrl_reg   = 0x07 ;
//--- for leakage analysis.
//				PMU->pmu_slp_power_ctrl_reg   = 0x04 ;  //bg_33 = bg_18 = on.
//				PMU->pmu_pmu_ctrl_reg |=  pmu_pmu_ctrl_reg_ctrl_ldo18bb1_on_bit | pmu_pmu_ctrl_reg_ctrl_ldo18bb2_on_bit;
//			  PMU->pmu_slp_plli_ctrl_reg |= (1<<9 | 1<<10 | 1<<11 | 1<<12);
//			  RFC->rf_plln_ctrl_reg = 0x7;
//
//				PMU->pmu_pmu_ctrl_reg |= 1 << 4;
//				RFC->rfc_plli_ctrl_reg |= (1<<9 | 1<<10 | 1<<11 | 1<<12);
//			  delayUs(100, HIGH_CLOCK);
//				PMU->pmu_pmu_ctrl_reg &= ~(1 << 4);
//
//				PMU->pmu_sram_cfg &= ~pmu_sram_cfg_sram_retn_sel_bit;
//				PMU->pmu_pmu_ctrl_reg |= pmu_pmu_ctrl_reg_ctrl_hold_ana_cfg_bit;

    }

    //--- turn on clk_50m out, at an analog IO.
    //RFC->rfc_plli_ctrl_reg |= rfc_plli_ctrl_reg_xo50m_en_refclk_out_bit;
    //PMU->pmu_pmu_ctrl_reg |= pmu_pmu_ctrl_reg_manu_ctrl_en_bit;
    //delayUs(100,HIGH_CLOCK);
    //PMU->pmu_pmu_ctrl_reg &= ~pmu_pmu_ctrl_reg_manu_ctrl_en_bit;

}
void turnOnPlli(void)
{
    int32 regVal;
    regVal = 0xfffffff;
    regVal &= ~(rfc_plli_ctrl_reg_plli_en_ckdig_125m_bit | rfc_plli_ctrl_reg_plli_en_ckadc_bit | rfc_plli_ctrl_reg_xo50m_en_ckbist_bit | rfc_plli_ctrl_reg_xo50m_mode_bit);
    RFC->rfc_plli_ctrl_reg = regVal;
    regVal= RFC->rf_plli_rega_reg;
    regVal &= ~(rf_plli_rega_reg_plli_reg1_mask<<rf_plli_rega_reg_plli_reg1_shift);
    regVal |= 0xff<<rf_plli_rega_reg_plli_reg1_shift;
    RFC->rf_plli_rega_reg = regVal;

    PMU->pmu_pmu_ctrl_reg |= pmu_pmu_ctrl_reg_manu_ctrl_en_bit;
    for(int32 i=0 ; i<1000; i++) {}
    RFC->rfc_plli_ctrl_reg |= rfc_plli_ctrl_reg_plli_en_ckdig_125m_bit| rfc_plli_ctrl_reg_plli_en_ckadc_bit;

    PMU->pmu_pmu_ctrl_reg &= ~pmu_pmu_ctrl_reg_manu_ctrl_en_bit;
    RFC->rf_plli_rega_reg |= 1 <<(rf_plli_rega_reg_plli_reg2_shift + 3);
}


/****************************************************************************************
*Function name :
*Description   :
*Paramater	   :
     ¡ü       ________                                           ________
     |      |        |                                         |        |
     |      |        |                                         |        |
     |     	|        |                                         |        |
     |     	|        |                                         |        |
     |     	|        |_________                                |        |_________
     |      |        |         |                               |        |         |
     |      |        |         |                               |        |         |
_____|______|________|_________|_______________________________|________|_________|________
     |         radio    active                  sleep
     |
     |
radio :fmcw ->ActiveFastMode
       hold ->ActiveSlowMode
*        @ :
*        @ :
*Paramater	selection:

*Power consumption :
*Return        :
****************************************************************************************/
void lowPowerSwitchPll(int32 plliOn,int32 pllnOn)
{
    plliConfig(plliOn);
    pllnConfig(pllnOn);
    if((plliOn==1) || (pllnOn==1))
    {
        delayUs(50,HIGH_CLOCK); //wait for plli&plln settle
    }
}

void lowPowerSwitchFast(int32 rx0En, int32 rx1En, int32 tx0En, int32 tx1En, uint32 tiaHpf,uint32 rcHpf, uint32 pgaHpf)
{
    int32 dacPower=0;
    int32 biasRx0=rx0En*8;
    int32 biasRx1=rx1En*8;
    int32 anyRxTx=rx0En | rx1En | tx0En | tx1En;
    int32 biasAnyRxTx=anyRxTx*8;
    if(anyRxTx==1)
    {
        dacPower=POWER_ON;
    }
    else
    {
        dacPower=POWER_DOWN;
    }
    dacEnBias1Pd(anyRxTx, dacPower); //all bias except for PA&LNA
    //bias2_en: enable signal for ztc bias for BB
    baseBandBiasSetting(rx0En | rx1En);    
    //------------------------ RX ------------------ //
    //turn on tia pga    
    rfTiaEnable(rx0En, rx1En);
    rfTiaDacBias(biasRx0|biasRx1, biasRx1);
    rfVgaEnable(rx0En, rx1En);
    rfVgaDacBias(biasRx0|biasRx1, biasRx1);
    pgaHpfConfig(rx0En, rx1En, pgaHpf, pgaHpf);
    //turn on adc, gpadc
    adcEnable(rx0En|rx1En, rx1En);
    adcBiasSetting(biasRx0, biasRx0, biasRx0, biasRx0, biasRx0, biasRx0, biasRx1, biasRx1, biasRx1, biasRx1, biasRx1, biasRx1);
    //RF RX
    rxLnaEnable(rx0En, rx1En);
    mixerEnable(rx0En, rx1En);
    //------------------------ common setting ------------------ //
    //enable 20G and doubler
    rfbuff20G(rx0En | rx1En | tx0En | tx1En, rx0En | rx1En | tx0En | tx1En);
    //enable buffer1to4
    rfBuff77GEnable(anyRxTx, tx0En, tx1En, rx0En, rx1En);
    //enable bias for 20G(fmcw buffer), doubler and buffer1to4
    bias20GTo80G(biasAnyRxTx, biasAnyRxTx, biasAnyRxTx, biasAnyRxTx, biasAnyRxTx, biasAnyRxTx, biasAnyRxTx, biasAnyRxTx);
    //------------------------ TX ------------------ //
    txPaEnable(tx0En, tx1En);
}

void lowPowerSwitchSlow(int32 rx0En, int32 rx1En, int32 tx0En, int32 tx1En)
{
    int32 anyRxTx=rx0En | rx1En | tx0En | tx1En;
    if(anyRxTx==0)
    {
        //--- set P29 as output, 1'h0.
			  gpioOutput(22,0);
        gpioOutput(29,0);
        pllnVcoConfig(0, PLLN_VCO_LDO_1P8);
        pllnPllConfig(0, PLLN_PLL_LDO_2P5, 0);
        baseband18LDO(0, BB_LDO_1P8, 0, BB_LDO_1P8);
        plliLDOEnable(0, 0);
    }
    else
    {
        gpioOutput(22,1);
        gpioOutput(29,1);
        delayUs(200,HIGH_CLOCK);
        plliLDOEnable(ENABLE, ENABLE);
        delayUs(100,HIGH_CLOCK);  //100us for 10uF cap on board
        pllnVcoConfig(1, PLLN_VCO_LDO_1P8);
        delayUs(100,HIGH_CLOCK);
        pllnPllConfig(1, PLLN_PLL_LDO_2P5, anyRxTx);
        delayUs(100,HIGH_CLOCK);
        baseband18LDO(rx0En, BB_LDO_1P8, 0, BB_LDO_1P8);
        delayUs(100,HIGH_CLOCK);
        baseband18LDO(rx0En, BB_LDO_1P8, rx1En, BB_LDO_1P8);
        delayUs(100,HIGH_CLOCK);
			  rfPhaseShiftLUT(g_phaseShftLut0, g_phaseShftLut1);
    }
}

void adt3102GpioHz(void)
{
    gpioDirection(GPIO0 , 2);
    gpioDirection(GPIO1 , 2);
    gpioDirection(GPIO2 , 2);
    gpioDirection(GPIO3 , 2);
    gpioDirection(GPIO4 , 2);
    gpioDirection(GPIO5 , 2);
    gpioDirection(GPIO6 , 2);
    gpioDirection(GPIO7 , 2);
    gpioDirection(GPIO8 , 2);
    gpioDirection(GPIO9 , 2);
    gpioDirection(GPIO10, 2);
    gpioDirection(GPIO11, 2);
    gpioDirection(GPIO12, 2);
    gpioDirection(GPIO13, 2);
    gpioDirection(GPIO14, 2);
    gpioDirection(GPIO15, 2);
    gpioDirection(GPIO16, 2);
    gpioDirection(GPIO17, 2);
    gpioDirection(GPIO18, 2);
    gpioDirection(GPIO19, 2);
    gpioDirection(GPIO20, 2);
    gpioDirection(GPIO21, 2);
    gpioDirection(GPIO22, 2);
    gpioDirection(GPIO23, 2);
    gpioDirection(GPIO24, 2);
    gpioDirection(GPIO25, 2);
    gpioDirection(GPIO26, 2);
    gpioDirection(GPIO27, 2);
    gpioDirection(GPIO28, 2);
    gpioDirection(GPIO29, 2);
    gpioDirection(GPIO30, 2);
}

void rc32kOutputP30(void)
{
    //gpio mux to output
    GPIO->gpio_p30_config = 5;
    //RFC->rf_rc32k_reg_reg = 0x0A;
}

/****************************************************************************************
*
*Function name : pmuGpioWakeup
*Description   : select GPIO as wake up pin.

*Paramater	   :
*        @pinNumber    :gpio number, 0~30.
*        @pinPolarity  :GPIO wake up polarity
                       0: high level to wake up
                       1: low level to wake up.
*Return        : None
****************************************************************************************/
void pmuGpioWakeup(uint32 pinNumber, uint32 pinPolarity)
{
    // set as input
    *((uint32*)(GPIO_REGS + pinNumber + 12)) = 0x30;
    // wake up enable.
    GPIO->gpio_wake_en |= 1<<pinNumber;
    // set polarity.
    if(pinPolarity) {
        GPIO->gpio_wake_pol |= 1<<pinNumber;
    }
    else {
        GPIO->gpio_wake_pol &= ~(1<<pinNumber);
    }
}

/****************************************************************************************
*Function name : rc32kCalibration
*Description   : Calibrate RC 32K oscillator to close to 32.768khz with 20% accuracy.
               : rf_rc32k_reg_reg is a (sign+abs) format value to control rc32k's frequency.
               : for example, 0x3f is the smallest frequency.  0x1f is the largest frequency.
               : 0x20 is close to 0x0, but smaller than 0x0.
*Paramater	   : None
*Return        : None
****************************************************************************************/
void rc32kCalibration(void)
{
    int i;
    uint32 regVal;
    uint32 index;

    PMU->pmu_reg_ext_clk_en_reg &= ~pmu_reg_ext_clk_en_reg_cnr_high_ext_en_bit;
    PMU->pmu_clk_gate_reg |= pmu_clk_gate_reg_ctrl_clk_cali_en_bit;
    PMU->pmu_soft_reset_reg |= pmu_soft_reset_reg_ctrl_rst_cali_n_bit;
    //PMU->pmu_rc32k_calibration_reg = 0x5;
    PMU->pmu_pmu_ctrl_reg |= pmu_pmu_ctrl_reg_rc32k_cali_en_bit;

    //--- Try all setting and find the best one making 32k_prd = 0x5f5e, which is 32.768khz.
    //--- F_32k = 64/32k_prd*50mhz/4 = 32.768khz, so 32k_prd = 0x5f5e.
//    PMU->pmu_rc32k_calibration_reg = 0x5;
//    delayUs(100,HIGH_CLOCK);
//    for(i=0;i<64;i=i+1){
//        RFC->rf_rc32k_reg_reg = i;
//        delayUs(10000,HIGH_CLOCK);
//        regVal = PMU->pmu_pmu_state & 0xffff;
//        //save ./rc32k_cali.hex 0x20010000,0x20010100
//        *((uint32*)(0x20010000 + i*4)) = regVal;
//        //printf("cali:reg=%d, result=%d\r\n",i, regVal);
//    }

    // rc32k calibration.
    PMU->pmu_rc32k_calibration_reg = 0x0;   //using the smallest coef.
    delayUs(100,HIGH_CLOCK);                //delay 1 cycle making this coef active.
    index = 0;                              //index is from 0 ~ 63. the larger index means the higher frequency.

    for(i=5; i>=0; i=i-1) {
        // try setting different bits to 1, from bit5 to bit0.
        index |= 1<<i;
        // exchange index to register.
        if(index >= 0x20) {
            regVal = index - 0x20;
        }
        else {
            regVal = (~index) & 0xff ;
        }
        RFC->rf_rc32k_reg_reg = regVal;
        delayUs(100,HIGH_CLOCK);

        // read period of clock 32k.
        regVal = PMU->pmu_pmu_state & 0xffff;

        // if period is less than our target, then clear this bit.
        if(regVal < 0x5f5e)        {
            index &= ~(1<<i);
        }
    }

    // set as regular coef.
    PMU->pmu_rc32k_calibration_reg = 0x5;
}

/****************************************************************************************
*Function name : adcRawOutput
*Description   : ADC raw data output,  via P2(clk), P18~P7(bit[11:0]), P19(head).
               : 12bit format, please refer to pin mux excel document for more details. 
               : 1: enable.  0: disable. 
*Paramater	   : None
*Return        : None
****************************************************************************************/
void adcRawOutput(int enable)
{
    if(HIGH_CLOCK==HIGH_CLOCK_125M)
    {
        GPIO->gpio_bbadc_test = 0x06;
    }
    else 
    {
        GPIO->gpio_bbadc_test = 0x02;
    }
    
    if(enable)
    {
        // adc raw data output via P2(clk), P18~P7(bit[11:0]), P19(head). 
        for(int i=7;i<20;i++)
        {
            *((uint32*)(GPIO_REGS + i + 12)) = 0x5;
        }
        
        // hclkOutputP02
        GPIO->gpio_p02_config = 5|1<<9|1<<10;
        PMU->pmu_clk_gate_reg |= pmu_clk_gate_reg_ctrl_clk_adc_test_12b_en_bit;  
    }
    else
    {
        // all high-z. 
        for(int i=7;i<20;i++)
        {
            *((uint32*)(GPIO_REGS + i + 12)) = 0x10;  //high-z
        }
        
        // hclkOutputP02
        GPIO->gpio_p02_config = 0x10;
        PMU->pmu_clk_gate_reg &= ~pmu_clk_gate_reg_ctrl_clk_adc_test_12b_en_bit;        
    }   
}

