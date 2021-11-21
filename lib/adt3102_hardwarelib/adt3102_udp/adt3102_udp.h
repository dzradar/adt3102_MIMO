#ifndef __UDP_H
#define __UDP_H
#include "adt3102_type_define.h"
#include "uart_ctype_map.h"
#include "adt3102_udp.h"

void spiCsSelectUdp(void);
void spiCsDeselectUdp(void);
uint8 spiReadUdp(void);
void spiWriteUdp(uint8 txData);
void spiReadBurstUdp(uint8 *pbuf,uint16 len);
void spiWriteBurstUdp(uint8 *pbuf,uint16 len);
void register_wizchip(void);
void NetworkParameterConfiguration(void);
void Udp_Sample_adc(uint32 addr,uint32 flameNum);



#endif


