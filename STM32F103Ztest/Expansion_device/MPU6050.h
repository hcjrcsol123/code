#ifndef __MPU6050_H
#define __MPU6050_H
#include "driver.h"
#include "IIC.h"
#include "MPU6050_Reg.h"
#define MPU6050 1
	#if MPU6050
	extern uint8_t dpack[12];
	void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data);
	uint8_t MPU6050_ReadReg(uint8_t RegAddress,uint8_t* pack,uint8_t length);//0写1读
	void MPU6050_Init(void);
	uint8_t MPU6050_GetID(void);
	void MPU6050_GetData(int16_t *AccX, int16_t *AccY, int16_t *AccZ, 
							int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ);
	#endif
#endif
