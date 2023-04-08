#include "stm32f10x.h"
#include "mpu6050.h"
int16_t ACCX,ACCY,ACCZ,GYROX,GYROY,GYROZ;
void I2C2_init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2,ENABLE);//打开APB1外设的I2C2时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//配置对应GPIO时钟
    GPIO_InitTypeDef GPIO_InitStructure;//配置对用GPIO引脚

 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;//复用开漏
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);

    I2C_InitTypeDef I2C_Initstructure;
    I2C_Initstructure.I2C_Mode = I2C_Mode_I2C;
    I2C_Initstructure.I2C_ClockSpeed =  100000;//100khz
    I2C_Initstructure.I2C_DutyCycle = I2C_DutyCycle_2;//只有在100kz~400khz的状态才有用，100KKHZ之下固定1：1,这里是2:1
    I2C_Initstructure.I2C_Ack = I2C_Ack_Enable;//默认给从机应答
    I2C_Initstructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;//这是stm32作为从机会响应几位地址，这里响应7位
    I2C_Initstructure.I2C_OwnAddress1 = I2C_AcknowledgedAddress_7bit;//自身地址，用于stm32作为从机的时候使用
    I2C_Init(I2C2,&I2C_Initstructure);
    
    I2C_Cmd(I2C2,ENABLE);


}
//注意，硬件I2C的函数不会等主机是否已经发送完数，只会操作对应CR，所以可能要加入一定延时或者判断

void I2C_WaitEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT)//将I2C_CheckEvent进行封装，保证其不会卡死在循环。
{
    uint32_t count = 10000;//方式程序进入循环卡死 
    while(I2C_CheckEvent(I2Cx,I2C_EVENT)!=SUCCESS)
    {
        count--;
        if(count == 0)
        {
            break;
        }
    }
}
void mpu6050_SendReg(uint8_t Regadd, uint8_t Data)
{
   
    I2C_GenerateSTART(I2C2,ENABLE);
    I2C_WaitEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT);//检查主机模式选择,stm32默认从机。发送起始条件后转为主机模式，

    I2C_Send7bitAddress(I2C2,mpu6050ADD,I2C_Direction_Transmitter);//发送从机地址，该函数自带接收应答第三个参数是配置最低位，
    //如果选发送就置0，接收就置1.如果应答失败就会置标志位或者申请中断。
    I2C_WaitEvent(I2C2,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);//检查发送模式已选择事件是否完成

    I2C_SendData(I2C2,Regadd);
    I2C_WaitEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTING);//检查字节正在发送事件是否完成

    I2C_SendData(I2C2,Data);
    I2C_WaitEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTED);//检查字节发送完成事件是否完成
//如果要连续写入，则需要进行多次以上两行代码

    I2C_GenerateSTOP(I2C2,ENABLE);//结束
}


uint8_t mpu6050_ReadReg(uint8_t Regadd)//读取一个字节
{
    uint8_t Data;
    I2C_GenerateSTART(I2C2,ENABLE);
    I2C_WaitEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT);//检查主机模式选择,stm32默认从机。发送起始条件后转为主机模式，

    I2C_Send7bitAddress(I2C2,mpu6050ADD,I2C_Direction_Transmitter);//发送从机地址，该函数自带接收应答第三个参数是配置最低位，
    //如果选发送就置0，接收就置1.如果应答失败就会置标志位或者申请中断。
    I2C_WaitEvent(I2C2,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);//检查发送模式已选择事件是否完成

    I2C_SendData(I2C2,Regadd);
    I2C_WaitEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTED);//检查字节发送完成事件是否完成

    I2C_GenerateSTART(I2C2,ENABLE);//重复起始条件
    I2C_WaitEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT);//检查主机模式选择,stm32默认从机。发送起始条件后转为主机模式，

    I2C_Send7bitAddress(I2C2,mpu6050ADD,I2C_Direction_Receiver);//发送从机地址，该函数自带接收应答，第三个参数是配置最低位，
    I2C_WaitEvent(I2C2,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED);//检查接收模式已选择事件是否完成
    I2C_AcknowledgeConfig(I2C2,DISABLE);
    I2C_GenerateSTOP(I2C2,ENABLE);//注意！！！如果只接收一个字节，那么就要在字节接收之前将应答位置0（非应答），并且将停止位
    //置1后，才等待EV7事件，EV7事件是在接收完一个字节后产生。
    //否则非应答信号还没发出下一个字节就已经接收。如果要连续读取多个字节，那么就只用检测EV7事件就可以，最后一个字节采用上述两行。
    //否则参考手册EV6_1事件。
    I2C_WaitEvent(I2C2,I2C_EVENT_MASTER_BYTE_RECEIVED);//产生EV7事件，就代表DR已经存入一个字节。
    Data = I2C_ReceiveData(I2C2);//此时可以读取寄存器数据。


    I2C_AcknowledgeConfig(I2C2,ENABLE);//记得释放SDA。为方便指定地址接收多个字节。
    return Data;
}

