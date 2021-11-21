//--------------------------------------------------------------------
//Copyright(c)2020,Andar Technologise Inc.
//All Rights Reserved
//Confidential Property of Andar Technologies Inc.
//
//Module Description: mimo gesture process
//Created by :jiangqi
//$Revision: 1.0
//$Data: 2021/6/9
//--------------------------------------------------------------------
#ifndef __PROC_GEST_H__  
#define __PROC_GEST_H__ 
#include "adt3102_type_define.h"
#include "adt3102.h"
#include "peak_single_mimo.h"

#define NONE  0
#define PULL  1
#define PUSH  2
#define LEFT  3
#define RIGHT 4
#define UP    5
#define DOWN  6
/*********************************************************
Function name: procGesture
Description:   mimo gesture process
Paramater:     obj: new peak information in current frame.
Return:        None
*********************************************************/
uint32 procGesture(OBJ_PEAKSINGLE_TypeDef obj);

/*********************************************************
Function name: angleMean
Description:   calculate horizontal and elevate mean angle in two parts. For horizontal, calculate left
               and right independently. For elevate, calculate top and bottom.
Paramater:     objResult: peak samples
               rsltN     : current position in objResult
               sigTogPos  : speed sign toggle position
               angleHMeanA : first part mean of horizontal angle
               angleHMeanB : second part mean of horizontal angle
               angleEMeanA : first part mean of elevate angle
               angleEMeanB : second part mean of elevate angle
Return:        None
*********************************************************/
void angleMean(OBJ_TypeDef* objResult, uint16 rsltN,uint16 sigTogPos, float *angleHMeanA, float *angleHMeanB,float *angleEMeanA,float *angleEMeanB);

/*********************************************************
Function name: angleSignCorrect
Description:   correct angles with big variation comapring to estimated
Paramater:     objResult: peak samples
               rsltN : current position in objResult
               sigTogPos: speed sign toggle position
Return:        None
*********************************************************/
void angleSignCorrect(OBJ_TypeDef* objResult, uint16 rsltN,uint16 sigTogPos);
/*********************************************************
Function name: angleCorrect
Description:   correct angles with big variation comapring to estimated
Paramater:     objResult: peak samples
               rsltN    : current position in objResult
               sigTogPos: speed sign toggle position
Return:        None
*********************************************************/
void angleCorrect(OBJ_TypeDef* objResult, uint16 rsltN,uint16 sigTogPos);

/*********************************************************
Function name: gestDetect
Description:   recognize gesture type
Paramater:     objResult: peak samples
               rsltN    : current position in objResult
               sigInit : sign of angle when action started
               sigTogPos : position when the first sign toggle happened.
               ppDet : 1: push or pull detected
Return:        None
*********************************************************/
uint32 gestDetect(OBJ_TypeDef* obj_result, uint16 rsltN,int8 sigInit,uint16 sigTogPos, uint16 ppDet);

/*********************************************************
Function name: gestDisplay
Description:   show gest result
Paramater:     gestType : gesture type detected. 
                        PULL,PUSH,LEFT,RIGHT,UP,DOWN
Return:        none
*********************************************************/
void gestDisplay(uint32 gestType);

#endif

