#include "gyroscope.h"
#if gyroscope
uint8_t gyro_writeDatapacket[12]={0};
uint8_t gyro_readDatapacket[12]={0};
int16_t Angle[3]={0};
void gyro_Unlock(void)
{
    // uint8_t KEY_data[2]={0x88,0xb5};
    Software_IIC_start();
    Software_IIC_write_byte(gyro_address);
    if(Software_IIC_receive_ack()==1) Software_IIC_stop();
    // Software_IIC_write_byte(0xFF);
    // if(Software_IIC_receive_ack()==1) Software_IIC_stop();
    // Software_IIC_write_byte(0XAA);
    // if(Software_IIC_receive_ack()==1) Software_IIC_stop();
    Software_IIC_write_byte(KEY);
    if(Software_IIC_receive_ack()==1) Software_IIC_stop();
    Software_IIC_write_byte(0X88);
    if(Software_IIC_receive_ack()==1) Software_IIC_stop();
    Software_IIC_write_byte(0XB5);
    if(Software_IIC_receive_ack()==1) Software_IIC_stop();
    Software_IIC_stop();
}
void gyro_Save(uint8_t dataL,uint8_t dataH)
{
    gyro_Unlock();
    Software_IIC_start();
    Software_IIC_write_byte(gyro_address);
    if(Software_IIC_receive_ack()==1) Software_IIC_stop();
    // Software_IIC_write_byte(0xFF);
    // if(Software_IIC_receive_ack()==1) Software_IIC_stop();
    // Software_IIC_write_byte(0XAA);
    // if(Software_IIC_receive_ack()==1) Software_IIC_stop();
    Software_IIC_write_byte(SAVE);
    if(Software_IIC_receive_ack()==1) Software_IIC_stop();
    Software_IIC_write_byte(dataL);
    if(Software_IIC_receive_ack()==1) Software_IIC_stop();
    Software_IIC_write_byte(dataH);
    if(Software_IIC_receive_ack()==1) Software_IIC_stop();
    Software_IIC_stop();
}
uint8_t gyro_writeReg(uint8_t RegAddress, uint8_t DataL,uint8_t DataH)
{
    Software_IIC_start();
    Software_IIC_write_byte(gyro_address);
    if(Software_IIC_receive_ack()==1) Software_IIC_stop();
    // Software_IIC_write_byte(0xFF);
    // if(Software_IIC_receive_ack()==1) Software_IIC_stop();
    // Software_IIC_write_byte(0XAA);
    // if(Software_IIC_receive_ack()==1) Software_IIC_stop();
    Software_IIC_write_byte(RegAddress);
    if(Software_IIC_receive_ack()==1) Software_IIC_stop();
    Software_IIC_write_byte(DataL);
    if(Software_IIC_receive_ack()==1) Software_IIC_stop();
    Software_IIC_write_byte(DataH);
    if(Software_IIC_receive_ack()==1) Software_IIC_stop();
    Software_IIC_stop();
    return 1;
}
uint8_t gyro_readReg(uint8_t RegAddress,uint8_t *datapack,uint8_t length)
{

    uint8_t i=0;
    //uint8_t data=0;
    // gyro_Unlock();
    Software_IIC_start();
    Software_IIC_write_byte(gyro_address);
    if(Software_IIC_receive_ack()==1) Software_IIC_stop();
    Software_IIC_write_byte(RegAddress);
    if(Software_IIC_receive_ack()==1) Software_IIC_stop();
    // Software_IIC_write_byte(DataL);
    // if(Software_IIC_receive_ack()==1) Software_IIC_stop();
    // Software_IIC_write_byte(DataH);
    // if(Software_IIC_receive_ack()==1) Software_IIC_stop();
    
    Software_IIC_start();
    Software_IIC_write_byte(gyro_address+1);
    // if(Software_IIC_receive_ack()==1) Software_IIC_stop();
    for(i=0;i<length;i++)
    {
        datapack[i]=Software_IIC_read_byte();
        Software_IIC_send_ack(0);
    }
    Software_IIC_send_ack(1);
    Software_IIC_stop();
    return datapack[0];
}

void gyroscope_Init(void)
{
    gyro_Unlock();
    gyro_Save(0x01,0x00);
    Delay_ms(100);
    gyro_Unlock();
    gyro_writeReg(CALSW,0x01,0x00);//校准模式，正常工作
    gyro_writeReg(RSW,0x3e,0x00);//输出内容,角度，加速度，角加速度
    gyro_writeReg(RRATE,0x0b,0x00);//输出内容角度，加速度，角加速度
    // gyro_Save(0x00,0x00);
}

void gyro_getAngle(int16_t *Anglepack)
{
    // uint8_t datal=0,datah=0;
    gyro_readReg(0x3d,&gyro_readDatapacket[0],12);
    // Anglepack[0]=(short)((short)gyro_readDatapacket[2]<<8|gyro_readDatapacket[3]);
}
#endif

