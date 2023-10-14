#include "IIC.h"
#if Software_IIC
    void Software_IIC_Init(void){
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Pin = Software_IIC_SCL|Software_IIC_SDA;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(Software_IIC_PROT, &GPIO_InitStructure);

	GPIO_SetBits(Software_IIC_PROT,Software_IIC_SCL);
	GPIO_SetBits(Software_IIC_PROT,Software_IIC_SDA);
    }

    void Software_IIC_W_SCL(uint8_t BitValue){
        GPIO_WriteBit(Software_IIC_PROT,Software_IIC_SCL,(BitAction)BitValue);
        Delay_us(10);
    }

    void Software_IIC_W_SDA(uint8_t BitValue){
        GPIO_WriteBit(Software_IIC_PROT,Software_IIC_SDA,(BitAction)BitValue);
        Delay_us(10);
    }

    uint8_t Software_IIC_R_SDA(void){
        uint8_t BitValue;
        BitValue = GPIO_ReadInputDataBit(Software_IIC_PROT,Software_IIC_SDA);
        Delay_us(10);
        return BitValue;
    }

    void Software_IIC_start(void){
        Software_IIC_W_SDA(1);
        Software_IIC_W_SCL(1);

        Software_IIC_W_SDA(0);
        Software_IIC_W_SCL(0);
    }

    void Software_IIC_stop(void){
        Software_IIC_W_SDA(0);
        Software_IIC_W_SCL(1);
        Software_IIC_W_SDA(1);
    }

    void Software_IIC_write_byte(uint8_t byte){
        char i;
        for(i=0;i<8;i++)
        {
            Software_IIC_W_SDA(byte&(0x80>>i));
            Software_IIC_W_SCL(1);
            Software_IIC_W_SCL(0);
        }
    }

    uint8_t Software_IIC_read_byte(void){
        uint8_t byte=0;
        uint8_t i;
        Software_IIC_W_SDA(1);
        for(i = 0;i<8;i++)
        {
            Software_IIC_W_SCL(1);
            byte |= Software_IIC_R_SDA();
            byte<<=1;
            Software_IIC_W_SCL(0);
        }
        return byte;
    }

    void Software_IIC_send_ack(uint8_t BitValue){//发送应答信号给从机，
        Software_IIC_W_SDA(BitValue);
        Software_IIC_W_SCL(1);
        Software_IIC_W_SCL(0);
    }

    uint8_t Software_IIC_receive_ack(void){//接收从机发送的应答信号
        uint8_t BitValue;
        uint16_t i = 0;
        Software_IIC_W_SDA(1);
        Software_IIC_W_SCL(1);
        while(Software_IIC_R_SDA()){
            i++;
            if(i>300){
                // Software_IIC_stop();
                Serial1_Printf("no\r\n");
                return 1;
            }
        }
        BitValue = Software_IIC_R_SDA();
        Software_IIC_W_SCL(0);
        return BitValue;
    }

    void Software_IIC_write_bytes(uint8_t* arr,uint8_t length){
        uint8_t i;
        Software_IIC_start();
        for(i = 0;i<length;i++){
            Software_IIC_write_byte(arr[i]);
            if(Software_IIC_receive_ack()==1){
                break;//应答失败，退出
                }
        }
        Software_IIC_stop();
    }

    void Software_IIC_read_bytes(uint8_t* arr,uint8_t length){
        uint8_t i;
        Software_IIC_start();
        

    }
#endif
