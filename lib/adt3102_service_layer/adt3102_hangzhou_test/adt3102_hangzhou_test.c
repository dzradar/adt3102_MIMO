//--------------------------------------------------------------------
//Copyright(c)2020,Andar Technologise Inc.
//All Rights Reserved
//Confidential Property of Andar Technologies Inc.
//
//Module Description:
//  .
//  .
//  .
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
#include "arm_math.h"
#include "adt3102_system.h"
#include "adt3102_uart.h"
#include "adt3102_adc.h"
#include "adt3102_tx.h"
#include "adt3102_rx.h"
#include "adt3102_pmu.h"
#include "adt3102_adc.h"
#include "adt3102_dsp.h"
#include "adt3102_gpio.h"
#include "adt3102_dma.h"
#include "adt3102_cfar.h"
#include "adt3102_ft.h"
#include "adt3102_hangzhou_test.h"
#include "adt3102_memscheduling.h"
#include "dsp_ctype_map.h"
#include "rfc_ctype_map.h"
#include "gpio_ctype_map.h"
#include "pmu_ctype_map.h"
#include "dma_ctype_map.h"
#include "use_configuration_wizard.h"
#include "TinyFrame.h"
#include "cirqueue.h"
//
//------Global variable declaration
uint8 RXbuffer[2000] __attribute__ ((at(0X00006000)));


