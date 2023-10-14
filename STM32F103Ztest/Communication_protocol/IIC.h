#ifndef __IIC_H
#define __IIC_H
#include "stm32f10x.h"
#include "driver.h"
#include "MPU6050_Reg.h"
#include "Delay.h"
#include "Serial.h"
    #if IIC
    #define Software_IIC 1
    #define Hardware_IIC 0
    #define Software_IIC_SDA GPIO_Pin_9
    #define Software_IIC_SCL GPIO_Pin_8
    #define Software_IIC_PROT GPIOB
    void Software_IIC_Init(void);
    void Software_IIC_W_SCL(uint8_t BitValue);
    void Software_IIC_W_SDA(uint8_t BitValue);
    uint8_t Software_IIC_R_SDA(void);
    void Software_IIC_start(void);
    void Software_IIC_stop(void);
    void Software_IIC_write_byte(uint8_t byte);
    uint8_t Software_IIC_read_byte(void);
    void Software_IIC_send_ack(uint8_t BitValue);
    uint8_t Software_IIC_receive_ack(void);
    void Software_IIC_write_bytes(uint8_t* arr,uint8_t length);
    #endif
#endif
