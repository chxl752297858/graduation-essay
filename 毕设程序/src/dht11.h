/*--------------------------------------------------------------------------
DHT11.H

Header file for generic 80C52 and 80C32 microcontroller.
Copyright (c) 1988-2002 Keil Elektronik GmbH and Keil Software, Inc.
All rights reserved.
--------------------------------------------------------------------------*/

#ifndef __DHT11_H__
#define __DHT11_H__
#include"typedef.h"
sbit  DHT11  = P1^4 ;



//void Delay_N1ms(unsigned int t);
 void Delay(U16 j);
  void  Delay_10us(void);
   void  COM(void);
   	void RH(void);
#endif
