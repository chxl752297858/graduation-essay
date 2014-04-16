/***********************************************************
酷学之家 http://simcom.taobao.com/
联系方式：15296811438 林工
*************************************************************/

#include "systerm.h"



volatile unsigned char data timer0_cnt = 0;

volatile int data sec_cnt = 0;
volatile int data ms_cnt = 0;

extern U8  U8T_data_H,U8T_data_L,U8RH_data_H,U8RH_data_L,U8checkdata;
extern unsigned char T_UP,T_DOWN,HR_UP,HR_DOWN;
extern char timer_flagss;
//sbit LED  = P3^0;
//sbit LED1  = P3^1;



 /***************串口打印温湿度值程序程序************************/
/*函数原型:UART_PutStringAndNumH(char *buf,unsigned int num,char *buf1,unsigned int num1)
/*函数功能:接收指定手机号码短信命令
/*输入参数:char *buf,unsigned int num,char *buf1,unsigned int num1
		  
/*输出参数:无
/********************************************************************/

/******************************************************************************
 * UART_message: 
 * 函数原型: - UART_message(unsigned char tup,unsigned char tdown,unsigned char hup,unsigned char hdown,unsigned int num,unsigned int num1) 
 * 函数功能: 
 * 输入参数:   unsigned char tup,unsigned char tdown,unsigned char hup,unsigned char hdown,unsigned int num,unsigned int num1
 * 输出参数: 
 * 返回值: 
 * 
 * modification history
 * --------------------
 *    2014/04/12, ChenxianLong create this function
 * 
 ******************************************************************************/
void UART_message(unsigned char tup,unsigned char tdown,unsigned char hup,unsigned char hdown,unsigned int num,unsigned int num1)
  {
	unsigned char a[1],b[1],ta[1],tb[1],ha[1],hb[1];
	  
	  Uart1SendStr("t_up:");
      ta[0] = '0'+tup/10;
	  ta[1] = '0'+tup%10;
      Uart1SendHex(ta[0]);
	  Uart1SendHex(ta[1]);
	  Uart1SendHex('.');
	  
	  Uart1SendStr("t_down:");
      tb[0] = '0'+tdown/10;
	  tb[1] = '0'+tdown%10;
      Uart1SendHex(tb[0]);
	  Uart1SendHex(tb[1]);
	  Uart1SendHex('.');

	  Uart1SendStr("h_up:");
      ha[0] = '0'+hup/10;
	  ha[1] = '0'+hup%10;
      Uart1SendHex(ha[0]);
	  Uart1SendHex(ha[1]);
	  Uart1SendHex('%');
	  Uart1SendHex('.');

	  Uart1SendStr("h_down:");
      hb[0] = '0'+hdown/10;
	  hb[1] = '0'+hdown%10;
      Uart1SendHex(hb[0]);
	  Uart1SendHex(hb[1]);
	   Uart1SendHex('%');
	  Uart1SendHex('.');
	   
	  Uart1SendStr("temper:");
      a[0] = '0'+num/10;
	  a[1] = '0'+num%10;

		Uart1SendHex(a[0]);
		Uart1SendHex(a[1]);
		Uart1SendHex('.');
		
		Uart1SendStr("himitity:");
		b[0] = '0'+num1/10;
		b[1] = '0'+num1%10;

	   Uart1SendHex(b[0]);
	   Uart1SendHex(b[1]);
	   Uart1SendHex('%');
	   Uart1SendHex('.');
	   Uart1SendStr("\r\n");
  }


/***************************初始化定时器0****************************/
/*函数原型:Timer0Init
/*函数功能:初始化定时器0
/*输入参数:无
/*输出参数:无
/********************************************************************/
void Timer0Init(void)
{

	TMOD|=0x01;                     //设置定时器0为工作方式1
	TH0=(65536-45872)/256;         //放入初值,11.0592M,50ms
	TL0=(65536-45872)%256;

	// 设置定时器0中断优先级(优先级1)
	IPH &= ~PT0H;
	PT0 = 1;

	EA=1;                           //开总中断
	ET0=1;                        	//开定时器0中断
    TR0=1;                          //启动定时器0
}


/*------------------------------------------------
                    定时器初始化子程序
------------------------------------------------*/
void Init_Timer1(void)
{
 TMOD |= 0x10;	  //使用模式1，16位定时器，使用"|"符号可以在使用多个定时器时不受影响 
 TH1=(65536-50000)%256;	      //给定初值，这里使用定时器最大值从0开始计数一直到65535溢出
 TL1=(65536-50000)/256;
 EA=1;            //总中断打开
 ET1=1;           //定时器中断打开
 TR1=1;           //定时器开关打开
}


/***************************定时器0中断******************************/
/*函数原型:Timer0
/*函数功能:每50ms执行一次中断，中断内部重新赋值
/*输入参数:无
/*输出参数:无
/********************************************************************/
void timer0() interrupt 1
{
	uchar i = 0;
	TH0=(65536-45872)/256;
	TL0=(65536-45872)%256;

#ifdef UART1_DEBUG//发送数据用于调试。
		Uart1SendStr(DebugBuf_U2);
		for(i=0; i<MAXBUF; i++)
		{
			DebugBuf_U2[i] = 0;	
		}
		Debug2RcvCnt = 0;

		Uart2SendStr(DebugBuf_U1);
		for(i=0; i<MAXBUF; i++)
		{
			DebugBuf_U1[i] = 0;	
		}
		Debug1RcvCnt = 0;

#endif

	timer0_cnt++;

	if(timer0_cnt == 40)
	{
		timer0_cnt = 0;
		sec_cnt++;
        RH();  //一秒钟更新一次温湿度数据
	if((sec_cnt==60)&&(timer_flagss==0))//一分钟内连续检测到异常就发送继续发送
	{
	
	 sec_cnt=0;
	 timer_flagss=1;
	
	}
	 #ifdef UART1_PRINT
		//将接收到的数据放入DebugBuf_U1，在定时器内，DebugBuf_U1会被串口2发送给GSM模块。
		//这样通过串口1发送到单片机的数据GSM模块就可以收到了，主要为了调试的方便。
     UART_message(T_UP,T_DOWN,HR_UP,HR_DOWN,U8T_data_H,U8RH_data_H);
	

	 #endif
	}
		
}


 /*------------------------------------------------
                 定时器中断子程序
------------------------------------------------*/
void Timer1_isr(void) interrupt 3 using 1
{
 unsigned char count;
 TH1=(65536-50000)%256;	      //给定初值，这里使用定时器最大值从0开始计数一直到65535溢出
 TL1=(65536-50000)/256;
 count++;
if(count==20)
 {
    count=0;


 }
}
/***************************定时函数******************************/
/*函数原型:void DelaySec(unsigned char sec)
/*函数功能:是函数等待一段时间
/*输入参数:sec，要等待的时间（秒）
/*输出参数:无
/********************************************************************/
void DelaySec(unsigned char sec)
{
	sec_cnt = 0;
	while(sec_cnt < sec);		
}


/******************************************************************************
 * DelayMs: 
 * 函数原型: - void DelayMs(unsigned char ms)
 * 函数功能: 
 * 输入参数: unsigned char ms
 * 输出参数: 无
 *  
 * 
 * modification history
 * --------------------
 *    2014/04/12, ChenxianLong create this function
 * 
 ******************************************************************************/
void DelayMs(unsigned char ms)
{
	ms_cnt = 0;
	while(ms_cnt < ms);		
}


