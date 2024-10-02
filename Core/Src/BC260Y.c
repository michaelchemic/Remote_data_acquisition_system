#include "BC260Y.h"
#include "string.h"
#include "usart.h"
#include "stm32f1xx_hal.h"
#include "stdio.h"

char *strx;

unsigned char BC260_RxBuffer[BC260_RX_BUFFER_SIZE], BC260_RxCounter;

BC260Y BC260Y_Status;

void Clear_Buffer()
{
    memset(BC260_RxBuffer, 0, sizeof(BC260_RxBuffer));                  // 清空缓冲区
    HAL_UART_Receive_IT(&huart2, BC260_RxBuffer, BC260_RX_BUFFER_SIZE); // 重新启动接收
}

void BC260Y_Init(void)
{

    uint8_t at_cmd[] = "AT\r\n";
    uint8_t cfun_cmd[] = "AT+CFUN=1\r\n";
    uint8_t cimi_cmd[] = "AT+CIMI\r\n";
    uint8_t cgatt_cmd[] = "AT+CGATT=1\r\n";
    uint8_t csq_cmd[] = "AT+CSQ\r\n";

    Clear_Buffer();
    // 启动 UART 接收中断
    HAL_UART_Receive_IT(&huart2, BC260_RxBuffer, BC260_RX_BUFFER_SIZE);

    // 发送AT命令，正常返回OK
    HAL_UART_Transmit(&huart2, at_cmd, sizeof(at_cmd) - 1, HAL_MAX_DELAY);
    HAL_Delay(800);

    // 如果没收到 OK，继续发送
    while (strstr((const char *)BC260_RxBuffer, "OK") == NULL)
    {
        HAL_UART_Transmit(&huart2, at_cmd, sizeof(at_cmd) - 1, HAL_MAX_DELAY);
        HAL_Delay(2000);
        Clear_Buffer();
    }

    // 设置全功能，返回OK
    HAL_UART_Transmit(&huart2, cfun_cmd, sizeof(cfun_cmd) - 1, HAL_MAX_DELAY);

    HAL_Delay(800);
    while (strstr((const char *)BC260_RxBuffer, "OK") == NULL)
    {
        HAL_UART_Transmit(&huart2, cfun_cmd, sizeof(cfun_cmd) - 1, HAL_MAX_DELAY);
        HAL_Delay(2000);
        Clear_Buffer();
    }

    // 设置全功能，返回OK
    HAL_UART_Transmit(&huart2, cgatt_cmd, sizeof(cgatt_cmd) - 1, HAL_MAX_DELAY);

    HAL_Delay(800);
    while (strstr((const char *)BC260_RxBuffer, "OK") == NULL)
    {
        HAL_UART_Transmit(&huart2, cgatt_cmd, sizeof(cgatt_cmd) - 1, HAL_MAX_DELAY);
        HAL_Delay(2000);
        Clear_Buffer();
    }

    // 查询(U)SIM 卡 IMSI正常返回OK
    HAL_UART_Transmit(&huart2, cimi_cmd, sizeof(cimi_cmd) - 1, HAL_MAX_DELAY);
    HAL_Delay(800);

    // 如果没识别到 SIM 卡 IMSI，继续发送
    while (strstr((const char *)BC260_RxBuffer, "460") == NULL)
    {
        HAL_UART_Transmit(&huart2, cimi_cmd, sizeof(cimi_cmd) - 1, HAL_MAX_DELAY);
        HAL_Delay(2000);
        Clear_Buffer();
    }
}