//----------------------------------------------------------------------------
//hangzhou test function
//----------------------------------------------------------------------------
void hangzhouTest(void)
{
    MY_NVIC_SetVectorTable(0x00000000, 0x8000);
    //-----Local variable declaration-----
    //-----Initialize functions and variables and simulation part-----
    setInterrupt(INT_UART0, ENABLEINT);
    setInterrupt(INT_FFT, ENABLEINT);
    setInterrupt(INT_DUALTIMER, ENABLEINT);
    setInterrupt(INT_RTC, ENABLEINT);
    setInterrupt(INT_TRNG, ENABLEINT);
    setInterrupt(INT_UART1, ENABLEINT);
    uartInit(UART0,115200, HIGH_CLOCK);
    uartInit(UART1,115200, HIGH_CLOCK);
    //-----init some module-----
    printf("[hangzhou]adt3102 basic project test mode.\r\n");
    switch_to_50m();
    
    //testPower();
    //hclkOutput();  //p02
    //rc32kOutput(); //p30

    //mcu_sleep_test_hangzhou(0);
    //test20GSingleFequence();
    //test20GFmcw();
    //testChannelAtb();
    //hclkOutput();

    // HZ original program for BIST test
    //testBist();
    // BIST test by Diep
    //testBistRXABB(); // test BB BIST
    //testBistRF();  //test RF BIST
    //testBistONOFFAMP(); //test BIST ON/OFF AMP

    //testMbist();
    //testAdcDC();
    //test20GInputBist();
    //test20GFmcwSingle();
    //testPower();
    //testTemprature();
    test80GFmcw();
    //test80GSingleFequence();
    //test80GSingleFequenceRx();
    //test80GSingleFequenceMaster();
    //test20GSingleFequenceUart();
    while(1)
    {
        //volatile uint64 sumtest;
        //volatile uint32 sumtest1;
        ////the hangzhou test code is written below
        //for(int32 i=0; i<64; i++)
        //{
        //    *((uint32*)0x20008000 +i)= i;
        //}
        //sumtest = accSum(0x20008000, 64);
        //printf("%d\r\n", sumtest);
        //sumtest1 = accOdr(0x20008000, 64, 13);
     
        //testSiggenDsp();
        //testBistDsp();
        //testIfInputDsp();
        //testIfInputDspFreeRun();
        //testAdc16mHclk125M();

        //fmcw single mode.
        //RF on  time: (CHIRP_T0+CHIRP_T1+CHIRP_T2)*SAMPLE_POINT=138*32=4416us
        //RF off time: 5ms
        //Refresh Rate: 1/(5+4.46)*1000=106Hz
        //
        //fmcwEnmode(REGISTER, SINGLE_MODE);
        //waitFmcwcycEnd();
        //delayUs(1000,HIGH_CLOCK);

        //tfTxExample();
        //tfRxExample();
        //TinyFrameRx revTest;
        //revTest = tinyFramefRx();
        //printfTf("test start \r\n");
        //printfTf("%x\r\n", revTest.receiveType);
        //if (revTest.receiveType == 0x100)
        //{
        //    printfTf("%f\r\n", revTest.revfloatData);
        //}else if(revTest.receiveType == 0x101)
        //{
        //    printfTf("%x\r\n", revTest.revuint32Data);
        //}else if(revTest.receiveType == 0x102)
        //{
        //    printfTf("%x\r\n", revTest.revuint32Data);
        //}else if(revTest.receiveType == 0x103)
        //{
        //    printfTf("%f\r\n", revTest.revfloatData);
        //}
        //printfTf("test end \r\n");
    }
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//Here are the cases
//----------------------------------------------------------------------------
/****************************************************************************************
*Function name : testSingleFequence
*Description   : single fequence test
****************************************************************************************/
void rc32kOutput(void)
{
    //gpio mux to output
    GPIO->gpio_p30_config = 5;
    RFC->rf_rc32k_reg_reg = 0x0A;
}
void hclkOutput(void)
{
    GPIO->gpio_p02_config = 5|1<<9|1<<10;
    PMU->pmu_clk_gate_reg |=1<<10;
}

void adcClkOutput(void)
{
    GPIO->gpio_p02_config = 4;
    PMU->pmu_clk_gate_reg |=0x0fff;
}

void plliOutput(void)
{
    GPIO->gpio_p24_config = 0x02;
    PMU->pmu_clk_gate_reg |=1<<12;
}

void manuPllnCapArray(int32 cpValue)
{
    RFC->rf_plln_regb_reg &= ~(0xf<<8);
    RFC->rf_plln_regb_reg |= cpValue<<8;
}

void testPower(void)
{
    //delay 1ms: Trigger POR at high temperature
    delayUs(1000 ,HIGH_CLOCK);
    baseband18LDO(ENABLE, BB_LDO_1P8, ENABLE, BB_LDO_1P8);
    delayUs(1000,HIGH_CLOCK);
    plliLDOEnable(ENABLE, ENABLE);
    delayUs(1000,HIGH_CLOCK);
    pllnVcoConfig(ENABLE, PLLN_VCO_LDO_1P8);
    delayUs(1000,HIGH_CLOCK);
    pllnPllConfig(ENABLE, PLLN_PLL_LDO_2P5, ENABLE);
    delayUs(1000,HIGH_CLOCK);
    dvdd12HpLDO(POWER_ON, DVDD1P2_HP);
    delayUs(1000,HIGH_CLOCK);
    dacEnBias1Pd(ENABLE, POWER_ON);
}
/****************************************************************************************
*Function name : test20GOutput test20GFmcw
*Description   : Rf 20G output Single Fequence and fmcw
*Paramater	   : none
*        @ :
*        @ :
*Paramater	selection: None
*Power consumption : none
*Return        : none
****************************************************************************************/
void test20GSingleFequence(void)
{
    volatile float vtuneValue;
    //power on
    testPower();
    //rf config
    plliDacSetting();
    plliConfig(ENABLE);
    pllnDacSetting();
    pllnConfig(ENABLE);
    fmcwModeSel(MODE_TEST);
    rfbuff20G(ENABLE, DISABLE);
    dacEnBias1Pd(1, 0);
    plliLock();

    pllnLock(76, 0);
    //manuPllnCapArray(0xD);//76G:B  77G:A,B
    fmcwConfig(76, 0, CHIRP_T0, CHIRP_T1, CHIRP_T2, CHIRP_NUM);
    fmcwEnmode(REGISTER, CONTINUE_MODE);
    
    vtuneValue = vtuneRead();
    //printf("%f\r\n", vtuneValue);
    rfPhaseShift(0x1f, 0, 0x1f, 0);
    rfPhaseShiftLUT(0, 0);
    RFC->rf_spare_reg = 0x01;
}
void test20GFmcw(void)
{
    //power on
    testPower();
    //rf config
    plliDacSetting();
    plliConfig(ENABLE);
    pllnDacSetting();
    pllnConfig(ENABLE);
    fmcwModeSel(MODE_TEST);
    rfbuff20G(ENABLE, DISABLE);
    dacEnBias1Pd(1, 0);
    plliLock();

    pllnLock(FSTART, 2000);
    fmcwConfig(FSTART, 2000, CHIRP_T0, CHIRP_T1, CHIRP_T2, CHIRP_NUM);
    fmcwEnmode(REGISTER, CONTINUE_MODE);
    //manuPllnCapArray(0xB);//76G:B  77G:A,B

    rfPhaseShift(0x1f, 0, 0x1f, 0);
    RFC->rf_spare_reg = 0x01;
}
void test20GFmcwSingle(void)
{
    //power on
    testPower();
    //rf config
    plliDacSetting();
    plliConfig(ENABLE);
    pllnDacSetting();
    pllnConfig(ENABLE);
    fmcwModeSel(MODE_TEST);
    rfbuff20G(ENABLE, DISABLE);
    dacEnBias1Pd(1, 0);
    plliLock();

    fmcwConfig(FSTART, 4000, CHIRP_T0, CHIRP_T1, CHIRP_T2, CHIRP_NUM);

    pllnLock(FSTART, 0);
    manuPllnCapArray(0xB);//76G:B  77G:A,B

    rfPhaseShift(0x1f, 0, 0x1f, 0);
    RFC->rf_spare_reg = 0x01;
}
/****************************************************************************************
*Function name : testChannel
*Description   : channel test
*Paramater	   : none
*        @ :
*        @ :
*Paramater	selection: None
*Power consumption : none
*Return        : none
****************************************************************************************/
void testChannelAtb(void)
{
    test20GSingleFequence();
    //set fmcw pll to doubler
    fmcwModeSel(MODE_INTER);

    //enable 20G atb        (rf atb select 000:pwd) 0x400030d4:8d200
    //RFC->rfc_tx_pa_reg |= 1<<6;
    //enable doubler atb    (rf atb select 001:pwd) 0x400030d4:8d183
    //RFC->rfc_tx_pa_reg |= 1<<7|1<<8;
    //enable buffer1to4 atb (rf atb select 010:pwd) 0x400030d4:8d140
    //RFC->rfc_tx_pa_reg |= 1<<5|1<<9|1<<8;
    //enable PA1 atb        (rf atb select 00:pwd)  0x400030d4:0d238
    //RFC->rfc_tx_pa_reg |= 1<<3;
    //enable PA0 atb        (rf atb select 00:pwd)  0x400030d4:0d238
    RFC->rfc_tx_pa_reg |= 1<<2;

    //enable pd Bias(din20)
    biasPowerdector(8);
    //enable gpadc
    gpadcEnable(GPADC_IN5);

    //enable 20G and doubler
    rfbuff20G(1, 1);
    //enable buffer1to4
    rfBuff77GEnable(1, 1, 1, 1, 1);
    //enable bias for 20G(fmcw buffer), doubler and buffer1to4
    bias20GTo80G(8, 8, 8, 8, 8, 8, 8, 8);
    //enable pa1 and pa0
    txPaEnable(1, 1);
    txPaBias(8, 8);

    //VBE+GPADC_AIO1
    RFC->rf_gpadc_reg_reg = (1<<(7+1))|1<<0|1<<5|1<<2;
    RFC->rf_gpadc_reg_reg |= (1<<(7+2));

}

/****************************************************************************************
*Function name : testChannel
*Description   : channel test
*Paramater	   : none
*        @ :
*        @ :
*Paramater	selection: None
*Power consumption : none
*Return        : none
****************************************************************************************/
void testAdcDC(void)
{
    test20GSingleFequence();
    //set fmcw pll to doubler
    fmcwModeSel(MODE_INTER);

    //enable 20G atb        (rf atb select 000:pwd) 0x400030d4:8d200
    //RFC->rfc_tx_pa_reg |= 1<<6;
    //enable doubler atb    (rf atb select 001:pwd) 0x400030d4:8d183
    //RFC->rfc_tx_pa_reg |= 1<<7|1<<8;
    //enable buffer1to4 atb (rf atb select 010:pwd) 0x400030d4:8d140
    //RFC->rfc_tx_pa_reg |= 1<<5|1<<9|1<<8;
    //enable PA1 atb        (rf atb select 00:pwd)  0x400030d4:0d238
    //RFC->rfc_tx_pa_reg |= 1<<3;
    //enable PA0 atb        (rf atb select 00:pwd)  0x400030d4:0d238
    RFC->rfc_tx_pa_reg |= 1<<2;

    //enable pd Bias(din20)
    biasPowerdector(8);
    //enable gpadc
    gpadcEnable(GPADC_IN5);

    //enable 20G and doubler
    rfbuff20G(1, 1);
    //enable buffer1to4
    rfBuff77GEnable(1, 1, 1, 1, 1);
    //enable bias for 20G(fmcw buffer), doubler and buffer1to4
    bias20GTo80G(8, 8, 8, 8, 8, 8, 8, 8);
    //enable pa1 and pa0
    txPaEnable(1, 1);
    txPaBias(8, 8);

    //adc DC setting
    //en_adc_test
    RFC->rfc_vga0_reg |= 1<<10;
    RFC->rfc_vga1_reg |= 1<<10;

    RFC->rf_adc0_regs_reg = 0x00031a1a;
    RFC->rf_spare_reg = 0x00000038;

}
/****************************************************************************************
*Function name : testChannel
*Description   : channel test
*Paramater	   : none
*        @ :
*        @ :
*Paramater	selection: None
*Power consumption : none
*Return        : none
****************************************************************************************/


// Original test program
void testBist(void)
{
    //test20GFmcw();
    test20GSingleFequence();
    //set fmcw pll to doubler
    fmcwModeSel(MODE_INTER);

    //enable 20G and doubler
    rfbuff20G(1, 1);
    //enable buffer1to4
    rfBuff77GEnable(1, 1, 1, 1, 1);
    //enable bias for 20G(fmcw buffer), doubler and buffer1to4
    bias20GTo80G(8, 8, 8, 8, 8, 8, 8, 8);
    //enable pa1 and pa0
    txPaEnable(1, 0);
    txPaBias(14, 0);

    //bist on amplifier
    bistOn(BIST_AMP, BIST_DIV32_SEL);
 
    rxLnaEnable(1, 1);
    mixerEnable(1, 1);
    lnaRegularBias(1, 1);
    lnaDacBias(14, 14, 14, 14, 14, 14);

    //bias2_en: enable signal for ztc bias for BB
    baseBandBiasSetting(ENABLE);

    //tia config
    rfTiaEnable(ENABLE, ENABLE);
    rfTiaGainSetting(TIA_GAIN_0DB, TIA_GAIN_0DB ,TIA_GAIN_0DB ,TIA_GAIN_0DB);
    rfTiaDacBias(8, 8);
    //pga config
    rfPgaGainCalEnable(0, 0, 0, 0);
    rfPgaGainCalSetting(PGA_CAL_P1, PGA_CAL_P1 ,PGA_CAL_P1 ,PGA_CAL_P1);
    rfVgaEnable(ENABLE, ENABLE);
    rfVgaGainSetting(PGA_GAIN_20DB, PGA_GAIN_20DB ,PGA_GAIN_20DB ,PGA_GAIN_20DB);
    rfVgaDacBias(8, 8);
    //filter config
    rcHpfConfig(0, 0, RC_HPF_150KHZ, RC_HPF_150KHZ);
    tiaHpfConfig(1, 1, TIA_HPF_25KHZ, TIA_HPF_25KHZ);
    pgaLpfConfig(PGA_LPF_8M, PGA_LPF_8M);
    pgaHpfConfig(1, 1, PGA_HPF_25KHZ, PGA_HPF_25KHZ);

    //enable vga en_buffer
    RFC->rfc_vga0_reg |= 1<<9;
    //en_buffer0 contorl if signal output gpio23, 24, 25, 26.
    //en_buffer1 contorl if signal output gpio27, 28, 29, 30.
    //when we test low power consumption, we need to connect p29 with vddrf enable.
    //Normally, p29 is high resistance, vga1 en_buffer will lower the p29 level.
    RFC->rfc_vga1_reg |= 1<<9;

    //???gpio mux to output bist if signal
    //GPIO->gpio_p24_config = 0x16;
    //GPIO->gpio_p25_config = 0x16;

    //for(int32 i1=0; i1<0x1f; i1++)
    //{
    //    for(int32 i2=0; i2<0x1f; i2++)
    //    {
    //        for(int32 j1=0; j1<0x1f; j1++)
    //        {
    //            for(int32 j2=0; j2<0x1f; j2++)
    //            {
    //                rfPhaseShift(i1, i2, j1, j2);
    //                delayUs(10000, HIGH_CLOCK);
    //            }
    //
    //        }
    //    }
    //}
}

//BIST for RX RF TX0 RX0
/**************************************************************************************************************
* Function description: BIST for RF loop (Tx0-RX0 on)
*                       Measure relative phase, RX gain flatness, RX RF gain steps
***************************************************************************************************************/
void testBistRF_TX0_RX0(void)
{
    //test20GFmcw();
    test20GSingleFequence();
    //set fmcw pll to doubler
    fmcwModeSel(MODE_INTER);
    //enable 20G and doubler
    rfbuff20G(1, 1);
    //enable buffer1to4 (EN stage1Buff77G, EN tx0Buffer, DIS_EN tx1Buffer, EN rx0Buffer, DIS_EN rx1Buffer)
    rfBuff77GEnable(1, 1, 0, 1, 0); //for tx0-rx0
    //rfBuff77GEnable(1, 1, 0, 1, 1);  //for tx0-rx1
    //enable bias for 20G(fmcw buffer), doubler and buffer1to4
    bias20GTo80G(8, 8, 8, 8, 8, 8, 8, 8);

    //enable pa0 and pa1
    //txPaEnable(1, 1);
    //txPaBias(8, 8);

    //enable pa0
    txPaEnable(1, 0);
    txPaBias(10, 10);

    //bist on amplifier
    bistOn(BIST_AMP, BIST_DIV64_SEL);

    //enable rx0 chain
    rxLnaEnable(1, 0);
    mixerEnable(1, 0);
    lnaRegularBias(1, 0);
    lnaDacBias(8, 8, 8, 8, 8, 8);

    //bias2_en: enable signal for ztc bias for BB
    baseBandBiasSetting(ENABLE);

    //tia config (enable tia0)
    rfTiaEnable(ENABLE, DISABLE); //enable tia0
    //rfTiaEnable(DISABLE, ENABLE); //enable tia1
    rfTiaGainSetting(TIA_GAIN_12DB, TIA_GAIN_12DB ,TIA_GAIN_12DB ,TIA_GAIN_12DB);
    rfTiaDacBias(8, 8);

    //pga config (enable vga0)
    //rfPgaGainCalEnable(ENABLE, ENABLE, ENABLE, ENABLE);
    rfPgaGainCalEnable(DISABLE, DISABLE, DISABLE, DISABLE);
    rfPgaGainCalSetting(PGA_CAL_P1, PGA_CAL_P1 ,PGA_CAL_P1 ,PGA_CAL_P1);
    rfVgaEnable(ENABLE, DISABLE);  //enable vga0
    //rfVgaEnable(DISABLE, ENABLE);  //enable vga1
    rfVgaGainSetting(PGA_GAIN_8DB, PGA_GAIN_8DB ,PGA_GAIN_8DB ,PGA_GAIN_8DB);
    rfVgaDacBias(8, 8);

    //filter config
    tiaHpfConfig(1, 1, TIA_HPF_100KHZ, TIA_HPF_100KHZ);
    rcHpfConfig(0, 0, RC_HPF_150KHZ, RC_HPF_150KHZ);
    pgaLpfConfig(PGA_LPF_8M, PGA_LPF_8M);
    pgaLpfCal(1, 1, 2, 2);
    pgaHpfConfig(1, 1, PGA_HPF_25KHZ, PGA_HPF_25KHZ);

    //enable vga en_buffer
    RFC->rfc_vga0_reg |= 1<<9;
    //en_buffer0 contorl if signal output gpio23, 24, 25, 26.
    //en_buffer1 contorl if signal output gpio27, 28, 29, 30.
    //when we test low power consumption, we need to connect p29 with vddrf enable.
    //Normally, p29 is high resistance, vga1 en_buffer will lower the p29 level.
    RFC->rfc_vga1_reg |= 1<<9;

    //???gpio mux to output bist if signal
    GPIO->gpio_p23_config = 0x16;
    GPIO->gpio_p24_config = 0x16;
    GPIO->gpio_p25_config = 0x16;
    GPIO->gpio_p26_config = 0x16;

    GPIO->gpio_p27_config = 0x16;
    GPIO->gpio_p28_config = 0x16;
    GPIO->gpio_p29_config = 0x16;
    GPIO->gpio_p30_config = 0x16;

    rfPhaseShiftLUT(0, 10);
    //while(1)
    //{
    //for(int32 i1=0; i1<0x3f; i1++)
    //{
    //rfPhaseShiftLUT(i1, i1);
    //delayUs(1000000, HIGH_CLOCK);
    //}
    //}
}

//BIST for RX RF TX1 RX0
/**************************************************************************************************************
* Function description: BIST for RF loop (Tx1-RX0 on)
*                       Measure relative phase, RX gain flatness, RX RF gain steps
***************************************************************************************************************/
void testBistRF_TX1_RX0(void)
{
    //test20GFmcw();
    test20GSingleFequence();
    //set fmcw pll to doubler
    fmcwModeSel(MODE_INTER);
    //enable 20G and doubler
    rfbuff20G(1, 1);
    //enable buffer1to4 (EN stage1Buff77G, DIS_EN tx0Buffer, EN tx1Buffer, EN rx0Buffer, DIS_EN rx1Buffer)
    rfBuff77GEnable(1, 0, 1, 1, 0); //for tx1-rx0
    //rfBuff77GEnable(1, 1, 0, 1, 1);  //for tx0-rx1
    //enable bias for 20G(fmcw buffer), doubler and buffer1to4
    bias20GTo80G(8, 8, 8, 8, 8, 8, 8, 8);

    //enable pa0 and pa1
    //txPaEnable(1, 1);
    //txPaBias(8, 8);

    //enable pa1
    txPaEnable(0, 1);
    txPaBias(10, 10);

    //bist on amplifier
    bistOn(BIST_AMP, BIST_DIV64_SEL);

    //enable rx0 chain
    rxLnaEnable(1, 0);
    mixerEnable(1, 0);
    lnaRegularBias(1, 0);
    lnaDacBias(8, 8, 8, 8, 8, 8);

    //bias2_en: enable signal for ztc bias for BB
    baseBandBiasSetting(ENABLE);

    //tia config (enable tia0)
    rfTiaEnable(ENABLE, DISABLE); //enable tia0
    //rfTiaEnable(DISABLE, ENABLE); //enable tia1
    rfTiaGainSetting(TIA_GAIN_12DB, TIA_GAIN_12DB ,TIA_GAIN_12DB ,TIA_GAIN_12DB);
    rfTiaDacBias(8, 8);

    //pga config (enable vga0)
    //rfPgaGainCalEnable(ENABLE, ENABLE, ENABLE, ENABLE);
    rfPgaGainCalEnable(DISABLE, DISABLE, DISABLE, DISABLE);
    rfPgaGainCalSetting(PGA_CAL_P1, PGA_CAL_P1 ,PGA_CAL_P1 ,PGA_CAL_P1);
    rfVgaEnable(ENABLE, DISABLE);  //enable vga0
    //rfVgaEnable(DISABLE, ENABLE);  //enable vga1
    rfVgaGainSetting(PGA_GAIN_8DB, PGA_GAIN_8DB ,PGA_GAIN_8DB ,PGA_GAIN_8DB);
    rfVgaDacBias(8, 8);

    //filter config
    tiaHpfConfig(1, 1, TIA_HPF_100KHZ, TIA_HPF_100KHZ);
    rcHpfConfig(0, 0, RC_HPF_150KHZ, RC_HPF_150KHZ);
    pgaLpfConfig(PGA_LPF_8M, PGA_LPF_8M);
    pgaLpfCal(1, 1, 2, 2);
    pgaHpfConfig(1, 1, PGA_HPF_25KHZ, PGA_HPF_25KHZ);

    //enable vga en_buffer
    RFC->rfc_vga0_reg |= 1<<9;
    //en_buffer0 contorl if signal output gpio23, 24, 25, 26.
    //en_buffer1 contorl if signal output gpio27, 28, 29, 30.
    //when we test low power consumption, we need to connect p29 with vddrf enable.
    //Normally, p29 is high resistance, vga1 en_buffer will lower the p29 level.
    RFC->rfc_vga1_reg |= 1<<9;

    //???gpio mux to output bist if signal
    GPIO->gpio_p23_config = 0x16;
    GPIO->gpio_p24_config = 0x16;
    GPIO->gpio_p25_config = 0x16;
    GPIO->gpio_p26_config = 0x16;

    GPIO->gpio_p27_config = 0x16;
    GPIO->gpio_p28_config = 0x16;
    GPIO->gpio_p29_config = 0x16;
    GPIO->gpio_p30_config = 0x16;

    rfPhaseShiftLUT(0, 10);
    //while(1)
    //{
    //for(int32 i1=0; i1<0x3f; i1++)
    //{
    //rfPhaseShiftLUT(i1, i1);
    //delayUs(1000000, HIGH_CLOCK);
    //}
    //}
}

//BIST for RX RF TX0 RX1
void testBistRF_TX0_RX1(void)
{
    //test20GFmcw();
    test20GSingleFequence();
    //set fmcw pll to doubler
    fmcwModeSel(MODE_INTER);
    //enable 20G and doubler
    rfbuff20G(1, 1);
    //enable buffer1to4 (EN stage1Buff77G, EN tx0Buffer, DIS_EN tx1Buffer, EN rx0Buffer, DIS_EN rx1Buffer)
    //rfBuff77GEnable(1, 1, 0, 1, 0); //for tx0-rx0
    rfBuff77GEnable(1, 1, 0, 0, 1);  //for tx0-rx1
    //enable bias for 20G(fmcw buffer), doubler and buffer1to4
    bias20GTo80G(8, 8, 8, 8, 8, 8, 8, 8);

    //enable pa0 and pa1
    //txPaEnable(1, 1);
    //txPaBias(8, 8);

    //enable pa0
    txPaEnable(1, 0);
    txPaBias(15, 15);

    //bist on amplifier
    bistOn(BIST_AMP, BIST_DIV64_SEL);

    //enable rx1 chain
    rxLnaEnable(0, 1);
    mixerEnable(0, 1);
    lnaRegularBias(0, 1);
    lnaDacBias(8, 8, 8, 8, 8, 8);

    //bias2_en: enable signal for ztc bias for BB
    baseBandBiasSetting(ENABLE);

    //tia config (enable tia0)
    //rfTiaEnable(ENABLE, DISABLE); //enable tia0
    rfTiaEnable(DISABLE, ENABLE); //enable tia1
    rfTiaGainSetting(TIA_GAIN_12DB, TIA_GAIN_12DB ,TIA_GAIN_12DB ,TIA_GAIN_12DB);
    rfTiaDacBias(8, 8);

    //pga config (enable vga0)
    //rfPgaGainCalEnable(ENABLE, ENABLE, ENABLE, ENABLE);
    rfPgaGainCalEnable(DISABLE, DISABLE, DISABLE, DISABLE);
    rfPgaGainCalSetting(PGA_CAL_P1, PGA_CAL_P1 ,PGA_CAL_P1 ,PGA_CAL_P1);
    //rfVgaEnable(ENABLE, DISABLE);  //enable vga0
    rfVgaEnable(DISABLE, ENABLE);  //enable vga1
    rfVgaGainSetting(PGA_GAIN_8DB, PGA_GAIN_8DB ,PGA_GAIN_8DB ,PGA_GAIN_8DB);
    rfVgaDacBias(8, 8);

    //filter config
    tiaHpfConfig(1, 1, TIA_HPF_100KHZ, TIA_HPF_100KHZ);
    rcHpfConfig(0, 0, RC_HPF_150KHZ, RC_HPF_150KHZ);
    pgaLpfConfig(PGA_LPF_8M, PGA_LPF_8M);
    pgaLpfCal(1, 1, 2, 2);
    pgaHpfConfig(1, 1, PGA_HPF_25KHZ, PGA_HPF_25KHZ);

    //enable vga en_buffer
    RFC->rfc_vga0_reg |= 1<<9;
    //en_buffer0 contorl if signal output gpio23, 24, 25, 26.
    //en_buffer1 contorl if signal output gpio27, 28, 29, 30.
    //when we test low power consumption, we need to connect p29 with vddrf enable.
    //Normally, p29 is high resistance, vga1 en_buffer will lower the p29 level.
    RFC->rfc_vga1_reg |= 1<<9;

    //???gpio mux to output bist if signal
    GPIO->gpio_p23_config = 0x16;
    GPIO->gpio_p24_config = 0x16;
    GPIO->gpio_p25_config = 0x16;
    GPIO->gpio_p26_config = 0x16;

    GPIO->gpio_p27_config = 0x16;
    GPIO->gpio_p28_config = 0x16;
    GPIO->gpio_p29_config = 0x16;
    GPIO->gpio_p30_config = 0x16;

    rfPhaseShiftLUT(2, 10);
    //while(1)
    //{
    //for(int32 i1=0; i1<0x3f; i1++)
    //{
    //rfPhaseShiftLUT(i1, i1);
    //delayUs(1000000, HIGH_CLOCK);
    //}
    //}
}


/**************************************************************************************************************
* Function description: BIST for RX ABB (only turn on the related circuits: BIST clock, BIST RXABB, and RX ABB)
*                       Turn on BIST RXABB for both ABB0 & ABB1
***************************************************************************************************************/
void testBistRXABB(void)
{
    //delay 1ms: Trigger POR at high temperature
    delayUs(1000,HIGH_CLOCK);
    baseband18LDO(ENABLE, BB_LDO_1P8, ENABLE, BB_LDO_1P8);

    //bist current switching on
    bistOn(BIST_ABB, BIST_DIV64_SEL);

    //bias2_en: enable signal for ztc bias for BB
    baseBandBiasSetting(ENABLE);

    //tia config
    rfTiaEnable(ENABLE, ENABLE);
    rfTiaGainSetting(TIA_GAIN_18DB, TIA_GAIN_18DB ,TIA_GAIN_18DB ,TIA_GAIN_18DB);
    rfTiaDacBias(8, 8);

    //pga config
    rfPgaGainCalEnable(DISABLE, DISABLE, DISABLE, DISABLE);
    //rfPgaGainCalEnable(ENABLE, ENABLE, ENABLE, ENABLE);
    rfPgaGainCalSetting(PGA_CAL_P1, PGA_CAL_P1 ,PGA_CAL_P1 ,PGA_CAL_P1);
    rfVgaEnable(ENABLE, ENABLE);
    rfVgaGainSetting(PGA_GAIN_10DB, PGA_GAIN_10DB ,PGA_GAIN_10DB ,PGA_GAIN_10DB);
    rfVgaDacBias(8, 8);

    //filter config
    tiaHpfConfig(1, 1, TIA_HPF_100KHZ, TIA_HPF_100KHZ);
    //rcHpfConfig(1, 1, RC_HPF_150KHZ, RC_HPF_150KHZ); setup for excel measurement
    //0: bypass RC filter (disable RC filter). 1: don't bypass RC filter (enable RC filter)
    rcHpfConfig(0, 0, RC_HPF_150KHZ, RC_HPF_150KHZ);
    pgaLpfConfig(PGA_LPF_8M, PGA_LPF_8M);
    //pgaLpfCal(1, 1, 3, 3); Original program; setup for excel measurement
    pgaLpfCal(1, 1, 2, 2);
    pgaHpfConfig(1, 1, PGA_HPF_25KHZ, PGA_HPF_25KHZ);

    //enable vga en_buffer
    RFC->rfc_vga0_reg |= 1<<9;
    //en_buffer0 contorl if signal output gpio23, 24, 25, 26.
    //en_buffer1 contorl if signal output gpio27, 28, 29, 30.
    //when we test low power consumption, we need to connect p29 with vddrf enable.
    //Normally, p29 is high resistance, vga1 en_buffer will lower the p29 level.
    RFC->rfc_vga1_reg |= 1<<9;

    //???gpio mux to output bist if signal
    GPIO->gpio_p23_config = 0x16;
    GPIO->gpio_p24_config = 0x16;
    GPIO->gpio_p25_config = 0x16;
    GPIO->gpio_p26_config = 0x16;

    GPIO->gpio_p27_config = 0x16;
    GPIO->gpio_p28_config = 0x16;
    GPIO->gpio_p29_config = 0x16;
    GPIO->gpio_p30_config = 0x16;
}

/**************************************************************************************************************
* Function description: Test BIST ON/OFF amplifier
*                       Measure BIST ON/OFF amplifier gain flatness vs. frquency
***************************************************************************************************************/
void testBistONOFFAMP_Full_Bk(void)
{
    //******************************************************************************************************
    // copy from testBistRF()

    //test20GFmcw();
    test20GSingleFequence();
    //set fmcw pll to doubler
    fmcwModeSel(MODE_INTER);
    //enable 20G and doubler
    rfbuff20G(1, 1);
    //enable buffer1to4 (EN stage1Buff77G, EN tx0Buffer, DIS_EN tx1Buffer, EN rx0Buffer, DIS_EN rx1Buffer)
    rfBuff77GEnable(1, 1, 0, 1, 0); //for tx0-rx0
    //rfBuff77GEnable(1, 1, 0, 1, 1);  //for tx0-rx1
    //enable bias for 20G(fmcw buffer), doubler and buffer1to4
    bias20GTo80G(8, 8, 8, 8, 8, 8, 8, 8);

    //enable pa0 and pa1
    //txPaEnable(1, 1);
    //txPaBias(8, 8);

    //enable pa0
    txPaEnable(1, 0);
    txPaBias(10, 10);

    //bist on amplifier
    bistOn(BIST_AMP, BIST_DIV64_SEL);

    //enable rx0 chain
    rxLnaEnable(1, 0);
    mixerEnable(1, 0);
    lnaRegularBias(1, 0);
    lnaDacBias(8, 8, 8, 8, 8, 8);

    //bias2_en: enable signal for ztc bias for BB
    baseBandBiasSetting(ENABLE);

    //tia config (enable tia0)
    rfTiaEnable(ENABLE, DISABLE); //enable tia0
    //rfTiaEnable(DISABLE, ENABLE); //enable tia1
    rfTiaGainSetting(TIA_GAIN_12DB, TIA_GAIN_12DB ,TIA_GAIN_12DB ,TIA_GAIN_12DB);
    rfTiaDacBias(8, 8);

    //pga config (enable vga0)
    //rfPgaGainCalEnable(ENABLE, ENABLE, ENABLE, ENABLE);
    rfPgaGainCalEnable(DISABLE, DISABLE, DISABLE, DISABLE);
    rfPgaGainCalSetting(PGA_CAL_P1, PGA_CAL_P1 ,PGA_CAL_P1 ,PGA_CAL_P1);
    rfVgaEnable(ENABLE, DISABLE);  //enable vga0
    //rfVgaEnable(DISABLE, ENABLE);  //enable vga1
    rfVgaGainSetting(PGA_GAIN_8DB, PGA_GAIN_8DB ,PGA_GAIN_8DB ,PGA_GAIN_8DB);
    rfVgaDacBias(8, 8);

    //filter config
    tiaHpfConfig(1, 1, TIA_HPF_100KHZ, TIA_HPF_100KHZ);
    rcHpfConfig(0, 0, RC_HPF_150KHZ, RC_HPF_150KHZ);
    pgaLpfConfig(PGA_LPF_8M, PGA_LPF_8M);
    pgaLpfCal(1, 1, 2, 2);
    pgaHpfConfig(1, 1, PGA_HPF_25KHZ, PGA_HPF_25KHZ);

    //enable vga en_buffer
    RFC->rfc_vga0_reg |= 1<<9;
    //en_buffer0 contorl if signal output gpio23, 24, 25, 26.
    //en_buffer1 contorl if signal output gpio27, 28, 29, 30.
    //when we test low power consumption, we need to connect p29 with vddrf enable.
    //Normally, p29 is high resistance, vga1 en_buffer will lower the p29 level.
    RFC->rfc_vga1_reg |= 1<<9;

    //???gpio mux to output bist if signal
    GPIO->gpio_p23_config = 0x16;
    GPIO->gpio_p24_config = 0x16;
    GPIO->gpio_p25_config = 0x16;
    GPIO->gpio_p26_config = 0x16;

    GPIO->gpio_p27_config = 0x16;
    GPIO->gpio_p28_config = 0x16;
    GPIO->gpio_p29_config = 0x16;
    GPIO->gpio_p30_config = 0x16;

    rfPhaseShiftLUT(0, 10);
    //while(1)
    //{
    //for(int32 i1=0; i1<0x3f; i1++)
    //{
    //rfPhaseShiftLUT(i1, i1);
    //delayUs(1000000, HIGH_CLOCK);
    //}
    //}
//*****************************************************************************************

    *(uint32*)0x4000c01c |= 1<<4; //manu_ctrl_en = 1.
    RFC->rf_bist_cfg_reg |= 1<<24;  //Enable test point mux for BIST
    RFC->rf_gpadc_reg_reg |= 0x11<<8;  //Enalbe in5 (GPADC) for BIST TP connect to in1 (AIO1)

}

/**************************************************************************************************************
* Function description: Test BIST ON/OFF amplifier
*                       Measure BIST ON/OFF amplifier gain flatness vs. frquency
***************************************************************************************************************/
void testBistONOFFAMP(void)
{
    //******************************************************************************************************
    // copy from testBistRF()

    //test20GFmcw();
    test20GSingleFequence();
    //set fmcw pll to doubler
    fmcwModeSel(MODE_INTER);
    //enable 20G and doubler
    rfbuff20G(1, 1);
    //enable buffer1to4 (EN stage1Buff77G, EN tx0Buffer, DIS_EN tx1Buffer, EN rx0Buffer, DIS_EN rx1Buffer)
    rfBuff77GEnable(1, 1, 0, 1, 0); //for tx0-rx0
    //rfBuff77GEnable(1, 1, 0, 1, 1);  //for tx0-rx1
    //enable bias for 20G(fmcw buffer), doubler and buffer1to4
    bias20GTo80G(8, 8, 8, 8, 8, 8, 8, 8);

    //enable pa0 and pa1
    //txPaEnable(1, 1);
    //txPaBias(8, 8);

    //enable pa0
    txPaEnable(1, 0);
    txPaBias(10, 10);

    //bist on amplifier
    bistOn(BIST_AMP, BIST_DIV64_SEL);

    //enable rx0 chain
    rxLnaEnable(1, 0);
    mixerEnable(1, 0);
    lnaRegularBias(1, 0);
    lnaDacBias(8, 8, 8, 8, 8, 8);

//*****************************************************************************************

    *(uint32*)0x4000c01c |= 1<<4; //manu_ctrl_en = 1.
    RFC->rf_bist_cfg_reg |= 1<<24;  //Enable test point mux for BIST
    RFC->rf_gpadc_reg_reg |= 0x11<<8;  //Enalbe in5 (GPADC) for BIST TP connect to in1 (AIO1)

}
void bistOn(uint16 bistSorSel, uint16 bistDivSel)
{
    RFC->rfc_plli_ctrl_reg |= 1<<2;
    PMU->pmu_pmu_ctrl_reg |= pmu_pmu_ctrl_reg_manu_ctrl_en_bit;
    delayUs(100, HIGH_CLOCK);
    PMU->pmu_pmu_ctrl_reg &= ~pmu_pmu_ctrl_reg_manu_ctrl_en_bit;

    biasBist(8, 8, 8, 8);
    //biasBist(10, 10, 4, 4);

    if (bistSorSel == 1)
    {
        //BIST_EN_ONOFF_AMP ON
        //RFC->rf_bist_cfg_reg = 0x07|bistDivSel<<8|0x01<<16|0x01<<24;
        // set bit24=0 to disable test point mux for BIST. When you do some bist testing, then enable it. from Diep.
        RFC->rf_bist_cfg_reg = 0x07|bistDivSel<<8|0x01<<16|0x00<<24;
    } else
    {
        //BIST_RX0_ABB_EN BIST_RX1_ABB_EN ON
        //RFC->rf_bist_cfg_reg = 0x35|bistDivSel<<8|0x01<<16|0x01<<24;
        // set bit24=0 to disable test point mux for BIST. When you do some bist testing, then enable it. from Diep.
        RFC->rf_bist_cfg_reg = 0x35|bistDivSel<<8|0x01<<16|0x00<<24;
    }
    RFC->rf_bist_load = 1;
}
/****************************************************************************************
*Function name : testMbist
*Description   :
*Paramater	   :
*        @ :
*        @ :
*Paramater	selection:

*Power consumption :
*Return        :
****************************************************************************************/
void testMbist(void)
{

    //generate 50M clock to gpio2
    hclkOutput();

}
/****************************************************************************************
*Function name : testDsp
*Description   : range fft, velocity fft, cfar, phase, angle ... test
****************************************************************************************/
void testSiggenDsp(void)
{
    //open function test20GSingleFequence() outside while

    //bias2_en: enable signal for ztc bias for BB
    baseBandBiasSetting(ENABLE);

    siggen(0x1f1f, 0x0000, 1, 1, 0x2000);
    adcEnable(1, 1);
    adcSampleInit(1, 1);
    adcWnpara(1, 1, 512);
    adcFirDownsample(0, DOWN_SAMPLE);
    dataSampleConfig(INTERVALNUM, 126, SAMPLE_POINT, DOWN_SAMPLE, CHIRP_NUM);
    adcBiasSetting(8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8);

    adcClkOutput();
    fmcwConfig(FSTART, 0, CHIRP_T0, CHIRP_T1, CHIRP_T2, CHIRP_NUM);
    fmcwEnmode(DATA_SAMPLE_MODULE, SINGLE_MODE);
    adcSampleStart(ADC01SEL, SAMPLE_POINT, CHIRP_NUM, ADC0_FIFO, ADC0_ADDRE, ADC1_FIFO, ADC1_ADDRE);
    //save .\Data_process\Keilsave\adc0_data.hex 0x20008000,0x20018000
    fftProcess(512, CHIRP_NUM, 0x20008000, 0x20008000, 4, 4);
    //save .\Data_process\Keilsave\fft0_data.hex 0x20008000,0x20018000
}

/****************************************************************************************
*Function name : testDsp
*Description   : range fft, velocity fft, cfar, phase, angle ... test
****************************************************************************************/
void testBistDsp(void)
{
    //open function testBist() outside while

    //fmcwEnmode(REGISTER, CONTINUE_MODE);

    adcEnable(1, 1);
    adcSampleInit(1, 1);
    adcWnpara(1, 1, 512);
    //set 1/2 down sample.
    //50M Hclk, adc read buffer rate:50M/4.5/2(adc0 and adc1)=5.555M>8.333/2M
    adcFirDownsample(1, DOWN_SAMPLE);
    dataSampleConfig(INTERVALNUM, 126, SAMPLE_POINT, DOWN_SAMPLE, CHIRP_NUM);
    adcBiasSetting(8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8);
    adcClkOutput();

    ////set adc is free runing mode
    //DSP->ds_ctrl = 1;

    adcSampleStart(ADC01SEL, SAMPLE_POINT, CHIRP_NUM, ADC0_FIFO, ADC0_ADDRE, ADC1_FIFO, ADC1_ADDRE);
    //save .\Data_process\Keilsave\adc0_data.hex 0x20008000,0x20018000
    //save .\Data_process\Keilsave\adc1_data.hex 0x20018000,0x20028000
    fftProcess(512, CHIRP_NUM, 0x20008000, 0x20008000, 4, 4);
    //save .\Data_process\Keilsave\fft0_data.hex 0x20008000,0x20018000
}
void test20GInputBist(void)
{
    //test20GFmcw();
    test20GSingleFequence();
    //set fmcw pll to doubler
    fmcwModeSel(MODE_EXTERN);

    //enable 20G and doubler
    rfbuff20G(1, 1);
    //enable buffer1to4
    rfBuff77GEnable(1, 1, 1, 1, 1);
    //enable bias for 20G(fmcw buffer), doubler and buffer1to4
    bias20GTo80G(8, 8, 8, 8, 8, 8, 8, 8);
    //enable pa1 and pa0
    txPaEnable(1, 1);
    txPaBias(8, 8);

    //bist on amplifier
    bistOn(BIST_AMP, BIST_DIV1024_SEL);

    rxLnaEnable(1, 1);
    mixerEnable(1, 1);
    lnaRegularBias(1, 1);
    lnaDacBias(8, 8, 8, 8, 8, 8);

    //bias2_en: enable signal for ztc bias for BB
    baseBandBiasSetting(ENABLE);

    //tia config
    rfTiaEnable(ENABLE, ENABLE);
    rfTiaGainSetting(TIA_GAIN_0DB, TIA_GAIN_0DB ,TIA_GAIN_0DB ,TIA_GAIN_0DB);
    rfTiaDacBias(8, 8);
    //pga config
    rfPgaGainCalEnable(0, 0, 0, 0);
    rfPgaGainCalSetting(PGA_CAL_P1, PGA_CAL_P1 ,PGA_CAL_P1 ,PGA_CAL_P1);
    rfVgaEnable(ENABLE, ENABLE);
    rfVgaGainSetting(PGA_GAIN_2DB, PGA_GAIN_2DB ,PGA_GAIN_2DB ,PGA_GAIN_2DB);
    rfVgaDacBias(8, 8);
    //filter config
    rcHpfConfig(1, 1, RC_HPF_150KHZ, RC_HPF_150KHZ);
    tiaHpfConfig(1, 1, TIA_HPF_100KHZ, TIA_HPF_100KHZ);
    pgaLpfConfig(PGA_LPF_2M, PGA_LPF_2M);
    pgaHpfConfig(1, 1, PGA_HPF_25KHZ, PGA_HPF_25KHZ);

    //enable vga en_buffer
    RFC->rfc_vga0_reg |= 1<<9;
    //en_buffer0 contorl if signal output gpio23, 24, 25, 26.
    //en_buffer1 contorl if signal output gpio27, 28, 29, 30.
    //when we test low power consumption, we need to connect p29 with vddrf enable.
    //Normally, p29 is high resistance, vga1 en_buffer will lower the p29 level.
    RFC->rfc_vga1_reg |= 1<<9;

    //???gpio mux to output bist if signal
    GPIO->gpio_p24_config = 0x06;
    GPIO->gpio_p25_config = 0x06;

    //enable 20G atb        (rf atb select 000:pwd) 0x400030d4:8d200
    //RFC->rfc_tx_pa_reg |= 1<<6;
    //enable doubler atb    (rf atb select 001:pwd) 0x400030d4:8d183
    //RFC->rfc_tx_pa_reg |= 1<<7|1<<8;
    //enable buffer1to4 atb (rf atb select 010:pwd) 0x400030d4:8d140
    //RFC->rfc_tx_pa_reg |= 1<<5|1<<9|1<<8;
    //enable PA1 atb        (rf atb select 00:pwd)  0x400030d4:0d238
    //RFC->rfc_tx_pa_reg |= 1<<3;
    //enable PA0 atb        (rf atb select 00:pwd)  0x400030d4:0d238
    RFC->rfc_tx_pa_reg |= 1<<2;

    //enable pd Bias(din20)
    biasPowerdector(8);
    //enable gpadc
    gpadcEnable(GPADC_IN5);
}
/****************************************************************************************
*Function name : testDsp
*Description   : range fft, velocity fft, cfar, phase, angle ... test
****************************************************************************************/
void testIfInputDsp(void)
{
    volatile int32 cfarFlag=1;
    char cfarRange[SAMPLE_POINT];
    memset(cfarRange, 0 , sizeof(cfarRange));
    //open function test20GSingleFequence() outside while

    //bias2_en: enable signal for ztc bias for BB
    baseBandBiasSetting(ENABLE);

    GPIO->gpio_p24_config = 0x10;
    GPIO->gpio_p25_config = 0x10;

    //en_adc_test
    RFC->rfc_vga0_reg |= 1<<10;
    RFC->rfc_vga1_reg |= 1<<10;

    adcEnable(1, 1);
    adcSampleInit(1, 1);
    adcWnpara(1, 1, 512);
    //set 1/2 down sample.
    //50M Hclk, adc read buffer rate:50M/4.5/2(adc0 and adc1)=5.555M>8.333/2M
    //if you want to set 1/2 down sample and bypass fir, must be set the fir factor
    //after bypass fir, the downsample is 0
    adcFirDownsample(0, DOWN_SAMPLE);
    dataSampleConfig(INTERVALNUM, 126, SAMPLE_POINT, DOWN_SAMPLE, CHIRP_NUM);
    adcBiasSetting(8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8);
    adcClkOutput();

    adcSampleStart(ADC01SEL, SAMPLE_POINT, CHIRP_NUM, ADC0_FIFO, ADC0_ADDRE, ADC1_FIFO, ADC1_ADDRE);
    //save .\Data_process\Keilsave\adc0_data.hex 0x20008000,0x20018000
    //save .\Data_process\Keilsave\adc1_data.hex 0x20018000,0x20028000
    fftProcess(512, CHIRP_NUM, 0x20008000, 0x20008000, 4, 4);
    //save .\Data_process\Keilsave\fft0_data.hex 0x20008000,0x20018000
    absAdt3102(512, 32, 0x20008000, 0x20008000);

    cfarFlag = cfarSum(8, 1, 0x20008000, 512, 80, cfarRange);
    if(cfarFlag == 0)
    {
        printf("[cfarSum]Find the target.\r\n");
        for(int32 detIndex=0; detIndex<sizeof(cfarRange); detIndex++)
        {
            if(cfarRange[detIndex] == 1)
            {
                printf("target point number:%d\r\n", detIndex);
            }
        }
    } else {
        printf("No target was found.\r\n");
    }
    cfarFlag = 1;
    memset(cfarRange, 0 , sizeof(cfarRange));
    cfarFlag = cfarOrd(8, 1, 0x20008000, 512, 6000, 1, cfarRange);
    if(cfarFlag == 0)
    {
        printf("[cfarOrd]Find the target.\r\n");
        for(int32 detIndex=0; detIndex<sizeof(cfarRange); detIndex++)
        {
            if(cfarRange[detIndex] == 1)
            {
                printf("target point number:%d\r\n", detIndex);
            }
        }
    } else {
        printf("No target was found.\r\n");
    }
}

/****************************************************************************************
*Function name : testDsp
*Description   : range fft, velocity fft, cfar, phase, angle ... test
****************************************************************************************/
void testIfInputDspFreeRun(void)
{
    //50M AHB Bus(32 bit), DMA efficiency is 4.5 cycle
    //adc buffer read rate is 50M/4.5/2(adc0 and adc1)=5.555M<8.3M
    //so we have to use 125M
    delayUs(1000, HIGH_CLOCK);
    switch_to_plli();
    delayUs(1000, HIGH_CLOCK_125M);
    volatile int32 cfarFlag=1;
    char cfarRange[SAMPLE_POINT];
    memset(cfarRange, 0 , sizeof(cfarRange));
    //open function test20GSingleFequence() outside while

    //bias2_en: enable signal for ztc bias for BB
    baseBandBiasSetting(ENABLE);

    GPIO->gpio_p24_config = 0x10;
    GPIO->gpio_p25_config = 0x10;

    //en_adc_test
    RFC->rfc_vga0_reg |= 1<<10;
    RFC->rfc_vga1_reg |= 1<<10;

    adcEnable(1, 1);
    adcSampleInit(1, 1);
    adcWnpara(1, 1, 512);
    //after bypass fir, the downsample is 0
    adcFirDownsample(1, DOWN_SAMPLE);
    //adcStartValidEnd(63);
    adcBiasSetting(8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8);
    adcClkOutput();

    //set adc is free runing mode
    DSP->ds_ctrl = 1;

    adcSampleStart(ADC01SEL, SAMPLE_POINT, CHIRP_NUM, ADC0_FIFO, ADC0_ADDRE, ADC1_FIFO, ADC1_ADDRE);
    //save .\Data_process\Keilsave\adc0_data.hex 0x20008000,0x20018000
    //save .\Data_process\Keilsave\adc1_data.hex 0x20018000,0x20028000
    fftProcess(512, CHIRP_NUM, 0x20008000, 0x20008000, 4, 4);
    //save .\Data_process\Keilsave\fft0_data.hex 0x20008000,0x20018000
    absAdt3102(512, 32, 0x20008000, 0x20008000);

    cfarFlag = cfarSum(8, 1, 0x20008000, 512, 50, cfarRange);
    if(cfarFlag == 0)
    {
        printf("[cfarSum]Find the target.\r\n");
        for(int32 detIndex=0; detIndex<sizeof(cfarRange); detIndex++)
        {
            if(cfarRange[detIndex] == 1)
            {
                printf("target point number:%d\r\n", detIndex);
            }
        }
    } else {
        printf("No target was found.\r\n");
    }
    cfarFlag = 1;
    memset(cfarRange, 0 , sizeof(cfarRange));
    cfarFlag = cfarOrd(8, 1, 0x20008000, 512, 6000, 1, cfarRange);
    if(cfarFlag == 0)
    {
        printf("[cfarOrd]Find the target.\r\n");
        for(int32 detIndex=0; detIndex<sizeof(cfarRange); detIndex++)
        {
            if(cfarRange[detIndex] == 1)
            {
                printf("target point number:%d\r\n", detIndex);
            }
        }
    } else {
        printf("No target was found.\r\n");
    }
}
/****************************************************************************************
*Function name : testDsp
*Description   : range fft, velocity fft, cfar, phase, angle ... test
****************************************************************************************/
void testAdc16mHclk125M(void)
{
    //open function test20GSingleFequence() outside while
    delayUs(1000, HIGH_CLOCK);
    switch_to_plli();
    delayUs(1000, HIGH_CLOCK_125M);
    hclkOutput();
    //bias2_en: enable signal for ztc bias for BB
    baseBandBiasSetting(ENABLE);

    GPIO->gpio_p24_config = 0x10;
    GPIO->gpio_p25_config = 0x10;

    //en_adc_test
    RFC->rfc_vga0_reg |= 1<<10;
    RFC->rfc_vga1_reg |= 1<<10;

    adcEnable(1, 1);
    adcSampleInit(1, 1);
    adcWnpara(1, 1, 512);
    adcFirDownsample(0, DOWN_SAMPLE);
    dataSampleConfig(INTERVALNUM, 126, SAMPLE_POINT, DOWN_SAMPLE, CHIRP_NUM);
    adcBiasSetting(8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8);
    adcClkSwitch(ADC_8P3M);
    adcClkOutput();

    adcSampleStart(ADC01SEL, SAMPLE_POINT, CHIRP_NUM, ADC0_FIFO, ADC0_ADDRE, ADC1_FIFO, ADC1_ADDRE);
    //save .\Data_process\Keilsave\adc0_data.hex 0x20008000,0x20018000
    //save .\Data_process\Keilsave\adc1_data.hex 0x20018000,0x20028000
    fftProcess(512, CHIRP_NUM, 0x20008000, 0x20008000, 4, 4);
    //save .\Data_process\Keilsave\fft0_data.hex 0x20008000,0x20018000
}
/****************************************************************************************
*Function name : testTemprature
*Description   : testTemprature test
****************************************************************************************/
void testTemprature(void)
{
    test20GSingleFequence();

    gpadcEnable(GPADC_AIO1);
    baseBandBiasSetting(ENABLE);
    biasGpadc(8, 8, 8);
    temperatureCompensation(TEMP20UA, KC1_X1PT, KC2_X1PT);
}
/****************************************************************************************
*Function name : testSingleFequence
*Description   : single fequence test
****************************************************************************************/
void test80GSingleFequence(void)
{
    //test20GFmcw();
    test20GSingleFequence();
    //set fmcw pll to doubler
    fmcwModeSel(MODE_INTER);

    rfbuff20G(ENABLE, ENABLE);
    bias20GTo80G(8, 8, 8, 8, 8, 8, 8, 8);
    rfBuff77GEnable(ENABLE, ENABLE, ENABLE, ENABLE, ENABLE);
    txPaEnable(ENABLE, ENABLE);
    txPaBias(8, 8);

    plliLock();
    pllnLock(76, 0);

    //output 50M clock
    RFC->rfc_plli_ctrl_reg |= 1<<3;
    PMU->pmu_pmu_ctrl_reg |= pmu_pmu_ctrl_reg_manu_ctrl_en_bit;
    delayUs(100,HIGH_CLOCK);
    PMU->pmu_pmu_ctrl_reg &= ~pmu_pmu_ctrl_reg_manu_ctrl_en_bit;

    fmcwConfig(76, 0, CHIRP_T0, CHIRP_T1, CHIRP_T2, CHIRP_NUM);
    fmcwEnmode(REGISTER, CONTINUE_MODE);

    //frequecyCounter(0, 0x20028000);
    
    rxLnaEnable(1, 1);
    mixerEnable(1, 1);
    lnaRegularBias(1, 1);
    lnaDacBias(8, 8, 8, 8, 8, 8);

    //bias2_en: enable signal for ztc bias for BB
    baseBandBiasSetting(ENABLE);

    //tia config
    rfTiaEnable(ENABLE, ENABLE);
    rfTiaGainSetting(TIA_GAIN_0DB, TIA_GAIN_0DB ,TIA_GAIN_0DB ,TIA_GAIN_0DB);
    rfTiaDacBias(8, 8);
    //pga config
    rfPgaGainCalEnable(0, 0, 0, 0);
    rfPgaGainCalSetting(PGA_CAL_P1, PGA_CAL_P1 ,PGA_CAL_P1 ,PGA_CAL_P1);
    rfVgaEnable(ENABLE, ENABLE);
    rfVgaGainSetting(PGA_GAIN_4DB, PGA_GAIN_4DB ,PGA_GAIN_4DB ,PGA_GAIN_4DB);
    rfVgaDacBias(8, 8);
    //filter config
    rcHpfConfig(0, 0, RC_HPF_400KHZ, RC_HPF_400KHZ);
    tiaHpfConfig(1, 1, TIA_HPF_25KHZ, TIA_HPF_25KHZ);
    pgaLpfConfig(PGA_LPF_2M, PGA_LPF_2M);
    pgaHpfConfig(1, 1, PGA_HPF_25KHZ, PGA_HPF_25KHZ);

    //enable vga en_buffer
    RFC->rfc_vga0_reg |= 1<<9;
    //en_buffer0 contorl if signal output gpio23, 24, 25, 26.
    //en_buffer1 contorl if signal output gpio27, 28, 29, 30.
    //when we test low power consumption, we need to connect p29 with vddrf enable.
    //Normally, p29 is high resistance, vga1 en_buffer will lower the p29 level.
    RFC->rfc_vga1_reg |= 1<<9;
    //enable pd Bias(din20)
    biasPowerdector(8);
    //enable gpadc
    gpadcEnable(GPADC_IN5);
    //enable PA1 atb        (rf atb select 00:pwd)  0x400030d4:0d238
    RFC->rfc_tx_pa_reg |= 1<<3;
    delayUs(100000, HIGH_CLOCK);
    printf("pa1 pd:%d\r\n", (*(uint32*)0x400030d4)&0xff);
    RFC->rfc_tx_pa_reg &= ~(1<<3);
    //enable PA0 atb        (rf atb select 00:pwd)  0x400030d4:0d238
    RFC->rfc_tx_pa_reg |= 1<<2;
    delayUs(100000, HIGH_CLOCK);
    printf("pa0 pd:%d\r\n", (*(uint32*)0x400030d4)&0xff);
    RFC->rfc_tx_pa_reg &= ~(1<<2);

//    delayUs(5000000,HIGH_CLOCK);
//    txPaBias(0, 8);
//    rfVgaGainSetting(PGA_GAIN_2DB, PGA_GAIN_2DB ,PGA_GAIN_2DB ,PGA_GAIN_2DB);
}
void test80GSingleFequenceDut(void)
{
    //test20GFmcw();
    test20GSingleFequence();
    //set fmcw pll to doubler
    fmcwModeSel(MODE_INTER);

    rfbuff20G(ENABLE, ENABLE);
    bias20GTo80G(8, 8, 8, 8, 8, 8, 8, 8);
    rfBuff77GEnable(ENABLE, ENABLE, ENABLE, ENABLE, ENABLE);
    txPaEnable(ENABLE, ENABLE);
    txPaBias(8, 8);

    plliLock();
    pllnLock(76.0005, 0);

    //output 50M clock
//    RFC->rfc_plli_ctrl_reg |= 1<<3;
//    PMU->pmu_pmu_ctrl_reg |= pmu_pmu_ctrl_reg_manu_ctrl_en_bit;
//    delayUs(100,HIGH_CLOCK);
//    PMU->pmu_pmu_ctrl_reg &= ~pmu_pmu_ctrl_reg_manu_ctrl_en_bit;

    fmcwConfig(76.0005, 0, CHIRP_T0, CHIRP_T1, CHIRP_T2, CHIRP_NUM);
    fmcwEnmode(REGISTER, CONTINUE_MODE);

    rxLnaEnable(1, 1);
    mixerEnable(1, 1);
    lnaRegularBias(1, 1);
    lnaDacBias(8, 8, 8, 8, 8, 8);

    //bias2_en: enable signal for ztc bias for BB
    baseBandBiasSetting(ENABLE);

    //tia config
    rfTiaEnable(ENABLE, ENABLE);
    rfTiaGainSetting(TIA_GAIN_0DB, TIA_GAIN_0DB ,TIA_GAIN_0DB ,TIA_GAIN_0DB);
    rfTiaDacBias(8, 8);
    //pga config
    rfPgaGainCalEnable(0, 0, 0, 0);
    rfPgaGainCalSetting(PGA_CAL_P1, PGA_CAL_P1 ,PGA_CAL_P1 ,PGA_CAL_P1);
    rfVgaEnable(ENABLE, ENABLE);
    rfVgaGainSetting(PGA_GAIN_4DB, PGA_GAIN_4DB ,PGA_GAIN_4DB ,PGA_GAIN_4DB);
    rfVgaDacBias(8, 8);
    //filter config
    rcHpfConfig(0, 0, RC_HPF_400KHZ, RC_HPF_400KHZ);
    tiaHpfConfig(1, 1, TIA_HPF_25KHZ, TIA_HPF_25KHZ);
    pgaLpfConfig(PGA_LPF_2M, PGA_LPF_2M);
    pgaHpfConfig(1, 1, PGA_HPF_25KHZ, PGA_HPF_25KHZ);

    //enable vga en_buffer
    RFC->rfc_vga0_reg |= 1<<9;
    //en_buffer0 contorl if signal output gpio23, 24, 25, 26.
    //en_buffer1 contorl if signal output gpio27, 28, 29, 30.
    //when we test low power consumption, we need to connect p29 with vddrf enable.
    //Normally, p29 is high resistance, vga1 en_buffer will lower the p29 level.
    RFC->rfc_vga1_reg |= 1<<9;

//    delayUs(5000000,HIGH_CLOCK);
//    txPaBias(0, 8);
//    rfVgaGainSetting(PGA_GAIN_2DB, PGA_GAIN_2DB ,PGA_GAIN_2DB ,PGA_GAIN_2DB);
}
/****************************************************************************************
*Function name : testSingleFequence
*Description   : single fequence test
****************************************************************************************/
void test80GSingleFequenceMaster(void)
{
    switch_to_50m();

    //power on
    testPower();

    //rf config
    plliDacSetting();
    plliConfig(ENABLE);
    pllnDacSetting();
    pllnConfig(ENABLE);

    fmcwModeSel(MODE_INTER);
    rfbuff20G(ENABLE, ENABLE);
    bias20GTo80G(8, 8, 8, 8, 8, 8, 8, 8);
    rfBuff77GEnable(ENABLE, ENABLE, ENABLE, ENABLE, ENABLE);
    txPaEnable(ENABLE, ENABLE);
    txPaBias(5, 0);
    rfPhaseShift(0x1f, 0, 0x1f, 0);

    plliLock();
    pllnLock(76.0017, 0);

//    RFC->rfc_plli_ctrl_reg |= 1<<6;
//    PMU->pmu_pmu_ctrl_reg |= pmu_pmu_ctrl_reg_manu_ctrl_en_bit;
//    delayUs(100,HIGH_CLOCK);
//    PMU->pmu_pmu_ctrl_reg &= ~pmu_pmu_ctrl_reg_manu_ctrl_en_bit;

    fmcwConfig(76.0017, 0, CHIRP_T0, CHIRP_T1, CHIRP_T2, CHIRP_NUM);
    fmcwEnmode(REGISTER, CONTINUE_MODE);

    rxLnaEnable(1, 1);
    mixerEnable(1, 1);
    lnaRegularBias(1, 1);
    lnaDacBias(8, 8, 8, 8, 8, 8);

    //bias2_en: enable signal for ztc bias for BB
    baseBandBiasSetting(ENABLE);

    //tia config
    rfTiaEnable(ENABLE, ENABLE);
    rfTiaGainSetting(TIA_GAIN_6DB, TIA_GAIN_6DB ,TIA_GAIN_6DB ,TIA_GAIN_6DB);
    rfTiaDacBias(8, 8);
    //pga config
    rfPgaGainCalEnable(0, 0, 0, 0);
    rfPgaGainCalSetting(PGA_CAL_P1, PGA_CAL_P1 ,PGA_CAL_P1 ,PGA_CAL_P1);
    rfVgaEnable(ENABLE, ENABLE);
    rfVgaGainSetting(PGA_GAIN_2DB, PGA_GAIN_2DB ,PGA_GAIN_2DB ,PGA_GAIN_2DB);
    rfVgaDacBias(8, 8);
    //filter config
    rcHpfConfig(1, 1, RC_HPF_150KHZ, RC_HPF_150KHZ);
    tiaHpfConfig(1, 1, TIA_HPF_25KHZ, TIA_HPF_25KHZ);
    pgaLpfConfig(PGA_LPF_8M, PGA_LPF_8M);
    pgaHpfConfig(1, 1, PGA_HPF_25KHZ, PGA_HPF_25KHZ);

//    //enable vga en_buffer
//    RFC->rfc_vga0_reg |= 1<<9;
//    //en_buffer0 contorl if signal output gpio23, 24, 25, 26.
//    //en_buffer1 contorl if signal output gpio27, 28, 29, 30.
//    //when we test low power consumption, we need to connect p29 with vddrf enable.
//    //Normally, p29 is high resistance, vga1 en_buffer will lower the p29 level.
//    RFC->rfc_vga1_reg |= 1<<9;

//    for(int32 i1=0; i1<0x1f; i1++)
//    {
//        for(int32 i2=0; i2<0x1f; i2++)
//        {
//            for(int32 j1=0; j1<0x1f; j1++)
//            {
//                for(int32 j2=0; j2<0x1f; j2++)
//                {
//                    rfPhaseShift(i1, i2, j1, j2);
//                    delayUs(10000, HIGH_CLOCK);
//                }
//
//            }
//        }
//    }


}

/****************************************************************************************
*Function name : testSingleFequenceUcw
*Description   : use use configuration wizard single fequence test
****************************************************************************************/
void testSingleFequenceUcw(void)
{
    switch_to_50m();
    adt3102RfUcw();
    adt3102AnalogUcw();
    adt3102PowerUcw();
}

/****************************************************************************************
*Function name : testFmcw
*Description   : fmcw test
****************************************************************************************/
void test80GFmcw(void)
{
    switch_to_50m();

    //power on
    testPower();

    //rf config
    plliDacSetting();
    plliConfig(ENABLE);
    pllnDacSetting();
    pllnConfig(ENABLE);
    fmcwModeSel(MODE_INTER);
    rfbuff20G(ENABLE, ENABLE);
    bias20GTo80G(8, 8, 8, 8, 8, 8, 8, 8);
    rfBuff77GEnable(ENABLE, ENABLE, ENABLE, ENABLE, ENABLE);
    txPaEnable(ENABLE, ENABLE);
    txPaBias(8, 8);
    rfPhaseShift(0x1f, 0, 0x1f, 0);
    plliLock();
    pllnLock(FSTART, FM);
    fmcwConfig(FSTART, FM, CHIRP_T0, CHIRP_T1, CHIRP_T2, CHIRP_NUM);

    //enable pa1 and pa0
    txPaEnable(1, 1);
    txPaBias(8, 8);

    rxLnaEnable(1, 1);
    mixerEnable(1, 1);
    lnaRegularBias(1, 1);
    lnaDacBias(8, 8, 8, 8, 8, 8);

    //bias2_en: enable signal for ztc bias for BB
    baseBandBiasSetting(ENABLE);

    //tia config
    rfTiaEnable(ENABLE, ENABLE);
    rfTiaGainSetting(TIA_GAIN_12DB, TIA_GAIN_12DB ,TIA_GAIN_12DB ,TIA_GAIN_12DB);
    rfTiaDacBias(8, 8);
    //pga config
    rfPgaGainCalEnable(ENABLE, ENABLE, ENABLE, ENABLE);
    rfPgaGainCalSetting(PGA_CAL_P1, PGA_CAL_P1 ,PGA_CAL_P1 ,PGA_CAL_P1);
    rfVgaEnable(ENABLE, ENABLE);
    rfVgaGainSetting(PGA_GAIN_4DB, PGA_GAIN_4DB ,PGA_GAIN_4DB ,PGA_GAIN_4DB);
    rfVgaDacBias(8, 8);
    //filter config
    tiaHpfConfig(1, 1, TIA_HPF_300KHZ, TIA_HPF_300KHZ);
    rcHpfConfig(0, 0, RC_HPF_150KHZ, RC_HPF_150KHZ);
    pgaLpfConfig(PGA_LPF_8M, PGA_LPF_8M);
    pgaLpfCal(0, 0, 3, 3);
    pgaHpfConfig(1, 1, PGA_HPF_25KHZ, PGA_HPF_25KHZ);

    //enable vga en_buffer
    RFC->rfc_vga0_reg |= 1<<9;
    //en_buffer0 contorl if signal output gpio23, 24, 25, 26.
    //en_buffer1 contorl if signal output gpio27, 28, 29, 30.
    //when we test low power consumption, we need to connect p29 with vddrf enable.
    //Normally, p29 is high resistance, vga1 en_buffer will lower the p29 level.
    RFC->rfc_vga1_reg |= 1<<9;

    //fmcwEnmode(REGISTER, CONTINUE_MODE);
//    for(int32 i1=0; i1<0x1f; i1++)
//    {
//        for(int32 i2=0; i2<0x1f; i2++)
//        {
//            for(int32 j1=0; j1<0x1f; j1++)
//            {
//                for(int32 j2=0; j2<0x1f; j2++)
//                {
//                    rfPhaseShift(i1, i2, j1, j2);
//                    delayUs(10000, HIGH_CLOCK);
//                }
//
//            }
//        }
//    }

}
/****************************************************************************************
*Function name : testAdcSample
*Description   : adc sample, analog test
****************************************************************************************/
void testAdcSample(void)
{
    switch_to_50m();
    test80GFmcw();
    adt3102AdcSetting(1, 1, 1,ADC_SAMPLE_RATE, CHIRP_NUM);
    fmcwConfig(FSTART, FM, CHIRP_T0, CHIRP_T1, CHIRP_T2, CHIRP_NUM);
    adcSampleStart(ADC01SEL, SAMPLE_POINT, CHIRP_NUM, ADC0_FIFO, ADC0_ADDRE, ADC1_FIFO, ADC1_ADDRE);

    fftProcess(512, CHIRP_NUM, 0x20008000, 0x20008000, 4, 4);
}


/****************************************************************************************
*Function name : testHighClock50or125M
*Description   : 50M to 125M
****************************************************************************************/
void testHighClock50or125M(void)
{
    switch_to_plli();
}


void testLowPowerConsumption(void)
{




}

void mcu_sleep_test_hangzhou(int32 ram_on)
{

    mcuSleepConfig(ram_on);

//--- put p02~p30 to highz.
//uart0. *(uint32*)0x40020030 = 0x10;
//uart0. *(uint32*)0x40020034 = 0x10;
//    *(uint32*)0x40020038 = 0x10;
//    *(uint32*)0x4002003c = 0x10;
//    *(uint32*)0x40020040 = 0x10;
//    *(uint32*)0x40020044 = 0x10;
//    *(uint32*)0x40020048 = 0x10;
//    *(uint32*)0x4002004c = 0x10;
//    *(uint32*)0x40020050 = 0x10;
//    *(uint32*)0x40020054 = 0x10;

//    *(uint32*)0x40020058 = 0x10;
//    *(uint32*)0x4002005c = 0x10;
//    //boot. *(uint32*)0x40020060 = 0x10;
//    *(uint32*)0x40020064 = 0x10;
//    *(uint32*)0x40020068 = 0x10;
//    *(uint32*)0x4002006c = 0x10;
//    *(uint32*)0x40020070 = 0x10;
//    *(uint32*)0x40020074 = 0x10;
//    *(uint32*)0x40020078 = 0x10;
//    //boot. *(uint32*)0x4002007c = 0x10;

//    *(uint32*)0x40020080 = 0x10;
//    *(uint32*)0x40020084 = 0x10;
//    *(uint32*)0x40020088 = 0x10;
//    *(uint32*)0x4002008c = 0x10;
//    *(uint32*)0x40020090 = 0x10;
//    *(uint32*)0x40020094 = 0x10;
//    *(uint32*)0x40020098 = 0x10;
//    *(uint32*)0x4002009c = 0x10;
//    *(uint32*)0x400200a0 = 0x10;
//    //p29, external_ldo_en. *(uint32*)0x400200a4 = 0x10;

//    *(uint32*)0x400200a8 = 0x10;
    *(uint32*)0x40020038 = 5;
    PMU->pmu_clk_gate_reg |=1<<10;

    //soc_NVIC[soc_SETENA] = 0x00; //disable all interrupt.
    //soc_NVIC[soc_SETENA] = 0xffffffff; //enable all interrupt.
    soc_NVIC[soc_SETENA] = 0x080 ; //only PMU interrupt.

    //--- set gpio4 as wake up pad.
    *(uint32*)0x40020040 = 0x30;  // set as input.
    *(uint32*)0x40020018 = 1<<4;  // wake up enable.
    *(uint32*)0x40020014 = 0x00;  // high to wake up.

    //--- test wakeup time.
    //gpioDirection(7,0);
    //gpioOutput(7,0);


//    //--- MCU is going to sleep.
//    *(uint32*)0xe000ed10 = 0x04;
//    __asm("wfi");
    mcuGotoSleep();

    //--- test wakeup time.
    //gpioOutput(7,1);

    printf("sleep wake up\r\n ");
    //--- Now, MCU is waken up.  and you will see P03 as a square wave output.
    *(uint32*)0x4002003C = 0x00;  // set p03 as output.
    while(1) {
        for(int32 i=0; i<10000; i++) {
            *(uint32*)0x40020020 = 1<<3;  // set p03 to 1.
        }

        for(int32 i=0; i<10000; i++) {
            *(uint32*)0x40020024 = 1<<3;  // set p03 to 0.
        }
    }

}

/****************************************************************************************
*Function name : testSingleFequence
*Description   : single fequence test
****************************************************************************************/
void test20GSingleFequenceUart(void)
{
    uint8 num=0;
    uint8 rwbuff[17];
    memset(&rwbuff, 0, sizeof(rwbuff));
    uint8 data;
    uint32 address=0,Value=0;

    switch_to_50m();

    volatile float vtuneValue;
    //power on
    testPower();
    //rf config
    plliDacSetting();
    plliConfig(ENABLE);
    pllnDacSetting();
    pllnConfig(ENABLE);
    fmcwModeSel(MODE_TEST);
    rfbuff20G(ENABLE, DISABLE);
    dacEnBias1Pd(1, 0);
    plliLock();

    pllnLock(76, 0);
    //manuPllnCapArray(0xD);//76G:B  77G:A,B
    fmcwConfig(76, 0, CHIRP_T0, CHIRP_T1, CHIRP_T2, CHIRP_NUM);
    fmcwEnmode(REGISTER, CONTINUE_MODE);

    vtuneValue = vtuneRead();
    printf("%f\r\n", vtuneValue);
    rfPhaseShift(0x1f, 0, 0x1f, 0);

    RFC->rf_spare_reg = 0x01;
    while(1)
    {
        if(0 == Read_RingBuff((uint8 *)&data ))
        {
            rwbuff[num]=data;
            num++;
            if((rwbuff[0]==0x0d)||(rwbuff[0]==0x0A))
            {
                for(int i=0; i<18; i++) {
                    rwbuff[i]=0;
                }
                num =0;
            }
            else if(rwbuff[0]!='r'&&rwbuff[0]!='w')
            {
                printf ("RW ERROR\r\n");
                for(int i=0; i<18; i++) {
                    rwbuff[i]=0;
                }
                num =0;
            } else if((rwbuff[0]=='r'&&rwbuff[1]>='g')||(rwbuff[0]=='r'&&rwbuff[2]>='g')
                      ||(rwbuff[0]=='r'&&rwbuff[3]>='g')||(rwbuff[0]=='r'&&rwbuff[4]>='g')
                      ||(rwbuff[0]=='r'&&rwbuff[5]>='g')||(rwbuff[0]=='r'&&rwbuff[6]>='g')
                      ||(rwbuff[0]=='r'&&rwbuff[7]>='g')||(rwbuff[0]=='r'&&rwbuff[8]>='g')
                      ||(rwbuff[0]=='r'&&rwbuff[9]>='g'))
            {
                printf("E01");
                for(int i=0; i<18; i++) {
                    rwbuff[i]=0;
                }
                num =0;

            }
            else if((rwbuff[0]=='w'&&rwbuff[1]>='g')||(rwbuff[0]=='w'&&rwbuff[2]>='g')
                    ||(rwbuff[0]=='w'&&rwbuff[3]>='g')||(rwbuff[0]=='w'&&rwbuff[4]>='g')
                    ||(rwbuff[0]=='w'&&rwbuff[5]>='g')||(rwbuff[0]=='w'&&rwbuff[6]>='g')
                    ||(rwbuff[0]=='w'&&rwbuff[7]>='g')||(rwbuff[0]=='w'&&rwbuff[8]>='g')
                    ||(rwbuff[0]=='w'&&rwbuff[9]>='g')||(rwbuff[0]=='w'&&rwbuff[10]>='g')
                    ||(rwbuff[0]=='w'&&rwbuff[11]>='g')||(rwbuff[0]=='w'&&rwbuff[12]>='g')
                    ||(rwbuff[0]=='w'&&rwbuff[13]>='g')||(rwbuff[0]=='w'&&rwbuff[14]>='g')
                    ||(rwbuff[0]=='w'&&rwbuff[15]>='g')||(rwbuff[0]=='w'&&rwbuff[16]>='g'))
            {
                for(int i=0; i<18; i++) {
                    rwbuff[i]=0;
                }
                num =0;
                printf("E02");
            }
            else if(rwbuff[0] == 'r'&&num ==9)
            {
                for(int i=1; i<9; i++)
                {
                    uint8 temp =CharToHex (rwbuff[i]);
                    address = (address<<(4))+temp ;
                }
                //printf("Read Register \r\n");
                //printf("address = 0x%x \r\n",address);
                //printf ("The value is 0x%x\r\n",*(uint32*)address);
                printf ("[R][0x%08x][0x%08x]\r\n",address,*(uint32*)address);
                for(int i=0; i<18; i++) {
                    rwbuff[i]=0;
                }
                num =0;
            } else if(rwbuff[0] == 'w'&&num ==17)
            {
                for(int i=1; i<9; i++)
                {
                    uint8 temp =CharToHex (rwbuff[i]);
                    address = (address<<(4))+temp ;
                }
                for(int i=9; i<17; i++)
                {
                    uint8 temp1 =CharToHex (rwbuff[i]);
                    Value = (Value<<(4))+temp1 ;
                }
                *(uint32*)address=Value;
                //printf("Write Register \r\n");
                //printf("address = 0x%x \r\n",address);
                //printf("Value = 0x%x \r\n",Value);
                printf ("[W][0x%08x][0x%08x]\r\n",address,*(uint32*)address);
                for(int i=0; i<18; i++) {
                    rwbuff[i]=0;
                }
                num =0;
            }
        }
        fmcwEnmode(REGISTER, CONTINUE_MODE);
        rfPhaseShift(0x1f, 0, 0x1f, 0);
        RFC->rf_spare_reg = 0x01;
    }
}

/****************************************************************************************
*Function name : testSingleFequence
*Description   : single fequence test
****************************************************************************************/
void test80GSingleFequenceRx(void)
{
    //test20GFmcw();
    test20GSingleFequence();
    //set fmcw pll to doubler
    fmcwModeSel(MODE_INTER);

    rfbuff20G(ENABLE, ENABLE);
    bias20GTo80G(8, 8, 8, 8, 8, 8, 8, 8);
    rfBuff77GEnable(ENABLE, ENABLE, ENABLE, ENABLE, ENABLE);
    //txPaEnable(ENABLE, ENABLE);
    //txPaBias(8, 8);

    plliLock();
    pllnLock(76.002, 0);

    //output 50M clock
    RFC->rfc_plli_ctrl_reg |= 1<<3;
    PMU->pmu_pmu_ctrl_reg |= pmu_pmu_ctrl_reg_manu_ctrl_en_bit;
    delayUs(100,HIGH_CLOCK);
    PMU->pmu_pmu_ctrl_reg &= ~pmu_pmu_ctrl_reg_manu_ctrl_en_bit;

    fmcwConfig(76.002, 0, CHIRP_T0, CHIRP_T1, CHIRP_T2, CHIRP_NUM);
    fmcwEnmode(REGISTER, CONTINUE_MODE);

    rxLnaEnable(1, 1);
    mixerEnable(1, 1);
    lnaRegularBias(1, 1);
    lnaDacBias(8, 8, 8, 8, 8, 8);

    //bias2_en: enable signal for ztc bias for BB
    baseBandBiasSetting(ENABLE);

    //tia config
    rfTiaEnable(ENABLE, ENABLE);
    rfTiaGainSetting(TIA_GAIN_6DB, TIA_GAIN_6DB ,TIA_GAIN_6DB ,TIA_GAIN_6DB);
    rfTiaDacBias(8, 8);
    //pga config
    rfPgaGainCalEnable(0, 0, 0, 0);
    rfPgaGainCalSetting(PGA_CAL_P1, PGA_CAL_P1 ,PGA_CAL_P1 ,PGA_CAL_P1);
    rfVgaEnable(ENABLE, ENABLE);
    rfVgaGainSetting(PGA_GAIN_10DB, PGA_GAIN_10DB ,PGA_GAIN_10DB ,PGA_GAIN_10DB);
    rfVgaDacBias(8, 8);
    //filter config
    rcHpfConfig(0, 0, RC_HPF_400KHZ, RC_HPF_400KHZ);
    tiaHpfConfig(1, 1, TIA_HPF_25KHZ, TIA_HPF_25KHZ);
    pgaLpfConfig(PGA_LPF_2M, PGA_LPF_2M);
    pgaHpfConfig(1, 1, PGA_HPF_25KHZ, PGA_HPF_25KHZ);

    //enable vga en_buffer
    RFC->rfc_vga0_reg |= 1<<9;
    //en_buffer0 contorl if signal output gpio23, 24, 25, 26.
    //en_buffer1 contorl if signal output gpio27, 28, 29, 30.
    //when we test low power consumption, we need to connect p29 with vddrf enable.
    //Normally, p29 is high resistance, vga1 en_buffer will lower the p29 level.
    RFC->rfc_vga1_reg |= 1<<9;

//    delayUs(5000000,HIGH_CLOCK);
//    txPaBias(0, 8);
//    rfVgaGainSetting(PGA_GAIN_2DB, PGA_GAIN_2DB ,PGA_GAIN_2DB ,PGA_GAIN_2DB);
}







