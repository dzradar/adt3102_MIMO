//--------------------------------------------------------------------
//Copyright(c)2020,Andar Technologise Inc.
//All Rights Reserved
//Confidential Property of Andar Technologies Inc.
//
//Module Description:
//
//
//
//
//
//
//Created by :wuhao
//$Revision: 1.0
//$Data: 2020/11/24
//--------------------------------------------------------------------
//
//All include header files
#include "adt3102_type_define.h"
#include "adt3102_uart.h"
#include "adt3102_spi.h"
#include "adt3102_udp.h"
#include "uart_ctype_map.h"
#include "gpio_ctype_map.h"
#include "stdio.h"
#include "adt3102.h"
//w5500 udp
#include "wizchip_conf.h"
#include "w5500.h"
#include "socket.h"
//
//------Global variable declaration

void spiCsSelectUdp(void)
{
    spiCsSelect(SPI0);
}

void spiCsDeselectUdp(void)
{
    spiCsDeselect(SPI0);
}

uint8 spiReadUdp(void)
{
    return spiRead(SPI0);
}
void spiWriteUdp(uint8 txData)
{
    spiWrite(SPI0, txData);
}

void spiReadBurstUdp(uint8 *pbuf,uint16 len)
{
    spiReadBurst(SPI0, pbuf, len);
}

void spiWriteBurstUdp(uint8 *pbuf,uint16 len)
{
    spiWriteBurst(SPI0, pbuf, len);
}


void register_wizchip(void)
{
    reg_wizchip_cs_cbfunc(spiCsSelectUdp,spiCsDeselectUdp);

    reg_wizchip_spi_cbfunc(spiReadUdp,spiWriteUdp);

    reg_wizchip_spiburst_cbfunc(spiReadBurstUdp,spiWriteBurstUdp);
}

void NetworkParameterConfiguration(void)
{
    uint8 tempstr[6];

    wiz_NetInfo gWIZNETINFO;

    gWIZNETINFO.dhcp = NETINFO_DHCP;
    gWIZNETINFO.ip[0] = 10;
    gWIZNETINFO.ip[1] = 10;
    gWIZNETINFO.ip[2] = 10;
    gWIZNETINFO.ip[3] = 100;

    gWIZNETINFO.mac[0] = 0xAA;
    gWIZNETINFO.mac[1] = 0xBB;
    gWIZNETINFO.mac[2] = 0xCC;
    gWIZNETINFO.mac[3] = 0xDD;
    gWIZNETINFO.mac[4] = 0xEE;
    gWIZNETINFO.mac[5] = 0xFF;

    gWIZNETINFO.sn[0] = 255;
    gWIZNETINFO.sn[0] = 255;
    gWIZNETINFO.sn[0] = 255;
    gWIZNETINFO.sn[0] = 0;

    gWIZNETINFO.gw[0] = 10;
    gWIZNETINFO.gw[1] = 10;
    gWIZNETINFO.gw[2] = 10;
    gWIZNETINFO.gw[3] = 10;



    netmode_type net_type;
    printf("prepare for wizchip sw reset \r\n");
    wizchip_sw_reset();
    printf("wizchip_sw_reset ok\r\n");
    ctlnetwork(CN_SET_NETINFO,(void*)&gWIZNETINFO);
    ctlnetwork(CN_GET_NETINFO,(void*)&gWIZNETINFO);
    ctlnetwork(CN_GET_NETMODE,(void*)&net_type);

    ctlwizchip(CW_GET_ID,(void*)tempstr);

    printf("=======The NetWork Set=========\r\n");

    printf("The IP ADDRESS   is: %d.%d.%d.%d \r\n",gWIZNETINFO.ip[0],gWIZNETINFO.ip[1],gWIZNETINFO.ip[2],gWIZNETINFO.ip[3]);
    printf("The MASK ADDRESS is: %d.%d.%d.%d \r\n",gWIZNETINFO.sn[0],gWIZNETINFO.sn[1],gWIZNETINFO.sn[2],gWIZNETINFO.sn[3]);

    printf("The NETMODE is : %d \r\n",net_type);
}
/****************************************************************************************
*
*Function name : updSendData
*Description   : Send data to PC through UDP
                 through NetAssist.exe receive data, save as dat file, receive without line feed
                 data process through adt300\adt3102_project\app\adt3102_basic\Data_process\matlab\udp_data_prosess.m
*Paramater	   :
*        @addr    :  data address
*        @flameNum:  send data frame number, 1 frame 1024 byte
                     data format: {buffhead + 1024xflameNum}
*Return        : None
****************************************************************************************/
void updSendData(uint32 addr,uint32 flameNum)
{
    uint8   remote_ip[4] = {10,10,10,10};        //configure remote IP address
    uint16  remote_port  = 6000;                 //configure remote port
    uint16  local_port   = 5000;                 //initialize a local port
    uint8   buffhead[4]  = {0x5a,0x5a,0x5a,0x5a};//frame header
    int32   send_len;
    int32 k   = 0;
    
    while(k<flameNum)
    {
        if(k == 64) {
            for(int32 i=0; i<10000; i++) {};
        }
        //printf("getSn_SR(0) is %x\r\n",getSn_SR(0));
        switch(getSn_SR(0))
        {
            case SOCK_UDP:
                //printf("The socket is open \r\n");
                if(getSn_IR(0) & Sn_IR_RECV)
                {
                    setSn_IR(0, Sn_IR_RECV);
                }
                if(k == 0) {
                    send_len = sendto(0,(uint8 *)(&buffhead),4, remote_ip, remote_port);
                }
                send_len  = sendto(0,(uint8 *)(addr + k*1024),1024, remote_ip, remote_port);
                if(send_len >0) k++;
                if(send_len < 0)
                {
                    //printf("send error \n");
                    break;
                }
                break;
            case SOCK_CLOSED:
                socket(0,Sn_MR_UDP,local_port,0);
                break;
        }
    }
}

