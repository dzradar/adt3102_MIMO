#ifndef __PMU__H
#define __PMU__H
#include "adt3102_type_define.h"
/****************************************************************************************
*Function name : adt3102PowerSw
*Description   : adt3102 power setting
*Paramater	   :
*        @ onOroff: power on plli pll, plli vco, plln pll, plln vco, bb1 and bb2, bias1 and so on
*        @ :   
*Paramater	selection:		

*Power consumption : None
*Return        :
****************************************************************************************/
void adt3102PowerSw(int onOroff);
/****************************************************************************************
*Function name : switch_to_50m
*Description   : select high_clk as 50MHz
*Paramater	   : None
*        @ :  
*        @ :   
*Paramater	selection:		None

*Power consumption : None
*Return        :
****************************************************************************************/
void switch_to_50m(void);
/****************************************************************************************
*Function name : switch_to_plli
*Description   : select high_clk as 125MHz
*Paramater	   : None
*        @ :  
*        @ :   
*Paramater	selection:		None

*Power consumption : None
*Return        :
****************************************************************************************/
void switch_to_plli(void);
/****************************************************************************************
*Function name : mcuGotoSleep
*Description   : adt3102 mcu go to sleep
*Paramater	   : None
*        @ :  
*        @ :   
*Paramater	selection:		None

*Power consumption : None
*Return        :
****************************************************************************************/
void mcuGotoSleep(void);
/****************************************************************************************
*Function name : mcuSleepConfig
*Description   : adt3102 setting for entering sleep mode
*Paramater	   : None
*        @ :  
*        @ :   
*Paramater	selection:		None

*Power consumption : None
*Return        :
****************************************************************************************/
void mcuSleepConfig(int32 ramOn);
/****************************************************************************************
*Function name : mcuGotoSleep
*Description   : adt3102 mcu go to idle mode
*Paramater	   : None
*        @ :  
*        @ :   
*Paramater	selection:		None

*Power consumption : None
*Return        :
****************************************************************************************/
void mcu_idle(void);
/****************************************************************************************
*Function name : lowPowerSwitchPll
*Description   : adt3102 low power mode, open or close plli or plln
*Paramater	   : None
*        @plliOn :  open or close plli vco
*        @pllnOn :  open or close plln vco
*Paramater	selection:		None

*Power consumption : None
*Return        :
****************************************************************************************/
void lowPowerSwitchPll(int32 plliOn,int32 pllnOn);
/****************************************************************************************
*Function name : lowPowerSwitchFast
*Description   : adt3102 in the low power consumption mode, the module that can not be 
                    switched in a short time needs a long stable time
*Paramater	   : None
*        @rx0En     :  rx0 enable
*        @rx1En     :  rx1 enable
*        @tx0En     :  tx0 enable
*        @tx1En     :  tx1 enable
*        @tiaHpf    :  tia hpf enable(tia0 and tia1)
*        @rcHpf     :  rc hpf enable(rx0 and rx1)
*        @pgaHpf    :  pga hpf enable(pga0 and pga1)
*Paramater	selection:		None

*Power consumption : None
*Return        :
****************************************************************************************/
void lowPowerSwitchFast(int32 rx0En, int32 rx1En, int32 tx0En, int32 tx1En, uint32 tiaHpf,uint32 rcHpf, uint32 pgaHpf);
/****************************************************************************************
*Function name : lowPowerSwitchSlow
*Description   : adt3102 in the low power consumption mode, the module that can be 
                    switched in a short time needs a short stable time
*Paramater	   : None
*        @rx0En     :  rx0 enable
*        @rx1En     :  rx1 enable
*        @tx0En     :  tx0 enable
*        @tx1En     :  tx1 enable
*Paramater	selection:		None

*Power consumption : None
*Return        :
****************************************************************************************/
void lowPowerSwitchSlow(int32 rx0En, int32 rx1En, int32 tx0En, int32 tx1En);
/****************************************************************************************
*Function name : adt3102GpioHz
*Description   : set adt3102 gpio is hz
*Paramater	   :None
*        @ :  
*        @ :   
*Paramater	selection:	None	

*Power consumption : None
*Return        :None
****************************************************************************************/
void adt3102GpioHz(void);
/****************************************************************************************
*Function name : rc32kOutputP30
*Description   : rc 32KHz osc output to gpio30
*Paramater	   :None
*        @ :  
*        @ :   
*Paramater	selection:	None	

*Power consumption : None
*Return        :None
****************************************************************************************/
void rc32kOutputP30(void);
/****************************************************************************************
*Function name : pmuGpioWakeup
*Description   : setting gpio for mcu weekup 
*Paramater	   :None
*        @pinNumber :  gpio number id, 0~30
*        @pinPolarity :   wake up edge of gpio
*Paramater	selection:	None	

*Power consumption : None
*Return        :None
****************************************************************************************/
void pmuGpioWakeup(uint32 pinNumber, uint32 pinPolarity);
/****************************************************************************************
*Function name : rc32kCalibration
*Description   : adjust the calibration parameters to make rc32KHz close to 32.768KHz 
*Paramater	   :None
*Paramater	selection:	None	

*Power consumption : None
*Return        :None
****************************************************************************************/
void rc32kCalibration(void);

/****************************************************************************************
*Function name : adcRawOutput
*Description   : ADC raw data output,  via P2(clk), P18~P7(bit[11:0]), P19(head).
               : 12bit format, please refer to pin mux excel document for more details. 
               : 1: enable.  0: disable. 
*Paramater	   : None
*Return        : None
****************************************************************************************/
void adcRawOutput(int enable);
#endif






