#ifndef __BC260Y_H
#define __BC260Y_H

#include "stm32f1xx_it.h"
#include "stm32f1xx_hal.h"

#define BC260_RX_BUFFER_SIZE 255 // 定义接收缓冲

void Clear_Buffer(void); // 清空缓存
void BC260Y_Init(void);
void Clear_Buffer(void);
void MQTT_Init(void);
void aliyunMQTT_PUBdata(float cod, float temp, float ntu);
uint16_t Mqttaliyun_Savedata(uint8_t *t_payload, float cod, float temp, float ntu);

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