void MQTT_Init(void)
{

    uint8_t at_qmtcfg[] = "AT+QMTCFG=ALIAUTH,0,\"k1swohbzYjV\",\"COD1\",\"f82020ff53e1dd7afd10c41e0c084826\"\r\n";
    uint8_t at_qmtopen[] = "AT+QMTOPEN=0,\"iot-as-mqtt.cn-shanghai.aliyuncs.com\",1883\r\n";
    uint8_t at_qmtconn[] = "AT+QMTCONN=0,\"COD1\"\r\n";
    uint8_t at_qmtsub[] = "AT+QMTSUB=0,1,\"/k1swohbzYjV/COD1/user/get\",0\r\n";

    // 启动 UART 接收中断
    HAL_UART_Receive_IT(&huart2, BC260_RxBuffer, BC260_RX_BUFFER_SIZE);

    HAL_UART_Transmit(&huart2, at_qmtcfg, sizeof(at_qmtcfg) - 1, HAL_MAX_DELAY);

    HAL_Delay(1000);
    while (strstr((const char *)BC260_RxBuffer, "OK") == NULL)
    {
        HAL_UART_Transmit(&huart2, at_qmtcfg, sizeof(at_qmtcfg) - 1, HAL_MAX_DELAY);
        HAL_Delay(2000);
    }

    HAL_UART_Transmit(&huart2, at_qmtopen, sizeof(at_qmtopen) - 1, HAL_MAX_DELAY);

    HAL_Delay(1000);
    while (strstr((const char *)BC260_RxBuffer, "OK") == NULL)
    {
        HAL_UART_Transmit(&huart2, at_qmtopen, sizeof(at_qmtopen) - 1, HAL_MAX_DELAY);
        HAL_Delay(2000);
    }

    HAL_UART_Transmit(&huart2, at_qmtconn, sizeof(at_qmtconn) - 1, HAL_MAX_DELAY);

    HAL_Delay(1000);
    while (strstr((const char *)BC260_RxBuffer, "OK") == NULL)
    {
        HAL_UART_Transmit(&huart2, at_qmtconn, sizeof(at_qmtconn) - 1, HAL_MAX_DELAY);
        HAL_Delay(2000);
    }

    HAL_UART_Transmit(&huart2, at_qmtsub, sizeof(at_qmtsub) - 1, HAL_MAX_DELAY);

    HAL_Delay(800);
    while (strstr((const char *)BC260_RxBuffer, "OK") == NULL)
    {
        HAL_UART_Transmit(&huart2, at_qmtsub, sizeof(at_qmtsub) - 1, HAL_MAX_DELAY);
        HAL_Delay(2000);
    }
    Clear_Buffer();
}

// 往阿里云MQTT服务器发送数据
// 当前RS485接入COD，数据有 COD值，TEMP温度，NTU（浊度）值，三个值。
void aliyunMQTT_PUBdata(float cod, float temp, float ntu)
{
    uint8_t t_payload[200], len;
    uint8_t at_qmtpub[] = "AT+QMTPUB=0,0,0,0,\"/k1swohbzYjV/COD1/user/update\"\r\n";
    // 发布主题
    HAL_UART_Transmit(&huart2, at_qmtpub, sizeof(at_qmtpub) - 1, HAL_MAX_DELAY);
    HAL_Delay(800);

    // 传感器数据导入
    len = Mqttaliyun_Savedata(t_payload, cod, temp, ntu);
    t_payload[len] = 0;

    // 传感器数据发送
    HAL_UART_Transmit(&huart2, t_payload, len, HAL_MAX_DELAY);

    while (!(USART2->SR & 0x40)) // 等待发送完成
    {
    }

    USART2->DR = (uint8_t)0x1A; // 发送结束符
    HAL_Delay(800);

    Clear_Buffer();
}

// 计算发送数据大小
uint16_t Mqttaliyun_Savedata(uint8_t *t_payload, float cod, float temp, float ntu)
{
    char t_text[200];
    char text[] = "{COD:%.2f,TEMP:%.2f,NTU:%.2f}";

    unsigned short text_len;

    sprintf(t_text, text, cod, temp, ntu);
    text_len = strlen(t_text) / sizeof(char);
    memcpy(t_payload, t_text, text_len);
    return text_len;
}
