#ifndef __I2C_H
#define __I2C_H

#include "adt3102_type_define.h"
#include "i2c_ctype_map.h"
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
void i2cInit(I2C_TypeDef *i2cX);
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
void i2cSendByte(I2C_TypeDef *i2cX, uint8 txd);
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
uint8 iicRecvByte(I2C_TypeDef *i2cX);





#endif








