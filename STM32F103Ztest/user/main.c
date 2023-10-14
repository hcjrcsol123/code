#include "stm32f10x.h"                  // Device header
#include "driver.h"
#include "Serial.h"
#include "MPU6050.h"
#include "IIC.h"
#include "Delay.h"
#include "gyroscope.h"
short AccX=0,AccY=0,AccZ=0;
short GyroX=0,GyroY=0,GyroZ=0;
float angle[12]={0};
short temp[3]={0};

int main(void)
{
    uint8_t address = 0;
        uint8_t i=0;
    // MPU6050_Init();
    Software_IIC_Init();
    Delay_ms(60);
    Serial1_Init();
    Delay_ms(60);
    // address = MPU6050_GetID();
    // gyroscope_Init();
// gyro_writeReg(0x00,0x01,0x00);
    // gyro_Unlock();
    // gyro_writeReg(0x00,0x01,0x00);
    // Delay_ms(500);
    // gyro_Unlock();
    // gyro_writeReg(0x00,0xff,0x00);
    gyro_Unlock();
    gyro_writeReg(0x01,0x00,0x00);
    gyro_writeReg(0x00,0x00,0x00);

    gyro_Unlock();
    gyro_writeReg(0x02,0x1e,0x00);
    gyro_writeReg(0x00,0x00,0x00);
    gyro_Unlock();
    gyro_writeReg(0x03,0x0b,0x00);
    gyro_writeReg(0x00,0x00,0x00);
    gyro_Unlock();
    gyro_writeReg(0x24,0x01,0x00);
    gyro_writeReg(0x00,0x00,0x00);
    gyro_Unlock();
    gyro_writeReg(0x2a,0xf4,0x01);
    gyro_writeReg(0x00,0x00,0x00);
    gyro_Unlock();
    gyro_writeReg(0x25,0x1e,0x00);
    gyro_writeReg(0x00,0x00,0x00);
    gyro_Unlock();
    gyro_writeReg(0x1c,0xf4,0x01);
    gyro_writeReg(0x00,0x00,0x00);
    gyro_Unlock();
    gyro_writeReg(0x1d,0xf4,0x01);
    gyro_writeReg(0x00,0x00,0x00);
    gyro_Unlock();
    gyro_writeReg(0x1e,0xf4,0x01);
    gyro_writeReg(0x00,0x00,0x00);
    gyro_Unlock();
    gyro_writeReg(0x20,0x03,0x00);
    gyro_writeReg(0x00,0x00,0x00);
    gyro_Unlock();
    gyro_writeReg(0x21,0x00,0x00);
    gyro_writeReg(0x00,0x00,0x00);
    gyro_writeReg(0x00,0xff,0x00);
    Delay_ms(500);
    // gyro_Unlock();
    // gyro_writeReg(0x1a,0x50,0x00);
    // gyro_writeReg(0x00,0x00,0x00);
	// gyro_writeReg(0x00,0xff,0x00);
    // Delay_ms(200);

    
    


    while (1)
    {
        // for(i=0;i<255;i++)
        // {
        //     Software_IIC_start();
        //     Software_IIC_write_byte(i);
        //     if(Software_IIC_receive_ack()==1) Software_IIC_stop();
        //     else break;
        // }
        // // gyro_writeReg(0x00,0x00,0x00);
		// gyro_readReg(0x1a,&gyro_readDatapacket[0],1);
		// Serial1_Printf("%x\r\n",i);
        // Serial1_Printf("%x\r\n",gyro_readDatapacket[0]);
        Delay_ms(100);
        gyro_readReg(0x3d,&gyro_readDatapacket[0],2);
        gyro_readReg(0x3e,&gyro_readDatapacket[3],2);
        gyro_readReg(0x3f,&gyro_readDatapacket[5],2);
        Delay_ms(1);
        // gyro_getAngle(&angle[0]);
        temp[0]=((short)(uint16_t)gyro_readDatapacket[1]<<8|gyro_readDatapacket[0]);
        temp[1]=((short)(uint16_t)gyro_readDatapacket[3]<<8|gyro_readDatapacket[2]);
        temp[2]=((short)(uint16_t)gyro_readDatapacket[5]<<8|gyro_readDatapacket[4]);
        angle[0]=temp[0]/32768.0f*180.0f;
        angle[1]=temp[1]/32768.0f*180.0f;
        angle[2]=temp[2]/32768.0f*180.0f;
        // Serial1_Printf("%x %x %x %x %x %x\r\n",gyro_readDatapacket[0],gyro_readDatapacket[1],gyro_readDatapacket[2],
        // gyro_readDatapacket[3],gyro_readDatapacket[4],gyro_readDatapacket[5],gyro_readDatapacket[6]);
        Serial1_Printf("%.2f %.2f %.2f\r\n",angle[0],angle[1],angle[2]);
        // Serial1_Printf("%d %d %d\r\n",temp[0],temp[1],temp[2]);
        // // Serial1_Printf("%x\r\n", gyro_readReg(27,&gyro_readDatapacket[0],2));
        // Delay_ms(300);
        // while (1)
        // {
        //     /* code */
        // }
        
    }
}
