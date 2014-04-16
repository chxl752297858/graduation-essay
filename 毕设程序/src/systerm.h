#ifndef __SYSTERM_H__
#define __SYSTERM_H__


#define uchar unsigned char
#define uint  unsigned int


#define IDLL_MODE		(PCON = PCON|0x03)

//串口1中断优先级(0-3)
#define		UART1_PRIO		2
// 串口2中断优先级(0-3)
#define		UART2_PRIO		3

//开启UART1调试功能
//#define UART1_DEBUG

//开启UART1打印输出温湿度数据功能
#define UART1_PRINT

#include "stc12c5a.h"
#include "gprs.h"
#include "timer.h"
#include"LCD12864.h"
#include "dht11.h"

#include <string.h>
#include <stdio.h>




#endif //__SYSTERM_H__