//--------------------------------------------------------------------
//Copyright(c)2020,Andar Technologise Inc.
//All Rights Reserved
//Confidential Property of Andar Technologies Inc.
//
//Module Description:
//  .range fft
//  .velocity fft
//  .cfar(Sum Sort)
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
#include "math.h"
#include "adt3102_type_define.h"
#include "adt3102.h"
#include "stdio.h"
#include "adt3102_dsp.h"
#include "adt3102_dma.h"
#include "adt3102_gpio.h"
#include "dsp_ctype_map.h"
#include "dma_ctype_map.h"
#include "adt3102_memscheduling.h"
//
//------Global variable declaration
/****************************************************************
Function name : cfarProcess

*Description   : cfar for sort or sum
CPU->Cfar module 
            |       train cell      |guard cell|Target|guard cell|       train cell      |
            |1 |1 |1 |1 |1 |1 |1 |1 |    0     |  0   |     0    |1 |1 |1 |1 |1 |1 |1 |1 |            
            |01|02|03|04|05|06|07|08|    (09)  | {10} |    (11)  |12|13|14|15|16|17|18|19|20|21|......

               |       train cell      |guard cell|Target|guard cell|       train cell      |
               |1 |1 |1 |1 |1 |1 |1 |1 |    0     |  0   |     0    |1 |1 |1 |1 |1 |1 |1 |1 |            
            |01|02|03|04|05|06|07|08|09|    (10)  | {11} |    (12)  |13|14|15|16|17|18|19|20|21|22|......

            1,enter the total number of data to be calculation,for example 19.
            2,Mark the data to be calculated as 1,otherwise it is 0.
            3,data input: (1)dataAdress -> CfarAdress   for exampple: 0x20008000 -> 0x40024000.
                             dataAdress is variable,CfarAdress is stable,and is 0x40024000.
                          (2)Generally,the data can be directly transmitted through the "for" loop.
                          (3)Write to the cfar start register,the cfar module can automatically additon
                             or sorting operations accordong to the mark bit.
            4,data output:(1)when the cfar operation is completed, 
                          (2)the result is stored is register cfar_acc_sum_l and cfar_acc_sum_h.
                             sum_num[37:0] = {cfar_acc_sum_h[5:0],cfar_acc_sum_l[31:0]}.
                             sum_num = (cfar_acc_sum_l >> 6) + ((cfar_acc_sum_h & 63)<<(32-6)).
                          (3)and output an end flag is "cfar_done".
            5,sum and sort can be executed in parallel
            
            Note:step 234 generally divide the data into 3 blocks
            
*Paramater      :
*        @  :train cell
*        @  :guard cell
*        @  :Threshold
*        @  :sample point
*        @  :chirp number
*
*Return        : cfar is successfull ,and return 0;
****************************************************************/
/* test code
volatile uint64 sumtest;
volatile uint32 sumtest1;
//the hangzhou test code is written below
for(int32 i=0; i<64; i++)
{
    *((uint32*)0x20008000 +i)= i;
}
sumtest = accSum(0x20008000, 64);
sumtest1 = accOdr(0x20008000, 64, 13);
*/
uint64 accSum(uint32 sumAddr, int32 transNum)
{
    /*WO
    [0] Cfar start. Write only 
    */
//DSP->cfar_str = ;
    /*RW
    [8] acc_sum_frz:Freeze cfar_reg_acc_sum for CPU to read. 
        Set to '0' after read. 
    [6:0] dat_num:Total data transfered to cfar, including disabled data in the stream. 
          Must be even number and max valid value is 64 decimal.
    */
//DSP->cfar_dat_number = ;
    /*RW
    [31:0] msk0:To indicate if data is valid for CFAR. 
           Mapping to data 0~31. Set to '0' if not used. 
    */
//DSP->cfar_mask0 = ;
    /*RW
    [31:0] msk1:To indicate if data is valid for CFAR. 
           Mapping to data 32~63. Set to '0' if not used.
    */
//DSP->cfar_mask1 = ;
    /*RW
    [1] ord_done:CFAR order sort done
    [0] acc_done:CFAR accumulation done
    */
//DSP->cfar_done = ;
    /*RO
    [5:0] acc_sum_hi:Accumulation of valid data in the date stream [37:32]. 
                     Set acc_sum_frz before read.
    */
//DSP->cfar_acc_sum_h = ;
    /*RO
    [31:0] acc_sum_lo:Accumulation of valid data in the date stream [31:0]. 
                      Set acc_sum_frz before read.
    */
//DSP->cfar_acc_sum_l = ;
    /*RW
    [5:0] ord_index:Index to indicate which data to report after order sort. 
                    Must set to none 0. Set to 1 means the seconde minimum data to report, 
                    set to field_dat_num-1 means the largest data will be reported.
    */
//DSP->cfar_ord_idx = ;
    /*RO
    [31:0] 	ord_val:CAFR value of the order sorted point
    */
//DSP->cfar_ord_value = ;
    volatile uint32 accDone;
    volatile uint32 AccSumL;
    volatile uint32 AccSumH;
    volatile uint64 AccSum;
    
    DSP->cfar_dat_number = transNum;
    DSP->cfar_str = 0x00;
    uint64 maskSum;
    if (transNum==64)
        maskSum = 0xffffffffffffffff;
    else
        maskSum = (1<<transNum)-1;//maskSum = pow(2, transNum)-1;
    DSP->cfar_mask0 = maskSum;
    DSP->cfar_mask1 = maskSum>>32;
    DSP->cfar_str = 0x01;
    memScheduling(sumAddr, CFAR_ADDR, 4, 0, transNum);
    accDone = 0;
    //write to CFAR
    while (accDone != 1)
    {
        accDone = DSP->cfar_done && cfar_done_acc_done_bit;
    }
    AccSumL = DSP->cfar_acc_sum_l;
    AccSumH = DSP->cfar_acc_sum_h;   
    AccSum = AccSumL + ((uint64)(AccSumH & 0x3f)<<32);
    return AccSum;
}
/****************************************************************************************
*Function name : 
*Description   : 
*Paramater	   :
*        @ :  
*        @ :   
*Paramater	selection:		

*Power consumption : 
*Return        :
****************************************************************************************/
uint32 accOdr(uint32 sumAddr, int32 transNum, int32 ordNum)
{
    uint32 accDone;
    volatile uint32 AccSumL;
    volatile uint32 AccSumH;
    volatile uint32 AccSum;
    DSP->cfar_ord_idx = ordNum;
    DSP->cfar_dat_number = transNum;
    DSP->cfar_str = 0x00;
    uint64 maskSum;
    if (transNum==64)
        maskSum = 0xffffffffffffffff;
    else
        maskSum = (1<<transNum)-1;//maskSum = pow(2, transNum)-1;
    DSP->cfar_mask0 = maskSum;
    DSP->cfar_mask1 = maskSum>>32;
    DSP->cfar_str = 0x01;
    //write to CFAR
    memScheduling(sumAddr, CFAR_ADDR, 4, 0, transNum);
    accDone = 0;
    while (accDone != 1)
    {
        accDone = DSP->cfar_done && cfar_done_acc_done_bit;
    }
    AccSumL = DSP->cfar_acc_sum_l;
    AccSumH = DSP->cfar_acc_sum_h;   
    AccSum = AccSumL + ((uint64)(AccSumH & 0x3f)<<32);
    return DSP->cfar_ord_value;
}

