#ifndef __ADT3102__H
#define __ADT3102__H
#include "adt3102_adc.h"
//<<< Use Configuration Wizard in Context Menu >>> 

//<s>Define Adt3102 Config File Version
//  <i>version
#define ADT3102_VERSION "1.0.0"

typedef struct
{
    char   valid;
    int32  rngIdx;
    int32  dopIdx;
    float  powerSignal;
    float  hori1;
    float  hori2;
    float  elev1;
    float  elev2;
} OBJ_TypeDef;

//--Debug define e.g.,printf, output result, analog IF sign enbuffer
//<q.0> Enable debug mode 
#define DEBUGMODE 0

//--High clock
#define HIGH_CLOCK_50M  1
#define HIGH_CLOCK_125M 0

#define UART0_PRINTF
//#define UART1_PRINTF

#define HIGH_CLOCK HIGH_CLOCK_125M

//---Tx
//<e>FMCW config
// <o>FSTART
//   <i>Default: 76G(Uint:GHz)
//   <76-80>
#define FSTART    76
// <o>FM
//   <i>Default: 2000(Uint:MHz)
//   <0-4000>
#define FM        4000
// <o>CHIRP_NUM
//   <i>Default: 32
//   <0-256>
#define CHIRP_NUM         64
#define CHIRP_NUM_LOG2    6

// should be 120ns times..
#define CHIRP_T0          70
#define CHIRP_T1          10
#define CHIRP_T2          110
#define CHIRP_PERIOD      (CHIRP_T0+CHIRP_T1+CHIRP_T2)

#define ADC_SAMPLE_RATE   ADC_8P3M       //ADC_16P6M: 16.667mhz.   ADC_8P3M: 8.333mhz
#define REMOVE_FIRST_CHIRP 1 //2db better
#define RANGE_MIN         3
#define RANGE_MAX         125
#define DOWN_SAMPLE       3  //actual down sample rate = 1/(DOWN_SAMPLE+1).
#define SAMPLE_POINT      128
#define MIMO              1          // 1: MIMO enable
#define PA1_ONLY          0
#define NFFT_VEL          (CHIRP_NUM/2)  //divid 2 for mimo
#define LOG2_NFFT_VEL     5 //6
#define OBJ_MAX           30
#define OBJ_MIN           4 
#define RANGE_STEP        4.2
#define SPEED_STEP        16 // cm/s
#define WN_BYPASS         0
#define FRAME_PERIOD_ACT  50000
#define FRAME_PERIOD_IDLE 50000
#define SAGC_EN           1
#define TF_ENABLE         0
#define FRAME_LOWPOWER    1
#define CHIRP_LOWPOWER    0
#define PRINT_DEBUG_EN    1
#define Board_2D          0   // select one of them
#define Board_3D          1
#define BG_REMOVAL_ENABLE 0
#define HAM_WIN_ENABLE    0

//memory address
#define RNG_FFT_CH0_ADDR 0x20008000                                         
#define RNG_FFT_CH1_ADDR (RNG_FFT_CH0_ADDR + SAMPLE_POINT*CHIRP_NUM*4)   //0x20010000

#define RNG_FFT_CH0_ODD_ADDR      RNG_FFT_CH0_ADDR                                        //0x20008000 - 0x2000bfff
#define RNG_FFT_CH0_EVEN_ADDR     (RNG_FFT_CH0_ODD_ADDR  + SAMPLE_POINT*CHIRP_NUM*2)   //0x2000c000 - 0x2000ffff
#define RNG_FFT_CH1_ODD_ADDR 	  (RNG_FFT_CH0_EVEN_ADDR + SAMPLE_POINT*CHIRP_NUM*2)   //0x20010000 - 0x20013fff
#define RNG_FFT_CH1_EVEN_ADDR 	  (RNG_FFT_CH1_ODD_ADDR  + SAMPLE_POINT*CHIRP_NUM*2)   //0x20014000 - 0x20017fff

#define DATA_MOVE_ADDR            0x20018000  //0x20018000 - 0x2001bfff

#define DOP_FFT_CH0_ODD_ADDR      0x20018000	//-0x2002bfff  16KB
#define DOP_FFT_CH0_EVEN_ADDR	  0x2001c000	//-0x2000ffff  16KB
#define DOP_FFT_CH1_ODD_ADDR	  0x20020000	//-0x20013fff  16KB
#define DOP_FFT_CH1_EVEN_ADDR 	  0x20024000	//-0x20017fff  16KB

