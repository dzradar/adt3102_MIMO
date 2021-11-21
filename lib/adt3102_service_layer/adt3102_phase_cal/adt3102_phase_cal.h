//--------------------------------------------------------------------
//Copyright(c)2021,Andar Technologise Inc.
//All Rights Reserved
//Confidential Property of Andar Technologies Inc.
//
//Module Description: phase calculate
//
//Created by :JiangQi
//$Revision: 0.1
//$Data: 2021/6/15
//--------------------------------------------------------------------
#ifndef __PHASE_CAL_H
#define __PHASE_CAL_H
#include "adt3102_type_define.h"

typedef struct
{
    float phaseCh0;
    float phaseCh1;
    float phaseDiff;
    float angle;
} PHASE_TypeDef;

/*********************************************************
Function name: phaseCal
Description:   Calculate phase and angle from 2 channels' I and Q
Paramater:     iCh0 : channel 0 I
               qCh0 : channel 0 Q
               iCh1 : channel 1 I
               qCh1 : channel 1 Q
           phaseCor : correction for phase difference between channel 0 and channel 1. Should be negative if channel 0 is leading.   
Return:        PHASE_TypeDef
            phaseCh0: phase of channel 0,in radian
            phaseCh1: phase of channel 1,in radian
           phaseDiff: phaseCh0-phaseCh1+phaseCor,in radian
               angle: object angle in degree 
*********************************************************/
PHASE_TypeDef phaseCal(int16 iCh0,int16 qCh0, int16 iCh1, int16 qCh1, float phaseCor);
/*********************************************************
Function name: angleCorrect1
Description:   correct angle 
Paramater:     angleIn : angle before correction
Return:        float, angle after correction
*********************************************************/
float angleCorrect1(float angleIn);
#endif



