#include "BC260Y.h"
#include "string.h"
#include "usart.h"
#include "stm32f1xx_hal.h"
#include "stdio.h"

char *strx;
unsigned char BC260_RxBuffer[255], BC260_RxCounter;

BC260Y BC260Y_Status;

void Clear_Buffer(void) // 清空缓存
{
    uint8_t i;
    HAL_UART_Transmit(&huart2, BC260_RxBuffer, strlen((char *)BC260_RxBuffer), HAL_MAX_DELAY);
    for (i = 0; i < 255; i++)
        BC260_RxBuffer[i] = 0; // 缓存清空
    BC260_RxCounter = 0;
    // 喂狗
}

void BC260Y_Init(void)
{
    uint8_t at_cmd[] = "AT\r\n";
    uint8_t cfun_cmd[] = "AT+CFUN=1\r\n";
    uint8_t cimi_cmd[] = "AT+CIMI\r\n";
    uint8_t cgatt_cmd[] = "AT+CGATT=1\r\n";
    uint8_t csq_cmd[] = "AT+CSQ\r\n";

    // 发送AT命令，正常返回OK
    HAL_UART_Transmit(&huart2, at_cmd, sizeof(at_cmd) - 1, HAL_MAX_DELAY);
    HAL_Delay(300);
    strx = strstr((const char *)BC260_RxBuffer, "OK"); // 返回OK
    Clear_Buffer();

    // 如果发送缓冲中等于空，重新再发AT命令，直到返回OK
    while (strx == NULL)
    {
        HAL_UART_Transmit(&huart2, at_cmd, sizeof(at_cmd) - 1, HAL_MAX_DELAY);
        HAL_Delay(300);
        strx = strstr((const char *)BC260_RxBuffer, "OK");
        Clear_Buffer();
    }

    // 设置全功能，返回OK
    HAL_UART_Transmit(&huart2, cfun_cmd, sizeof(cfun_cmd) - 1, HAL_MAX_DELAY);
    HAL_Delay(300);
    // 查询(U)SIM 卡 IMSI正常返回OK
    HAL_UART_Transmit(&huart2, cimi_cmd, sizeof(cimi_cmd) - 1, HAL_MAX_DELAY);
    HAL_Delay(300);

    strx = strstr((const char *)BC260_RxBuffer, "460"); // 返回460，表明识别到卡
    Clear_Buffer();

    // 如果发送缓冲中等于空，重新再发cimi_cmd命令，直到返回460
    while (strx == NULL)
    {
        HAL_UART_Transmit(&huart2, cimi_cmd, sizeof(cimi_cmd) - 1, HAL_MAX_DELAY);
        HAL_Delay(300);
        strx = strstr((const char *)BC260_RxBuffer, "460");
        Clear_Buffer();
    }

    HAL_UART_Transmit(&huart2, cgatt_cmd, sizeof(cgatt_cmd) - 1, HAL_MAX_DELAY);
    HAL_Delay(300);
    strx = strstr((const char *)BC260_RxBuffer, "OK");
    Clear_Buffer();

    // 查询网络激活状态
    HAL_UART_Transmit(&huart2, (uint8_t *)"AT+CGATT?\r\n", 12, HAL_MAX_DELAY);
    HAL_Delay(300);
    strx = strstr((const char *)BC260_RxBuffer, "+CGATT: 1");
    Clear_Buffer();

    while (strx == NULL)
    {
        HAL_UART_Transmit(&huart2, (uint8_t *)"AT+CGATT?\r\n", 12, HAL_MAX_DELAY);
        HAL_Delay(300);
        strx = strstr((const char *)BC260_RxBuffer, "+CGATT: 1");
        Clear_Buffer();
    }

    // 查询信号质量
    HAL_UART_Transmit(&huart2, csq_cmd, sizeof(csq_cmd) - 1, HAL_MAX_DELAY);
    HAL_Delay(300);
    Clear_Buffer();
}

// MQTT连接相关初始化
void MQTT_Init(void)
{
    // 打开MQTT客户端网络
    HAL_UART_Transmit(&huart2, (uint8_t *)"AT+QMTOPEN=0,\"iot-as-mqtt.cn-shanghai.aliyuncs.com\",1883\r\n", 36, HAL_MAX_DELAY);
    HAL_Delay(300);
    // 检查MQTT打开状态
    strx = strstr((const char *)BC260_RxBuffer, "+QMTOPEN: 0,0");

    // 清除缓冲
    while (strx == NULL)
    {
        strx = strstr((const char *)BC260_RxBuffer, "+QMTOPEN: 0,0");
        Clear_Buffer();
    }

    // 连接客户端到MQTT服务器
    HAL_UART_Transmit(&huart2, (uint8_t *)"AT+QMTCONN=0,\"COD1\"\r\n", 37, HAL_MAX_DELAY);
    HAL_Delay(300);

    // 检查连接状态
    strx = strstr((const char *)BC260_RxBuffer, "+QMTCONN: 0,0,0");

    // 清除缓冲
    while (strx == NULL)
    {
        strx = strstr((const char *)BC260_RxBuffer, "+QMTCONN: 0,0,0");
        Clear_Buffer();
    }

    // 订阅主题
    HAL_UART_Transmit(&huart2, (uint8_t *)"AT+QMTSUB=0,1,\"/k1swohbzYjV/COD1/user/get\",0\r\n", 28, HAL_MAX_DELAY); // 订阅主题
    HAL_Delay(500);

    // 检查订阅状态
    strx = strstr((const char *)BC260_RxBuffer, "+QMTSUB: 0,1,0,0");

    // 清除缓冲
    while (strx == NULL)
    {
        strx = strstr((const char *)BC260_RxBuffer, "+QMTSUB: 0,1,0,0");
        Clear_Buffer();
    }
}

