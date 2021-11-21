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
#include "adt3102_tx.h"
#include "adt3102_rtc.h"
#include "adt3102_rx.h"
#include "adt3102_pmu.h"
#include "adt3102_adc.h"
#include "adt3102_dma.h"
#include "dsp_ctype_map.h"
#include "rfc_ctype_map.h"
#include "dma_ctype_map.h"
#include "pmu_ctype_map.h"
#include "adt3102_melbourne_test.h"
#include "adt3102_hangzhou_test.h"
#include "adt3102_system.h"
#include "adt3102_uart.h"
#include "gpio_ctype_map.h"
#include "adt3102_gpio.h"
//
//------Global variable declaration


//----------------------------------------------------------------------------
//melbourne test function
//----------------------------------------------------------------------------
void melbourneTest(void)
{
    MY_NVIC_SetVectorTable(0x00000000, 0x8000);
    //-----Local variable declaration-----
    //-----Initialize functions and variables and simulation part-----
//    setInterrupt(INT_UART0, ENABLEINT);
//    setInterrupt(INT_FFT, ENABLEINT);
    setInterrupt(INT_DUALTIMER, ENABLEINT);
    setInterrupt(INT_RTC, ENABLEINT);
//    setInterrupt(INT_TRNG, ENABLEINT);

    uartInit(UART1,115200, HIGH_CLOCK);
    //-----init some module-----
    printf("[hangzhou]adt3102 basic project test mode.\r\n");
	
		// for socket's master, Mbist test .
	  if(0) {
			for(int i=0;i<31;i=i+1)
			{
				if(i==4){
					gpioPullUpDown(4,1);  //P04 input with pullup, it's Flash's CSN.
				}
				else{
					gpioPullUpDown(i,0);  //all input with pulldown. 
				}
			}

			gpioOutput(22,0);	  //b_te		
			gpioOutput(27,0);   //reset		
			gpioOutput(21,0);   //dft_en, mbist.
			gpioOutput(20,1);	  //dft_en, mbist.
			gpioOutput(30,1);	  //dft_en, mbist. 
			delayUs(10,HIGH_CLOCK);
			
			gpioOutput(27,1);	

			PMU->pmu_clk_high_reg |= 1<<5; 		//hclk = 50m/4 = 12.5Mhz.
			delayUs(10,HIGH_CLOCK);
			hclkOutput();
			
			gpioOutput(22,1);  //b_te
			
			//wait mbist "done", and check mbist "fail"
			//PMU->pmu_clk_high_reg &= ~(1<<5); 
			while(1);
		}  //if(1) for socket's master, Mbist test . 
		
		
	
	  
    
	  
    if(1) {
        adt3102DigiInit();
        setInterrupt(INT_PMU, ENABLEINT);
        gpioDirection(9,0);
        gpioOutput(9,1);
        
        while(1) {
            rtcTimerStart(10000);
            gpioOutput(9,0);
            //delayUs(100,HIGH_CLOCK);
            mcuGotoSleep();
            gpioOutput(9,1);
            delayUs(500,HIGH_CLOCK);            
        }
        
    }        
        
        
        // test external clock for ADC, from P05 pin. 
    if(0){ 
        mcu_sleep_test(0);
//			testPower();
//			adcEnable(1, 1);						
//			delayUs(100,HIGH_CLOCK);

//			ExtClockP05ForADC();
//			// [7]: clkin_sel: ADC sampling clock selection: [0]=250MHz clock from PLL; [1]=250MHz clock from external clock source.
//			RFC->rf_adc0_regs_reg |= 1<<7;  
//			
//			// Now watching P02 by oscilloscope, it will be the adc's clk_out. 
//			adcClkOutput();
//			
//			while(1);
		}
		
		if(0){
	    //switch_to_50m();
	    //mcu_sleep_test_hangzhou(0);
	    //test20GSingleFequence();
	    //test20GFmcw();
	    //testChannelAtb();
	    //testDsp();
	    //hclkOutput();
	    //testBist();
			//while(1);
			//example set bit4 to 1, and keep others bits as previous value. 
			//*(uint32*)0x40020018 |= 1<<4;  // set bit4 to 1.
			//*(uint32*)0x40020018 &= ~(1<<4);  // clear bit4 to 0.
			//adcClkOutput();
			
			//---- switch mcu's hclk to 125mhz.
			hclkOutput(); //p02
			test20GSingleFequence();
			delayUs(1000,HIGH_CLOCK_50M);
			switch_to_plli();
			delayUs(1000,HIGH_CLOCK_125M);
			
			//---- switch end.
  	}
			
    if(1)
    {
        //the melbourne test code is written below
        //example();
			
				if(1){
					*(uint32*)0x4000c01c |= 1<<4; //manu_ctrl_en = 1. 
					
				}
				if(0){  // hf setting
					*(uint32*)0x40003110 = 0x08;     // bias1 reg1, <3>: dac<9:24> enable
//					*(uint32*)0x40003204 = 0x0000000E;            // plln 20g buf, test mode
//					*(uint32*)0x40003278 = 0x00000001;            // <0> enable pll output to 20G buf
//					*(uint32*)0x40003208 = 0x1f;     // 77G buf enable, <4> 1st stage, <3:2> tx0,1 en; <1:0> rx 0,1 en
//					*(uint32*)0x4000320c = 0x03;		 // PA0,1 enable
					*(uint32*)0x400030d8 = 0x2100;		 //gpadc, enable aio1 sw
//					*(uint32*)0x400031b8 = 0x00000110;     //	enable low noise bg			
//					*(uint32*)0x40003160 = 0x00001EF2;     // plli ctrl
//					*(uint32*)0x40003164 = 0x3F0114FF;     // enable 1G test buffer
//					*(uint32*)0x40003168 = 0x9A61093F;     // cp_offp=7, cp_offn=7
//					*(uint32*)0x4000316c = 0x0;            // atb
//		
//					*(uint32*)0x400031a4 = 0x7;            // enable plln ldo and bias					
//					*(uint32*)0x40003184 = 0x2520883F;            // plln enable,cp_off=1
//					*(uint32*)0x40003188 = 0x20044BBF;            // plln vco_sw=11, ibc_10G=00,cp_sw=7
//					*(uint32*)0x4000318c = 0x00000440;            // plln en_ld=1, mmd_dset=0
//					*(uint32*)0x40003198 = 0x00000830;            // plln ldo setting	
//					
//					*(uint32*)0x40003800 |= 1<<2;            // bypass delta-sigma
//					*(uint32*)0x40003838 = 0x0000001c;            // <6:2>: plln divider setting,Div<4:0>
				}
				if(0){  // yuan setting
					*(uint32*)0x400031b8 = 0x00000110;     //	enable low noise bg			
					*(uint32*)0x40003160 = 0x00001EF2;     // plli ctrl. external osc mode
					//*(uint32*)0x40003160 = 0x00001EBA;     // plli ctrl. internal osc mode, refclk_out=1
					*(uint32*)0x40003164 = 0x3F0114FF;     // plli reg1~4. enable 1G test buffer
					*(uint32*)0x40003168 = 0x9A61093F;     // plli reg5~8. cp_offp=7, cp_offn=7
					*(uint32*)0x4000316c = 0x0;            // plli reg9~10. atb
					//read 0x40003170 for plli lock status
					
					*(uint32*)0x400031a4 = 0x7;            // enable plln ldo and bias					
					*(uint32*)0x40003184 = 0xB420883F;            // plln reg1~4. plln enable,cp_off=5, lpf c3=00
					//*(uint32*)0x40003188 = 0x20044BBF;            // plln reg5~8. plln vco_sw=11, ibc_10G=00,cp_sw=7
					*(uint32*)0x40003188 = 0x200447BF;            // plln reg5~8. plln vco_sw=7, ibc_10G=00,cp_sw=7
					//*(uint32*)0x4000318c = 0x000004C9;            // plln reg9~12. plln en_ld=1, mmd_dset=1
					*(uint32*)0x4000318c = 0x000004C8;            // plln reg9~12. plln en_ld=1, mmd_dset=0
					*(uint32*)0x40003190 = 0x00000001;            // plln reg13. plln lpf c1					
					*(uint32*)0x40003198 = 0x00000830;            // plln ldo setting

					*(uint32*)0x40003800 |= 1<<2;            // bypass delta-sigma
					//*(uint32*)0x40003838 = 0x00000018;            // <6:2>: plln divider setting,Div<4:0>=6
					*(uint32*)0x40003838 = 0x00000020;            // <6:2>: plln divider setting,Div<4:0>=8
					//read 0x40003194 for plln lock status

					*(uint32*)0x40003110 = 0x08;     // bias1 reg1, <3>: dac<9:24> enable					
					*(uint32*)0x40003204 = 0x0000000E;            // plln 20g buf
					*(uint32*)0x40003278 = 0x00000001;            // <0> enable pll output to 20G buf					
				}
				
				if(1){  // yw setting
					// PLLI setting (Turn off YS setting)
					*(uint32*)0x400031b8 = 0x00000210;			// PLL's LDO & BG: Vbg=max; Eneable low-noise BG
					*(uint32*)0x40003160 = 0x00001eb2;      // PLLI ctrl
				//*(uint32*)0x40003164 = 0x3f0104ff;			// reg1 to reg4; adc_clk=250M
					*(uint32*)0x40003164 = 0x3f0106ff;			// reg1 to reg4; adc_clk=125M
					*(uint32*)0x40003168 = 0x9a61093f;			// reg5 to reg8
					
					*(uint32*)0x40003120 = 0x00000104;			// bias2_reg: set bias2 config and enable it
				//*(uint32*)0x400030d8 = 0x00004100;			// GPASC input mux selection
				//*(uint32*)0x400031b8 = 0x00000510;	    // LDO_BG:
					
				*(uint32*)0x4000c01c = 0x00000717;			// Turn on LDO181&2
				//adcClkOutput();  //P02

				*(uint32*)0x400030e0 = 0x00031a1a;			// Enable ADC0
			  *(uint32*)0x400030f8 = 0x00030000;			// Enable ADC1
				*(uint32*)0x40003274 = 0x00000030;			// Set Vref1=0.8V (default); Vref=Max (70)
				*(uint32*)0x40003278 = 0x00000030;    // Set Vref0=0.8V
				//--- turn off ADC0 and ADC1.
				//*(uint32*)0x400030e0 = 0x00001a1a;			// disable ADC0
				//*(uint32*)0x400030f8 = 0x00000000;			// disable ADC1
				//*(uint32*)0x4000c01c &= ~(1<<8 | 1<<9); // turn off LDO_BB1 and LDO_BB2.

				while(1) {
					testIfInputDspMel(); //P24 and P25	
				}					

 				}				
        //mcu_sleep_test(0);
    }
} 
/****************************************************************************************
*Function name : testDsp
*Description   : range fft, velocity fft, cfar, phase, angle ... test
****************************************************************************************/
void testIfInputDspMel(void)
{
    //open function test20GSingleFequence() outside while
    
    //bias2_en: enable signal for ztc bias for BB
    baseBandBiasSetting(ENABLE);
    
    GPIO->gpio_p23_config = 0x10;
    GPIO->gpio_p24_config = 0x10;    
    
    //en_adc_test
    RFC->rfc_vga0_reg |= 1<<10;
    RFC->rfc_vga1_reg |= 1<<10;
    
    adcEnable(1, 1);
    adcSampleInit(1,1);
    adcWnpara(1, 1, 512);
    //set 1/2 down sample.
    //50M Hclk, adc read buffer rate:50M/4.5/2(adc0 and adc1)=5.555M>8.333/2M
    //if you want to set 1/2 down sample and bypass fir, must be set the fir factor
    //after bypass fir, the downsample is 0 
    adcFirDownsample(0,DOWN_SAMPLE);
    dataSampleConfig(INTERVALNUM, 126, SAMPLE_POINT, DOWN_SAMPLE, CHIRP_NUM);
	  *(uint32*)0x4002204c=0x01;//bit0: adc free running mode
    adcBiasSetting(8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8);
    adcClkOutput();
    
    adcSampleStart(ADC01SEL,SAMPLE_POINT, CHIRP_NUM, ADC0_FIFO, ADC0_ADDRE, ADC1_FIFO, ADC1_ADDRE);
    //save .\Data_process\Keilsave\adc0_data.hex 0x20008000,0x20018000
    //save .\Data_process\Keilsave\adc1_data.hex 0x20018000,0x20028000
    //fftProcess(512, CHIRP_NUM, 0x20008000, 4, 4);
    //save .\Data_process\Keilsave\fft0_data.hex 0x20008000,0x20018000
}

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
//Here are the cases
//----------------------------------------------------------------------------
//Direct write register test adt3102
//for example
void example(void)
{

    *(uint32*)0x4000c01c = 0x78;
	
    

}



