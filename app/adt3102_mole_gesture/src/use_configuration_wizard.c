//--------------------------------------------------------------------
//Copyright(c)2020,Andar Technologise Inc.
//All Rights Reserved
//Confidential Property of Andar Technologies Inc.
//
//Module Description:
//  .The use_configuration_wizard.c file can be configured in the Configuration Wizard window
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
#include "adt3102_tx.h"
#include "adt3102_rx.h"
#include "adt3102_pmu.h"
#include "adt3102_adc.h"
#include "adt3102_dma.h"
#include "adt3102_system.h"
#include "dsp_ctype_map.h"
#include "rfc_ctype_map.h"
#include "dma_ctype_map.h"
//
//------Global variable declaration
//*** <<< Use Configuration Wizard in Context Menu >>> ***

// <h> Adt3102 Rf Config  
void adt3102RfUcw(void)
{
    plliDacSetting();
    plliConfig(ENABLE);
    pllnDacSetting();
    pllnConfig(ENABLE);
//  <e> Fmcw Buffer Mode
//   <o.0..1>   MODE_INTER <0=> MODE_INTER  <1=> MODE_EXTERN  <2=> MODE_CASCADED <3=> MODE_TEST
    fmcwModeSel(0x1);
//  </e>
    
//  <e> Rf Buffer 20G Enable
//   <q> buffer 20G Enable
//   <q1> doubler Enable    
    rfbuff20G(1, 1);
//  </e> 
    
//  <e> Bias 20G To 80G
//   <o.0..3> bias1_din15 ib_db77 <0=> 0 <1=> 1 <2=> 2   <3=> 3   <4=> 4   <5=> 5   <6=> 6   <7=> 7 
//                      <8=> 8 <9=> 9 <10=> 10 <11=> 11 <12=> 12 <13=> 13 <14=> 14 <15=> 15 
//   <o1.0..3> 	bias1_din14 ib_buf38  <0=> 0 <1=> 1 <2=> 2   <3=> 3   <4=> 4   <5=> 5   <6=> 6   <7=> 7 
//                      <8=> 8 <9=> 9 <10=> 10 <11=> 11 <12=> 12 <13=> 13 <14=> 14 <15=> 15 
//   <o2.0..3> bias1_din13 ib_db38  <0=> 0 <1=> 1 <2=> 2   <3=> 3   <4=> 4   <5=> 5   <6=> 6   <7=> 7 
//                      <8=> 8 <9=> 9 <10=> 10 <11=> 11 <12=> 12 <13=> 13 <14=> 14 <15=> 15 
//   <o3.0..3> 	bias1_din12 20G buf bias2  <0=> 0 <1=> 1 <2=> 2   <3=> 3   <4=> 4   <5=> 5   <6=> 6   <7=> 7 
//                      <8=> 8 <9=> 9 <10=> 10 <11=> 11 <12=> 12 <13=> 13 <14=> 14 <15=> 15 
//   <o4.0..3> 	bias1_din11 20G buf <0=> 0 <1=> 1 <2=> 2   <3=> 3   <4=> 4   <5=> 5   <6=> 6   <7=> 7 
//                      <8=> 8 <9=> 9 <10=> 10 <11=> 11 <12=> 12 <13=> 13 <14=> 14 <15=> 15 
//   <o5.0..3> bias1_din10 buf_1to4, tx loop <0=> 0 <1=> 1 <2=> 2   <3=> 3   <4=> 4   <5=> 5   <6=> 6   <7=> 7 
//                      <8=> 8 <9=> 9 <10=> 10 <11=> 11 <12=> 12 <13=> 13 <14=> 14 <15=> 15 
//   <o6.0..3> 	bias1_din9, buf_1to4, lo loop <0=> 0 <1=> 1 <2=> 2   <3=> 3   <4=> 4   <5=> 5   <6=> 6   <7=> 7 
//                      <8=> 8 <9=> 9 <10=> 10 <11=> 11 <12=> 12 <13=> 13 <14=> 14 <15=> 15 
//   <o7.0..3> bias1_din8 buf_1to4, rx loop  <0=> 0 <1=> 1 <2=> 2   <3=> 3   <4=> 4   <5=> 5   <6=> 6   <7=> 7 
//                      <8=> 8 <9=> 9 <10=> 10 <11=> 11 <12=> 12 <13=> 13 <14=> 14 <15=> 15 
    bias20GTo80G(8, 8, 8, 8, 8, 8, 8, 8);
//  </e>   

//  <e> Rf Buf77g Ctrl
//   <q> 77G buffer 1st stage enable 
//   <q1> tx0 buffer enable 
//   <q2> tx1 buffer enable
//   <q3> rx0 buffer enable 
//   <q4> rx1 buffer enable 
    rfBuff77GEnable(1, 1, 1, 1, 1);
//  </e>   

//  <e> Pa Enable
//   <q> Pa0 Enable 
//   <q1> Pa1 Enable
    txPaEnable(1, 1);
//  </e> 

//  <e> Pa Bias
//   <o.0..3> bias1_din1: ch1 PA <0=> 0 <1=> 1 <2=> 2   <3=> 3   <4=> 4   <5=> 5   <6=> 6   <7=> 7 
//                      <8=> 8 <9=> 9 <10=> 10 <11=> 11 <12=> 12 <13=> 13 <14=> 14 <15=> 15 
//   <o1.0..3> bias1_din0: ch0 PA  <0=> 0 <1=> 1 <2=> 2   <3=> 3   <4=> 4   <5=> 5   <6=> 6   <7=> 7 
//                      <8=> 8 <9=> 9 <10=> 10 <11=> 11 <12=> 12 <13=> 13 <14=> 14 <15=> 15 
    txPaBias(8, 8);
//  </e> 

    rfPhaseShift(0x1f, 0, 0x1f, 0);
    plliLock();
    pllnLock(FSTART, FM);
    
//  <e> Fmcw Config    
// <o>FSTART
//   <i>Default: 76G(Uint:GHz)
//   <76-80> 
// <o1>FM
//   <i>Default: 2000(Uint:MHz)
//   <0-4000>
// <o2>CHIRP_T0
//   <i>Default: 128(Uint:us)
//   <0-256>
// <o3>CHIRP_T1
//   <i>Default: 10(Uint:us)
//   <0-256>
// <o4>CHIRP_T2
//   <i>Default: 0(Uint:us)
//   <0-256>
// <o5>CHIRP_NUM
//   <i>Default: 32
//   <0-256>
    fmcwConfig(76, 3000, 128, 10, 0, 16);
//  </e>         

//  <e> Lna Enable
//   <q> Lna0 Enable 
//   <q1> Lna1 Enable
    rxLnaEnable(1, 1); 
//  </e> 

//  <e> Mixer Enable
//   <q> Mixer0 Enable 
//   <q1> Mixer1 Enable
    mixerEnable(1, 1);
//  </e> 

//  <e> Lna Bias Mode
//   <q> Ch0 regular bias mode Enable 
//   <q1> Ch1 regular bias mode Enable
    lnaRegularBias(1, 1);
//  </e>

//  <e> Lna Bias Setting
//   <o.0..3> bias1_din2: ch0, LNA bias1 <0=> 0 <1=> 1 <2=> 2   <3=> 3   <4=> 4   <5=> 5   <6=> 6   <7=> 7 
//                      <8=> 8 <9=> 9 <10=> 10 <11=> 11 <12=> 12 <13=> 13 <14=> 14 <15=> 15 
//   <o1.0..3>  bias1_din3: ch0, LNA bias2 <0=> 0 <1=> 1 <2=> 2   <3=> 3   <4=> 4   <5=> 5   <6=> 6   <7=> 7 
//                      <8=> 8 <9=> 9 <10=> 10 <11=> 11 <12=> 12 <13=> 13 <14=> 14 <15=> 15 
//   <o2.0..3>  bias1_din4: ch0, LNA bias3 <0=> 0 <1=> 1 <2=> 2   <3=> 3   <4=> 4   <5=> 5   <6=> 6   <7=> 7 
//                      <8=> 8 <9=> 9 <10=> 10 <11=> 11 <12=> 12 <13=> 13 <14=> 14 <15=> 15 
//   <o3.0..3>  bias1_din5: ch1, LNA bias1 <0=> 0 <1=> 1 <2=> 2   <3=> 3   <4=> 4   <5=> 5   <6=> 6   <7=> 7 
//                      <8=> 8 <9=> 9 <10=> 10 <11=> 11 <12=> 12 <13=> 13 <14=> 14 <15=> 15 
//   <o4.0..3> 	bias1_din6: ch1, LNA bias2 <0=> 0 <1=> 1 <2=> 2   <3=> 3   <4=> 4   <5=> 5   <6=> 6   <7=> 7 
//                      <8=> 8 <9=> 9 <10=> 10 <11=> 11 <12=> 12 <13=> 13 <14=> 14 <15=> 15 
//   <o5.0..3>  bias1_din7: ch1, LNA bias3 <0=> 0 <1=> 1 <2=> 2   <3=> 3   <4=> 4   <5=> 5   <6=> 6   <7=> 7 
//                      <8=> 8 <9=> 9 <10=> 10 <11=> 11 <12=> 12 <13=> 13 <14=> 14 <15=> 15 
    lnaDacBias(9, 9, 9, 9, 9, 9); 
//  </e>    
}
// </h> 

