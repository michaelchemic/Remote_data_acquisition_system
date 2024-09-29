#ifndef __BC260Y_H
#define __BC260Y_H	
#include "usart.h"
#include <stm32f10x.h>
#include "delay.h"
void Clear_Buffer(void);//清空缓存	
void BC260Y_Init(void);
void BC260Y_PDPACT(void);
void BC260Y_ConTCP(void);
void BC260Y_CreateUDPSokcet(void);
void BC260Y_RECData(void);
void BC260Y_Senddata(uint8_t *len,uint8_t *data);
void BC260Y_Senddatahex(uint8_t *len,uint8_t *data);
void BC260Y_CreateSokcet(void);
void Clear_Buffer(void);
void BC260Y_ChecekConStatus(void);
void aliyunMQTT_PUBdata(u8 temp,u8 humi);

typedef struct
{
    uint8_t CSQ;    
    uint8_t Socketnum;   //编号
    uint8_t reclen;   //获取到数据的长度
    uint8_t res;      
    uint8_t recdatalen[10];
    uint8_t recdata[100];
    uint8_t netstatus;//网络指示灯
} BC260Y;

#endif





