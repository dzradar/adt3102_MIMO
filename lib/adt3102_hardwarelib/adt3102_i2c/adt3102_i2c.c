//--------------------------------------------------------------------
//Copyright(c)2020,Andar Technologise Inc.
//All Rights Reserved
//Confidential Property of Andar Technologies Inc.
//
//Module Description:
//  .i2c initial
//  .i2c read and write data
//  .
//  
//Created by :wuhao
//$Revision: 1.0
//$Data: 2020/11/24
//--------------------------------------------------------------------
//
//All include header files
#include "stdio.h"
#include "adt3102_type_define.h"
#include "gpio_ctype_map.h"
#include "i2c_ctype_map.h"
#include "adt3102_i2c.h"
//
//------Global variable declaration


/****************************************************************************************
*
*Function name : i2cInit
*Description   : iic0 and iic1 setting
*Paramater	   :
*        @*i2cX   : iic include iic0 iic1
*        @
*
*Return        : None
****************************************************************************************/
void i2cInit(I2C_TypeDef *i2cX)
{
    if (i2cX == I2C0)
    {
        GPIO->gpio_p07_config=0x81;//i2c0scl
        GPIO->gpio_p08_config=0x81;//i2c0sda
	}else{
        GPIO->gpio_p17_config=0x81;//i2c1scl
        GPIO->gpio_p18_config=0x81;//i2c1sda
    }
    //i2c_i2cm_cfg[23:8]=  50M/(5*f)-1   100KHz=99=0x63.
	//1khz:2710  2khz:1388  3khz:D05
	i2cX->i2c_i2cm_cfg=0x3180;//I2C master config register
	i2cX->i2c_cfg=0xC0000;
}
	
/****************************************************************************************
*
*Function name : i2cSendByte
*Description   : iic0 and iic1 send 1byte data
*Paramater	   :
*        @*i2cX   : iic include iic0 iic1
*        @
*
*Return        : None
****************************************************************************************/
void i2cSendByte(I2C_TypeDef *i2cX, uint8 txd)
{ 
	i2cX->i2cir_tfifo_wr_data=txd;
}
/****************************************************************************************
*
*Function name : iicReadByte
*Description   : iic0 and iic1 receive 1byte data
*Paramater	   :
*        @*i2cX   : iic include iic0 iic1
*        @
*
*Return        : None
****************************************************************************************/
uint8 iicRecvByte(I2C_TypeDef *i2cX)
{
	uint8 receive;
	i2cX->i2c_i2cm_tar=0x50;
	i2cX->i2c_i2cm_cr=0xe8;
	receive=I2C0->i2cir_rfifo_rd_data;
	return receive;
}