// <h> Adt3102 Analog Config 
void adt3102AnalogUcw(void)
{ 
//  <e> ib power dector bias
//   <o.0..3> <0=> 0 <1=> 1 <2=> 2   <3=> 3   <4=> 4   <5=> 5   <6=> 6   <7=> 7 
//            <8=> 8 <9=> 9 <10=> 10 <11=> 11 <12=> 12 <13=> 13 <14=> 14 <15=> 15  
    biasPowerdector(8);
//  </e>
    
//  <e> Bist bias Setting
//   <o.0..3> bias1_din17, bist IPTAT0 <0=> 0 <1=> 1 <2=> 2   <3=> 3   <4=> 4   <5=> 5   <6=> 6   <7=> 7 
//                      <8=> 8 <9=> 9 <10=> 10 <11=> 11 <12=> 12 <13=> 13 <14=> 14 <15=> 15 
//   <o1.0..3> bias1_din18, bist IPTAT1 <0=> 0 <1=> 1 <2=> 2   <3=> 3   <4=> 4   <5=> 5   <6=> 6   <7=> 7 
//                      <8=> 8 <9=> 9 <10=> 10 <11=> 11 <12=> 12 <13=> 13 <14=> 14 <15=> 15 
//   <o2.0..3> bias2_din17: Bist bias0 <0=> 0 <1=> 1 <2=> 2   <3=> 3   <4=> 4   <5=> 5   <6=> 6   <7=> 7 
//                      <8=> 8 <9=> 9 <10=> 10 <11=> 11 <12=> 12 <13=> 13 <14=> 14 <15=> 15 
//   <o3.0..3> bias2_din18: Bist bias1 <0=> 0 <1=> 1 <2=> 2   <3=> 3   <4=> 4   <5=> 5   <6=> 6   <7=> 7 
//                      <8=> 8 <9=> 9 <10=> 10 <11=> 11 <12=> 12 <13=> 13 <14=> 14 <15=> 15 
    biasBist(8, 8 ,8 ,8);
//  </e>    
   
//  <e> Gpadc bias Setting
//   <o.0..3> BB bias iout20 for GPADC test <0=> 0 <1=> 1 <2=> 2   <3=> 3   <4=> 4   <5=> 5   <6=> 6   <7=> 7 
//                      <8=> 8 <9=> 9 <10=> 10 <11=> 11 <12=> 12 <13=> 13 <14=> 14 <15=> 15 
//   <o1.0..3> GPADC bias <0=> 0 <1=> 1 <2=> 2   <3=> 3   <4=> 4   <5=> 5   <6=> 6   <7=> 7 
//                      <8=> 8 <9=> 9 <10=> 10 <11=> 11 <12=> 12 <13=> 13 <14=> 14 <15=> 15     
    biasGpadc(8, 8, 8);
//  </e>      
    
//  <e> Tia Enable
//   <q> Tia0 Enable 
//   <q1> Tia1 Enable
    rfTiaEnable(1, 1);
//  </e> 

//  <e> Tia Gain Setting
//   <o.0..3> Tia0I diff gain setting <0=> 0 <1=> 1 <2=> 2   <3=> 3   <4=> 4   <5=> 5   <6=> 6   <7=> 7 
//                      <8=> 8 <9=> 9 <10=> 10 <11=> 11 <12=> 12 <13=> 13 <14=> 14 <15=> 15 
//   <o1.0..3> Tia0Q diff gain setting <0=> 0 <1=> 1 <2=> 2   <3=> 3   <4=> 4   <5=> 5   <6=> 6   <7=> 7 
//                      <8=> 8 <9=> 9 <10=> 10 <11=> 11 <12=> 12 <13=> 13 <14=> 14 <15=> 15 
//   <o2.0..3> Tia1I diff gain setting <0=> 0 <1=> 1 <2=> 2   <3=> 3   <4=> 4   <5=> 5   <6=> 6   <7=> 7 
//                      <8=> 8 <9=> 9 <10=> 10 <11=> 11 <12=> 12 <13=> 13 <14=> 14 <15=> 15 
//   <o3.0..3> Tia1Q diff gain setting <0=> 0 <1=> 1 <2=> 2   <3=> 3   <4=> 4   <5=> 5   <6=> 6   <7=> 7 
//                      <8=> 8 <9=> 9 <10=> 10 <11=> 11 <12=> 12 <13=> 13 <14=> 14 <15=> 15 
    rfTiaGainSetting(8, 8 ,8 ,8);
//  </e> 

//  <e> Tia Bias Setting
//   <o.0..3>  tiaI bias <0=> 0 <1=> 1 <2=> 2   <3=> 3   <4=> 4   <5=> 5   <6=> 6   <7=> 7 
//                      <8=> 8 <9=> 9 <10=> 10 <11=> 11 <12=> 12 <13=> 13 <14=> 14 <15=> 15 
//   <o1.0..3> tiaQ bias <0=> 0 <1=> 1 <2=> 2   <3=> 3   <4=> 4   <5=> 5   <6=> 6   <7=> 7 
//                      <8=> 8 <9=> 9 <10=> 10 <11=> 11 <12=> 12 <13=> 13 <14=> 14 <15=> 15     
    rfTiaDacBias(10, 10);
//  </e>
    
//  <e> Pga calibration gain Enable
//   <q>  Pga0I calibration gain Enable
//   <q1> Pga0Q calibration gain Enable
//   <q2> Pga1I calibration gain Enable
//   <q3> Pga1Q calibration gain Enable  
    rfPgaGainCalEnable(1, 1, 1, 1);
//  </e>

//  <e>  Pga calibration gain Setting
//   <o.0..3> Pga0I calibration gain setting <0=> 0 <1=> 1 <2=> 2   <3=> 3   <4=> 4   <5=> 5   <6=> 6   <7=> 7 
//                      <8=> 8 <9=> 9 <10=> 10 <11=> 11 <12=> 12 <13=> 13 <14=> 14 <15=> 15 
//   <o1.0..3> Pga0Q calibration gain setting <0=> 0 <1=> 1 <2=> 2   <3=> 3   <4=> 4   <5=> 5   <6=> 6   <7=> 7 
//                      <8=> 8 <9=> 9 <10=> 10 <11=> 11 <12=> 12 <13=> 13 <14=> 14 <15=> 15 
//   <o2.0..3> Pga1I calibration gain setting <0=> 0 <1=> 1 <2=> 2   <3=> 3   <4=> 4   <5=> 5   <6=> 6   <7=> 7 
//                      <8=> 8 <9=> 9 <10=> 10 <11=> 11 <12=> 12 <13=> 13 <14=> 14 <15=> 15 
//   <o3.0..3> Pga1Q calibration gain setting <0=> 0 <1=> 1 <2=> 2   <3=> 3   <4=> 4   <5=> 5   <6=> 6   <7=> 7 
//                      <8=> 8 <9=> 9 <10=> 10 <11=> 11 <12=> 12 <13=> 13 <14=> 14 <15=> 15 
    rfPgaGainCalSetting(8, 8 ,8 ,8);
//  </e>    

//  <e> Pga Enable
//   <q> Pga0 Enable 
//   <q1> Pga1 Enable
    rfVgaEnable(1, 1);
//  </e> 

//  <e> Pga Gain Setting
//   <o.0..3> Pga0I diff gain setting <0=> 0 <1=> 1 <2=> 2   <3=> 3   <4=> 4   <5=> 5   <6=> 6   <7=> 7 
//                      <8=> 8 <9=> 9 <10=> 10 <11=> 11 <12=> 12 <13=> 13 <14=> 14 <15=> 15 
//   <o1.0..3> Pga0Q diff gain setting <0=> 0 <1=> 1 <2=> 2   <3=> 3   <4=> 4   <5=> 5   <6=> 6   <7=> 7 
//                      <8=> 8 <9=> 9 <10=> 10 <11=> 11 <12=> 12 <13=> 13 <14=> 14 <15=> 15 
//   <o2.0..3> Pga1I diff gain setting <0=> 0 <1=> 1 <2=> 2   <3=> 3   <4=> 4   <5=> 5   <6=> 6   <7=> 7 
//                      <8=> 8 <9=> 9 <10=> 10 <11=> 11 <12=> 12 <13=> 13 <14=> 14 <15=> 15 
//   <o3.0..3> Pga1Q diff gain setting <0=> 0 <1=> 1 <2=> 2   <3=> 3   <4=> 4   <5=> 5   <6=> 6   <7=> 7 
//                      <8=> 8 <9=> 9 <10=> 10 <11=> 11 <12=> 12 <13=> 13 <14=> 14 <15=> 15 
    rfVgaGainSetting(8, 8 ,8 ,8); 
//  </e>   

//  <e> Pga Bias Setting
//   <o.0..3>  PgaI bias <0=> 0 <1=> 1 <2=> 2   <3=> 3   <4=> 4   <5=> 5   <6=> 6   <7=> 7 
//                      <8=> 8 <9=> 9 <10=> 10 <11=> 11 <12=> 12 <13=> 13 <14=> 14 <15=> 15 
//   <o1.0..3> PgaQ bias <0=> 0 <1=> 1 <2=> 2   <3=> 3   <4=> 4   <5=> 5   <6=> 6   <7=> 7 
//                      <8=> 8 <9=> 9 <10=> 10 <11=> 11 <12=> 12 <13=> 13 <14=> 14 <15=> 15 
    rfVgaDacBias(8, 8);    
//  </e>

//  <e> Tia Hpf Config
//   <o.0..3>  tia0 Hpf Config <0=> TIA_HPF_25KHZ   <150=> TIA_HPF_50KHZ <1=> TIA_HPF_100KHZ   
//                             <2=> TIA_HPF_150KHZ  <3=> TIA_HPF_300KHZ  <400=> TIA_HPF_400KHZ
//   <o1.0..3>  tia1 Hpf Config <0=> TIA_HPF_25KHZ   <150=> TIA_HPF_50KHZ <1=> TIA_HPF_100KHZ   
//                             <2=> TIA_HPF_150KHZ  <3=> TIA_HPF_300KHZ  <400=> TIA_HPF_400KHZ
    tiaHpfConfig(1, 1, 2, 2);
//  </e>    

//  <e> Pga Lpf Config
//   <o.0..1>  pga0 Lpf Config <0=> PGA_LPF_2M   <1=> PGA_LPF_4M 
//                             <2=> PGA_LPF_6M   <3=> PGA_LPF_8M  
//   <o1.0..1>  pga1 Lpf Config <0=> PGA_LPF_2M  <1=> PGA_LPF_4M 
//                             <2=> PGA_LPF_6M   <3=> PGA_LPF_8M    
    pgaLpfConfig(1, 1);
//  </e>     
    
//  <e> Pga Hpf Config
//   <o.0..1>  pga0 Hpf Config  <0=> PGA_HPF_25KHZ    <1=> PGA_HPF_50KHZ 
//                              <2=> PGA_HPF_100KHZ   <3=> PGA_HPF_500KHZ  
//   <o1.0..1>  pga1 Hpf Config <0=> PGA_HPF_25KHZ   <1=> PGA_HPF_50KHZ 
//                              <2=> PGA_HPF_100KHZ   <3=> PGA_HPF_500KHZ    
    pgaHpfConfig(1, 1, 1, 1); 
//  </e>
}
// </h> 