#define ABS_CH0_ODD_ADDR     	  0x20008000  //-0x2001bfff  16KB
#define ABS_CH0_EVEN_ADDR		  0x2000c000  //-0x2001ffff  16KB
#define ABS_CH1_ODD_ADDR  		  0x20010000  //-0x20023fff  16KB
#define ABS_CH1_EVEN_ADDR 		  0x20014000  //-0x20027fff  16KB

#define ABS_MEAN_RV_ADDR		  0x20008000  //-0x20027fff  16KB

#define RV_LIST_ADDR      0x20028000    //start 0x20008000,size RANGE_ADDR_MAX*CHIRP_NUM*4=0x4000
#define TARGET_LIST_ADDR  (RV_LIST_ADDR + SAMPLE_POINT*CHIRP_NUM*2)    //start 0x2002C000,size RANGE_ADDR_MAX*CHIRP_NUM*4=0x4000
#define NOISE_V0_ADD      0x2002A000
#define NOISE_VM_ADD      (NOISE_V0_ADD + SAMPLE_POINT*4)   // 0x2002A000+128*4
#define BG_REMOVAL_CH1_ADD    (NOISE_VM_ADD + SAMPLE_POINT*4)   // 0x2002A000+128*4 +128*4  
#define BG_REMOVAL_CH2_ADD    (BG_REMOVAL_CH1_ADD + SAMPLE_POINT*4)   // 0x2002A000+128*4 +128*4*2  
#define BG_REMOVAL_CH3_ADD    (BG_REMOVAL_CH2_ADD + SAMPLE_POINT*4)   // 0x2002A000+128*4 +128*4*3  
#define BG_REMOVAL_CH4_ADD    (BG_REMOVAL_CH3_ADD + SAMPLE_POINT*4)   // 0x2002A000+128*4 +128*4*4  

#define DOP_COMP_COEFF      0x2002F000      // float32  4K   32 doppler *  float * IQ
#define HAMM_WIN_RANGE      (DOP_COMP_COEFF + CHIRP_NUM*4)  //  Hamming  128 * float
#define HAMM_WIN_DOPPLER    (HAMM_WIN_RANGE + SAMPLE_POINT*4)  //  Hamming  128 * float  // doppler_win size 32 * float

//</e>
//---Rx

//---Adc
// FFT buf max size 512 points  int32 (I, Q)
#define FFT_BUF_HF1 0x20030000         //FFT buffer start address input for 1st half
#define FFT_BUF_HF2 0x20030000 + 256*4 //FFT buffer start address input for 2nd half
#if (ADC_SAMPLE_RATE == ADC_16P6M)
  #define SAMPLE_END_NUM  ceil((CHIRP_T1+CHIRP_T2)*16.666)
  #define INTERVALNUM     (CHIRP_T0+CHIRP_T1+CHIRP_T2)*1000/60	
#else
  #define SAMPLE_END_NUM  ceil((CHIRP_T1+CHIRP_T2)*8.333)
  #define INTERVALNUM     (CHIRP_T0+CHIRP_T1+CHIRP_T2)*1000/120	
#endif
////150k~350k
//#define FIRREG0   0x3FFE00
/*#define FIRREG1   0x3FFFFFF
#define FIRREG2   0x3FFBFFE
#define FIRREG3   0x3FF7FFC
#define FIRREG4   0x3FF3FFA
#define FIRREG5   0x3FEFFF8
#define FIRREG6   0x3FF3FF8
#define FIRREG7   0x1FFC
#define FIRREG8   0x1E007
#define FIRREG9   0x4E01A
#define FIRREG10  0x90037
#define FIRREG11  0xDE05B
#define FIRREG12  0x132084
#define FIRREG13  0x1800AD
#define FIRREG14  0x1BC0D0
#define FIRREG15  0x1DC0E8
#define FIRREG16  0xF0
*/
#define FIRREG0   0x0
#define FIRREG1   0x0
#define FIRREG2   0x0
#define FIRREG3   0x0
#define FIRREG4   0x0
#define FIRREG5   0x0
#define FIRREG6   0x0
#define FIRREG7   0x0
#define FIRREG8   0x0
#define FIRREG9   0x0
#define FIRREG10  0x0
#define FIRREG11  0x0
#define FIRREG12  0x0
#define FIRREG13  0x0
#define FIRREG14  0x0
#define FIRREG15  0x0
#define FIRREG16  0xfff

#define FC        (FIRREG16 | (DOWN_SAMPLE << fir_cof_sample_rate_down_sample_rate_shift))


//---FFT

//---Cfar
#define CFAR_ADDR 0x40024000
#endif
