/***********************************************************
��ѧ֮�� http://simcom.taobao.com/
��ϵ��ʽ��15296811438 �ֹ�
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



 /***************���ڴ�ӡ��ʪ��ֵ�������************************/
/*����ԭ��:UART_PutStringAndNumH(char *buf,unsigned int num,char *buf1,unsigned int num1)
/*��������:����ָ���ֻ������������
/*�������:char *buf,unsigned int num,char *buf1,unsigned int num1
		  
/*�������:��
/********************************************************************/

/******************************************************************************
 * UART_message: 
 * ����ԭ��: - UART_message(unsigned char tup,unsigned char tdown,unsigned char hup,unsigned char hdown,unsigned int num,unsigned int num1) 
 * ��������: 
 * �������:   unsigned char tup,unsigned char tdown,unsigned char hup,unsigned char hdown,unsigned int num,unsigned int num1
 * �������: 
 * ����ֵ: 
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


/***************************��ʼ����ʱ��0****************************/
/*����ԭ��:Timer0Init
/*��������:��ʼ����ʱ��0
/*�������:��
/*�������:��
/********************************************************************/
void Timer0Init(void)
{

	TMOD|=0x01;                     //���ö�ʱ��0Ϊ������ʽ1
	TH0=(65536-45872)/256;         //�����ֵ,11.0592M,50ms
	TL0=(65536-45872)%256;

	// ���ö�ʱ��0�ж����ȼ�(���ȼ�1)
	IPH &= ~PT0H;
	PT0 = 1;

	EA=1;                           //�����ж�
	ET0=1;                        	//����ʱ��0�ж�
    TR0=1;                          //������ʱ��0
}


/*------------------------------------------------
                    ��ʱ����ʼ���ӳ���
------------------------------------------------*/
void Init_Timer1(void)
{
 TMOD |= 0x10;	  //ʹ��ģʽ1��16λ��ʱ����ʹ��"|"���ſ�����ʹ�ö����ʱ��ʱ����Ӱ�� 
 TH1=(65536-50000)%256;	      //������ֵ������ʹ�ö�ʱ�����ֵ��0��ʼ����һֱ��65535���
 TL1=(65536-50000)/256;
 EA=1;            //���жϴ�
 ET1=1;           //��ʱ���жϴ�
 TR1=1;           //��ʱ�����ش�
}


/***************************��ʱ��0�ж�******************************/
/*����ԭ��:Timer0
/*��������:ÿ50msִ��һ���жϣ��ж��ڲ����¸�ֵ
/*�������:��
/*�������:��
/********************************************************************/
void timer0() interrupt 1
{
	uchar i = 0;
	TH0=(65536-45872)/256;
	TL0=(65536-45872)%256;

#ifdef UART1_DEBUG//�����������ڵ��ԡ�
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
        RH();  //һ���Ӹ���һ����ʪ������
	if((sec_cnt==60)&&(timer_flagss==0))//һ������������⵽�쳣�ͷ��ͼ�������
	{
	
	 sec_cnt=0;
	 timer_flagss=1;
	
	}
	 #ifdef UART1_PRINT
		//�����յ������ݷ���DebugBuf_U1���ڶ�ʱ���ڣ�DebugBuf_U1�ᱻ����2���͸�GSMģ�顣
		//����ͨ������1���͵���Ƭ��������GSMģ��Ϳ����յ��ˣ���ҪΪ�˵��Եķ��㡣
     UART_message(T_UP,T_DOWN,HR_UP,HR_DOWN,U8T_data_H,U8RH_data_H);
	

	 #endif
	}
		
}


 /*------------------------------------------------
                 ��ʱ���ж��ӳ���
------------------------------------------------*/
void Timer1_isr(void) interrupt 3 using 1
{
 unsigned char count;
 TH1=(65536-50000)%256;	      //������ֵ������ʹ�ö�ʱ�����ֵ��0��ʼ����һֱ��65535���
 TL1=(65536-50000)/256;
 count++;
if(count==20)
 {
    count=0;


 }
}
/***************************��ʱ����******************************/
/*����ԭ��:void DelaySec(unsigned char sec)
/*��������:�Ǻ����ȴ�һ��ʱ��
/*�������:sec��Ҫ�ȴ���ʱ�䣨�룩
/*�������:��
/********************************************************************/
void DelaySec(unsigned char sec)
{
	sec_cnt = 0;
	while(sec_cnt < sec);		
}


/******************************************************************************
 * DelayMs: 
 * ����ԭ��: - void DelayMs(unsigned char ms)
 * ��������: 
 * �������: unsigned char ms
 * �������: ��
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