// <h> Adt3102 Power Config 
void adt3102PowerUcw(void)
{   
//  <e> Plli Ldo Enable
//   <q> Vco Enable 
//   <q1> Pll Enable    
    plliLDOEnable(1, 2);
//  </e>      

//  <e> Plln Vco Ldo Enable  
//   <q> Pll Vco Ldo Enable
//   <o1.0..1> Pll Vco Ldo Setting <0=> 1.8v <1=> 2.0v <2=> 2.2v <3=> 2.5v  
    pllnVcoConfig(1, 0);
//  </e>      
    
//  <e> Plln Pll Ldo Enable  
//   <q> Pll Ldo Enable
//   <o1.0..3> Pll Ldo Setting <0=> 1.8v <1=> 2.0v <2=> 2.2v <3=> 2.5v 
    pllnPllConfig(1, 0, 1);
//  </e>  
    
//  <e> Baseband Ldo Setting  
//   <q> Baseband0 Ldo Enable
//   <o1.0..1> Baseband0 Setting <0=> 1.8v <1=> 2.0v <2=> 2.2v <3=> 2.5v
//   <q> Baseband1 Ldo Enable
//   <o1.0..1> Baseband1 Setting <0=> 1.8v <1=> 2.0v <2=> 2.2v <3=> 2.5v  
    baseband18LDO(1, 0, 1, 0);
//  </e>    
  
//  <e> Dvdd12 High Power Ldo Setting 
//   <q> Dvdd12 High Power Ldo Power On
//   <o1.0..2> Dvdd12 High Power Setting <0=> 1.2v <1=> 0.7v <2=> 0.75v <3=> 0.8v 
//                                       <4=> 0.85v <5=> 1.0v <6=> 1.1v <7=> 1.3v
    dvdd12HpLDO(1, 0);
//  </e>  

//  <e> Dvdd12 Low Power Ldo Setting 
//   <q> Dvdd12 Low Power Ldo Power On
//   <o1.0..2> Dvdd12 Low Power Setting <0=> 1.2v <1=> 0.7v <2=> 0.75v <3=> 0.8v 
//                                      <4=> 0.85v <5=> 1.0v <6=> 1.1v <7=> 1.3v
//    dvdd12LpLDO(1, 0);
//  </e>

//  <e> Dac enalbe Bias1 Power 
//   <q> Dac enalbe 
//   <q1> Bias1 Power '0' enable, '1' disable.
    dacEnBias1Pd(1, 0);
//  </e>
}
// </h> 
//*** <<< end of configuration section >>>    ***














