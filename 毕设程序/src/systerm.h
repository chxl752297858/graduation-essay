#ifndef __SYSTERM_H__
#define __SYSTERM_H__


#define uchar unsigned char
#define uint  unsigned int


#define IDLL_MODE		(PCON = PCON|0x03)

//����1�ж����ȼ�(0-3)
#define		UART1_PRIO		2
// ����2�ж����ȼ�(0-3)
#define		UART2_PRIO		3

//����UART1���Թ���
//#define UART1_DEBUG

//����UART1��ӡ�����ʪ�����ݹ���
#define UART1_PRINT

#include "stc12c5a.h"
#include "gprs.h"
#include "timer.h"
#include"LCD12864.h"
#include "dht11.h"

#include <string.h>
#include <stdio.h>




#endif //__SYSTERM_H__