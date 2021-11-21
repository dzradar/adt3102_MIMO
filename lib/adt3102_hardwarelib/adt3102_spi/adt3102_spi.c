//--------------------------------------------------------------------
//Copyright(c)2020,Andar Technologise Inc.
//All Rights Reserved
//Confidential Property of Andar Technologies Inc.
//
//Module Description:
//  .spi initial
//  .spi read and write data
//  .
//  
//Created by :wuhao
//$Revision: 1.0
//$Data: 2020/11/24
//--------------------------------------------------------------------
//
//All include header files
#include "adt3102_type_define.h"
#include "gpio_ctype_map.h"
#include "spi_ctype_map.h"
#include "adt3102_spi.h"
#include "adt3102_gpio.h"
#include "adt3102.h"
#include "stdio.h"
//
//------Global variable declaration

/****************************************************************************************
*
*Function name : spiInit
*Description   :  ___     ___     ___     _/////_     ___     ___     ___     ___     ___
            _____|   |___|   |___|   |___|       |___|   |___|   |___|   |___|   |___|   |___|
                 
                 
                


*Paramater		 : 
*          @SpiX :include SpiX SPI1


*Return        : None
****************************************************************************************/
void spiInit(SPI_TypeDef *SpiX)
{
    if (SpiX == SPI0)
    {
        GPIO->gpio_p03_config=1<<0|1<<9|1<<10; //sclk  GPIO3
        GPIO->gpio_p04_config=1<<0|1<<9|1<<10; //csn   GPIO4
        GPIO->gpio_p05_config=1<<0|1<<9|1<<10; //miso  GPIO5
        GPIO->gpio_p06_config=1<<0|1<<9|1<<10; //mosi  GPIO6
    }else{
        GPIO->gpio_p13_config=1<<0|1<<9|1<<10; //sclk  GPI13
        GPIO->gpio_p14_config=1<<0|1<<9|1<<10; //csn   GPI14
        GPIO->gpio_p15_config=1<<0|1<<9|1<<10; //miso  GPI15
        GPIO->gpio_p16_config=1<<0|1<<9|1<<10; //mosi  GPI16
    }
    SpiX->SSPCR0=0x07|0<<6|0<<7;
	SpiX->SSPCPSR=6;
	SpiX->SSPCR1=1<<1;
}
/****************************************************************************************
*
*Function name : spiCsSelect spiCsDeselect
*Description   : manually set the cs signal of spi 
                 spiCsSelect:high level     spiCsDeselect:low level

*Paramater	   : 
*          @SpiX :include SpiX SPI1
*          @ :

*Return        : None
****************************************************************************************/
void spiCsSelect(SPI_TypeDef *SpiX)
{
    if (SpiX == SPI0)
    {
        gpioOutput(4,1);
    }else{
        gpioOutput(14,1);
    }
}
void spiCsDeselect(SPI_TypeDef *SpiX)
{
    if (SpiX == SPI0)
    {
        gpioOutput(4,0);
    }else{
        gpioOutput(14,0);
    }
}

/****************************************************************************************
*
*Function name : spi read and write data
*Description   : 
                 spiWrite: spi write 1 byte data
                 spiRead: spi read 1 byte data
                 spiWriteBurst: spi batch write data
                 spiReadBurst:  spi batch read data
                 spiReadWriteByte: spi write and read 1byte data

*Paramater	   : 
*          @SpiX :include SpiX SPI1
*          @ :

*Return        : None
****************************************************************************************/
//SPI--UPD spi function
void spiWrite(SPI_TypeDef *SpiX,uint8 txData)
{  
    while((SpiX->SSPSR&0x02)!=0x02);
    while((SpiX->SSPSR&0x10)!=0x00);
    SpiX->SSPDR=txData;		
    while((SpiX->SSPSR&0x04)!=0x04);
    while((SpiX->SSPSR&0x10)!=0x00);
}
uint8 spiRead(SPI_TypeDef *SpiX)
{ 
    while((SpiX->SSPSR&0x02)!=0x02);
    while((SpiX->SSPSR&0x10)!=0x00);
    SpiX->SSPDR=0xaa;
    while((SpiX->SSPSR&0x04)!=0x04);
    while((SpiX->SSPSR&0x10)!=0x00);
    return SpiX->SSPDR; 
}