void mcu_sleep_test(int32 ram_on)
{
    
    
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
    //--- disable debugger's wake up function. 
    *(uint32*)0x4000c004 = 0x03;
    //soc_NVIC[soc_SETENA] = 0x00; //disable all interrupt. 
    //soc_NVIC[soc_SETENA] = 0xffffffff; //enable all interrupt. 
    soc_NVIC[soc_SETENA] = 0x080 ; //only PMU interrupt.

    //--- set gpio2 as wake up pad. 
    *(uint32*)0x40020040 = 0x30;  // p02 is input. 
    *(uint32*)0x40020018 = 1<<4;  // p02 wake up enable. 
    *(uint32*)0x40020014 = 0x00;  // p02 is high to wake up.  

    //--- set P29 as output, 1'h1.  
    *(uint32*)0x40020020 = 0x20000000;  // P29 output is high. .  
    *(uint32*)0x400200a4 = 0x01;  //    p29 as external LDO enable, for VDD_RF.  
             
    // ram_on = 0, sleep with PD_RAM off.
    // ram_on = 1, sleep with PD_RAM on, keep RAM powered on.
    //int32 regVal=0;

    //--- disable debugger's wake up function. 
    *(uint32*)0x4000c004 = 0x03;

    //--- MUST enable all of clock gating cells. Otherwise, the sleep will fail.  
    PMU->pmu_reg_ext_clk_en_reg = 0x0 ; 
    
    //  //--- MUST make sure PMU's state is idle before going to sleep. 
    //  //--- MUST check RTC's counter, making sure it will not wake up in 3 cycles. 

    //  regVal = PMU->pmu_pmu_state;
    //  regVal = regVal & (pmu_pmu_state_cur_state_mask << pmu_pmu_state_cur_state_shift);
    //  while( regVal != 0x00 )
    //  {
    //    regVal = PMU->pmu_pmu_state;
    //    regVal = regVal & (pmu_pmu_state_cur_state_mask << pmu_pmu_state_cur_state_shift);
    //  }

    // 0x010007 is for ASIC simulation, a short simulation time. 
    // 0x0100ff maybe for REAL CHIP.
    PMU->pmu_pmu_stage_time       = 0x0100ff;   

    //  // disable Manu mode, and configure sleeping registers.
    //  PMU->pmu_pmu_ctrl_reg         &= ~(1<<4) ;   
    //  //delay_us_rtl(10) ; //10us for ASIC simulation, 100us for REAL CHIP.
    //  delayUs(100,HIGH_CLOCK); 

    PMU->pmu_slp_xo50m_cfg        = 0x0 ;   
    PMU->pmu_slp_plli_ctrl_reg    = 0x0 ;   
    PMU->pmu_slp_ldo_bg_reg_reg   = 0x0 ;   
    PMU->pmu_slp_ldo12n_rega_reg  = 0x1800 ;   //0x400 is 0.7v. 
    if(ram_on) { 
    PMU->pmu_slp_power_ctrl_reg   = 0x1f ;
    }
    else {
    PMU->pmu_slp_power_ctrl_reg   = 0x07 ;
    }  

    //--- MCU is going to sleep. 
    *(uint32*)0xe000ed10 = 0x04;
    __asm("wfi");
    
    
    printf("sleep wake up\r\n ");
    //--- Now, MCU is waken up.  and you will see P03 as a square wave output. 
    *(uint32*)0x4002003C = 0x00;  // set p03 as output. 
    while(1) { 
        for(int32 i=0;i<10000;i++) {
            *(uint32*)0x40020020 = 1<<3;  // set p03 to 1. 
        }
        
        for(int32 i=0;i<10000;i++) {
            *(uint32*)0x40020024 = 1<<3;  // set p03 to 0. 
        }				
    }
            
}


void ExtClockP05ForADC(void)
{
	uint32 regVal;
	// set P05 as external adc clock input. 
	GPIO->gpio_p05_config = 0x4;
	
	// switch hclk to ext_adc_clk. 
	delayUs(100,HIGH_CLOCK);
	regVal = PMU->pmu_clk_high_reg;
	regVal &= ~(pmu_clk_high_reg_ctrl_high_clk_sel_mask << pmu_clk_high_reg_ctrl_high_clk_sel_shift);
	regVal |= 0x2 << pmu_clk_high_reg_ctrl_high_clk_sel_shift;
	PMU->pmu_clk_high_reg = regVal;
	
	// hclk output. 
	hclkOutput();
}








