//--------------------------------------------------------------------
//Copyright(c)2020,Andar Technologise Inc.
//All Rights Reserved
//Confidential Property of Andar Technologies Inc.
//
//Module Description:
//  .interrupt hander function
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
#include "stdio.h"
#include "adt3102.h"
#include "adt3102_dma.h"
#include "adt3102_tx.h"
#include "adt3102_rx.h"
#include "uart_ctype_map.h"
#include "dma_ctype_map.h"
#include "gpio_ctype_map.h"
#include "dsp_ctype_map.h"
#include "timer_ctype_map.h"
#include "pmu_ctype_map.h"
#include "rtc_ctype_map.h"
#include "rfc_ctype_map.h"
#include "trng_ctype_map.h"
#include "adt3102_inthandler.h"
#include "adt3102_timer.h"
#include "adt3102_gpio.h"
//
//------Global variable declaration
extern uint8 g_uartRecvFlag;
extern int v_g_fftFlag;
extern int g_delayUsFlag;
extern int v_g_trngFlag;
extern volatile int32 g_sendCount;
extern uint32 g_frameStart;
/****************************************************************************************
*
*Function name : uart0Handler uart1Handler uart2Handler
*Description   : uart's interrupt entry function
*Paramater	   : Nane
*
*Note          :1,you need to turn on the total interrupt, for example, you want to use
                the uart0Hander ,the code you need to initialize is :
                soc_NVIC[soc_SETENA] = 0x01;
                2,you need to change the interrupt entry name in the startip_CMSDK_CM3.s
                to uart0Handler
*Return        Nane
****************************************************************************************/
//watchdog interrupt
void NMI_Handler(void)
{ 

}
void uart0Handler(void)
{
    UART0->INTSTATUS=0x02;
    if(UART0->STATE&(1<<1))
    {
        g_uartRecvFlag = 1;
    }
}
void uart1Handler(void)
{
    UART1->INTSTATUS=0x02;
    if(UART1->STATE&(1<<1))
    {
        g_uartRecvFlag = 1;
    }
}
void uart2Handler(void)
{
    UART2->INTSTATUS=0x02;
    if(UART1->STATE&(1<<1))
    {
        g_uartRecvFlag = 1;
    }
}
void timer0Handler(void)//Timer0
{ 	
	Timer0->INTSTATUS=1;// clear timer interrupt
    if(g_sendCount%2)
    {
        if(MIMO==1)
        {
            if(PA1_ONLY==0)
            {
                //enable PA0 and disable PA1
                //txPaEnable(ENABLE, DISABLE);
                RFC->rfc_tx_pa_reg &= ~rfc_tx_pa_reg_pa1_en_bit;
                RFC->rfc_tx_pa_reg |= rfc_tx_pa_reg_pa0_en_bit;
                
            }
        }
        gpioOutput(9,1);
    } else
    {
        if(MIMO==1)
        {
            //enable PA1 and disable PA0
            //txPaEnable(DISABLE, ENABLE); 
            RFC->rfc_tx_pa_reg &= ~rfc_tx_pa_reg_pa0_en_bit;
            RFC->rfc_tx_pa_reg |= rfc_tx_pa_reg_pa1_en_bit;
        }
        gpioOutput(9,0);
    }
    g_sendCount++;
//    if(g_sendCount>=CHIRP_NUM)
//    {
//        timerOff(Timer0);
//    }
}

void timer1Handler(void)      
{
    Timer1->INTSTATUS=1;// clear timer interrupt
	timerOff(Timer1);
    g_frameStart=1;
}
void dualtimer0Handler(void)  
{
    if((Dualtimer->TMEER2MIS&0x01) == 1)
    {
        Dualtimer->TIMER2INTCLR = 1;
        g_delayUsFlag = 1;
    }
}
void uartOverflowHandler(void)
{

}
void pmuHandler(void)         
{
    PMU->pmu_int_clear_reg = 0x1f;
    //printf("pmu interrupt\r\n");
    
}
void spi0Handler(void)        
{

}
void spi1Handler(void)        
{

}
void rtcHandler(void)         
{
    RTC->RTCICR = 0x01;
    RTC->RTCLR=0;//update value
    printf("RTC\r\n");
}
void i2c0Handler(void)        
{

}
void i2c1Handler(void)        
{

}
void fftHandler(void)         
{
    v_g_fftFlag = 0;
    DSP->dsp_int_clear = dsp_int_clear_fft_clr0_bit;   //clear fft interrupt
    //printf("The fft_flag_m =%d \r\n",v_g_fftFlag);
}
void gpioHandler(void)//GPIO
{ 
	GPIO->gpio_int_clear=1<<0;
}

void dma0Handler(void)        
{

}
void dma1Handler(void)        
{

}
void trngHandler(void)     
{
    TRNG->RNG_ICR = 1; 
    v_g_trngFlag = 1;
//    printf("TRNG interrupt\r\n");
}












