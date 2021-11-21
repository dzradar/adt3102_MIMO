//--------------------------------------------------------------------
//Copyright(c)2020,Andar Technologise Inc.
//All Rights Reserved
//Confidential Property of Andar Technologies Inc.
//
//Module Description: Radar switcher for ADT3102 Hare DEV board
//
//Created by :JiangQi
//$Revision: 0.1
//$Data: 2021/3/25
//--------------------------------------------------------------------
//
//------All include header files
#include "CM3DS_MPS2.h"
#include "stdio.h"
#include "math.h"
#include "adt3102.h"
#include "adt3102_system.h"
#include "adt3102_tx.h"
#include "adt3102_rx.h"
#include "adt3102_pmu.h"
#include "adt3102_timer.h"
#include "adt3102_dsp.h"
#include "adt3102_gpio.h"
#include "adt3102_memscheduling.h"
#include "adt3102_phase_cal.h"
#include "adt3102_type_define.h"
#include "adt3102_uart.h"
#include "adt3102_system.h"
#include "adt3102_rtc.h"
#include "adt3102_sagc.h"
#include "adt3102_trng.h"
#include "adt3102_watchdog.h"
#include "pmu_ctype_map.h"
#include "rfc_ctype_map.h"
#include "dsp_ctype_map.h"
#include "rtc_ctype_map.h"
#include "static_remove.h"
#include "gpio_ctype_map.h"
#include "adt3102_uart.h"
#include "TinyFrame.h"
#include "adt3102_utilities.h"
#include "adt3102_proc_gesture.h"
#include "peak_single_mimo.h"


//------Global variable declaration
int32 g_jlinkDataFlag __attribute__((at(0x20005800)));
uint8 g_actGoing;
int32 g_rx0En=1;
int32 g_rx1En=1;
int32 g_tx0En=1;
int32 g_tx1En=1;
uint32 g_tiaGain0=TIA_GAIN_6DB; //0,6,12,18
uint32 g_pgaGain0=PGA_GAIN_44DB;//PGA_GAIN_30DB;
uint32 g_tiaGain1=TIA_GAIN_6DB;
uint32 g_pgaGain1=PGA_GAIN_44DB;//PGA_GAIN_30DB;
uint32 g_tiaHpfEn=0;
uint32 g_rcHpfEn=1;
uint32 g_pgaHpfEn=1;
uint32 g_tiaHpf=TIA_HPF_100KHZ; //25,50,100,300
uint32 g_rcHpf=RC_HPF_150KHZ;   //150, 400
uint32 g_pgaHpf=PGA_HPF_50KHZ; //25,50,100,500
uint32 g_pgaLpf=PGA_LPF_4M;
uint32 g_PaGain0=14;
uint32 g_PaGain1=14;
uint32 g_phaseShftLut0=0;
uint32 g_phaseShftLut1=0;
volatile int32 g_sendCount = 0;
uint32 g_absMergeArray[RANGE_MAX][CHIRP_NUM/2] __attribute__((at(ABS_MEAN_RV_ADDR)));
#if REMOVE_FIRST_CHIRP
uint32 g_discardChirpReserve0[SAMPLE_POINT] __attribute__((at(RNG_FFT_CH0_ADDR-SAMPLE_POINT*4)));
uint32 g_discardChirpReserve1[SAMPLE_POINT] __attribute__((at(RNG_FFT_CH1_ADDR-SAMPLE_POINT*4))); //overlap with end of ch0
#endif

uint32 rva_range_average_noise_v0[RVA_MAX_NSAMPLES_U16]  __attribute__((at(NOISE_V0_ADD)));
uint32 rva_range_average_noise_vm[RVA_MAX_NSAMPLES_U16]  __attribute__((at(NOISE_VM_ADD)));
//uint32 rva_range_average_noise[2][RVA_MAX_NSAMPLES_U16] __attribute__((at(NOISE_V0_ADD)));

