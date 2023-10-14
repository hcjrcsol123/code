#include "MPU6050.h"
	#if MPU6050
	#define MPU6050_ADDRESS		0xD0
	uint8_t dpack[12]={0};
	void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data)
	{
		Software_IIC_start();
		Software_IIC_write_byte(MPU6050_ADDRESS);//默认最后一位写
		if(Software_IIC_receive_ack()==1) Software_IIC_stop();
		Software_IIC_write_byte(RegAddress);
		if(Software_IIC_receive_ack()==1) Software_IIC_stop();
		Software_IIC_write_byte(Data);
		if(Software_IIC_receive_ack()==1) Software_IIC_stop();
		Software_IIC_stop();
	}

	uint8_t MPU6050_ReadReg(uint8_t RegAddress,uint8_t* pack,uint8_t length)//0写1读
	{
		uint8_t Data=0;
		uint8_t i=0;
		Software_IIC_start();
		Software_IIC_write_byte(MPU6050_ADDRESS);
    	if(Software_IIC_receive_ack()==1) Software_IIC_stop();
		Software_IIC_write_byte(RegAddress);
    	if(Software_IIC_receive_ack()==1) Software_IIC_stop();
		
		Software_IIC_start();
		Software_IIC_write_byte(MPU6050_ADDRESS | 0x01);//头字节读
		if(Software_IIC_receive_ack()==1) Software_IIC_stop();
		for(i=0;i<length;i++)
		{
			dpack[i]=Software_IIC_read_byte();
			Software_IIC_send_ack(0);
		}
		// Data = Software_IIC_read_byte();
		Software_IIC_send_ack(1);
		Software_IIC_stop();
		
		return Data;
	}

	void MPU6050_Init(void)
	{
		Software_IIC_Init();
		MPU6050_WriteReg(MPU6050_PWR_MGMT_1, 0x01);
		MPU6050_WriteReg(MPU6050_PWR_MGMT_2, 0x00);
		MPU6050_WriteReg(MPU6050_SMPLRT_DIV, 0x09);
		MPU6050_WriteReg(MPU6050_CONFIG, 0x06);
		MPU6050_WriteReg(MPU6050_GYRO_CONFIG, 0x18);
		MPU6050_WriteReg(MPU6050_ACCEL_CONFIG, 0x18);
	}

	uint8_t MPU6050_GetID(void)
	{
		return MPU6050_ReadReg(MPU6050_WHO_AM_I,&dpack[0],1);
	}

	// void MPU6050_GetData(int16_t *AccX, int16_t *AccY, int16_t *AccZ, 
	// 					int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ)
	// {
	// 	uint8_t DataH, DataL;
		
	// 	DataH = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_H);
	// 	DataL = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_L);
	// 	*AccX = (DataH << 8) | DataL;
		
	// 	DataH = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_H);
	// 	DataL = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_L);
	// 	*AccY = (DataH << 8) | DataL;
		
	// 	DataH = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_H);
	// 	DataL = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_L);
	// 	*AccZ = (DataH << 8) | DataL;
		
	// 	DataH = MPU6050_ReadReg(MPU6050_GYRO_XOUT_H);
	// 	DataL = MPU6050_ReadReg(MPU6050_GYRO_XOUT_L);
	// 	*GyroX = (DataH << 8) | DataL;
		
	// 	DataH = MPU6050_ReadReg(MPU6050_GYRO_YOUT_H);
	// 	DataL = MPU6050_ReadReg(MPU6050_GYRO_YOUT_L);
	// 	*GyroY = (DataH << 8) | DataL;
		
	// 	DataH = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_H);
	// 	DataL = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_L);
	// 	*GyroZ = (DataH << 8) | DataL;
	// }
	#endif