// 往阿里云MQTT服务器发送数据
// 当前RS485接入COD，数据有 COD值，TEMP温度，NTU（浊度）值，三个值。
void aliyunMQTT_PUBdata(uint16_t cod, uint8_t temp, uint8_t ntu)
{
    uint8_t t_payload[200], len;

    // 发布主题
    HAL_UART_Transmit(&huart2, (uint8_t *)"AT+QMTPUB=0,0,0,0,\"k1swohbzYjV/COD1/user/update\" \r\n", 32, HAL_MAX_DELAY);
    HAL_Delay(300);

    // 传感器数据导入
    len = Mqttaliyun_Savedata(t_payload, cod, temp, ntu);
    t_payload[len] = 0;

    // 传感器数据发送
    HAL_UART_Transmit(&huart2, t_payload, len, HAL_MAX_DELAY);

    while (!(USART2->SR & 0x40)) // 等待发送完成
    {
    }

    USART2->DR = (uint8_t)0x1A; // 发送结束符
    HAL_Delay(100);

    // 检查发布状态
    strx = strstr((const char *)BC260_RxBuffer, "+QMTPUB: 0,0,0");

    // 清除缓冲
    while (strx == NULL)
    {
        strx = strstr((const char *)BC260_RxBuffer, "+QMTPUB: 0,0,0");
        Clear_Buffer();
    }
    Clear_Buffer();
}

// 计算发送数据大小
uint16_t Mqttaliyun_Savedata(uint8_t *t_payload, uint16_t cod, uint8_t temp, uint8_t ntu)
{
    char text[] = "{\"Current_COD\":{\"value\":%d},\"Current_TEMP\":{\"value\":%d},Current_NTU\":{\"value\":%d}}"; //     char json[]="{\"datastreams\":[{\"id\":\"location\",\"datapoints\":[{\"value\":{\"lon\":%2.6f,\"lat\":%2.6f}}]}]}";
    char t_text[200];
    unsigned short json_len;
    sprintf(t_text, text, cod, temp, ntu);
    json_len = strlen(t_text) / sizeof(char);
    memcpy(t_payload, t_text, json_len);
    return json_len;
}

// void BC260Y_Senddata(uint8_t *len, uint8_t *data) // 发送字符串数据
// {
//     char send_cmd[50];
//     sprintf(send_cmd, "AT+QSOSEND=0,%s,%s\r\n", len, data);
//     HAL_UART_Transmit(&huart2, (uint8_t *)send_cmd, strlen(send_cmd), HAL_MAX_DELAY);
// }

// void BC260Y_Senddatahex(uint8_t *len, uint8_t *data) // 发送十六进制数据
// {
//     char send_cmd[50];
//     sprintf(send_cmd, "AT+QISENDEX=0,%s,%s\r\n", len, data);
//     HAL_UART_Transmit(&huart2, (uint8_t *)send_cmd, strlen(send_cmd), HAL_MAX_DELAY);
//     HAL_Delay(300);
//     while (strx == NULL)
//     {
//         strx = strstr((const char *)RxBuffer, "SEND OK"); // 检查是否发送成功
//         HAL_Delay(100);
//     }
//     Clear_Buffer();
// }

// void BC260Y_ConTCP(void)
// {
//     // 关闭socket连接
//     HAL_UART_Transmit(&huart2, (uint8_t *)"AT+QICLOSE=0\r\n", 14, HAL_MAX_DELAY);
//     HAL_Delay(300);
//     Clear_Buffer();
// }

// void BC260Y_CreateTCPSokcet(void) // 创建TCP socket
// {
//     HAL_UART_Transmit(&huart2, (uint8_t *)"AT+QIOPEN=1,0,\"TCP\",\"47.99.80.89\",14269,1234,1\r\n", 50, HAL_MAX_DELAY); // 创建TCP连接
//     HAL_Delay(300);
//     strx = strstr((const char *)RxBuffer, "+QIOPEN: 0,0"); // 检查是否成功连接
//     while (strx == NULL)
//     {
//         strx = strstr((const char *)RxBuffer, "+QIOPEN: 0,0");
//         HAL_Delay(100);
//     }
//     Clear_Buffer();
// }