//float echo_u_ch2[RVA_MAX_NSAMPLES_U16][RVA_HISTORY_SIZE_U16] __attribute__((at(DOP_FFT_CH0_EVEN_ADDR)));
//float echo_u_ch3[RVA_MAX_NSAMPLES_U16][RVA_HISTORY_SIZE_U16] __attribute__((at(DOP_FFT_CH1_ODD_ADDR)));

RV_List rv_list __attribute__((at(RV_LIST_ADDR)));
Raw_Target_List target_list __attribute__((at(TARGET_LIST_ADDR)));

uint32 cfar_window[cfar_window_size_u16];

//static float cali_phase[4] = {0, -2.0013, -5.8067, -1.3927};  // board 906
//static float cali_phase[4] = {0, -2.1822, 0.1006, -1.9484};
//static float cali_phase[4] = {0, 0.9, 0.6224, 1.3877};  // mimo azimuth  709
//static float cali_phase[4] = {0,0.8704,0.8725,1.7471};  // Board 814
static float cali_phase[4] = { 0,-2.1461, 0.2007, -1.8289};  // Board 602 ?
float r_cali[4], i_cali[4];

uint8 initial_flag = 1;
float iqcoef = 0.001f;
uint8 update_flag = 1;
uint8 bg_estimate_time = 10; //100 -> 5s
volatile uint32 g_frameStart=0;
int main(void)
{
    //-----Local variable declaration-----
//    int32 powerThd= -30;//-32; //in dBFS, must be negative. Ref to 60db IFgain
//    int32 powerThdAdj;
//    uint32 dopMin=2;
//    uint32 firstPeakOnly=0;
//    uint32 distComp=0;
//    uint32 curTiaGain;
//    uint32 curVgaGain;
//	uint8  ledCount = 0;
//    OBJ_PEAKSINGLE_TypeDef obj;	
    uint32 sagcTarget; 
    uint32 framePeriod=FRAME_PERIOD_ACT;
    const char VER[4]="v1.1";
    MY_NVIC_SetVectorTable(0x00000000, 0x8000);
    
    //-----Initialize some basic functions -----
    setInterrupt(INT_UART1, DISABLEINT);  //disable
    setInterrupt(INT_TIMER0, ENABLEINT);
    setInterrupt(INT_TIMER1, ENABLEINT);
    setInterrupt(INT_DUALTIMER, ENABLEINT);
    setInterrupt(INT_FFT, ENABLEINT);
    setInterrupt(INT_PMU, ENABLEINT);
    //LED init

    //-----init some module-----
    //power on
    adt3102PowerSw(ON); 
    //turn on RF and analog, config adc  
    adt3102DigiInit();    
    ledInit();
	ledAllon();	
	#if REMOVE_FIRST_CHIRP
        adt3102RfInit(g_rx0En, g_rx1En, g_tx0En, g_tx1En, g_phaseShftLut0, g_phaseShftLut1,CHIRP_NUM+1);
	#else
        adt3102RfInit(g_rx0En, g_rx1En, g_tx0En, g_tx1En, g_phaseShftLut0, g_phaseShftLut1,CHIRP_NUM);
	#endif
    adt3102Analog();
    #if REMOVE_FIRST_CHIRP
        adt3102AdcSetting(g_rx0En, g_rx1En,1,ADC_SAMPLE_RATE,CHIRP_NUM+1);
	#else
        adt3102AdcSetting(g_rx0En, g_rx1En,1,ADC_SAMPLE_RATE,CHIRP_NUM);
	#endif
    if(HIGH_CLOCK==HIGH_CLOCK_125M)
    {
        switch125MHclk();
    }
    uartInit(UART0,1384200, HIGH_CLOCK);
    printf("ADT3102 gesture detection %s \r\n", VER);  
    timerInit(Timer1,(float)framePeriod,HIGH_CLOCK);
    gpioDirection(9,0);
    gpioDirection(2,0);
	ledAlloff();	
    

    
    target_list.magicword[0] = 0x0102;
    target_list.magicword[1] = 0x0304;
    target_list.magicword[2] = 0x0506;
    target_list.magicword[3] = 0x0708;
    target_list.framenumber = 1;
            
    for (int i = 0; i< 4; i++)
    {
        r_cali[i] = cos(cali_phase[i]);
        i_cali[i] = sin(cali_phase[i]);
    }
    
    // generate hamming filter
    // range filter
    gen_hamm_filter(HAMM_WIN_RANGE, RVA_MAX_NSAMPLES_U16);
    // doppler filter
    gen_hamm_filter(HAMM_WIN_DOPPLER, RVA_HISTORY_SIZE_U16);
    
    //single tone
    //fmcwChirpSingleFrequency(); 
    //bistOn(BIST_AMP, BIST_DIV128_SEL);
    //siggen(0x1000, 0x0000, 0, 1, 0); DSP->ds_adc_format_ctrl = ds_adc_format_ctrl_offset_auto_bit;//uint16 step, uint16 chgStep, uint16 comp, uint16 siggenEn, uint16 ang
    while(1)//repeat sending chirps
    {
        //Frame rate control using timer0
        while(g_frameStart==0);
        g_frameStart=0;
        timerInit(Timer1,(float)framePeriod,HIGH_CLOCK);
        #if FRAME_LOWPOWER
            lowPowerSwitchPll(1,1);
            lowPowerSwitchFast(g_rx0En, g_rx1En, g_tx0En, g_tx1En, g_tiaHpf,g_rcHpf,g_pgaHpf);                
			//rfPhaseShiftLUT(g_phaseShftLut0, g_phaseShftLut1);
        #endif
        if (PA1_ONLY==1)
        {
            //enable PA0 and disable PA1
            txPaEnable(DISABLE, ENABLE);    
        }
        else
        {
            //enable PA1 and disable PA0
            txPaEnable(ENABLE, DISABLE);
        } 
        #if REMOVE_FIRST_CHIRP
            g_sendCount=-1;
            timerInit(Timer0,CHIRP_PERIOD,HIGH_CLOCK);
            adcSampleStart(ADC01SEL, SAMPLE_POINT, CHIRP_NUM+1, ADC0_FIFO, RNG_FFT_CH0_ADDR-SAMPLE_POINT*4, ADC1_FIFO, RNG_FFT_CH1_ADDR-SAMPLE_POINT*4);   
        #else        
            g_sendCount=0;
	        timer0Init(CHIRP_PERIOD);
            adcSampleStart(ADC01SEL, SAMPLE_POINT, CHIRP_NUM, ADC0_FIFO, RNG_FFT_CH0_ADDR, ADC1_FIFO, RNG_FFT_CH1_ADDR); 
        #endif
        timerOff(Timer0);
        #if FRAME_LOWPOWER
            lowPowerSwitchPll(1,0);
            lowPowerSwitchFast(0, 0, 0, 0, g_tiaHpf,g_rcHpf,g_pgaHpf);
        #endif
//        curTiaGain=g_tiaGain1;
//        curVgaGain=g_pgaGain1;
        #if SAGC_EN
            #if WN_BYPASS
                sagcTarget=800;
            #else
                sagcTarget=500;
            #endif  
            sAgc(0,RNG_FFT_CH1_ADDR, SAMPLE_POINT*8,sagcTarget,&g_tiaGain1,&g_pgaGain1);
            g_tiaGain0=g_tiaGain1;
            g_pgaGain0=g_pgaGain1;
            rfTiaGainSetting(g_tiaGain0,g_tiaGain0,g_tiaGain1,g_tiaGain1);
            rfVgaGainSetting(g_pgaGain0,g_pgaGain0,g_pgaGain1,g_pgaGain1);  
        #endif
        #if TF_ENABLE
            tinyFramefTx(0x0B04, (uint8*)(RNG_FFT_CH0_ADDR+SAMPLE_POINT*4), SAMPLE_POINT*4);
            tinyFramefTx(0x0B05, (uint8*)(RNG_FFT_CH1_ADDR+SAMPLE_POINT*4), SAMPLE_POINT*4);
        #endif
        //save .\matlab\adc0_odd_data.hex  0x20008000,0x2000bfff;save .\matlab\adc0_even_data.hex 0x2000c000,0x2000ffff;save .\matlab\adc1_odd_data.hex  0x20010000,0x20013fff;save .\matlab\adc1_even_data.hex 0x20014000,0x20017fff

        for(int i=0; i<CHIRP_NUM/2; i++)
        {
            memScheduling(RNG_FFT_CH0_ODD_ADDR+(i*2+1)*SAMPLE_POINT*4, DATA_MOVE_ADDR+i*SAMPLE_POINT*4, 4, 4, SAMPLE_POINT);
        }
        for(int i=0; i<CHIRP_NUM/2; i++)
        {
            memScheduling(RNG_FFT_CH0_ODD_ADDR+(i*2)*SAMPLE_POINT*4, RNG_FFT_CH0_ODD_ADDR+i*SAMPLE_POINT*4, 4, 4, SAMPLE_POINT);
        }
        for(int i=0; i<CHIRP_NUM/2; i++)
        {
            memScheduling(DATA_MOVE_ADDR+i*SAMPLE_POINT*4, RNG_FFT_CH0_EVEN_ADDR+i*SAMPLE_POINT*4, 4, 4, SAMPLE_POINT);
        }
        for(int i=0; i<CHIRP_NUM/2; i++)
        {
            memScheduling(RNG_FFT_CH1_ODD_ADDR+(i*2+1)*SAMPLE_POINT*4, DATA_MOVE_ADDR+i*SAMPLE_POINT*4, 4, 4, SAMPLE_POINT);
        }
        for(int i=0; i<CHIRP_NUM/2; i++)
        {
            memScheduling(RNG_FFT_CH1_ODD_ADDR+(i*2)*SAMPLE_POINT*4, RNG_FFT_CH1_ODD_ADDR+i*SAMPLE_POINT*4, 4, 4, SAMPLE_POINT);
        }
        for(int i=0; i<CHIRP_NUM/2; i++)
        {
            memScheduling(DATA_MOVE_ADDR+i*SAMPLE_POINT*4,RNG_FFT_CH1_EVEN_ADDR+i*SAMPLE_POINT*4, 4, 4, SAMPLE_POINT);
        }
        //setInterrupt(INT_UART1, DISABLEINT);
        //setInterrupt(INT_TIMER0, DISABLEINT);
        
        
        //g_jlinkDataFlag=1;
		//while( g_jlinkDataFlag==1);
        //continue;
        
        #if HAM_WIN_ENABLE
        // do hamning filter for 1D FFT
        Ham_range_filter(RNG_FFT_CH0_ODD_ADDR, HAMM_WIN_RANGE);
        Ham_range_filter(RNG_FFT_CH0_EVEN_ADDR, HAMM_WIN_RANGE);
        Ham_range_filter(RNG_FFT_CH1_ODD_ADDR, HAMM_WIN_RANGE);
        Ham_range_filter(RNG_FFT_CH1_EVEN_ADDR, HAMM_WIN_RANGE);
        #endif
      
        // 1D FFT
        fftProcess(SAMPLE_POINT, CHIRP_NUM/2, RNG_FFT_CH0_ODD_ADDR ,RNG_FFT_CH0_ODD_ADDR, 4, 4);//0x20008000
        fftProcess(SAMPLE_POINT, CHIRP_NUM/2, RNG_FFT_CH0_EVEN_ADDR,RNG_FFT_CH0_EVEN_ADDR, 4, 4);//0x200C0000
        fftProcess(SAMPLE_POINT, CHIRP_NUM/2, RNG_FFT_CH1_ODD_ADDR ,RNG_FFT_CH1_ODD_ADDR, 4, 4);//0x20010000
        fftProcess(SAMPLE_POINT, CHIRP_NUM/2, RNG_FFT_CH1_EVEN_ADDR,RNG_FFT_CH1_EVEN_ADDR, 4, 4);//0x20140000
        #if TF_ENABLE
            tinyFramefTx(0x0C00, (uint8*)(RNG_FFT_CH0_ODD_ADDR+SAMPLE_POINT*4), SAMPLE_POINT*4);
            tinyFramefTx(0x0D00, (uint8*)(RNG_FFT_CH1_EVEN_ADDR+SAMPLE_POINT*4), SAMPLE_POINT*4);
        #endif        
        

        #if BG_REMOVAL_ENABLE
        if (initial_flag == 1)
        {
            initial_flag++;
            // initial dc removal data
            for(int i= 0; i<RANGE_MAX; i++)
            {
                *((int32 *)(BG_REMOVAL_CH1_ADD+ i*4)) = *((int32 *)(RNG_FFT_CH0_ODD_ADDR  + i*4));
                *((int32 *)(BG_REMOVAL_CH2_ADD+ i*4)) = *((int32 *)(RNG_FFT_CH0_EVEN_ADDR  + i*4));
                *((int32 *)(BG_REMOVAL_CH3_ADD+ i*4)) = *((int32 *)(RNG_FFT_CH1_ODD_ADDR  + i*4));
                *((int32 *)(BG_REMOVAL_CH4_ADD+ i*4)) = *((int32 *)(RNG_FFT_CH1_EVEN_ADDR  + i*4));
            }
        }
        else if (initial_flag < bg_estimate_time)
        {
            // 10s big coeff
            initial_flag++;
            iqcoef = 0.3f;
        }
        else if (initial_flag == bg_estimate_time)
        {
            // small coeff
            initial_flag++;
            iqcoef = 0.05f;
        }
        else
        {
            update_flag = 0;
            iqcoef = 0.05f;
        }
        
        //save .\matlab\range0_odd_data.hex  0x20008000,0x2000bfff;save .\matlab\range0_even_data.hex 0x2000c000,0x2000ffff;save .\matlab\range1_odd_data.hex  0x20010000,0x20013fff;save .\matlab\range1_even_data.hex 0x20014000,0x20017fff
        background_removal(RNG_FFT_CH0_ODD_ADDR , BG_REMOVAL_CH1_ADD, SAMPLE_POINT,SAMPLE_POINT, NFFT_VEL,LOG2_NFFT_VEL, iqcoef, update_flag);
        background_removal(RNG_FFT_CH0_EVEN_ADDR, BG_REMOVAL_CH2_ADD, SAMPLE_POINT,SAMPLE_POINT, NFFT_VEL,LOG2_NFFT_VEL, iqcoef, update_flag);
        background_removal(RNG_FFT_CH1_ODD_ADDR , BG_REMOVAL_CH3_ADD, SAMPLE_POINT,SAMPLE_POINT, NFFT_VEL,LOG2_NFFT_VEL, iqcoef, update_flag);
        background_removal(RNG_FFT_CH1_EVEN_ADDR, BG_REMOVAL_CH4_ADD, SAMPLE_POINT,SAMPLE_POINT, NFFT_VEL,LOG2_NFFT_VEL, iqcoef, update_flag);
        #endif
        //save .\matlab\range0_odd_dc_rmv.hex  0x20008000,0x2000bfff;save .\matlab\range0_even_dc_rmv.hex 0x2000c000,0x2000ffff;save .\matlab\range1_odd_dc_rmv.hex  0x20010000,0x20013fff;save .\matlab\range1_even_dc_rmv.hex 0x20014000,0x20017fff
        
        #if HAM_WIN_ENABLE
        // do hamning filter for 2D FFT
        Ham_doppler_filter(RNG_FFT_CH0_ODD_ADDR, HAMM_WIN_DOPPLER);
        Ham_doppler_filter(RNG_FFT_CH0_EVEN_ADDR, HAMM_WIN_DOPPLER);
        Ham_doppler_filter(RNG_FFT_CH1_ODD_ADDR, HAMM_WIN_DOPPLER);
        Ham_doppler_filter(RNG_FFT_CH1_EVEN_ADDR, HAMM_WIN_DOPPLER);
        #endif
        //2D FFT
        velocityFftProcess(NFFT_VEL, RANGE_MAX, RNG_FFT_CH0_ODD_ADDR ,DOP_FFT_CH0_ODD_ADDR , 4*SAMPLE_POINT, 4);
        velocityFftProcess(NFFT_VEL, RANGE_MAX, RNG_FFT_CH0_EVEN_ADDR,DOP_FFT_CH0_EVEN_ADDR, 4*SAMPLE_POINT, 4);
        velocityFftProcess(NFFT_VEL, RANGE_MAX, RNG_FFT_CH1_ODD_ADDR ,DOP_FFT_CH1_ODD_ADDR , 4*SAMPLE_POINT, 4);
        velocityFftProcess(NFFT_VEL, RANGE_MAX, RNG_FFT_CH1_EVEN_ADDR,DOP_FFT_CH1_EVEN_ADDR, 4*SAMPLE_POINT, 4); 
        //save .\matlab\Velocity0_even_data.hex  0x20028000,0x2002c000;save .\matlab\Velocity0_odd_data.hex 0x2000c000,0x20010000;save .\matlab\Velocity1_even_data.hex  0x20010000,0x20014000;save .\matlab\Velocity1_odd_data.hex 0x20014000,0x20018000
        //setInterrupt(INT_UART1, ENABLEINT);
        //setInterrupt(INT_TIMER0, ENABLEINT);
           


        // phase compensation
        ch_phase_compensation(DOP_FFT_CH0_ODD_ADDR, DOP_FFT_CH0_EVEN_ADDR, DOP_FFT_CH1_ODD_ADDR, DOP_FFT_CH1_EVEN_ADDR, r_cali, i_cali);
        //ch_phase_compensation_lut(echo_u_ch2, echo_u_ch3);
        

        
        absAdt3102(NFFT_VEL, RANGE_MAX,DOP_FFT_CH0_ODD_ADDR , ABS_CH0_ODD_ADDR );
        absAdt3102(NFFT_VEL, RANGE_MAX,DOP_FFT_CH0_EVEN_ADDR, ABS_CH0_EVEN_ADDR);
        absAdt3102(NFFT_VEL, RANGE_MAX,DOP_FFT_CH1_ODD_ADDR , ABS_CH1_ODD_ADDR );
        absAdt3102(NFFT_VEL, RANGE_MAX,DOP_FFT_CH1_EVEN_ADDR, ABS_CH1_EVEN_ADDR);
        //save .\matlab\Velocity0_even_abs.hex  0x20018000,0x2001c000;save .\matlab\Velocity0_odd_abs.hex 0x2001c000,0x20020000;save .\matlab\Velocity1_even_abs.hex  0x20020000,0x20024000;save .\matlab\Velocity1_odd_abs.hex 0x20024000,0x20028000
        

        
        
        //Average the results of Velocity_FFT of ADC0 and ADC1
        for(int i =0;i<RANGE_MAX*CHIRP_NUM/2;i=i+1)        
        {
            int32 absCh0Odd;
            int32 absCh0Even;
            int32 absCh1Odd;
            int32 absCh1Even;
            absCh0Odd  = *((int32 *)(ABS_CH0_ODD_ADDR  + i*4));
            absCh0Even = *((int32 *)(ABS_CH0_EVEN_ADDR + i*4));
            absCh1Odd  = *((int32 *)(ABS_CH1_ODD_ADDR  + i*4));
            absCh1Even = *((int32 *)(ABS_CH1_EVEN_ADDR + i*4));
            *((int32 *)(ABS_MEAN_RV_ADDR+ i*4))= (absCh0Odd>>2) + (absCh0Even>>2) + (absCh1Odd>>2) + (absCh1Even>>2);
            //*((int32 *)(RNG_FFT_CH0_ADDR+ i*4))= (absCh1Odd + absCh1Even)/2;
            //*((int32 *)(ABS_MEAN_RV_ADDR+ i*4))= (absCh0Odd + absCh0Even)/2;
        }
        

        
        //save ..\matlab\abs_merge_data.hex 0x20008000,0x2000bfff
        dopplerSwapMimo(NFFT_VEL, RANGE_MAX, g_absMergeArray);
        //powerThdAdj=powerThd+(curTiaGain*6+curVgaGain*2-60);//powerThd is based on IFgain = 50db, if IFgain changed, thd changes automatically.
        //peakSingleMimo(NFFT_VEL, dopMin, RANGE_MIN, RANGE_MAX, g_absMergeArray, powerThdAdj, firstPeakOnly, distComp, &obj);
        //ledCount ++;
		//uint32 actionType =  procGesture(obj);
	
                   

             
         
        // step 2  CFAR estimate noise    
        noise_estimation_cfar( (uint32 *)ABS_MEAN_RV_ADDR, cfar_window, rva_range_average_noise_v0, RVA_HISTORY_SIZE_U16/2);  // v=0
        noise_estimation_cfar( (uint32 *)ABS_MEAN_RV_ADDR, cfar_window, rva_range_average_noise_vm, 0);   // v=min


        // step 3 peak list
        rva_detect_peaks( (uint32 *)ABS_MEAN_RV_ADDR, rva_range_average_noise_v0, rva_range_average_noise_vm, (uint32 *)&rv_list);
        

        // DBF angle detecton
        // 1D 
        //DBF_3DFFT(DOP_FFT_CH0_ODD_ADDR, DOP_FFT_CH1_ODD_ADDR, (uint32 *) &rv_list, (uint32 *)&target_list);
        #if Board_2D 
            // 2D
            DBF_3DFFT_MIMO(DOP_FFT_CH0_ODD_ADDR, DOP_FFT_CH1_ODD_ADDR, DOP_FFT_CH0_EVEN_ADDR, DOP_FFT_CH1_EVEN_ADDR, (uint32 *) &rv_list, (uint32 *)&target_list);
        #endif
        #if Board_3D
            // 3D
            DBF_3DFFT_MIMO_3D(DOP_FFT_CH0_ODD_ADDR, DOP_FFT_CH1_ODD_ADDR, DOP_FFT_CH0_EVEN_ADDR, DOP_FFT_CH1_EVEN_ADDR, (uint32 *) &rv_list, (uint32 *)&target_list);
        #endif 
         
        //g_jlinkDataFlag=1;
		//while( g_jlinkDataFlag==1);
        //continue;
                    
        // output point cloud --- kang
        if (target_list.Ntargets > 0)
        {
            // data length : 9 bytes TF + 16 header + num Points * 6 float  * 4 bytes + 2 CRC
            // example: 1 target output 9+16+16+2 = 43
            tinyFramefTx(0xFF02, (uint8*)(&target_list), (24*target_list.Ntargets+16));
 /*
            printfDebug("framenr -----# %d \r\n",target_list.framenumber);
            printfDebug("points # %d \r\n",target_list.Ntargets);
            for (int i=0; i<target_list.Ntargets;i++)
            {
                printfDebug("points # %d \r\n",i);
                printfDebug("r # %f \r\n",target_list.r_flt32[i*6]);
                printfDebug("v # %f \r\n",target_list.r_flt32[i*6+1]);
                printfDebug("x # %f \r\n",target_list.r_flt32[i*6+2]);
                printfDebug("y # %f \r\n",target_list.r_flt32[i*6+3]);
                printfDebug("z # %f \r\n",target_list.r_flt32[i*6+4]);
                printfDebug("snr # %f \r\n\r\n",target_list.r_flt32[i*6+5]);
            }*/
        }
        target_list.framenumber++;          
        
		/*if(actionType)
		{
			ledDisplay(actionType);
			ledCount=0;
			
		}
		if(ledCount>10)
		{
			ledAlloff();
			ledCount=0;
		}
        if(g_actGoing==1)
        {
            framePeriod=FRAME_PERIOD_ACT;
            dopMin=2;
        }
        else
        {
            framePeriod=FRAME_PERIOD_IDLE; 
            dopMin=1;
        }            
        if(obj.valid==1)
        {
            float powerDbDisp=obj.powerSignal-(int32)(curTiaGain*6+curVgaGain*2-60);
            printfDebug("tiaGain %d ,vgaGain %d, total gain %d.  Distance: %.3f, Speed: %.3f, Power(dBFS): %.0f \r\n", curTiaGain,curVgaGain,curTiaGain*6+curVgaGain*2,obj.rngIdx*RANGE_STEP/100,(obj.dopIdx-NFFT_VEL/2)*SPEED_STEP/100,powerDbDisp);
        }*/
    } 
}
