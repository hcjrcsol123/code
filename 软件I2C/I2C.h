//一主多从模式
#ifndef __I2C_H
#define __I2C_H
#include "stm32f10x.h" 
#define SCL_PIN GPIO_Pin_10
#define SDA_PIN GPIO_Pin_11
#define I2C_PROT GPIOB


void MYI2C_W_SCL(uint8_t BitValue);
void MYI2C_W_SDA(uint8_t BitValue);
uint8_t MYI2C_R_SDA(void);
void I2CInit(void);
void I2C_START(void);
void I2C_stop(void);
void I2C_SendByte(uint8_t Byte);
uint8_t I2C_ReadByte(void);
void I2C_Send_Ack(uint8_t AckBit);
uint8_t I2C_Read_Ack(void);
#endif  
