#ifndef __SPI_H
#define __SPI_H
#include "adt3102_type_define.h"
#include "spi_ctype_map.h"
/****************************************************************************************
*
*Function name : spiInit
*Description   :  ___     ___     ___     _/////_     ___     ___     ___     ___     ___
            _____|   |___|   |___|   |___|       |___|   |___|   |___|   |___|   |___|   |___|
                 
                 
                


*Paramater		 : 
*          @SpiX :include SpiX SPI1


*Return        : None
****************************************************************************************/
void spiInit(SPI_TypeDef *SpiX);
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
void spiCsSelect(SPI_TypeDef *SpiX);
void spiCsDeselect(SPI_TypeDef *SpiX);
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
void spiWrite(SPI_TypeDef *SpiX,uint8 txData);
uint8 spiRead(SPI_TypeDef *SpiX);
void spiWriteBurst(SPI_TypeDef *SpiX,uint8 *pbuf,uint16 len);
void spiReadBurst(SPI_TypeDef *SpiX,uint8 *pbuf,uint16 len);
uint8 spiReadWriteByte(SPI_TypeDef *SpiX,uint8 TxData);
void Wait_bsy(SPI_TypeDef *SpiX);
void flashPd(SPI_TypeDef *SpiX);
void flashReleasePd(SPI_TypeDef *SpiX);

#endif

