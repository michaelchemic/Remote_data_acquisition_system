#ifndef __BC260Y_H
#define __BC260Y_H

#include "stm32f1xx_it.h"
#include "stm32f1xx_hal.h"

void Clear_Buffer(void); // 清空缓存
void BC260Y_Init(void);
void Clear_Buffer(void);
void MQTT_Init(void);
void aliyunMQTT_PUBdata(uint16_t cod, uint8_t temp, uint8_t ntu);
uint16_t Mqttaliyun_Savedata(uint8_t * t_payload, uint16_t cod, uint8_t temp, uint8_t ntu);
//uint8_t Mqttaliyun_Savedata(uint8_t *t_payload, uint8_t temp, uint8_t humi);
//void MQTT_PUBdata(uint8_t temp, uint8_t humi);
// void BC260Y_PDPACT(void);
// void BC260Y_ConTCP(void);
// void BC260Y_CreateTCPSokcet(void);
// void BC260Y_RECData(void);
// void BC260Y_Senddata(uint8_t *len, uint8_t *data);
// void BC260Y_Senddatahex(uint8_t *len, uint8_t *data);
// void BC260Y_CreateSokcet(void);
// void BC260Y_ChecekConStatus(void);

typedef struct
{
    uint8_t CSQ;
    uint8_t Socketnum; // 编号
    uint8_t reclen;    // 获取到数据的长度
    uint8_t res;
    uint8_t recdatalen[10];
    uint8_t recdata[100];
    uint8_t netstatus; // 网络指示灯
} BC260Y;

#endif
