#ifndef __CFAR_H
#define __CFAR_H
#include "adt3102_type_define.h"
#include "adt3102.h" 
/****************************************************************************************
*Function name : accSum
*Description   : sum multiple numbers, the number of summated numbers is less than 64
*Paramater	   :
*        @sumAddr  :  The first address of the sum
*        @transNum :  The number of numbers to be summed
*Paramater	selection:		

*Power consumption : 
*Return        :The result of summation
****************************************************************************************/
uint64 accSum(uint32 sumAddr, int32 transNum);
/****************************************************************************************
*Function name : accOdr
*Description   : sort some numbers, the number of sorted numbers is less than 64
*Paramater	   :
*        @sumAddr  :  The first address of the sort
*        @transNum :  The number of numbers to be sorted
*        @ordNum   :  Index to indicate which data to report after order sort. Must set to none 0. 
                      Set to 1 means the seconde minimum data to report, 
                      set to field_dat_num-1 means the largest data will be reported.
*Paramater	selection:		

*Power consumption : 
*Return        :The result of sorted
****************************************************************************************/
uint32 accOdr(uint32 sumAddr, int32 transNum, int32 ordNum);
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
int32 cfarSum(uint32 trainCell, uint32 guardCell, uint32 srcAddr, uint32 srcLen, uint64 threshold,char *det);
int32 cfarOrd(uint32 trainCell, uint32 guardCell, uint32 srcAddr, uint32 srcLen, int32 threshold, int32 ordNum, char *det);
int32 cfarSum_1_4(uint32 srcAddr, uint64 threshold, char *det);


#endif




