#ifndef __SERIAL_H
#define __SERIAL_H
#include "driver.h"
    #if Serial1
        extern uint8_t Serial_TxPacket[];
        extern uint8_t Serial_RxPacket[];

        void Serial1_Init(void);
        void Serial1_SendByte(uint8_t Byte);
        void Serial1_SendArray(uint8_t *Array, uint16_t Length);
        void Serial1_SendString(char *String);
        void Serial1_SendNumber(uint32_t Number, uint8_t Length);
        void Serial1_Printf(char *format, ...);

        void Serial1_SendPacket(void);
        uint8_t Serial1_GetRxFlag(void);
    #endif

#endif
