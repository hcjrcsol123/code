#ifndef _GYROSCOPE_h
#define _GYROSCOPE_h
#include "driver.h"
#include "IIC.h"
#define gyroscope 1
#if gyroscope
#define gyro_address 0xa0//0x50<<1
#define SAVE 		0x00
#define CALSW 		0x01
#define RSW 		0x02
#define RRATE		0x03
#define KEY         0x69



extern uint8_t gyro_writeDatapacket[12];
extern uint8_t gyro_readDatapacket[12];
void gyro_Unlock(void);
void gyro_Save(uint8_t dataL,uint8_t dataH);
uint8_t gyro_writeReg(uint8_t RegAddress, uint8_t DataL,uint8_t DataH);
uint8_t gyro_readReg(uint8_t RegAddress,uint8_t *datapack,uint8_t length);
void gyroscope_Init(void);
void gyro_getAngle(int16_t *Anglepack);

#endif
#endif
