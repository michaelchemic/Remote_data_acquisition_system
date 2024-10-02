#ifndef __COD_H
#define __COD_H

#include "stm32f1xx_it.h"
#include "stm32f1xx_hal.h"

#define COD_RX_BUFFER_SIZE 11 // 定义接收缓冲

// 下发帧：发送读取命令到地址为 0x01 的设备，
// 使用功能码 0x03 来读取寄存器的值。

#define DEVICE_ADDRESS 0x01
#define FUNCTION_CODE 0x03

void Modbus_Receive_Init(UART_HandleTypeDef *huart3);
void COD_Clear_RxBuffer(void);                                                                // 清空缓存
uint16_t ModRTU_CRC(uint8_t *buf, int len);                                                   // CRC计算函数
void Modbus_Read_Register(UART_HandleTypeDef *huart3, uint16_t reg_addr, uint16_t reg_count); // 发送读取命令


#endif
