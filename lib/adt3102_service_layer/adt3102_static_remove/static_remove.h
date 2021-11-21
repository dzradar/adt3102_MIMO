//--------------------------------------------------------------------
//Copyright(c)2020,Andar Technologise Inc.
//All Rights Reserved
//Confidential Property of Andar Technologies Inc.
//
//Module Description:
//---------------------------------------------------------------
//To remove static object in rangeMax*nFftVel range. 
//Data arrangement (x0,0),(x0,1),(x0,2), ....,(x0,15),(x1,0),(x1,1)....
//  
//
//Created by :jiangqi
//$Revision: 1.5
//$Data: 2020/11/9
//--------------------------------------------------------------------
#ifndef __STATIC_REMOVE_H__  
#define __STATIC_REMOVE_H__ 
#include "adt3102.h"
/*********************************************************
Function name: staticRemove
Description:   To remove static object in 1st FFT table
Paramater:     srcAddr   : 1st FFT table start address
               nFftRange : FFT table range size
               rangeMax  : max range need to be processed
               nFftVel   : chirp number of FFT table
               nLog2FftVel : log2(nFftVel)
Return:        void
*********************************************************/
void staticRemove(int32 srcAddr,uint16 nFftRange,uint16 rangeMax, uint16 nFftVel,uint16 nLog2FftVel);

#endif

