#include "stm32f10x.h"
#include "I2C.h"
#include "Delay.h"



void I2CInit(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Pin = SDA_PIN|SCL_PIN;//SCL PB10   SDA PB11
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(I2C_PROT, &GPIO_InitStructure);

	GPIO_SetBits(I2C_PROT,SDA_PIN);
	GPIO_SetBits(I2C_PROT,SCL_PIN);

}

void MYI2C_W_SCL(uint8_t BitValue)
{
	GPIO_WriteBit(I2C_PROT,SCL_PIN,(BitAction)BitValue);
}

void MYI2C_W_SDA(uint8_t BitValue)//写入数据
{
	GPIO_WriteBit(I2C_PROT,SDA_PIN,(BitAction)BitValue);
}
uint8_t MYI2C_R_SDA(void)//读取数据
{
	uint8_t Bitvalue;
	Bitvalue = GPIO_ReadInputDataBit(I2C_PROT,SDA_PIN);
	return Bitvalue;
}



void I2C_START(void)
{
	MYI2C_W_SDA(1);//释放SDA和SCL 注意，这里顺序最好不要换！
	MYI2C_W_SCL(1);
	

	MYI2C_W_SDA(0);//先拉低SDA
	MYI2C_W_SCL(0);//再拉底SCL

}

void I2C_stop(void)
{
	MYI2C_W_SDA(0);//SDA先下拉 顺序不要换！
	MYI2C_W_SCL(1);
	MYI2C_W_SDA(1);
	//终止
}


//除了终止条件，所有的单元都会以SCL低电平结束，方便单元拼接
void I2C_SendByte(uint8_t Byte)//SCL低电平变换数据，高电平保持数据
{
	uint8_t count1 = 0;
	for(count1 = 0;count1<8;count1++)
	{
		MYI2C_W_SDA(Byte & (0x80 >> count1));//因为高位先行，所以先将最高位将1000 0000相与		
		//下一次传输就与次高位相与
		MYI2C_W_SCL(1);//释放SDA，立即传送。

		MYI2C_W_SCL(0);//再拉底SCL
	}
}


uint8_t I2C_ReadByte(void)//接收数据，为了不干扰从机发数据，主机需要释放SDA
{
	uint8_t Byte = 0x00;
	uint8_t count = 0;
	MYI2C_W_SDA(1);
	for(count = 0;count<8;count++)
	{
		MYI2C_W_SCL(1);
		if(MYI2C_R_SDA() == 1)//如果等于0，则默认输入0.
		{
			Byte |= (0x80 >> count);
		}
		MYI2C_W_SCL(0);//拉低SCL，让从机更改数据
	}
	return Byte;
}

void I2C_Send_Ack(uint8_t AckBit)//SCL低电平变换数据，高电平保持数据
{
	MYI2C_W_SDA(AckBit);//主机将AckBit放到SDA上
	MYI2C_W_SCL(1);//释放SDA，立即传送。
	MYI2C_W_SCL(0);//再拉底SCL，进入下一个时序单元

}


uint8_t I2C_Read_Ack(void)//接收应答，为了不干扰从机发数据，主机需要释放SDA
{
	uint8_t AckBit;
	MYI2C_W_SDA(1);
	MYI2C_W_SCL(1);
	AckBit = MYI2C_R_SDA();
	MYI2C_W_SCL(0);//拉低SCL，进入下一个时序单元
	return AckBit;

}