/****************************************************************************************
*Function name : 
*Description   : 
*Paramater	   :
*        @ :  
*        @ :   
*Paramater	selection:		

*Power consumption : 
*Return        :
****************************************************************************************/
int32 cfarSum(uint32 trainCell, uint32 guardCell, uint32 srcAddr, uint32 srcLen, uint64 threshold, char *det)
{
    volatile uint32 arrayLenLeft=0;
    volatile uint32 arrayLenRight=0;
    volatile uint32 arrayAddrLeft=0;
    volatile uint32 arrayAddrRight=0;
    volatile uint64 cfarSumValueLeft=0;
    volatile uint64 cfarSumValueRight=0;
    volatile uint64 cfarSumValue=0;
    uint32 targetVelue;
    int32 detectFlag = 1;
    int32 fronty1, fronty2, reary1, reary2;
    
    if((srcLen > trainCell + guardCell+1) && (srcLen > (guardCell<<1) + 2))
    {            
        fronty1 =  guardCell;
        fronty2 = (trainCell + guardCell) ;
        /*  rear side edge points */
        reary1 = (srcLen - trainCell) - guardCell - 1;
        reary2 = srcLen - (guardCell<<1) + 1;
    }
    else
    {
        fronty1 = 0;
        fronty2 = 0;
        reary1 = 0;
        reary2 = 0;
    }
    for(int32 arrayIndex=0; arrayIndex<srcLen; arrayIndex++)
    {
        if(arrayIndex<fronty1)
        {
            arrayLenLeft      = 0;
            arrayLenRight     = trainCell<<1;
            arrayAddrLeft     = 0;
            arrayAddrRight    = srcAddr+((guardCell+arrayIndex+1)<<2);
        }else if(arrayIndex<=fronty2)
        {
            arrayLenLeft      = arrayIndex-guardCell;
            arrayLenRight     = (trainCell<<1) - (arrayIndex-guardCell);
            arrayAddrLeft     = srcAddr;
            arrayAddrRight    = srcAddr+((guardCell+arrayIndex+1)<<2);  
        }else if(arrayIndex<reary1)//21
        {
            arrayLenLeft      = trainCell;
            arrayLenRight     = trainCell;
            arrayAddrLeft     = srcAddr+((arrayIndex-guardCell-trainCell)<<2);
            arrayAddrRight    = srcAddr+((guardCell+arrayIndex+1)<<2);
        }else if(arrayIndex<reary2)//29
        {
            arrayLenLeft      = arrayIndex-reary1+trainCell;
            arrayLenRight     = trainCell-(arrayIndex-reary1);
            arrayAddrLeft     = srcAddr+((srcLen-(trainCell<<1)-(guardCell<<1)-1)<<2);
            arrayAddrRight    = srcAddr+((arrayIndex+guardCell+1)<<2);
        }else
        {
            arrayLenLeft      = trainCell<<1;
            arrayLenRight     = 0;
            arrayAddrLeft     = srcAddr+((arrayIndex-guardCell-(trainCell<<1))<<2);
            arrayAddrRight    = 0;      
        }
        cfarSumValueLeft  = accSum(arrayAddrLeft,  arrayLenLeft);
        cfarSumValueRight = accSum(arrayAddrRight, arrayLenRight);       
        switch (trainCell)
        {
            case 4:
                cfarSumValue = (cfarSumValueLeft + cfarSumValueRight)>>3;
                break;
            case 5:
                cfarSumValue = (cfarSumValueLeft + cfarSumValueRight)>>3;
                break;            
            case 6:
                cfarSumValue = (cfarSumValueLeft + cfarSumValueRight)>>3;
                break;
            case 7:
                cfarSumValue = (cfarSumValueLeft + cfarSumValueRight)>>4;
                break;      
            case 8:
                cfarSumValue = (cfarSumValueLeft + cfarSumValueRight)>>4;
                break; 
            case 9:
                cfarSumValue = (cfarSumValueLeft + cfarSumValueRight)>>4;
                break;             
        }       
        targetVelue = *((uint32 *)srcAddr + arrayIndex);
        if (targetVelue>(cfarSumValue*threshold)) 
        {
            det[arrayIndex]=1;        
            detectFlag = 0;
        }
    }
    return detectFlag;
} 