void spiWriteBurst(SPI_TypeDef *SpiX,uint8 *pbuf,uint16 len)
{
	volatile uint8 temp;
    uint16 j=0;
	for(int32 i=0;i<len-8;i=i+8)
	{
        while((SpiX->SSPSR&0x01)!=0x01);
        SpiX->SSPDR=pbuf[i];		
        SpiX->SSPDR=pbuf[i+1];		
        SpiX->SSPDR=pbuf[i+2];		
        SpiX->SSPDR=pbuf[i+3];		
        SpiX->SSPDR=pbuf[i+4];		
        SpiX->SSPDR=pbuf[i+5];		
        SpiX->SSPDR=pbuf[i+6];		
        SpiX->SSPDR=pbuf[i+7];		
        while((SpiX->SSPSR&0x08)!=0x08);
        temp = SpiX->SSPDR;
        temp = SpiX->SSPDR;
        temp = SpiX->SSPDR;
        temp = SpiX->SSPDR;
        temp = SpiX->SSPDR;
        temp = SpiX->SSPDR;
        temp = SpiX->SSPDR;
        temp = SpiX->SSPDR;
        j=j+8;
	}
    
    for(int32 k=j;k<len;k++)
	{
        while((SpiX->SSPSR&0x02)!=0x02);
        SpiX->SSPDR=pbuf[k];		
        while((SpiX->SSPSR&0x04)!=0x04);
        temp = SpiX->SSPDR;
	}
}

void spiReadBurst(SPI_TypeDef *SpiX,uint8 *pbuf,uint16 len)
{
	volatile uint8 temp;
    uint16 j=0;
	for(int32 i=0;i<len-8;i=i+8)
	{
        while((SpiX->SSPSR&0x01)!=0x01);
        for (int32 p=0;p<8;p++)
        {
          SpiX->SSPDR=0xaa;
        }            
        while((SpiX->SSPSR&0x08)!=0x08);
        pbuf[i]   = SpiX->SSPDR; 
        pbuf[i+1] = SpiX->SSPDR; 
        pbuf[i+2] = SpiX->SSPDR; 
        pbuf[i+3] = SpiX->SSPDR; 
        pbuf[i+4] = SpiX->SSPDR; 
        pbuf[i+5] = SpiX->SSPDR; 
        pbuf[i+6] = SpiX->SSPDR; 
        pbuf[i+7] = SpiX->SSPDR; 
        j=j+8;
	}
    
    for(int32 k=j;k<len;k++)
	{
        while((SpiX->SSPSR&0x02)!=0x02);
        SpiX->SSPDR=0xaa;
        while((SpiX->SSPSR&0x04)!=0x04);
        pbuf[k] = SpiX->SSPDR;
	}
}

uint8 spiReadWriteByte(SPI_TypeDef *SpiX,uint8 TxData)
{
	SpiX->SSPDR=TxData;		
	while((SpiX->SSPSR&0x01)!=0x01){};
    while((SpiX->SSPSR&0x04)!=0x04){};
    return SpiX->SSPDR; 
}

void Wait_bsy(SPI_TypeDef *SpiX)
{
  while((SpiX->SSPSR&0x10)==0x10);//bsyλ
}

void flashPd(SPI_TypeDef *SpiX)
{
    spiReadWriteByte(SpiX, 0xB9);
	Wait_bsy(SpiX);
}

void flashReleasePd(SPI_TypeDef *SpiX)
{
    spiReadWriteByte(SpiX, 0xAB);
	Wait_bsy(SpiX);
}


