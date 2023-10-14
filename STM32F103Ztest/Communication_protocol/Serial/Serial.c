#include "Serial.h"
#if Serial1
	#if Serial1_hex
	uint8_t Serial1_TxPacket[5];				//FF 01 02 03 04 FE
	uint8_t Serial1_RxPacket[5];
	uint8_t Serial1_RxFlag;
	#elif Serial1_string
	char Serial1_RxPacket[100];				//"@MSG\r\n"
	uint8_t Serial1_RxFlag;
	#endif
	
	void Serial1_Init(void)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
		
		GPIO_InitTypeDef GPIO_InitStructure;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		
		USART_InitTypeDef USART_InitStructure;
		USART_InitStructure.USART_BaudRate = 115200;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
		USART_InitStructure.USART_Parity = USART_Parity_No;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_Init(USART1, &USART_InitStructure);
		
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
		
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
		
		NVIC_InitTypeDef NVIC_InitStructure;
		NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
		NVIC_Init(&NVIC_InitStructure);
		
		USART_Cmd(USART1, ENABLE);
	}

	void Serial1_SendByte(uint8_t Byte)
	{
		USART_SendData(USART1, Byte);
		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	}

	void Serial1_SendArray(uint8_t *Array, uint16_t Length)
	{
		uint16_t i;
		for (i = 0; i < Length; i ++)
		{
			Serial1_SendByte(Array[i]);
		}
	}

	void Serial1_SendString(char *String)
	{
		uint8_t i;
		for (i = 0; String[i] != '\0'; i ++)
		{
			Serial1_SendByte(String[i]);
		}
	}

	uint32_t Serial1_Pow(uint32_t X, uint32_t Y)
	{
		uint32_t Result = 1;
		while (Y --)
		{
			Result *= X;
		}
		return Result;
	}

	void Serial1_SendNumber(uint32_t Number, uint8_t Length)
	{
		uint8_t i;
		for (i = 0; i < Length; i ++)
		{
			Serial1_SendByte(Number / Serial1_Pow(10, Length - i - 1) % 10 + '0');
		}
	}

	int fputc(int ch, FILE *f)
	{
		Serial1_SendByte(ch);
		return ch;
	}

	void Serial1_Printf(char *format, ...)
	{
		char String[100];
		va_list arg;
		va_start(arg, format);
		vsprintf(String, format, arg);
		va_end(arg);
		Serial1_SendString(String);
	}


	void Serial1_SendPacket(void)
	{
		Serial1_SendByte(0xFF);
		Serial1_SendArray(Serial1_TxPacket, 5);
		Serial1_SendByte(0xFE);
	}

	uint8_t Serial1_GetRxFlag(void)
	{
		if (Serial1_RxFlag == 1)
		{
			Serial1_RxFlag = 0;
			return 1;
		}
		return 0;
	}
	#if Serial1_hex
	void USART1_IRQHandler(void)
	{
		static uint8_t RxState = 0;
		static uint8_t pRxPacket = 0;
		if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
		{
			uint8_t RxData = USART_ReceiveData(USART1);
			
			if (RxState == 0)
			{
				if (RxData == 0xFF)
				{
					RxState = 1;
					pRxPacket = 0;
				}
			}
			else if (RxState == 1)
			{
				Serial1_RxPacket[pRxPacket] = RxData;
				pRxPacket ++;
				if (pRxPacket > 4)
				{
					RxState = 2;
				}
			}
			else if (RxState == 2)
			{
				if (RxData == 0xFE)
				{
					RxState = 0;
					Serial1_RxFlag = 1;
				}
			}
			
			USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		}
	}
	#elif Serial1_string
	void USART1_IRQHandler(void)
	{
		static uint8_t RxState = 0;
		static uint8_t pRxPacket = 0;
		if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
		{
			uint8_t RxData = USART_ReceiveData(USART1);
			
			if (RxState == 0)
			{
				if (RxData == '@' && Serial1_RxFlag == 0)
				{
					RxState = 1;
					pRxPacket = 0;
				}
			}
			else if (RxState == 1)
			{
				if (RxData == '\r')
				{
					RxState = 2;
				}
				else
				{
					Serial1_RxPacket[pRxPacket] = RxData;
					pRxPacket ++;
				}
			}
			else if (RxState == 2)
			{
				if (RxData == '\n')
				{
					RxState = 0;
					Serial1_RxPacket[pRxPacket] = '\0';
					Serial1_RxFlag = 1;
				}
			}
			
			USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		}
	}
	#endif
#endif