/****************************************************************************************
*Function name : 
*Description   : 
*Paramater	   :
*        @ :  
*        @ :   
*Paramater	selection:		

*Power consumption : 
*Return        :
****************************************************************************************/
int32 cfarOrd(uint32 trainCell, uint32 guardCell, uint32 srcAddr, uint32 srcLen, int32 threshold, int32 ordNum, char *det)
{
    uint32 arrayLenLeft;
    uint32 arrayLenRight;
    uint32 arrayAddrLeft;
    uint32 arrayAddrRight;
    uint32 cfarOrdValue;
    uint32 arrayOrdNum=trainCell*2;
    volatile uint32 arrayOrd[arrayOrdNum];
    int32 detectFlag = 1;
    int32 fronty1, fronty2, reary1, reary2;
    uint32 targetVelue;
    
    if(srcLen > trainCell + guardCell+1 && srcLen > 2*guardCell + 2)
    {            
        fronty1 =  guardCell;
        fronty2 = (trainCell + guardCell) ;
        /*  rear side edge points */
        reary1 = (srcLen - trainCell) - guardCell - 1;
        reary2 = srcLen - 2*guardCell + 1;
    }
    else
    {
        fronty1 = 0;
        fronty2 = 0;
        reary1 = 0;
        reary2 = 0;
    }

    for(int32 arrayIndex=0; arrayIndex<srcLen; arrayIndex++)
    {
        if(arrayIndex<fronty1)
        {
            arrayLenLeft      = 0;
            arrayLenRight     = trainCell*2;
            arrayAddrLeft     = 0;
            arrayAddrRight    = srcAddr+(guardCell+arrayIndex+1)*4;
        }else if(arrayIndex<=fronty2)
        {
            arrayLenLeft      = arrayIndex-guardCell;
            arrayLenRight     = trainCell*2 - (arrayIndex-guardCell);
            arrayAddrLeft     = srcAddr;
            arrayAddrRight    = srcAddr+(guardCell+arrayIndex+1)*4;  
        }else if(arrayIndex<reary1)//21
        {
            arrayLenLeft      = trainCell;
            arrayLenRight     = trainCell;
            arrayAddrLeft     = srcAddr+(arrayIndex-guardCell-trainCell)*4;
            arrayAddrRight    = srcAddr+(guardCell+arrayIndex+1)*4;
        }else if(arrayIndex<reary2)//29
        {
            arrayLenLeft      = arrayIndex-reary1+trainCell;
            arrayLenRight     = trainCell-(arrayIndex-reary1);
            arrayAddrLeft     = srcAddr+(srcLen-trainCell*2-guardCell*2-1)*4;
            arrayAddrRight    = srcAddr+(arrayIndex+guardCell+1)*4;
        }else
        {
            arrayLenLeft      = trainCell*2;
            arrayLenRight     = 0;
            arrayAddrLeft     = srcAddr+(arrayIndex-guardCell-trainCell*2)*4;
            arrayAddrRight    = 0;      
        }
        memScheduling(arrayAddrLeft,  (uint32)&arrayOrd, 4, 4, arrayLenLeft);
        memScheduling(arrayAddrRight, (uint32)&arrayOrd+arrayLenLeft, 4, 4, arrayLenRight);
        cfarOrdValue = accOdr((uint32)&arrayOrd, arrayOrdNum, ordNum);
        targetVelue = *((uint32 *)srcAddr + arrayIndex);
        if (targetVelue>(cfarOrdValue*threshold)) 
        {
            det[arrayIndex]=1;
            detectFlag = 0;
        }
    }
    return detectFlag;
} 
/****************************************************************************************
*Function name : 
*Description   : 
*Paramater	   :
*        @ :  
*        @ :   
*Paramater	selection:		

*Power consumption : 
*Return        :
****************************************************************************************/
int32 cfarSum_1_4(uint32 srcAddr, uint64 threshold, char *det)
{
    volatile uint64 cfarSumValue=0;
    uint32 accDone;
    volatile uint32 AccSumL;
    volatile uint32 AccSumH;
    volatile uint64 AccSum;
    uint32 targetVelue;
    int32 detectFlag = 1;
    volatile uint32 regVal;
    DSP->cfar_str = 0x00;
 
    for(int32 arrayIndex=0; arrayIndex<100; arrayIndex++)
    {
        if(arrayIndex<1)
        {
            DSP->cfar_mask0 = 0x000003FC;
            DSP->cfar_mask1 = 0x00000000;
        }else if(arrayIndex<=5)
        {
            DSP->cfar_mask0 = 0x7ff^(0x7<<(arrayIndex-1));
            DSP->cfar_mask1 = 0x00000000;
        }else if(arrayIndex<94)//21
        {
            DSP->cfar_mask0 = 0x0000078f;
            DSP->cfar_mask1 = 0x00000000;
        }else if(arrayIndex<99)//29
        {
            DSP->cfar_mask0 = 0x000007ff^(7<<(arrayIndex-94));
            DSP->cfar_mask1 = 0x00000000;
        }else
        {
            DSP->cfar_mask0 = 0x000000ff;
            DSP->cfar_mask1 = 0x00000000;
        }
       
        DSP->cfar_dat_number = 11;
        DSP->cfar_str = 0x01;
        uint32 * cfarAddrTemp;
        cfarAddrTemp = (uint32 *)(CFAR_ADDR);
        gpioOutput(30, 0);
        for (int k=0; k<11; k=k+1)
        {
            *(cfarAddrTemp) = *((uint32*)srcAddr+k);
        }
        //memScheduling((uint32)&arrayTemp, CFAR_ADDR, 4, 0, 8);
        
        gpioOutput(30, 1);
        accDone = 0;
        //write to CFAR
        while (1 != accDone)
        {
            accDone = DSP->cfar_done && cfar_done_acc_done_bit;
        }
        AccSumL = DSP->cfar_acc_sum_l;
        AccSumH = DSP->cfar_acc_sum_h;   
        AccSum = AccSumL + ((uint64)(AccSumH & 0x3f)<<32);
        
        //cfarSumValue = accSum((uint32)&arrayTemp, 8); 
        cfarSumValue = AccSum>>3;
      
        targetVelue = *((uint32 *)srcAddr + arrayIndex);
        if (targetVelue>(cfarSumValue*threshold)) 
        {
            det[arrayIndex]=1;        
            detectFlag = 0;
        }
    }
    return detectFlag;
} 
