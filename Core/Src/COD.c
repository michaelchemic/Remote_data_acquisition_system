#include "COD.h"
#include "string.h"
#include "usart.h"
#include "stm32f1xx_hal.h"
#include "stdio.h"

uint8_t COD_RxBuffer[COD_RX_BUFFER_SIZE];
uint8_t COD_RxCompleteFlag = 0; // 接收完成标志

uint16_t cod_value = 0;
uint8_t temp_value = 0;
uint8_t ntu_value = 0;

float cod = 0.0;
float temp = 0.0;
float ntu = 0.0;

// CRC计算函数
uint16_t ModRTU_CRC(uint8_t *buf, int len)
{
    uint16_t crc = 0xFFFF;
    for (int pos = 0; pos < len; pos++)
    {
        crc ^= (uint16_t)buf[pos]; // XOR byte into least sig. byte of crc
        for (int i = 8; i != 0; i--)
        { // Loop over each bit
            if ((crc & 0x0001) != 0)
            {              // If the LSB is set
                crc >>= 1; // Shift right and XOR 0xA001
                crc ^= 0xA001;
            }
            else           // Else LSB is not set
                crc >>= 1; // Just shift right
        }
    }
    // Note, this number has low and high bytes swapped, so use it accordingly (or swap bytes)
    return crc;
}

// 发送读取命令
void Modbus_Read_Register(UART_HandleTypeDef *huart3, uint16_t reg_addr, uint16_t reg_count)
{
    uint8_t frame[8];
    frame[0] = DEVICE_ADDRESS;          // 设备地址
    frame[1] = FUNCTION_CODE;           // 功能码
    frame[2] = (reg_addr >> 8) & 0xFF;  // 寄存器地址高位
    frame[3] = reg_addr & 0xFF;         // 寄存器地址低位
    frame[4] = (reg_count >> 8) & 0xFF; // 读取寄存器数量高位
    frame[5] = reg_count & 0xFF;        // 读取寄存器数量低位

    uint16_t crc = ModRTU_CRC(frame, 6); // 计算CRC校验码
    frame[6] = crc & 0xFF;               // CRC低位
    frame[7] = (crc >> 8) & 0xFF;        // CRC高位

    // 发送命令帧
    HAL_UART_Transmit(huart3, frame, sizeof(frame), HAL_MAX_DELAY);
}

void COD_Clear_RxBuffer(void) // 清空缓存
{
    memset(COD_RxBuffer, 0, COD_RX_BUFFER_SIZE);
}

// 初始化中断接收
void Modbus_Receive_Init(UART_HandleTypeDef *huart3)
{
    // 启动UART3的中断接收
    if (HAL_UART_Receive_IT(huart3, COD_RxBuffer, COD_RX_BUFFER_SIZE) != HAL_OK)
    {
        // 接收启动失败处理
        Error_Handler();
    }
}

// 中断接收回调函数
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    // 如果是 UART3 触发的中断
    if (huart->Instance == USART3)
    {
        // 解析数据
        cod_value = (COD_RxBuffer[3] << 8) | COD_RxBuffer[4];
        temp_value = (COD_RxBuffer[5] << 8) | COD_RxBuffer[6];
        ntu_value = (COD_RxBuffer[7] << 8) | COD_RxBuffer[8];

        cod = cod_value / 10.0f;
        temp = temp_value / 10.0f;
        ntu = ntu_value / 10.0f;

        // 清空接收缓冲区
        COD_Clear_RxBuffer();

        // 继续启动中断接收
        if (HAL_UART_Receive_IT(huart, COD_RxBuffer, COD_RX_BUFFER_SIZE) != HAL_OK)
        {
            // 启动失败的处理
            Error_Handler();
        }
    }
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    // 如果是 UART3 触发的错误中断
    if (huart->Instance == USART3)
    {
        // 错误处理，比如重新启动接收
        HAL_UART_Receive_IT(huart, COD_RxBuffer, COD_RX_BUFFER_SIZE);
    }
}
