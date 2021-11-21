//--------------------------------------------------------------------
//Copyright(c)2020,Andar Technologise Inc.
//All Rights Reserved
//Confidential Property of Andar Technologies Inc.
//
//Module Description: angle process for mimo gesture
//Created by :jiangqi
//$Revision: 1.0
//$Data: 2020/11/9
//--------------------------------------------------------------------
#ifndef __ANGLE_PROC_H
#define __ANGLE_PROC_H
#include "adt3102_type_define.h"
#include "adt3102.h"

/*********************************************************
Function name: angleProc
Description:   Calculate horizontal and elevation angle 
Paramater:     max_pos_rng : range position of peak search result 
               max_pos_dop : doppler position of peak search result 
               rslt_n      : indicate how many valid peaks there are in current action
               obj_result  : structure including angle process result
Return:        void
*********************************************************/
void angleProc(uint16 max_pos_rng, uint16 max_pos_dop,int32 rslt_n, OBJ_TypeDef* obj_result);

#endif



