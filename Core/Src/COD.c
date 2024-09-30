#include "COD.h"
#include "string.h"
#include "usart.h"
#include "stm32f1xx_hal.h"
#include "stdio.h"

// CRC计算函数
uint16_t ModRTU_CRC(uint8_t *buf, int len) {
    uint16_t crc = 0xFFFF;
    for (int pos = 0; pos < len; pos++) {
        crc ^= (uint16_t)buf[pos];          // XOR byte into least sig. byte of crc
        for (int i = 8; i != 0; i--) {      // Loop over each bit
            if ((crc & 0x0001) != 0) {      // If the LSB is set
                crc >>= 1;                  // Shift right and XOR 0xA001
                crc ^= 0xA001;
            } else                          // Else LSB is not set
                crc >>= 1;                  // Just shift right
        }
    }
    // Note, this number has low and high bytes swapped, so use it accordingly (or swap bytes)
    return crc;
}



// 发送读取命令
void Modbus_Read_Register(UART_HandleTypeDef *huart3, uint16_t reg_addr, uint16_t reg_count) {
    uint8_t frame[8];
    frame[0] = DEVICE_ADDRESS;              // 设备地址
    frame[1] = FUNCTION_CODE;               // 功能码
    frame[2] = (reg_addr >> 8) & 0xFF;      // 寄存器地址高位
    frame[3] = reg_addr & 0xFF;             // 寄存器地址低位
    frame[4] = (reg_count >> 8) & 0xFF;     // 读取寄存器数量高位
    frame[5] = reg_count & 0xFF;            // 读取寄存器数量低位

    uint16_t crc = ModRTU_CRC(frame, 6);    // 计算CRC校验码
    frame[6] = crc & 0xFF;                  // CRC低位
    frame[7] = (crc >> 8) & 0xFF;           // CRC高位

    // 发送命令帧
    HAL_UART_Transmit(huart3, frame, sizeof(frame), HAL_MAX_DELAY);
}

// 接收响应并解析数据
void Modbus_Receive_Response(UART_HandleTypeDef *huart3) {
    uint8_t response[11];                   // 响应帧的字节数
    HAL_UART_Receive(huart3, response, sizeof(response), HAL_MAX_DELAY);

    // 校验CRC
    uint16_t crc_received = (response[9] << 8) | response[8];
    uint16_t crc_calculated = ModRTU_CRC(response, 8);
    if (crc_received != crc_calculated) {
        printf("CRC Error!\n");
        return;
    }

    // 解析数据
    uint16_t cod_value = (response[3] << 8) | response[4];
    uint16_t temp_value = (response[5] << 8) | response[6];
    uint16_t turbidity_value = (response[7] << 8) | response[8];

    float cod = cod_value / 10.0f;
    float temperature = temp_value / 10.0f;
    float turbidity = turbidity_value / 10.0f;

    printf("COD: %.1f mg/L, Temperature: %.1f C, Turbidity: %.1f NTU\n", cod, temperature, turbidity);
}




