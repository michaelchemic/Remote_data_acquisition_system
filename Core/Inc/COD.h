#ifndef __COD_H
#define __COD_H	

#include "stm32f1xx_it.h"
#include "stm32f1xx_hal.h"

//下发帧：发送读取命令到地址为 0x01 的设备，
//使用功能码 0x03 来读取寄存器的值。

#define DEVICE_ADDRESS 0x01
#define FUNCTION_CODE  0x03

uint16_t ModRTU_CRC(uint8_t *buf, int len); //CRC计算函数
void Modbus_Read_Register(UART_HandleTypeDef *huart3, uint16_t reg_addr, uint16_t reg_count);// 发送读取命令
void Modbus_Receive_Response(UART_HandleTypeDef *huart3);// 接收响应并解析数据


#endif

