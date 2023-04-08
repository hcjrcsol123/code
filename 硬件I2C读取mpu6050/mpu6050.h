#ifndef __MPU6050_H
#define __MPU6050_H
#include "OLED.h"
#include "MPU6050_Reg.h"
#define mpu6050ADD 0xd0
extern int16_t ACCX;
extern int16_t ACCY;
extern int16_t ACCZ;
extern int16_t GYROX;
extern int16_t GYROY;
extern int16_t GYROZ;
void mpu6050_SendReg(uint8_t Regadd, uint8_t Data);
uint8_t mpu6050_ReadReg(uint8_t Regadd);//读取一个字节
void mpu6050_Init(void);
void mpu6050_GetData(int16_t *accx,int16_t *accy,int16_t *accz,int16_t *gyrox,int16_t *gyroy,int16_t *gyroz);//用传址调用，实现多参数返回。
void I2C2_init(void);
void I2C_WaitEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT);//将I2C_CheckEvent进行封装，保证其不会卡死在循环。
#endif
