
#include "systerm.h"

uchar GsmRcv[MAXRCV] = {0};
uchar DebugBuf_U2[MAXBUF] = {0};
uchar DebugBuf_U1[MAXBUF] = {0};
uchar GsmRcvCnt = 0;
uchar GsmAtFlag = 0;
uchar Debug2RcvCnt = 0;
uchar Debug1RcvCnt = 0;

/***************************串口2初始化函数函数*****************************/
/*函数原型:char Uart2Init(char smod,char brtx12,unsigned char reload)
/*函数功能:初始化用于连接gprs的串口2 ,串口2只能设置为独立波特率提供波特率
			并打开串口2中断和全局中断
/*输入参数: smod	s2smod位			0/1
			brtx12	brtx12位  			0/1
			reload	reload寄存器数值  	0-255
/*输出参数:	-1		s2smod位错误，没有写入0/1
			-2		brtx12位错误，没有写入0/1
			0
/********************************************************************/
char Uart2Init(char s2smod,char brtx12,unsigned char reload)
{
	S2CON = 0X50;//8位可变波特率，无奇偶位
	BRT	= reload;//设置独立波特率发生器波特率

	if(s2smod == 1)
	{
		AUXR |= S2SMOD;   //S2SMOD = 1;//波特率倍速位
	}
	else if(s2smod == 0)
	{
		AUXR &= (~S2SMOD);   //S2SMOD = 0;//取消波特率倍速位
	}
	else
	{
		return -1;
	}
		
	if(brtx12 == 1)
	{
		AUXR |= BRTx12;//BRTx12 = 1;1T模式	
	}
	else if(brtx12 == 0)
	{
		AUXR &= (~BRTx12);//BRTx12 = 0;12T模式		
	}
	else
	{
		return -2;
	}		
	AUXR |= BRTR;//开启波特率发生器

	//设置串口2中断优先级	
	IP2H |= PS2H;		//设置串口2优先级为3(最高)
	IP2 |= PS2;

	if (UART2_PRIO & 0x02)
	{
		IP2H |= PS2H;
	}
	else
	{
		IP2H &= ~PS2H;
	}
 	if (UART2_PRIO & 0x01)
	{
		IP2 |= PS2;
	}
	else
	{
		IP2 &= ~PS2;
	}	
	
																					
	IE2 |= ES2;//	ES2 = 1;	   //允许串口2中断
	EA = 1;	   //开总中断
	return 0;	
}


/***************************串口2发送函数函数************************/
/*函数原型:void Serial_Init(void)
/*函数功能:串口2发送函数，发送单一数据
/*输入参数:数据
/*输出参数:无
/********************************************************************/
void Uart2SendHex(uchar i)
{
	S2BUF = i;//装入数据
	while((S2CON & S2TI) == 0);
	S2CON &= (~S2TI);//S2TI = 0;		
}

/***************************串口2发送字符串函数**********************/
/*函数原型:void Uart2SendStr(char* data_at)
/*函数功能:串口2发送函数，发送字符串
/*输入参数:字符串指针
/*输出参数:无
/********************************************************************/
void Uart2SendStr(char* data_at)
{

	unsigned char cnt=0;
	ET0=0;//关闭定时器0			
	while(*(data_at+cnt))	//判断一串数据是否结束
	{
		S2BUF = *(data_at+cnt);//装入数据
		while((S2CON & S2TI) == 0);
		S2CON &= (~S2TI);//S2TI = 0;
		cnt++;

	}
	ET0=1;//打开定时器0		
}

/***************************串口1初始化函数**********************/
/*函数原型:char Uart1Init(char smod,char brtx12,unsigned char reload)
/*函数功能:串口1初始化
/*输入参数:	smod	smod位	0/1
			brtx12	brtx12位  0/1
			reload	reload寄存器数值  0-255
   
/*输出参数:	-1		smod位错误，没有写入0/1
			-2		brtx12位错误，没有写入0/1
			0
/*备注：串口设置位独立波特率提供波特率，和串口2使用同一个波特率，也可以使用定时器1来提供波特率
/********************************************************************/
char Uart1Init(char smod,char brtx12,unsigned char reload)
{
	SCON =  0X50;//8位可变波特率，无奇偶位(SM0=0,SM1=1),使能串口接收模块(REN=1)
	BRT	= reload;//设置独立波特率发生器波特率

	if(smod == 1)
	{
		PCON |= SMOD;   //SMOD = 1;//波特率倍速位
	}
	else if(smod == 0)
	{
		PCON &= (~SMOD);   //SMOD = 0;//取消波特率倍速位
	}
	else
	{
		return -1;
	}
		
	if(brtx12 == 1)
	{
		AUXR |= BRTx12;//BRTx12 = 1;1T模式	
	}
	else if(brtx12 == 0)
	{
		AUXR &= (~BRTx12);//BRTx12 = 0;12T模式		
	}
	else
	{
		return -2;
	}
	
  	AUXR |= S1BRS;//串口1设置为使用独立波特率发生器										
	AUXR |= BRTR;//开启波特率发生器
	
	//设置串口1中断优先级
	if (UART1_PRIO & 0x02)
	{
		IPH |= PSH;
	}
	else
	{
		IPH &= ~PSH;
	}
 	if (UART1_PRIO & 0x01)
	{
		PS = 1;			
	}
	else
	{
		PS = 0;
	}									
											
	ES = 1;	   //开串口中断
	EA = 1;	   //开总中断
	return 0;	
}

