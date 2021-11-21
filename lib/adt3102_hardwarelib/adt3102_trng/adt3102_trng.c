//--------------------------------------------------------------------
//Copyright(c)2020,Andar Technologise Inc.
//All Rights Reserved
//Confidential Property of Andar Technologies Inc.
//
//Module Description:
//  .TRNG config
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
#include "adt3102_trng.h"
#include "trng_ctype_map.h"
//
//------Global variable declaration
int32 v_g_trngFlag =0;

/****************************************************************************************
*
*Function name : trngInit
*Description   : trng Init 
*Paramater	   :
*        @ :
*        @ :
        uint32 trngTestBuff[5];
        trngInit(trngTestBuff);
        for(int32 i=0; i<6; i++)
        {
            printf("%x\r\n", trngTestBuff[i]);
        }
*Return        : None
****************************************************************************************/
void trngInit(uint32 *trngBuff)
{
    if((TRNG->AUTOCORR_STATISTIC ) > 0xffff)
    {
        TRNG->TRNG_SW_RESET = 1; 
    } 
    TRNG->RNG_IMR = 0xE;
    /*RW
    [1:0] select the number of inverters(out of four possible selections) in the ring 
    oscillator(the entropy source):
        00: select the shortest inverter chain length
        01: select the short inverter chain length
        10: select the long inverter chain length
        11: select the longest inverter chain length
    */
    TRNG->TRNG_CONFIG = 0x10;
    /*RW  
    The RND_SOURCE_ENABLE register controls whether the entropy source,ring oscillator
    1 : enable
    0 : disable
    */    
    TRNG->RND_SOURCE_ENABLE = 1;


    while(v_g_trngFlag == 0);
    //printf("TRNG\r\n");
    trngBuff[0] = TRNG->EHRDATA0;
    trngBuff[1] = TRNG->EHRDATA1;
    trngBuff[2] = TRNG->EHRDATA2;
    trngBuff[3] = TRNG->EHRDATA3;
    trngBuff[4] = TRNG->EHRDATA4;
    trngBuff[5] = TRNG->EHRDATA5;  
    v_g_trngFlag = 0;
}


