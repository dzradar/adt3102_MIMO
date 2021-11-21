//--------------------------------------------------------------------
//Copyright(c)2020,Andar Technologise Inc.
//All Rights Reserved
//Confidential Property of Andar Technologies Inc.
//
//Module Description:
//  .software agc
//  .
//  .
//  .
//  .

//Created by :jiangqi
//$Revision: 1.0
//$Data: 2021/04/30
//--------------------------------------------------------------------
//
//All include header files
#include "stdlib.h"
#include "math.h"
#include "adt3102_type_define.h"
#include "adt3102_sagc.h"
#include "adt3102_system.h"
#include "adt3102_uart.h"
#include "adt3102.h"
//
//------Global variable declaration

/****************************************************************************************
*
*Function name : sagc
*Description   : software if agc using only one channel input
*Paramater	   :
*        @highGainEn : Set to 1 to make highest tiaGain 3, otherwise 2.
*        @srcAddr : If data address. Data arrangement is 16 bit I in lower address and 16bit Q in higher. 
*        @length: Samples to calculate input amplitute.  
*        @avgTarget: target output average amplitute for agc. Sinwave ideal average is 4096/pi=1303.
*        @pTiaGain,pPgaGain : pointer to tiaGain and pgaGain
*function call example: sAgc(RNG_FFT_CH1_ADDR, NFFT_RNG*8,1000,&tiaGain,&vgaGain);
*Return        : None
****************************************************************************************/
void sAgc(int16 highGainEn, int32 srcAddr, int16 length, int16 avgTarget, uint32 *pTiaGain, uint32 *pPgaGain)
{
	volatile int32* pData;
	int32 iqRead;
	int16 iRead,qRead;
    float totalGain;
	int32 sumI = 0;
	int32 sumQ = 0;
	int32 sumSel = 0;
	float avgSel = 0;
    float gainOffset=0;
    totalGain=(*pTiaGain)*6+(*pPgaGain)*2;
    for(unsigned int i =0; i<length; i++)
    {
		pData = (volatile int32*)(srcAddr + i*4);
		iqRead = *pData;
		iRead  = iqRead & (0xffff);
		qRead  = iqRead >> 16;
        sumI = sumI + abs(iRead);
        sumQ = sumQ + abs(qRead);
    }
    sumSel=sumI>sumQ ? sumI : sumQ;
    avgSel=(sumSel>>4)/length;
    gainOffset=20*log10(avgTarget/avgSel);
//    if(fabsf(gainOffset)<3.0)
//    {
//        return;
//    }
    totalGain=totalGain+gainOffset/2;
    if(totalGain<50)
    {
        *pTiaGain=0; //0db
        //if(totalGain<40)
        //{   
        //    totalGain=40;
        //}
        *pPgaGain=(uint32)round(totalGain/2);
    }
    else if(totalGain<56)
    {
        *pTiaGain=1; //6db
        *pPgaGain=(uint32)round((totalGain-6)/2);
    }    
    else if((highGainEn==0) || ((highGainEn==1)&&(totalGain<60)))
    {
        *pTiaGain=2; //12db
        *pPgaGain=(uint32)round((totalGain-12)/2);
    }    
    else
    {
        *pTiaGain=3; //18db
        *pPgaGain=(uint32)round((totalGain-18)/2);
    }  
    if(*pPgaGain>24)
    {
        *pPgaGain=24;
    }
    //printfDebug("Average ADC in: %f , gainoffset %f, totalgain %f, TiaGain %d , PgaGain %d .\r\n", avgSel,gainOffset,totalGain,*pTiaGain,*pPgaGain);
}