/***************************串口1发送函数**********************/
/*函数原型:void Uart1SendHex(char i)
/*函数功能:串口1发送函数
/*函数功能:串口1发送函数，发送单一数据
/*输入参数:数据
/*输出参数:无
/**************************************************************/
void Uart1SendHex(char i)
{
	TI = 0; //清空发送完中断请求标志位
	SBUF = i;  //将数据放入寄存器发送
	while(TI == 0);//等待发送完毕，发送完毕 TI == 1
	TI = 0;	//清空发送完中断请求标志位
}

/***************************串口1发送字符串函数**********************/
/*函数原型:void Uart1SendStr(char* data_at)
/*函数功能:串口1发送函数，发送字符串
/*输入参数:字符串指针
/*输出参数:无
/********************************************************************/
void Uart1SendStr(char* at)
{
	unsigned char cnt=0;
	ET0=0;//关闭定时器0
	while(*(at+cnt))	//判断一串数据是否结束
	{
		SBUF=*(at+cnt);	//发送数据
		while(TI==0);	//查询发送是否结束
		TI=0;	//清除发送一标志位
		cnt++;	//准备发送一个数据
	}
	ET0=1;//打开定时器0		
}

/***************************串口1发送接收中断函数********************/
/*函数原型:void Uart1InterruptReceive(void) interrupt 4
/*函数功能:串口2发送接收中断函数
/*输入参数:无
/*输出参数:无
/*备注：接收函数内部不可以放入其他的中断或者发送函数，否则影响接收数据
/********************************************************************/
void Uart1InterruptReceive(void) interrupt 4
{
	char tmp = 0;
	if(RI)
	{

		ES=0;//关串行口中断
		RI=0;//接收中断信号清零，表示将继续接收
		tmp = SBUF;//接收数据

#ifdef UART1_DEBUG
		//将接收到的数据放入DebugBuf_U1，在定时器内，DebugBuf_U1会被串口2发送给GSM模块。
		//这样通过串口1发送到单片机的数据GSM模块就可以收到了，主要为了调试的方便。
		DebugBuf_U1[Debug1RcvCnt] = tmp;
		Debug1RcvCnt++;
		if(Debug1RcvCnt>=MAXBUF-1)
		{
			Debug1RcvCnt = 0;	
		}
#endif


	}	
}

/***************************串口2发送接收中断函数**********************/
/*函数原型:void Uart2InterruptReceive(void) interrupt 8
/*函数功能:串口2发送接收中断函数
/*输入参数:无
/*输出参数:无
/*备注：接收函数内部不可以放入其他的中断或者发送函数，否则影响接收数据
		用于接收GPRS模块传输来的数据，将数据保存到GsmRcv[],在函数中使用strstr判断数据。
/********************************************************************/
void Uart2InterruptReceive(void) interrupt 8
{
	unsigned char tmp = 0;
	uchar i = 0;

	IE2 &= (~ES2);//关闭串口2中断//ES2 = 0;
	if(S2CON & S2RI)	//if(S2RI == 1)
	{

		S2CON &= (~S2RI);//S2RI = 0;
		tmp = S2BUF;

#ifdef UART1_DEBUG
		//将接收到的数据放入DebugBuf_U2，在定时器内，DebugBuf_U2会被串口1发送给PC机。
		//这样GSM发送给单片机的数据也可以在电脑上观察到了。
		DebugBuf_U2[Debug2RcvCnt] = tmp;
		Debug2RcvCnt++;
		if(Debug2RcvCnt>=MAXBUF-1)
		{
			Debug2RcvCnt = 0;	
		}
#endif
				
		GsmRcv[GsmRcvCnt] = tmp;
		GsmRcvCnt++;

		if(GsmRcvCnt > MAXRCV)
		{
			GsmRcvCnt = 0;
		}
		
	}

	IE2 |= ES2;//ES2 = 1;
}


/***************************GPRS接收缓冲数据初始化函数***************/
/*函数原型:void CleanGsmRcv(void)
/*函数功能:将GsmRcv[]数据清除，并且使缓冲指针移到第一位。
/*输入参数:无
/*输出参数:无
/********************************************************************/
void CleanGsmRcv(void)
{
	uchar i = 0;
	for(i=0; i<=MAXRCV; i++)
	{
		GsmRcv[i] = 0;	
	}

	GsmRcvCnt = 0;	
}
/***************************GPRS缓冲数据握手函数***************/
/*函数原型:bit Hand(unsigned char *a)
/*函数功能:判断收到的数据是否出现判断的数据。
/*输入参数:判断出现的数据
/*输出参数:	1:出现了判断的数据
			0:没有出现判断的数据
/********************************************************************/
bit Hand(unsigned char *a)
{ 
    if(strstr(GsmRcv,a)!=NULL)
	    return 1;
	else
		return 0;
}


