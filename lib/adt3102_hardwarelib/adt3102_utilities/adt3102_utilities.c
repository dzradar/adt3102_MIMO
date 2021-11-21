//--------------------------------------------------------------------
//Copyright(c)2020,Andar Technologise Inc.
//All Rights Reserved
//Confidential Property of Andar Technologies Inc.
//
//Module Description:
//  adc config
//  adc sampling data with dma
//  
//  
//
//Created by :qijiang
//$Revision: 1.0
//$Data: 2021/05/07
//--------------------------------------------------------------------
//
//All include header files
#include "adt3102.h"
#include "adt3102_utilities.h"
#include "adt3102_type_define.h"
#include "rfc_ctype_map.h"
#include "pmu_ctype_map.h"
#include "adt3102_system.h"
#include "adt3102_tx.h"
//
//------Global variable declaration


/****************************************************************************************
*
*Function name : fmcwChirpSingleFrequency
*Description   : Set FMCW to single tone mode. Tone frequency is FMCW start frequency.

*Paramater	   : None
*Return        : None
****************************************************************************************/
void fmcwChirpSingleFrequency(void)
{
  // set a single frequency, Only for test purpose.  
  // for example : 
  // FMCW_chirp(76,4000,346,20,0,32); //FMCW config Fstart ,FM , T0, T1, T2, chirp number; 
  // fmcwChirpSingleFrequency();   // then you will get a single frequency.
  RFC->fmcw_reg19_reg &= 0x0f ; //((ch0_df_step &0xF)<<4);// ch0 df_step
  RFC->fmcw_reg20_reg = 0x0 ;   //(ch0_df_step &0xFF)>>4; // ch0 df_step

  RFC->fmcw_reg24_reg &= 0x0f ; //((ch1_df_step &0xF)<<4);// ch1 df_step
  RFC->fmcw_reg25_reg = 0x0 ;   //(ch1_df_step &0xFF)>>4; // ch1 df_step

  RFC->fmcw_reg29_reg &= 0x0f ; //((ch2_df_step &0xF)<<4);// ch2 df_step
  RFC->fmcw_reg30_reg = 0x0 ;   //(ch2_df_step &0xFF)>>4; // ch2 df_step

  RFC->fmcw_reg18_reg &= 0xfd ; //ch0's clear [1] = 0. 
  RFC->fmcw_reg23_reg &= 0xfd ; //ch1's clear [1] = 0. 
  RFC->fmcw_reg28_reg &= 0xfd ; //ch2's clear [1] = 0. 
}

/****************************************************************************************
*
*Function name : bistOn
*Description   : Enable bist

*Paramater	   : bistSorSel: bist source select. 1 : from RF 0: from baseband
                 bistDivSel: bist divided frequency from 50MHz.
*Return        : None
****************************************************************************************/
void bistOn(uint16 bistSorSel, uint16 bistDivSel)
{
    RFC->rfc_plli_ctrl_reg |= 1<<2;
    PMU->pmu_pmu_ctrl_reg |= pmu_pmu_ctrl_reg_manu_ctrl_en_bit;
    delayUs(100, HIGH_CLOCK);    
    PMU->pmu_pmu_ctrl_reg &= ~pmu_pmu_ctrl_reg_manu_ctrl_en_bit;
    
    biasBist(8, 8, 8, 8);
    
    if (bistSorSel == 1)
    {
        //BIST_EN_ONOFF_AMP ON
        RFC->rf_bist_cfg_reg = 0x07|bistDivSel<<8|0x01<<16|0x01<<24;
    }else
    {
        //BIST_RX0_ABB_EN BIST_RX1_ABB_EN ON
        RFC->rf_bist_cfg_reg = 0x35|bistDivSel<<8|0x01<<16|0x01<<24;
    }
    RFC->rf_bist_load = 1;
}


