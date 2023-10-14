#ifndef __DRIVER_H
#define __DRIVER_H
#include "stm32f10x.h"                  // Device headerSerial1_RxPacket
#include <stdio.h>
#include <stdarg.h>
#endif

#define OLED 0
#define IIC 1
//串口开关
#define Serial1 1//打开串口1
#define Serial2 0

#if Serial1
#define Serial1_hex 1 //数据包发送
#define Serial1_string 0 //文本发送
#endif
#if Serial2
#define Serial_hex 1 //数据包发送
#define Serial_string 0 //文本发送
#endif