void mpu6050_Init(void)
{
    I2C2_init();
    mpu6050_SendReg(MPU6050_PWR_MGMT_1,0x01);//配置电源,接触睡眠，选择陀螺仪时钟
    mpu6050_SendReg(MPU6050_PWR_MGMT_2,0x00);//电源寄存器2，6轴不待机
    mpu6050_SendReg(MPU6050_SMPLRT_DIV,0x09);//分频，10分频
    mpu6050_SendReg(MPU6050_CONFIG,0x06);//配置寄存器，最大滤波参数
    mpu6050_SendReg(MPU6050_GYRO_CONFIG,0x18);//陀螺仪配置寄存器，最大量程
    mpu6050_SendReg(MPU6050_ACCEL_CONFIG,0x18);//acc配置寄存器，最大量程
}

void mpu6050_GetData(int16_t *accx,int16_t *accy,int16_t *accz,int16_t *gyrox,int16_t *gyroy,int16_t *gyroz)//用传址调用，实现多参数返回。
{
    uint16_t Data_h,Data_l;//数据是以补码的形式进行的有符号数，直接赋值给int16_t没问题。
    Data_h=mpu6050_ReadReg(MPU6050_ACCEL_XOUT_H);
    Data_l=mpu6050_ReadReg(MPU6050_ACCEL_XOUT_L);//分别为x轴acc数据寄存器的高八位，低八位。
    *accx = (Data_h<<8)|(Data_l);

    Data_h=mpu6050_ReadReg(MPU6050_ACCEL_YOUT_H);
    Data_l=mpu6050_ReadReg(MPU6050_ACCEL_YOUT_L);//分别为y轴acc数据寄存器的高八位，低八位。
    *accy = (Data_h<<8)|(Data_l);

    Data_h=mpu6050_ReadReg(MPU6050_ACCEL_ZOUT_H);
    Data_l=mpu6050_ReadReg(MPU6050_ACCEL_ZOUT_H);//分别为z轴acc数据寄存器的高八位，低八位。
    *accz = (Data_h<<8)|(Data_l);

    Data_h=mpu6050_ReadReg(MPU6050_GYRO_XOUT_H);
    Data_l=mpu6050_ReadReg(MPU6050_GYRO_XOUT_L);//分别为x轴gyro数据寄存器的高八位，低八位。
    *gyrox = (Data_h<<8)|(Data_l);

    Data_h=mpu6050_ReadReg(MPU6050_GYRO_YOUT_H);
    Data_l=mpu6050_ReadReg(MPU6050_GYRO_YOUT_L);//分别为y轴gyro数据寄存器的高八位，低八位。
    *gyroy = (Data_h<<8)|(Data_l);

    Data_h=mpu6050_ReadReg(MPU6050_GYRO_ZOUT_H);
    Data_l=mpu6050_ReadReg(MPU6050_GYRO_ZOUT_L);//分别为z轴gyro数据寄存器的高八位，低八位。
    *gyroz = (Data_h<<8)|(Data_l);
}




