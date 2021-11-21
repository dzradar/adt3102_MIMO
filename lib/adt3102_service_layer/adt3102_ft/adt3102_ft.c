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
//$Revision: 
//$Data: 2021.5.21
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
#include "adt3102_hangzhou_test.h"
#include "adt3102_memscheduling.h"
#include "dsp_ctype_map.h"
#include "rfc_ctype_map.h"
#include "gpio_ctype_map.h"
#include "pmu_ctype_map.h"
#include "dma_ctype_map.h"
#include "use_configuration_wizard.h"
#include "TinyFrame.h"
#include "adt3102_ft.h"
#include "cirqueue.h"
//------Global variable declaration
volatile uint32 fttestIndex __attribute__ ((at(0x0000fff0)));
extern uint8 g_rwbuff[17]; 
extern uint8 g_num; 
void ftTest(void)
{
    MY_NVIC_SetVectorTable(0x00000000, 0x8000); 
    //-----Local variable declaration-----
    fttestIndex = 0;
    volatile uint32 fttestIndexLast = 0;
    memset(&g_rwbuff, 0, sizeof(g_rwbuff));
    g_num = 0;
    //-----Initialize functions and variables and simulation part-----
    setInterrupt(INT_UART0, ENABLEINT);
    setInterrupt(INT_FFT, ENABLEINT);
    setInterrupt(INT_DUALTIMER, ENABLEINT);
    setInterrupt(INT_RTC, ENABLEINT);
    setInterrupt(INT_TRNG, ENABLEINT);
    uartInit(UART0,115200, HIGH_CLOCK);
    uartInit(UART1,115200, HIGH_CLOCK);
    RingBuff_Init();
    //-----init some module-----
    printf("[hangzhou]adt3102 basic project test mode.\r\n");
    switch_to_50m();
    //-------------------Test debug------------------------------------------------
    //fttestVdd12(); 
    //fttestPlli();
    //fttest50Moscillator();
    //fttestPlln();
    //fttestBaseband();
    //fttestLPvdd12();
    //fttestRc32K();
    //fttestRfBias();
    //fttestPlliPllnBias();
    //fttestPlliPllnBiasVcoBuff();
    //fttestRfcurrent();
    //fttestBBbiasCurrent();
    //fttestPlliLock();
    //fttestPllnLock();
    fttestChannelPd();
    //fttestgpadcIN1();
    //fttestVgaBist();
    //fttestDut();
    //fttestMaster();
    //fttestAdc();
    //mbistMaster();
    
    while(1){}
    //-------------------Test debug------------------------------------------------
    
    while(1)
    {
        uartWRegister();
        if(fttestIndex != fttestIndexLast)
        {
            switch (fttestIndex)
            {
                //reset
                case 0x5a:
                    system_reset();
                    break;
                case 0x221:
                    fttestIndexLast = fttestIndex;
                    delayUs(1000,HIGH_CLOCK);
                    dvdd12HpLDO(POWER_ON, DVDD1P2_HP);
                    break;
                case 0x222:
                    fttestIndexLast = fttestIndex;
                    dvdd12HpLDO(POWER_ON, DVDD1P1_HP);
                    break;
                case 0x223:
                    fttestIndexLast = fttestIndex;
                    dvdd12HpLDO(POWER_ON, DVDD1P3_HP);
                    break;
                case 0x224:
                    fttestIndexLast = fttestIndex;
                    dvdd12HpLDO(POWER_ON, DVDD1P4_HP);
                    break;
                case 0x231:
                    fttestIndexLast = fttestIndex;
                    plliLDOEnable(ENABLE, ENABLE);
                    //plli pll 1.8v
                    uint32 regVel;
                    regVel = RFC->rf_plli_ldo_cfg_reg;
                    regVel &= ~(0x3<<4);
                    regVel |= (0<<4);
                    RFC->rf_plli_ldo_cfg_reg = regVel;
                    break;
                case 0x232:
                    fttestIndexLast = fttestIndex;
                    plliLDOEnable(ENABLE, ENABLE);
                    //plli pll 1.5v
                    regVel |= RFC->rf_plli_ldo_cfg_reg;
                    regVel &= ~(0x3<4);
                    regVel |= (1<<4);
                    RFC->rf_plli_ldo_cfg_reg = regVel;
                    break;                
                case 0x233:
                    fttestIndexLast = fttestIndex;
                    plliLDOEnable(ENABLE, ENABLE);
                    //plli pll 2.2v
                    regVel |= RFC->rf_plli_ldo_cfg_reg;
                    regVel &= ~(0x3<<4);
                    regVel |= (2<<4);
                    RFC->rf_plli_ldo_cfg_reg = regVel;
                    break;
                case 0x234:
                    fttestIndexLast = fttestIndex;
                    plliLDOEnable(ENABLE, ENABLE);
                    //plli pll 2.5v
                    regVel |= RFC->rf_plli_ldo_cfg_reg;
                    regVel &= ~(0x3<<4);
                    regVel |= (3<<4);
                    RFC->rf_plli_ldo_cfg_reg = regVel;
                    break;
                case 0x235:
                    fttestIndexLast = fttestIndex;
                    plliLDOEnable(ENABLE, ENABLE);
                    //plli vco 1.8v
                    regVel |= RFC->rf_plli_ldo_cfg_reg;
                    regVel &= ~(0x3<<0);
                    regVel |= (0<<0);
                    RFC->rf_plli_ldo_cfg_reg = regVel;
                    break;
                case 0x236:
                    fttestIndexLast = fttestIndex;
                    plliLDOEnable(ENABLE, ENABLE);
                    //plli pll 2.0v
                    regVel |= RFC->rf_plli_ldo_cfg_reg;
                    regVel &= ~(0x3<0);
                    regVel |= (1<<0);
                    RFC->rf_plli_ldo_cfg_reg = regVel;
                    break;
                case 0x237:
                    fttestIndexLast = fttestIndex;
                    plliLDOEnable(ENABLE, ENABLE);
                    //plli pll 2.2v
                    regVel |= RFC->rf_plli_ldo_cfg_reg;
                    regVel &= ~(0x3<<0);
                    regVel |= (2<<0);
                    RFC->rf_plli_ldo_cfg_reg = regVel;
                    break;
                case 0x238:
                    fttestIndexLast = fttestIndex;
                    plliLDOEnable(ENABLE, ENABLE);
                    //plli pll 2.5v
                    regVel |= RFC->rf_plli_ldo_cfg_reg;
                    regVel &= ~(0x3<<0);
                    regVel |= (3<<0);
                    RFC->rf_plli_ldo_cfg_reg = regVel;
                    break;
                case 0x240:
                    fttestIndexLast = fttestIndex;
                    fttest50Moscillator();
                    break;                
                case 0x340:
                    fttestIndexLast = fttestIndex;
                    fttestGpioOutputH();
                    break;                     
                case 0x341:
                    fttestIndexLast = fttestIndex;
                    fttestGpioOutputL();
                    break;                  
                case 0x342:
                    fttestIndexLast = fttestIndex;
                    fttestGpioInput();
                    break;                     
                case 0x421:
                    fttestIndexLast = fttestIndex;
                    //plln vco 1.8v
                    pllnVcoConfig(ENABLE, PLLN_VCO_LDO_1P8);
                    break; 
                case 0x422:
                    fttestIndexLast = fttestIndex;  
                    //plln vco 2.0v
                    pllnVcoConfig(ENABLE, PLLN_VCO_LDO_2P0);
                    break;
                case 0x423:
                    fttestIndexLast = fttestIndex;
                    //plln vco 2.2v
                    pllnVcoConfig(ENABLE, PLLN_VCO_LDO_2P2);
                    break; 
                case 0x424:
                    fttestIndexLast = fttestIndex;  
                    //plln vco 2.5v
                    pllnVcoConfig(ENABLE, PLLN_VCO_LDO_2P5);
                    break; 
                case 0x425:
                    fttestIndexLast = fttestIndex;
                    //plln pll 1.8v
                    pllnPllConfig(ENABLE, PLLN_PLL_LDO_1P8, ENABLE);
                    break; 
                case 0x426:
                    fttestIndexLast = fttestIndex;  
                    //plln pll 2.0v
                    pllnPllConfig(ENABLE, PLLN_PLL_LDO_2P0, ENABLE);
                    break; 
                case 0x427:
                    fttestIndexLast = fttestIndex;
                    //plln pll 2.2v
                    pllnPllConfig(ENABLE, PLLN_PLL_LDO_2P2, ENABLE);
                    break; 
                case 0x428:
                    fttestIndexLast = fttestIndex;
                    //plln pll 2.4v
                    pllnPllConfig(ENABLE, PLLN_PLL_LDO_2P5, ENABLE);
                    break; 
                case 0x431:
                    fttestIndexLast = fttestIndex;
                    //bb1 and bb2 1.8v
                    baseband18LDO(ENABLE, BB_LDO_1P8, ENABLE, BB_LDO_1P8);
                    break; 
                case 0x432:
                    fttestIndexLast = fttestIndex;
                    //bb1 and bb2 2.0v
                    baseband18LDO(ENABLE, BB_LDO_2P0, ENABLE, BB_LDO_2P0);
                    break; 
                case 0x433:
                    fttestIndexLast = fttestIndex;
                    //bb1 and bb2 2.2v
                    baseband18LDO(ENABLE, BB_LDO_2P2, ENABLE, BB_LDO_2P2);
                    break; 
                case 0x434:
                    fttestIndexLast = fttestIndex;
                    //bb1 and bb2 2.5v
                    baseband18LDO(ENABLE, BB_LDO_2P5, ENABLE, BB_LDO_2P5);
                    break; 
                case 0x441:
                    fttestIndexLast = fttestIndex;
                    fttestRfBiasVbe();
                    break;
                case 0x442:
                    fttestIndexLast = fttestIndex;
                    fttestRfBiasVbg();
                    break;
                //0x7:1.3v.   0x0:1.2v,   0x6:1.1v,   0x5:1.0v,    0x4:0.85v,   0x3:0.8v,   0x2:0.75v.   0x1:0.7v,
                case 0x511:
                    fttestIndexLast = fttestIndex;
                    fttestLPvdd12(0x0);
                    break;
                case 0x512:
                    fttestIndexLast = fttestIndex;
                    fttestLPvdd12(0x1);
                    break;               
                case 0x513:
                    fttestIndexLast = fttestIndex;
                    fttestLPvdd12(0x2);
                    break;    
                case 0x514:
                    fttestIndexLast = fttestIndex;
                    fttestLPvdd12(0x3);
                    break;
                case 0x515:
                    fttestIndexLast = fttestIndex;
                    fttestLPvdd12(0x4);
                    break;
                case 0x516:
                    fttestIndexLast = fttestIndex;
                    fttestLPvdd12(0x5);
                    break;
                case 0x517:
                    fttestIndexLast = fttestIndex;
                    fttestLPvdd12(0x6);
                    break;
                case 0x518:
                    fttestIndexLast = fttestIndex;
                    fttestLPvdd12(0x7);
                    break;
                case 0x530:
                    //printf("rc32KHz test\r\n");
                    fttestIndexLast = fttestIndex;
                    fttestRc32K();
                    rc32kCalibration();
                    //printf("0x%x\r\n", RFC->rf_rc32k_reg_reg);
                    //printf("0x%x\r\n", PMU->pmu_pmu_state);
                    break;                
                case 0x540:
                    fttestIndexLast = fttestIndex;
                    fttestPllilocktotalcurrent();
                    break;                 
                case 0x541:
                    fttestIndexLast = fttestIndex;
                    pllibiascurrentdac12();
                    break;    
                case 0x542:
                    fttestIndexLast = fttestIndex;
                    pllibiascurrentitest();
                    break;  
                case 0x551:
                    fttestIndexLast = fttestIndex;
                    fttestPllndvdd33();
                    break; 
                case 0x552:
                    fttestIndexLast = fttestIndex;
                    fttestPllnpllnblocks();
                    break; 
                case 0x553:
                    fttestIndexLast = fttestIndex;
                    fttestPllnvcobuff();
                    break; 
                case 0x554:
                    fttestIndexLast = fttestIndex;
                    fttestPllnvcoldo();
                    break; 
                case 0x555:
                    fttestIndexLast = fttestIndex;
                    fttestPllnpllldo();
                    break; 
                case 0x556:
                    fttestIndexLast = fttestIndex;
                    fttestPllndac12();
                    break; 
                case 0x557:
                    fttestIndexLast = fttestIndex;
                    fttestPllnvcoitest();
                    break;  
                case 0x610:
                    fttestIndexLast = fttestIndex;
                    fttestRfcurrentbasic();
                    break; 
                case 0x611:
                    fttestIndexLast = fttestIndex;
                    fttestRfcurrent20G();
                    break; 
                case 0x612:
                    fttestIndexLast = fttestIndex;
                    fttestRfcurrentDoubler();
                    break;                 
                case 0x613:
                    fttestIndexLast = fttestIndex;
                    fttestRfcurrent1to4();
                    break; 
                case 0x614:
                    fttestIndexLast = fttestIndex;
                    fttestRfcurrent2PA();
                    break;
                case 0x615:
                    fttestIndexLast = fttestIndex;
                    fttestRfcurrent2LNA();
                    break;
                case 0x621:
                    fttestIndexLast = fttestIndex;
                    fttestRfcurrent2TIA();
                    break;                 
                case 0x622:
                    fttestIndexLast = fttestIndex;
                    fttestRfcurrent2PGA();
                    break; 
                case 0x623:
                    fttestIndexLast = fttestIndex;
                    fttestRfcurrent2ADC();
                    break; 
                case 0x630:
                    fttestIndexLast = fttestIndex;
                    fttestBBbiasCurrent();
                    break;                  
                case 0x711:
                    fttestIndexLast = fttestIndex;
                    fttestPlliLock();
                    break;                 
                case 0x721:
                    fttestIndexLast = fttestIndex;
                    fttestPllnLock76G();
                    break;                 
                case 0x722:
                    fttestIndexLast = fttestIndex;
                    fttestPllnLock77G();
                    break;                 
                case 0x723:
                    fttestIndexLast = fttestIndex;
                    fttestPllnLock78G();
                    break;                 
                case 0x724:
                    fttestIndexLast = fttestIndex;
                    fttestPllnLock79G();
                    break;                  
                case 0x725:
                    fttestIndexLast = fttestIndex;
                    fttestPllnLock80G();
                    break;                 
                case 0x726:
                    fttestIndexLast = fttestIndex;
                    fttestPllnLock81G();
                    break;                  
                case 0x811:
                    fttestIndexLast = fttestIndex;
                    fttestChannelPd20G();
                    break;                  
                case 0x812:
                    fttestIndexLast = fttestIndex;
                    fttestChannelPddoubler();
                    break;                 
                case 0x813:
                    fttestIndexLast = fttestIndex;
                    fttestChannelPd20GRx();
                    break;                 
                case 0x814:
                    fttestIndexLast = fttestIndex;
                    fttestChannelPd20GTx();
                    break;                 
                case 0x815:
                    fttestIndexLast = fttestIndex;
                    fttestChannelPd20GPA1();
                    break; 
                case 0x816:
                    fttestIndexLast = fttestIndex;
                    fttestChannelPd20GPA0();
                    break; 
                case 0x910:
                    fttestIndexLast = fttestIndex;
                    fttestgpadcIN1gpadc();
                    break; 
                case 0x921:
                    fttestIndexLast = fttestIndex;
                    fttestgpadcATBdcTestPA();
                    break; 
                case 0x922:
                    fttestIndexLast = fttestIndex;
                    fttestgpadcATBdcTest20G();
                    break; 
                case 0x923:
                    fttestIndexLast = fttestIndex;
                    fttestgpadcATBdcTestDoubler();
                    break; 
                case 0x924:
                    fttestIndexLast = fttestIndex;
                    fttestgpadcATBdcTestPGA();
                    break; 
                case 0x925:
                    fttestIndexLast = fttestIndex;
                    fttestgpadcATBdcTestADC();
                    break;         
                case 0x926:
                    fttestIndexLast = fttestIndex;
                    fttestgpadcATBdcTestBist();
                    break;                  
                case 0xA00:
                    fttestIndexLast = fttestIndex;
                    fttestVgaBist();
                    break;          
                case 0xA01:
                    fttestIndexLast = fttestIndex;
                    fttestDut();
                    break;   
                case 0xA02:
                    fttestIndexLast = fttestIndex;
                    fttestMaster();
                    break;       
                case 0xA03:
                    fttestIndexLast = fttestIndex;
                    mbistMaster();
                    break;             
                case 0xA04:
                    fttestIndexLast = fttestIndex;
                    fttestAdc();
                    break;                  
                case 0xB3:
                    
                    break;
                default:
                    
                
                    break;
            }
        } 
    } 
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//Here are the ft test cases
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//for ft test doc 4.2.2
//----------------------------------------------------------------------------
void fttestVdd12(void)
{ 
    delayUs(1000,HIGH_CLOCK);
    dvdd12HpLDO(POWER_ON, DVDD1P2_HP);
    delayUs(1000,HIGH_CLOCK);
    dvdd12HpLDO(POWER_ON, DVDD1P1_HP);
    delayUs(1000,HIGH_CLOCK);
    dvdd12HpLDO(POWER_ON, DVDD1P3_HP);
    delayUs(1000,HIGH_CLOCK);
    dvdd12HpLDO(POWER_ON, DVDD1P4_HP);
}
//----------------------------------------------------------------------------
//for ft test doc 4.2.3
//----------------------------------------------------------------------------
void fttestPlli(void)
{   
    plliLDOEnable(ENABLE, ENABLE);
    //plli pll 1.8v
    uint32 regVel;
    regVel = RFC->rf_plli_ldo_cfg_reg|1<<6;
    regVel &= ~(0x3<<4);
    regVel |= (0<<4);
    RFC->rf_plli_ldo_cfg_reg = regVel;
    delayUs(1000,HIGH_CLOCK);
    //plli pll 1.5v
    regVel |= RFC->rf_plli_ldo_cfg_reg;
    regVel &= ~(0x3<4);
    regVel |= (1<<4);
    RFC->rf_plli_ldo_cfg_reg = regVel;
    delayUs(1000,HIGH_CLOCK);
    //plli pll 2.2v
    regVel |= RFC->rf_plli_ldo_cfg_reg;
    regVel &= ~(0x3<<4);
    regVel |= (2<<4);
    RFC->rf_plli_ldo_cfg_reg = regVel;
    delayUs(1000,HIGH_CLOCK);
    //plli pll 2.5v
    regVel |= RFC->rf_plli_ldo_cfg_reg;
    regVel &= ~(0x3<<4);
    regVel |= (3<<4);
    RFC->rf_plli_ldo_cfg_reg = regVel;
    delayUs(1000,HIGH_CLOCK);
    
    //plli vco 1.8v
    regVel |= RFC->rf_plli_ldo_cfg_reg|1<<6;
    regVel &= ~(0x3<<0);
    regVel |= (0<<0);
    RFC->rf_plli_ldo_cfg_reg = regVel;
    delayUs(1000,HIGH_CLOCK);
    //plli pll 2.0v
    regVel |= RFC->rf_plli_ldo_cfg_reg;
    regVel &= ~(0x3<0);
    regVel |= (1<<0);
    RFC->rf_plli_ldo_cfg_reg = regVel;
    delayUs(1000,HIGH_CLOCK);
    //plli pll 2.2v
    regVel |= RFC->rf_plli_ldo_cfg_reg;
    regVel &= ~(0x3<<0);
    regVel |= (2<<0);
    RFC->rf_plli_ldo_cfg_reg = regVel;
    delayUs(1000,HIGH_CLOCK);
    //plli pll 2.5v
    regVel |= RFC->rf_plli_ldo_cfg_reg;
    regVel &= ~(0x3<<0);
    regVel |= (3<<0);
    RFC->rf_plli_ldo_cfg_reg = regVel;
    delayUs(1000,HIGH_CLOCK);
    
}
//----------------------------------------------------------------------------
//for ft test doc 4.2.4
//----------------------------------------------------------------------------
void fttest50Moscillator(void)
{   
    testPower();
    //50M output to gpio2
    hclkOutput();
    delayUs(1000,HIGH_CLOCK);
    //output 50M clock
    RFC->rfc_plli_ctrl_reg |= 1<<3;  
    PMU->pmu_pmu_ctrl_reg |= pmu_pmu_ctrl_reg_manu_ctrl_en_bit;
    delayUs(1000,HIGH_CLOCK);
    PMU->pmu_pmu_ctrl_reg &= ~pmu_pmu_ctrl_reg_manu_ctrl_en_bit;
}
void fttestGpioOutputH(void)
{   
    testPower();
    gpioOutput(0,  1);
    gpioOutput(1,  1);
    gpioOutput(2,  1);
    gpioOutput(4,  1);
    gpioOutput(7,  1);
    gpioOutput(8,  1);
    gpioOutput(9,  1);
    gpioOutput(13, 1);
    gpioOutput(14, 1);
    gpioOutput(15, 1);
    gpioOutput(16, 1);
    gpioOutput(17, 1);
    gpioOutput(18, 1);
    gpioOutput(20, 1);
    gpioOutput(21, 1);
}
void fttestGpioOutputL(void)
{   
    testPower();
    gpioOutput(0,  0);
    gpioOutput(1,  0);
    gpioOutput(2,  0);
    gpioOutput(4,  0);
    gpioOutput(7,  0);
    gpioOutput(8,  0);
    gpioOutput(9,  0);
    gpioOutput(13, 0);
    gpioOutput(14, 0);
    gpioOutput(15, 0);
    gpioOutput(16, 0);
    gpioOutput(17, 0);
    gpioOutput(18, 0);
    gpioOutput(20, 0);
    gpioOutput(21, 0);
}
void fttestGpioInput(void)
{   
    testPower();
    gpioDirection(0, 1);
    gpioDirection(1, 1);
    gpioDirection(2,  1);
    gpioDirection(4,  1);
    gpioDirection(7,  1);
    gpioDirection(8,  1);
    gpioDirection(9,  1);
    gpioDirection(13, 1);
    gpioDirection(14, 1);
    gpioDirection(15, 1);
    gpioDirection(16, 1);
    gpioDirection(17, 1);
    gpioDirection(18, 1);
    gpioDirection(20, 1);
    gpioDirection(21, 1);
    uint32 testGpioInput;
    testGpioInput = GPIO->gpio_in;
    testGpioInput = testGpioInput&(0x37E397);
    if (testGpioInput == 0x37E397)
    {
        printf("gpio input test is normal.\r\n");
    }
}
//----------------------------------------------------------------------------
//for ft test doc 4.4.1
//----------------------------------------------------------------------------
void fttestDc(void)
{   
    testPower();
    //50M output to gpio2
    hclkOutput();
    delayUs(1000,HIGH_CLOCK);
    //output 50M clock
    RFC->rfc_plli_ctrl_reg |= 1<<3;  
    PMU->pmu_pmu_ctrl_reg |= pmu_pmu_ctrl_reg_manu_ctrl_en_bit;
    delayUs(1000,HIGH_CLOCK);
    PMU->pmu_pmu_ctrl_reg &= ~pmu_pmu_ctrl_reg_manu_ctrl_en_bit;
}
//----------------------------------------------------------------------------
//for ft test doc 4.4.2
//----------------------------------------------------------------------------
void fttestLdoPor(void)
{   

}
//----------------------------------------------------------------------------
//for ft test doc 4.4.3
//----------------------------------------------------------------------------
void fttestPlln(void)
{
    //plln vco 1.8v
    pllnVcoConfig(ENABLE, PLLN_VCO_LDO_1P8);
    delayUs(1000,HIGH_CLOCK);
    //plln vco 2.0v
    pllnVcoConfig(ENABLE, PLLN_VCO_LDO_2P0);
    delayUs(1000,HIGH_CLOCK);
    //plln vco 2.2v
    pllnVcoConfig(ENABLE, PLLN_VCO_LDO_2P2);
    delayUs(1000,HIGH_CLOCK);
    //plln vco 2.5v
    pllnVcoConfig(ENABLE, PLLN_VCO_LDO_2P5);
    delayUs(1000,HIGH_CLOCK);
    
    //plln pll 1.8v
    pllnPllConfig(ENABLE, PLLN_PLL_LDO_1P8, ENABLE);
    delayUs(1000,HIGH_CLOCK);
    //plln pll 2.0v
    pllnPllConfig(ENABLE, PLLN_PLL_LDO_2P0, ENABLE);
    delayUs(1000,HIGH_CLOCK);
    //plln pll 2.2v
    pllnPllConfig(ENABLE, PLLN_PLL_LDO_2P2, ENABLE);
    delayUs(1000,HIGH_CLOCK);
    //plln pll 2.4v
    pllnPllConfig(ENABLE, PLLN_PLL_LDO_2P5, ENABLE);
    delayUs(1000,HIGH_CLOCK);   
}
//----------------------------------------------------------------------------
//for ft test doc 4.4.4
//----------------------------------------------------------------------------
void fttestBaseband(void)
{
    //bb1 and bb2 1.8v
    baseband18LDO(ENABLE, BB_LDO_1P8, ENABLE, BB_LDO_1P8);
    delayUs(1000,HIGH_CLOCK);
    //bb1 and bb2 2.0v
    baseband18LDO(ENABLE, BB_LDO_2P0, ENABLE, BB_LDO_2P0);
    delayUs(1000,HIGH_CLOCK);
    //bb1 and bb2 2.2v
    baseband18LDO(ENABLE, BB_LDO_2P2, ENABLE, BB_LDO_2P2);
    delayUs(1000,HIGH_CLOCK);
    //bb1 and bb2 2.5v
    baseband18LDO(ENABLE, BB_LDO_2P5, ENABLE, BB_LDO_2P5);
    delayUs(1000,HIGH_CLOCK);
}
//----------------------------------------------------------------------------
//for ft test doc 4.4.5
//----------------------------------------------------------------------------
void fttestRfBias(void)
{
    test20GSingleFequence();
    gpadcEnable(GPADC_AIO1);
    dacEnBias1Pd(ENABLE, POWER_ON);
    biasGpadc(0, 0, 8);
    
    baseBandBiasSetting(ENABLE);
    
    //VBE+GPADC_AIO1
    RFC->rf_gpadc_reg_reg = (3<<8)|1<<0|1<<5|1<<2;
    //VBG+GPADC_AIO1
    RFC->rf_gpadc_reg_reg = (1<<(7+1))|1<<0|1<<5|1<<2;
    RFC->rf_gpadc_reg_reg |= (1<<(7+6));    
}
void fttestRfBiasVbe(void)
{
    test20GSingleFequence();
    gpadcEnable(GPADC_AIO1);
    dacEnBias1Pd(ENABLE, POWER_ON);
    biasGpadc(0, 0, 8);
    
    baseBandBiasSetting(ENABLE);
    
    //VBE+GPADC_AIO1
    RFC->rf_gpadc_reg_reg = (3<<8)|1<<0|1<<5|1<<2;  
}
void fttestRfBiasVbg(void)
{
    fttestRfBiasVbe();
    
    //VBE+GPADC_AIO1
    RFC->rf_gpadc_reg_reg = (3<<8)|1<<0|1<<5|1<<2;  
}
//----------------------------------------------------------------------------
//for ft test doc 4.5.1
//----------------------------------------------------------------------------
void fttestLPvdd12(int slpLdo12)
{
    mcuSleepConfig(0);
    
    //0x7:1.3v.   0x0:1.2v,   0x6:1.1v,   0x5:1.0v,    0x4:0.85v,   0x3:0.8v,   0x2:0.75v.   0x1:0.7v,
    PMU->pmu_slp_ldo12n_rega_reg  = slpLdo12 << pmu_slp_ldo12n_rega_reg_slp_ldo12_lp_vcfg_shift ;

    //*(uint32*)0x400200a8 = 0x10;
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
//----------------------------------------------------------------------------
//for ft test doc 4.5.2
//----------------------------------------------------------------------------
void fttestPortest(void)
{


}
//----------------------------------------------------------------------------
//for ft test doc 4.5.3
//----------------------------------------------------------------------------
void fttestRc32K(void)
{
    rc32kOutput();
    for(int i=0; i<16; i++)
    {
        RFC->rf_rc32k_reg_reg = i;
    }
}
//----------------------------------------------------------------------------
//for ft test doc 4.5.4
//----------------------------------------------------------------------------
void fttestPlliPllnBias(void)
{
    //plli lock total current
    plliLDOEnable(ENABLE, ENABLE);
    delayUs(1000,HIGH_CLOCK);
    
    plliDacSetting();
    plliConfig(ENABLE);
    
    dacEnBias1Pd(0, 0);
    plliLock();
    
    //plli dac12=8
    uint32 regVel = 0;
    regVel = RFC->rf_plli_dacb_reg|8<<12;
    RFC->rf_plli_dacb_reg = regVel;
    
    //plli dac12=0
    regVel = 0<<12;
    RFC->rf_plli_dacb_reg = regVel;
    
    //en_vco_itest =1
    RFC->rf_plli_rega_reg |= 1<<13;
    //en_vco_itest =0
    RFC->rf_plli_rega_reg &= ~(1<<13);
}
void fttestPllilocktotalcurrent(void)
{
    //plli lock total current
    plliLDOEnable(ENABLE, ENABLE);
    delayUs(1000,HIGH_CLOCK);
    
    plliDacSetting();
    plliConfig(ENABLE);
    
    dacEnBias1Pd(0, 0);
    plliLock();
}
void pllibiascurrentdac12(void)
{
    fttestPllilocktotalcurrent();
    
    //plli dac12=8
    uint32 regVel = 0;
    regVel = RFC->rf_plli_dacb_reg|8<<12;
    RFC->rf_plli_dacb_reg = regVel;
}
void pllibiascurrentitest(void)
{
    fttestPllilocktotalcurrent();
    
    //plli dac12=8
    uint32 regVel = 0;
    regVel = RFC->rf_plli_dacb_reg|8<<12;
    RFC->rf_plli_dacb_reg = regVel;
}
//----------------------------------------------------------------------------
//for ft test doc 4.5.5
//----------------------------------------------------------------------------
void fttestPlliPllnBiasVcoBuff(void)
{
    //plln lock total current
    fttestPlliPllnBias();
    dacEnBias1Pd(1, 0);
    pllnVcoConfig(ENABLE, PLLN_VCO_LDO_1P8);
    delayUs(1000,HIGH_CLOCK);
    pllnPllConfig(ENABLE, PLLN_PLL_LDO_2P5, ENABLE);
    delayUs(1000,HIGH_CLOCK);
    
    pllnDacSetting();
    pllnConfig(ENABLE);
    fmcwModeSel(MODE_TEST);
    rfbuff20G(ENABLE, DISABLE);
    
    pllnLock(76, 0);
    //manuPllnCapArray(0xD);//76G:B  77G:A,B
    fmcwConfig(76, 0, CHIRP_T0, CHIRP_T1, CHIRP_T2, CHIRP_NUM);
    fmcwEnmode(REGISTER, CONTINUE_MODE);
    
    //close vco ldo
    pllnVcoConfig(DISABLE, PLLN_VCO_LDO_1P8);
    delayUs(1000,HIGH_CLOCK);
    
    //close pll ldo
    pllnPllConfig(DISABLE, PLLN_PLL_LDO_2P5, ENABLE);
    delayUs(1000,HIGH_CLOCK); 
    
    pllnVcoConfig(ENABLE, PLLN_VCO_LDO_1P8);
    delayUs(1000,HIGH_CLOCK);
    pllnPllConfig(ENABLE, PLLN_PLL_LDO_2P5, ENABLE);
    delayUs(1000,HIGH_CLOCK);
    
    //plln dac12=8
    uint32 regVel = 0;
    regVel = RFC->rf_plli_dacb_reg;
    regVel |= 8<<12;
    RFC->rf_plln_dacb_reg = regVel;
    
    //plli dac12=0
    regVel = RFC->rf_plli_dacb_reg;
    regVel &= ~(8<<12);
    RFC->rf_plln_dacb_reg = regVel;
    
    //en_vco_itest =1
    RFC->rf_plln_regb_reg |= 1<<30;
}
void fttestPllndvdd33(void)
{
    //plln lock total current
    fttestPlliPllnBias();
    dacEnBias1Pd(1, 0);
    pllnVcoConfig(ENABLE, PLLN_VCO_LDO_1P8);
    delayUs(1000,HIGH_CLOCK);
    pllnPllConfig(ENABLE, PLLN_PLL_LDO_2P5, ENABLE);
    delayUs(1000,HIGH_CLOCK);
    
    pllnDacSetting();
    pllnConfig(ENABLE);
    fmcwModeSel(MODE_TEST);
    rfbuff20G(ENABLE, DISABLE);
    
    pllnLock(76, 0);
    //manuPllnCapArray(0xD);//76G:B  77G:A,B
    fmcwConfig(76, 0, CHIRP_T0, CHIRP_T1, CHIRP_T2, CHIRP_NUM);
    fmcwEnmode(REGISTER, CONTINUE_MODE);
}
void fttestPllnpllnblocks(void)
{
    //turn off plln blocks
    //plln_reg9.en_ld=0
    RFC->rf_plln_regc_reg &= ~(1<<7);
    //plln_reg1= 0x1A 
    RFC->rf_plln_rega_reg &= ~(0xff);
    RFC->rf_plln_rega_reg |= 0x1A;
    //rf_plln_ctrl_reg.plln_en_bias=0
    pllnPllConfig(ENABLE, PLLN_PLL_LDO_2P5, DISABLE);
    //rfc_plli_ctrl_reg.plli_en_ckplln=0
    RFC->rfc_plli_ctrl_reg &= ~(1<<1);
    //plli pll ldo open defualt, go to sleep close by FSM
    PMU->pmu_pmu_ctrl_reg |= pmu_pmu_ctrl_reg_manu_ctrl_en_bit;
    delayUs(100,HIGH_CLOCK);
    PMU->pmu_pmu_ctrl_reg &= ~pmu_pmu_ctrl_reg_manu_ctrl_en_bit;
    delayUs(1000,HIGH_CLOCK);
}
void fttestPllnvcobuff(void)
{
    //Turn off PLLN VCO and buffers.
    RFC->rf_plln_rega_reg &= ~(0xff);
    delayUs(1000,HIGH_CLOCK); 
}
void fttestPllnvcoldo(void)
{
    //Turn off PLLN VCO LDO.
    pllnVcoConfig(DISABLE, PLLN_PLL_LDO_1P8);
    delayUs(1000,HIGH_CLOCK); 
}
void fttestPllnpllldo(void)
{
    //Turn off PLLN LDO.
    pllnPllConfig(DISABLE, PLLN_PLL_LDO_2P5, DISABLE);
    delayUs(1000,HIGH_CLOCK); 
}



void fttestPllndac12(void)
{
    fttestPllndvdd33();
    
    //plln dac12=8
    uint32 regVel = 0;
    regVel = RFC->rf_plli_dacb_reg;
    regVel |= 8<<12;
}
void fttestPllnvcoitest(void)
{
    fttestPllndvdd33();
    
    //en_vco_itest =1
    RFC->rf_plln_regb_reg |= 1<<30;
}
//----------------------------------------------------------------------------
//for ft test doc 4.6.1 and 4.6.2
//----------------------------------------------------------------------------
void fttestRfcurrent(void)
{
    fttestPlliPllnBiasVcoBuff();
    rfPhaseShift(0x1f, 0, 0x1f, 0);
    RFC->rf_spare_reg = 0x01;
    fmcwModeSel(MODE_INTER);
    //20G buff Rf current
    rfbuff20G(DISABLE, DISABLE);
    rfbuff20G(ENABLE, DISABLE);
    
    //enable 20G and doubler
    rfbuff20G(1, 1);
    //enable buffer1to4
    rfBuff77GEnable(1, 1, 1, 1, 1);
    //enable bias for 20G(fmcw buffer), doubler and buffer1to4
    bias20GTo80G(8, 8, 8, 8, 8, 8, 8, 8);
    //enable pa1 and pa0
    txPaEnable(1, 1);
    txPaBias(8, 8);
    
    //enable lna
    rxLnaEnable(1, 1);
    mixerEnable(1, 1);
    lnaRegularBias(1, 1);
    lnaDacBias(8, 8, 8, 8, 8, 8);
    
    //through bist check PA function
    //bist on amplifier
    bistOn(BIST_AMP, BIST_DIV32_SEL);
    
    //bias2_en: enable signal for ztc bias for BB
    baseBandBiasSetting(ENABLE);
    delayUs(1000 ,HIGH_CLOCK);
    baseband18LDO(ENABLE, BB_LDO_1P8, ENABLE, BB_LDO_1P8);
    
    //tia config
    rfTiaEnable(ENABLE, ENABLE);
    rfTiaGainSetting(TIA_GAIN_6DB, TIA_GAIN_6DB ,TIA_GAIN_6DB ,TIA_GAIN_6DB);
    rfTiaDacBias(8, 8);
    //pga config
    rfPgaGainCalEnable(ENABLE, ENABLE, ENABLE, ENABLE);
    rfPgaGainCalSetting(PGA_CAL_P1, PGA_CAL_P1 ,PGA_CAL_P1 ,PGA_CAL_P1);
    rfVgaEnable(ENABLE, ENABLE);
    rfVgaGainSetting(PGA_GAIN_30DB, PGA_GAIN_30DB ,PGA_GAIN_30DB ,PGA_GAIN_30DB);
    rfVgaDacBias(8, 8);
    //filter config
    tiaHpfConfig(1, 1, TIA_HPF_25KHZ, TIA_HPF_25KHZ);
    rcHpfConfig(0, 0, RC_HPF_150KHZ, RC_HPF_150KHZ);
    pgaLpfConfig(PGA_LPF_8M, PGA_LPF_8M);
    pgaLpfCal(0, 0, 3, 3);
    pgaHpfConfig(1, 1, PGA_HPF_500KHZ, PGA_HPF_500KHZ);
    
    //enable vga en_buffer
    //RFC->rfc_vga0_reg |= 1<<9;
    //en_buffer0 contorl if signal output gpio23, 24, 25, 26. 
    //en_buffer1 contorl if signal output gpio27, 28, 29, 30.  
    //when we test low power consumption, we need to connect p29 with vddrf enable.
    //Normally, p29 is high resistance, vga1 en_buffer will lower the p29 level.
    //RFC->rfc_vga1_reg |= 1<<9;  
    
    adcEnable(1, 1);
    adcBiasSetting(8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8);   
}


void fttestRfcurrentbasic(void)
{
    fttestPlliPllnBiasVcoBuff();
    rfPhaseShift(0x1f, 0, 0x1f, 0);
    RFC->rf_spare_reg = 0x01;
    fmcwModeSel(MODE_INTER);
    //20G buff Rf current
    rfbuff20G(DISABLE, DISABLE);
}
void fttestRfcurrent20G(void)
{
    //fttestRfcurrentbasic();
    rfbuff20G(ENABLE, DISABLE);
}
void fttestRfcurrentDoubler(void)
{
    //fttestRfcurrent20G();
    
    //enable 20G and doubler
    rfbuff20G(1, 1);
}
void fttestRfcurrent1to4(void)
{
    //fttestRfcurrentDoubler();
    
    //enable buffer1to4
    rfBuff77GEnable(1, 1, 1, 1, 1);
    //enable bias for 20G(fmcw buffer), doubler and buffer1to4
    bias20GTo80G(8, 8, 8, 8, 8, 8, 8, 8);
}
void fttestRfcurrent2PA(void)
{
    //fttestRfcurrent1to4();
    //enable pa1 and pa0
    txPaEnable(1, 1);
    txPaBias(8, 8); 
}
void fttestRfcurrent2LNA(void)
{
    //fttestRfcurrent2PA();
    
    //enable lna
    rxLnaEnable(1, 1);
    mixerEnable(1, 1);
    lnaRegularBias(1, 1);
    lnaDacBias(8, 8, 8, 8, 8, 8);
}
void fttestRfcurrent2TIA(void)
{
    //fttestRfcurrent2LNA();
    
    //bias2_en: enable signal for ztc bias for BB
    baseBandBiasSetting(ENABLE);
    delayUs(1000 ,HIGH_CLOCK);
    baseband18LDO(ENABLE, BB_LDO_1P8, ENABLE, BB_LDO_1P8);
    
    //tia config
    rfTiaEnable(ENABLE, ENABLE);
    rfTiaGainSetting(TIA_GAIN_6DB, TIA_GAIN_6DB ,TIA_GAIN_6DB ,TIA_GAIN_6DB);
    rfTiaDacBias(8, 8);   
}
void fttestRfcurrent2PGA(void)
{
    //fttestRfcurrent2TIA();

    //pga config
    rfPgaGainCalEnable(ENABLE, ENABLE, ENABLE, ENABLE);
    rfPgaGainCalSetting(PGA_CAL_P1, PGA_CAL_P1 ,PGA_CAL_P1 ,PGA_CAL_P1);
    rfVgaEnable(ENABLE, ENABLE);
    rfVgaGainSetting(PGA_GAIN_30DB, PGA_GAIN_30DB ,PGA_GAIN_30DB ,PGA_GAIN_30DB);
    rfVgaDacBias(8, 8);
}
void fttestRfcurrent2ADC(void)
{
   fttestRfcurrent();
}
//----------------------------------------------------------------------------
//for ft test doc 4.6.3
//----------------------------------------------------------------------------
void fttestBBbiasCurrent(void)
{
    //bias2_en: enable signal for ztc bias for BB
    baseBandBiasSetting(ENABLE);
    delayUs(1000 ,HIGH_CLOCK);
    baseband18LDO(ENABLE, BB_LDO_1P8, ENABLE, BB_LDO_1P8);
    
    RFC->rf_bias2_dinc_reg |= (8<<12);
    //RFC->rf_bias2_dinc_reg &= ~(8<<12);
}
//----------------------------------------------------------------------------
//for ft test doc 4.7.1
//----------------------------------------------------------------------------
void fttestPlliLock(void)
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
    *(uint32*)0x20009038 = (*(uint32*)0x40003194)&(1<<2);
    
    frequecyCounter(1, 0x20009034);
}
//----------------------------------------------------------------------------
//for ft test doc 4.7.2
//----------------------------------------------------------------------------
void fttestPllnLock(void)
{
    fttestPlliLock();

    fmcwConfig(76, 0, CHIRP_T0, CHIRP_T1, CHIRP_T2, CHIRP_NUM);
    
    pllnLock(76, 0);   
    
    baseband18LDO(ENABLE, BB_LDO_1P8, ENABLE, BB_LDO_1P8);
    baseBandBiasSetting(ENABLE);
    biasGpadc(0, 0, 8);
    //GPADC_IN18_2+GPADC_AIO1
    RFC->rf_gpadc_reg_reg = (1<<(7+1))|1<<0|1<<5|1<<2;
    RFC->rf_gpadc_reg_reg |= (1<<(7+10)); 
    
    //enable atb and sel vti_cp
    RFC->rf_plln_regc_reg |=1<<25| 7<<28;
    //enable atb and atb sel
    RFC->rf_plln_regc_reg |=1<<20| 1<<21;
    
    //plln lock to 20.25G
    fmcwConfig(81, 0, CHIRP_T0, CHIRP_T1, CHIRP_T2, CHIRP_NUM);
    pllnLock(81, 0);   
    
    //sweep plln freq 19GHz to 20GHz, automatic search vco_sw
    pllnLock(76, 4000);
    fmcwConfig(76, 4000, CHIRP_T0, CHIRP_T1, CHIRP_T2, CHIRP_NUM);
    fmcwEnmode(REGISTER, CONTINUE_MODE);
    
//    PMU->pmu_soft_reset_reg = 1<<10;
//    PMU->pmu_plli_lock_ctrl = 1<<0;
//    delayUs(10000, HIGH_CLOCK);
//    PMU->pmu_plli_lock_ctrl = 0<<0;
//    //printf("%d\r\n", *(uint32*)0x4000c058);
//    PMU->pmu_plli_lock_ctrl = 1<<1;
//    delayUs(200, HIGH_CLOCK);
//    PMU->pmu_plli_lock_ctrl = 1<<0;
//    PMU->pmu_soft_reset_reg &= ~(1<<10);
//    *(uint32*)0x20009034 = PMU->pmu_plli_lock_result;
//    //test 19GHz, 19.25Ghz, 19.5GHz, 19.75GHz, 20GHz PLLN vtune and lock detector
//    fmcwConfig(76, 0, CHIRP_T0, CHIRP_T1, CHIRP_T2, CHIRP_NUM);
//    fmcwEnmode(REGISTER, CONTINUE_MODE);
//    fmcwConfig(77, 0, CHIRP_T0, CHIRP_T1, CHIRP_T2, CHIRP_NUM);  
//    fmcwEnmode(REGISTER, CONTINUE_MODE);
//    fmcwConfig(78, 0, CHIRP_T0, CHIRP_T1, CHIRP_T2, CHIRP_NUM);  
//    fmcwEnmode(REGISTER, CONTINUE_MODE);
//    fmcwConfig(79, 0, CHIRP_T0, CHIRP_T1, CHIRP_T2, CHIRP_NUM); 
//    fmcwEnmode(REGISTER, CONTINUE_MODE);    
//    fmcwConfig(80, 0, CHIRP_T0, CHIRP_T1, CHIRP_T2, CHIRP_NUM);
//    fmcwEnmode(REGISTER, CONTINUE_MODE);  
//    fmcwConfig(81, 0, CHIRP_T0, CHIRP_T1, CHIRP_T2, CHIRP_NUM);
//    fmcwEnmode(REGISTER, CONTINUE_MODE);    
}
void frequecyCounter(int refclkSel, uint32 saveAddr)
{
    PMU->pmu_soft_reset_reg = 1<<10;
    //select PLLI's ck_dig 125mhz to lock detector. 
    //enable counter to count clock's cycle.
    PMU->pmu_plli_lock_ctrl = refclkSel<<2|1<<0;
    //hold 10ms
    delayUs(10000, HIGH_CLOCK);
    //hold counter's current value, for MCU reading results. 
    PMU->pmu_plli_lock_ctrl &= ~(1<<0);
    //read counter result
    *(uint32*)saveAddr = PMU->pmu_plli_lock_result;
    //printf("%d\r\n", PMU->pmu_plli_lock_result);
    //clear counter
    PMU->pmu_plli_lock_ctrl |= 1<<1;
    delayUs(300, HIGH_CLOCK);
    PMU->pmu_plli_lock_ctrl &= ~(1<<1);
    PMU->pmu_soft_reset_reg &= ~(1<<10); 
}
void fttestPllnLock76G(void)
{
    //printf("sweep_plln_freq");
    fttestPllnLock();

    fmcwConfig(75, 0, CHIRP_T0, CHIRP_T1, CHIRP_T2, CHIRP_NUM);
    fmcwEnmode(REGISTER, CONTINUE_MODE);
    delayUs(20000, HIGH_CLOCK);
    //printf("19G Lock detector is %d\r\n", (*(uint32*)0x40003194)&(1<<2));
    *(uint32*)0x20009058=(*(uint32*)0x40003194)&(1<<2);
    frequecyCounter(0, 0x2000903c);

    fmcwConfig(76, 0, CHIRP_T0, CHIRP_T1, CHIRP_T2, CHIRP_NUM);
    fmcwEnmode(REGISTER, CONTINUE_MODE);
    delayUs(20000, HIGH_CLOCK);
    //printf("19G Lock detector is %d\r\n", (*(uint32*)0x40003194)&(1<<2));
    *(uint32*)0x20009004=(*(uint32*)0x40003194)&(1<<2);
    frequecyCounter(0, 0x20009040);
    
    fmcwConfig(77, 0, CHIRP_T0, CHIRP_T1, CHIRP_T2, CHIRP_NUM);
    fmcwEnmode(REGISTER, CONTINUE_MODE);
    delayUs(20000, HIGH_CLOCK);
    //printf("19.25G Lock detector is %d\r\n", (*(uint32*)0x40003194)&(1<<2));
    *(uint32*)0x20009008=(*(uint32*)0x40003194)&(1<<2);
    frequecyCounter(0, 0x20009044);
    
    fmcwConfig(78, 0, CHIRP_T0, CHIRP_T1, CHIRP_T2, CHIRP_NUM);
    fmcwEnmode(REGISTER, CONTINUE_MODE);
    delayUs(20000, HIGH_CLOCK);
    //printf("19.5G Lock detector is %d\r\n", (*(uint32*)0x40003194)&(1<<2));
    *(uint32*)0x2000900c=(*(uint32*)0x40003194)&(1<<2);
    frequecyCounter(0, 0x20009048);
    
    fmcwConfig(79, 0, CHIRP_T0, CHIRP_T1, CHIRP_T2, CHIRP_NUM);
    fmcwEnmode(REGISTER, CONTINUE_MODE);
    delayUs(20000, HIGH_CLOCK);
    //printf("19.75G GLock detector is %d\r\n", (*(uint32*)0x40003194)&(1<<2));
    *(uint32*)0x20009010=(*(uint32*)0x40003194)&(1<<2);
    frequecyCounter(0, 0x2000904c);
    
    fmcwConfig(80, 0, CHIRP_T0, CHIRP_T1, CHIRP_T2, CHIRP_NUM);
    fmcwEnmode(REGISTER, CONTINUE_MODE);
    delayUs(20000, HIGH_CLOCK);
    //printf("20G Lock detector is %d\r\n", (*(uint32*)0x40003194)&(1<<2));
    *(uint32*)0x20009014=(*(uint32*)0x40003194)&(1<<2);
    frequecyCounter(0, 0x20009050);
    
    fmcwConfig(81, 0, CHIRP_T0, CHIRP_T1, CHIRP_T2, CHIRP_NUM);
    fmcwEnmode(REGISTER, CONTINUE_MODE);
    delayUs(20000, HIGH_CLOCK);
    //printf("20.25G Lock detector is %d\r\n", (*(uint32*)0x40003194)&(1<<2));
    *(uint32*)0x20009018=(*(uint32*)0x40003194)&(1<<2);
    frequecyCounter(0, 0x20009054);
}
void fttestPllnLock77G(void)
{
    fttestPllnLock();
    fmcwConfig(77, 0, CHIRP_T0, CHIRP_T1, CHIRP_T2, CHIRP_NUM);
    fmcwEnmode(REGISTER, CONTINUE_MODE);
    delayUs(20000, HIGH_CLOCK);
    printf("Lock detector is %d\r\n", (*(uint32*)0x40003194)&(1<<2));
}
void fttestPllnLock78G(void)
{
    fttestPllnLock();
    fmcwConfig(78, 0, CHIRP_T0, CHIRP_T1, CHIRP_T2, CHIRP_NUM);
    fmcwEnmode(REGISTER, CONTINUE_MODE);
    delayUs(20000, HIGH_CLOCK);
    printf("Lock detector is %d\r\n", (*(uint32*)0x40003194)&(1<<2));
}
void fttestPllnLock79G(void)
{
    fttestPllnLock();
    fmcwConfig(79, 0, CHIRP_T0, CHIRP_T1, CHIRP_T2, CHIRP_NUM);
    fmcwEnmode(REGISTER, CONTINUE_MODE);
    delayUs(20000, HIGH_CLOCK);
    printf("Lock detector is %d\r\n", (*(uint32*)0x40003194)&(1<<2));
}
void fttestPllnLock80G(void)
{
    fttestPllnLock();
    fmcwConfig(80, 0, CHIRP_T0, CHIRP_T1, CHIRP_T2, CHIRP_NUM);
    fmcwEnmode(REGISTER, CONTINUE_MODE);
    delayUs(20000, HIGH_CLOCK);
    printf("Lock detector is %d\r\n", (*(uint32*)0x40003194)&(1<<2));
}
void fttestPllnLock81G(void)
{
    fttestPllnLock();
    fmcwConfig(81, 0, CHIRP_T0, CHIRP_T1, CHIRP_T2, CHIRP_NUM);
    fmcwEnmode(REGISTER, CONTINUE_MODE);
    delayUs(20000, HIGH_CLOCK);
    printf("Lock detector is %d\r\n", (*(uint32*)0x40003194)&(1<<2));
}
//----------------------------------------------------------------------------
//for ft test doc 4.8
//----------------------------------------------------------------------------
void fttestChannelPd(void)
{
    test20GSingleFequence();
    //set fmcw pll to doubler
    fmcwModeSel(MODE_INTER);
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
    txPaEnable(1, 0);
    txPaBias(14, 14);   
    
//    //enable 20G atb        (rf atb select 000:pwd) 0x400030d4:8d200
//    RFC->rfc_tx_pa_reg |= 1<<6;
//    RFC->rfc_tx_pa_reg &= ~(1<<6);
//    //enable doubler atb    (rf atb select 001:pwd) 0x400030d4:8d183
//    RFC->rfc_tx_pa_reg |= 1<<7|1<<8;
//    RFC->rfc_tx_pa_reg &= ~(1<<7|1<<8);
//    //enable buffer1to4 atb (rf atb select 010:pwd) 0x400030d4:8d140 Rx buff
//    RFC->rfc_tx_pa_reg |= 1<<5|1<<8;
//    RFC->rfc_tx_pa_reg &= ~(1<<5|1<<8);
//    //enable buffer1to4 atb (rf atb select 010:pwd) 0x400030d4:8d140 Tx buff
//    RFC->rfc_tx_pa_reg |= 1<<5|1<<9|1<<8;
//    RFC->rfc_tx_pa_reg &= ~(1<<5|1<<9|1<<8);
//    //enable PA1 atb        (rf atb select 00:pwd)  0x400030d4:0d238
//    RFC->rfc_tx_pa_reg |= 1<<3;
//    RFC->rfc_tx_pa_reg &= ~(1<<3);
//    //enable PA0 atb        (rf atb select 00:pwd)  0x400030d4:0d238
//    RFC->rfc_tx_pa_reg |= 1<<2;
//    RFC->rfc_tx_pa_reg &= ~(1<<2);
}
void fttestChannelPd20G(void)
{
    fttestChannelPd();
    //enable 20G atb        (rf atb select 000:pwd) 0x400030d4:8d200
    RFC->rfc_tx_pa_reg |= 1<<6;
    //RFC->rfc_tx_pa_reg &= ~(1<<6);
    delayUs(20000, HIGH_CLOCK);
    //printf("20G power detector is %d\r\n", (*(uint32*)0x400030d4)&0xff);
    *(uint32*)0x2000901c=(*(uint32*)0x400030d4)&0xff;
    RFC->rfc_tx_pa_reg &= ~(1<<6);
}
void fttestChannelPddoubler(void)
{
    fttestChannelPd();
    //enable doubler atb    (rf atb select 001:pwd) 0x400030d4:8d183
    RFC->rfc_tx_pa_reg |= 1<<7|1<<8;
    //RFC->rfc_tx_pa_reg &= ~(1<<7|1<<8);
    delayUs(20000, HIGH_CLOCK);
    //printf("doubler power detector is %d\r\n", (*(uint32*)0x400030d4)&0xff);
    *(uint32*)0x20009020=(*(uint32*)0x400030d4)&0xff;
    RFC->rfc_tx_pa_reg &= ~(1<<7|1<<8);
}
void fttestChannelPd20GRx(void)
{
    fttestChannelPd();
    //enable buffer1to4 atb (rf atb select 010:pwd) 0x400030d4:8d140 Rx buff
    RFC->rfc_tx_pa_reg |= 1<<5|1<<8;
    //RFC->rfc_tx_pa_reg &= ~(1<<5|1<<8);
    delayUs(20000, HIGH_CLOCK);
    //printf("Rx buff power detector is %d\r\n", (*(uint32*)0x400030d4)&0xff);
    *(uint32*)0x20009024=(*(uint32*)0x400030d4)&0xff;
    RFC->rfc_tx_pa_reg &= ~(1<<5|1<<8);
}
void fttestChannelPd20GTx(void)
{
    fttestChannelPd();
    //enable buffer1to4 atb (rf atb select 010:pwd) 0x400030d4:8d140 Tx buff
    RFC->rfc_tx_pa_reg |= 1<<5|1<<9|1<<8;
    //RFC->rfc_tx_pa_reg &= ~(1<<5|1<<9|1<<8);
    delayUs(20000, HIGH_CLOCK);
    //printf("Tx buff power detector is %d\r\n", (*(uint32*)0x400030d4)&0xff);
    *(uint32*)0x20009028=(*(uint32*)0x400030d4)&0xff;
    RFC->rfc_tx_pa_reg &= ~(1<<5|1<<9|1<<8);
}
void fttestChannelPd20GPA1(void)
{
    fttestChannelPd();
    //enable PA1 atb        (rf atb select 00:pwd)  0x400030d4:0d238
    RFC->rfc_tx_pa_reg |= 1<<3;
    //RFC->rfc_tx_pa_reg &= ~(1<<3);
    delayUs(20000, HIGH_CLOCK);
    //printf("PA1 power detector is %d\r\n", (*(uint32*)0x400030d4)&0xff);
    *(uint32*)0x2000902c=(*(uint32*)0x400030d4)&0xff;
    RFC->rfc_tx_pa_reg &= ~(1<<3);
}
void fttestChannelPd20GPA0(void)
{
    fttestChannelPd();
    //enable PA0 atb        (rf atb select 00:pwd)  0x400030d4:0d238
    RFC->rfc_tx_pa_reg |= 1<<2;
    //RFC->rfc_tx_pa_reg &= ~(1<<2);
    delayUs(20000, HIGH_CLOCK);
    //printf("PA0 power detector is %d\r\n", (*(uint32*)0x400030d4)&0xff);
    *(uint32*)0x20009030=(*(uint32*)0x400030d4)&0xff;
    RFC->rfc_tx_pa_reg &= ~(1<<2);
}
//----------------------------------------------------------------------------
//for ft test doc 4.9.1
//----------------------------------------------------------------------------
void fttestgpadcIN1(void)
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
    pllnLock(77, 0);
    
    //output 50M clock
    RFC->rfc_plli_ctrl_reg |= 1<<3;  
    PMU->pmu_pmu_ctrl_reg |= pmu_pmu_ctrl_reg_manu_ctrl_en_bit;
    delayUs(100,HIGH_CLOCK);
    PMU->pmu_pmu_ctrl_reg &= ~pmu_pmu_ctrl_reg_manu_ctrl_en_bit;
    
    fmcwConfig(77, 0, CHIRP_T0, CHIRP_T1, CHIRP_T2, CHIRP_NUM);
    fmcwEnmode(REGISTER, CONTINUE_MODE);

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
    rfPgaGainCalEnable(0, 0, 0, 0);
    rfPgaGainCalSetting(PGA_CAL_P1, PGA_CAL_P1 ,PGA_CAL_P1 ,PGA_CAL_P1);
    rfVgaEnable(ENABLE, ENABLE);
    rfVgaGainSetting(PGA_GAIN_20DB, PGA_GAIN_20DB ,PGA_GAIN_20DB ,PGA_GAIN_20DB);
    rfVgaDacBias(8, 8);
    //filter config
    rcHpfConfig(1, 1, RC_HPF_150KHZ, RC_HPF_150KHZ);
    tiaHpfConfig(1, 1, TIA_HPF_25KHZ, TIA_HPF_25KHZ);
    pgaLpfConfig(PGA_LPF_2M, PGA_LPF_2M);
    pgaHpfConfig(1, 1, PGA_HPF_25KHZ, PGA_HPF_25KHZ);
    
    adt3102AdcSetting(1, 1, 1, ADC_SAMPLE_RATE, CHIRP_NUM);
    
    //enable vga en_buffer
    RFC->rfc_vga0_reg |= 1<<9;
    //en_buffer0 contorl if signal output gpio23, 24, 25, 26.
    //en_buffer1 contorl if signal output gpio27, 28, 29, 30.
    //when we test low power consumption, we need to connect p29 with vddrf enable.
    //Normally, p29 is high resistance, vga1 en_buffer will lower the p29 level.
    RFC->rfc_vga1_reg |= 1<<9;
    
    //enable gpadc
//    gpadcEnable(GPADC_IN5);
//    RFC->rf_gpadc_reg_reg |= (1<<(7+1));
//    //PA0 DC Bias 0x10
//    RFC->rfc_tx_pa_reg |= 1<<2;
//    RFC->rfc_tx_pa_reg |= 2<<8;
//    RFC->rfc_tx_pa_reg &= ~(1<<2);
//    RFC->rfc_tx_pa_reg &= ~(2<<8);
//    
//    //20GHz buff DC Bias
//    RFC->rfc_tx_pa_reg |= 1<<6;
//    RFC->rfc_tx_pa_reg |= 1<<8;
//    RFC->rfc_tx_pa_reg &= ~(1<<6);
//    RFC->rfc_tx_pa_reg &= ~(1<<8);
//    
//    //doubler DC bias
//    RFC->rfc_tx_pa_reg |= 1<<7;
//    RFC->rfc_tx_pa_reg |= 2<<8;
//    RFC->rfc_tx_pa_reg &= ~(1<<7);
//    RFC->rfc_tx_pa_reg &= ~(2<<8);
//    
//    //PGA DC Bias 0x10
//    RFC->rfc_vga0_reg |= 1<<0|1<<1;
//    RFC->rfc_tx_pa_reg |= 0<<8;
//    RFC->rfc_vga0_reg &= ~(1<<0|1<<1);
//    

//    //bist DC
//    bistOn(BIST_AMP, BIST_DIV32_SEL);
//    RFC->rf_bist_cfg_reg |=(1<<24|1<<25); 
//    RFC->rf_bist_cfg_reg &=~(1<<24|1<<25);
//    
//    //ADC Vref
//    gpadcEnable(ADC_ATB_OUT);
//    RFC->rf_gpadc_reg_reg |= (1<<(7+1));
//    RFC->rf_adc0_regs_reg |= (1<<21|1<<19|1<<18|1<<17|1<<16);
//    RFC->rfc_tx_pa_reg |= 0<<8;
//    RFC->rf_adc0_regs_reg &= ~(1<<18|1<<17);
}
void fttestgpadcIN1gpadc(void)
{
    fttestgpadcIN1();
    delayUs(20000, HIGH_CLOCK);
    printf("the value of the adc is%d\r\n", (*(uint32*)0x400030d4)&0xff);
}
void fttestgpadcATBdcTestPA(void)
{
    //enable gpadc
    fttestgpadcIN1();
    gpadcEnable(GPADC_IN5);
    RFC->rf_gpadc_reg_reg |= (1<<(7+1));
    //PA0 DC Bias 0x10
    RFC->rfc_tx_pa_reg |= 1<<2;
    RFC->rfc_tx_pa_reg |= 2<<8;
}
void fttestgpadcATBdcTest20G(void)
{
    fttestgpadcIN1();
    gpadcEnable(GPADC_IN5);
    RFC->rf_gpadc_reg_reg |= (1<<(7+1));
    //20GHz buff DC Bias
    RFC->rfc_tx_pa_reg |= 1<<6;
    RFC->rfc_tx_pa_reg |= 1<<8;
}
void fttestgpadcATBdcTestDoubler(void)
{
    fttestgpadcIN1();
    gpadcEnable(GPADC_IN5);
    RFC->rf_gpadc_reg_reg |= (1<<(7+1));
    //doubler DC bias
    RFC->rfc_tx_pa_reg |= 1<<7;
    RFC->rfc_tx_pa_reg |= 2<<8;
}
void fttestgpadcATBdcTestPGA(void)
{
    fttestgpadcIN1();
     gpadcEnable(VGA_ATB_OUT);
    RFC->rf_gpadc_reg_reg |= (1<<(7+1));
    //PGA DC Bias 0x10
    RFC->rfc_vga0_reg |= 1<<13|1<<12|1<<0|1<<1;
}
void fttestgpadcATBdcTestADC(void)
{
    fttestgpadcIN1();
    gpadcEnable(ADC_ATB_OUT);
    RFC->rf_gpadc_reg_reg |= (1<<(7+1));
    RFC->rf_adc0_regs_reg |= (1<<21|1<<19|1<<18|1<<17|1<<16);
    RFC->rfc_tx_pa_reg |= 0<<8;
}
void fttestgpadcATBdcTestBist(void)
{
    fttestgpadcIN1();
    //bist Vref
    bistOn(BIST_AMP, BIST_DIV32_SEL);
    RFC->rf_bist_cfg_reg |=(1<<24|1<<25); 
}
//----------------------------------------------------------------------------
//for ft test doc 4.10
//----------------------------------------------------------------------------
void fttestVgaBist(void)
{
    testBist();
}
//----------------------------------------------------------------------------
//for ft test doc 4.11.1 and 4.11.2
//----------------------------------------------------------------------------
void fttestDut(void)
{
    test80GSingleFequenceDut();
}
void fttestMaster(void)
{
    test80GSingleFequenceMaster();
}
//----------------------------------------------------------------------------
//for ft test doc 4.11.3 and 4.11.4
//----------------------------------------------------------------------------
void fttestAdc(void)
{
    fmcwEnmode(DATA_SAMPLE_MODULE, SINGLE_MODE);
    volatile int cfarFlag=1;
    char cfarRangeTest[SAMPLE_POINT];
    memset(cfarRangeTest, 0 , sizeof(cfarRangeTest));
    uint32 targetValue;
    uint32 targetValuetotal;
    uint32 targetIndex;
    uint32 noiseValue;
    switch125MHclk();
    adcEnable(1, 1);
    adcSampleInit(1, 1);
    adcClkSwitch(ADC_SAMPLE_RATE);
    adcWnpara(1, 1, 512);
    //set 1/2 down sample.
    //50M Hclk, adc read buffer rate:50M/4.5/2(adc0 and adc1)=5.555M>8.333/2M
    adcFirDownsample(0, DOWN_SAMPLE);
    dataSampleConfig(INTERVALNUM, SAMPLE_END_NUM, SAMPLE_POINT, DOWN_SAMPLE, CHIRP_NUM);
    adcBiasSetting(8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8);
    for (int i = 0; i<5; i++)
    {
        adcSampleStart(ADC01SEL, SAMPLE_POINT, CHIRP_NUM, ADC0_FIFO, RNG_FFT_CH0_ADDR, ADC1_FIFO, RNG_FFT_CH1_ADDR);
    }
    adcSampleStart(ADC01SEL, SAMPLE_POINT, CHIRP_NUM, ADC0_FIFO, RNG_FFT_CH0_ADDR, ADC1_FIFO, RNG_FFT_CH1_ADDR);
    //save .\Data_process\Keilsave\adc0_data.hex 0x20008000,0x20018000
    //save .\Data_process\Keilsave\adc1_data.hex 0x20018000,0x20028000
    fftProcess(SAMPLE_POINT, CHIRP_NUM, RNG_FFT_CH0_ADDR ,RNG_FFT_CH0_ADDR, 4, 4); 
    fftProcess(SAMPLE_POINT, CHIRP_NUM, RNG_FFT_CH1_ADDR ,RNG_FFT_CH1_ADDR, 4, 4); 
    //save .\Data_process\Keilsave\fft0_data.hex 0x20008000,0x20018000
    absAdt3102(512, 32, 0x20008000, 0x20008000);
    delayUs(1000, HIGH_CLOCK);
    targetValue = accOdr(0x20008000+10*4, 32, 31);
    for(int detIndex=0; detIndex<SAMPLE_POINT/2; detIndex++)
    {
        if ((*((uint32 *)0x20008000 + detIndex)) == targetValue)
        {
            //printf("target point number:%d\r\n", detIndex);
            //printf("dut if signal frequecy:    %fMHz\r\n", 8.333/512.0*(float)detIndex);
            *(uint32*)0x20009000=detIndex;
            targetIndex = detIndex;
            break;
        }
    }
    *(uint64*)0x2001A000 = 0;
    *(uint64*)0x2001A010 = 0;
    for (int i=0; i<5; i++)
    {
        targetValuetotal = *((uint32 *)0x20008000 + targetIndex - 2 +i) + targetValuetotal;
    }
    *(uint64*)0x2001A000 = targetValuetotal;
    for (int i=0; i<SAMPLE_POINT; i++)
    {
        noiseValue = *((uint32 *)0x20008000 + i) + noiseValue;
    }
    *(uint64*)0x2001A010 = noiseValue-targetValuetotal;
    switch_to_50m();
}

void mbistMaster(void)
{
    //pull up iopad_dft_en
    gpioOutput(GPIO30, 1);
    //pull up iopad_20
    gpioOutput(GPIO20, 1);
    //pull down iopad_21
    gpioOutput(GPIO21, 0);
    //pull down iopad_04 ,te
    gpioOutput(GPIO22, 0);
    //reset
    gpioOutput(GPIO27, 1);
    delayUs(100, HIGH_CLOCK);
    gpioOutput(GPIO27, 0);
    delayUs(100, HIGH_CLOCK);
    gpioOutput(GPIO27, 1);
    //input 50M from master
    hclkOutput();

    
    delayUs(1000, HIGH_CLOCK);
    
    //pull up iopad_04 ,te
    gpioOutput(GPIO22, 1);
    
    gpioDirection(GPIO9, 1);
    gpioDirection(GPIO10, 1);
}




