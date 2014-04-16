
#include "systerm.h"

uchar GsmRcv[MAXRCV] = {0};
uchar DebugBuf_U2[MAXBUF] = {0};
uchar DebugBuf_U1[MAXBUF] = {0};
uchar GsmRcvCnt = 0;
uchar GsmAtFlag = 0;
uchar Debug2RcvCnt = 0;
uchar Debug1RcvCnt = 0;

/***************************����2��ʼ����������*****************************/
/*����ԭ��:char Uart2Init(char smod,char brtx12,unsigned char reload)
/*��������:��ʼ����������gprs�Ĵ���2 ,����2ֻ������Ϊ�����������ṩ������
			���򿪴���2�жϺ�ȫ���ж�
/*�������: smod	s2smodλ			0/1
			brtx12	brtx12λ  			0/1
			reload	reload�Ĵ�����ֵ  	0-255
/*�������:	-1		s2smodλ����û��д��0/1
			-2		brtx12λ����û��д��0/1
			0
/********************************************************************/
char Uart2Init(char s2smod,char brtx12,unsigned char reload)
{
	S2CON = 0X50;//8λ�ɱ䲨���ʣ�����żλ
	BRT	= reload;//���ö��������ʷ�����������

	if(s2smod == 1)
	{
		AUXR |= S2SMOD;   //S2SMOD = 1;//�����ʱ���λ
	}
	else if(s2smod == 0)
	{
		AUXR &= (~S2SMOD);   //S2SMOD = 0;//ȡ�������ʱ���λ
	}
	else
	{
		return -1;
	}
		
	if(brtx12 == 1)
	{
		AUXR |= BRTx12;//BRTx12 = 1;1Tģʽ	
	}
	else if(brtx12 == 0)
	{
		AUXR &= (~BRTx12);//BRTx12 = 0;12Tģʽ		
	}
	else
	{
		return -2;
	}		
	AUXR |= BRTR;//���������ʷ�����

	//���ô���2�ж����ȼ�	
	IP2H |= PS2H;		//���ô���2���ȼ�Ϊ3(���)
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
	
																					
	IE2 |= ES2;//	ES2 = 1;	   //������2�ж�
	EA = 1;	   //�����ж�
	return 0;	
}


/***************************����2���ͺ�������************************/
/*����ԭ��:void Serial_Init(void)
/*��������:����2���ͺ��������͵�һ����
/*�������:����
/*�������:��
/********************************************************************/
void Uart2SendHex(uchar i)
{
	S2BUF = i;//װ������
	while((S2CON & S2TI) == 0);
	S2CON &= (~S2TI);//S2TI = 0;		
}

/***************************����2�����ַ�������**********************/
/*����ԭ��:void Uart2SendStr(char* data_at)
/*��������:����2���ͺ����������ַ���
/*�������:�ַ���ָ��
/*�������:��
/********************************************************************/
void Uart2SendStr(char* data_at)
{

	unsigned char cnt=0;
	ET0=0;//�رն�ʱ��0			
	while(*(data_at+cnt))	//�ж�һ�������Ƿ����
	{
		S2BUF = *(data_at+cnt);//װ������
		while((S2CON & S2TI) == 0);
		S2CON &= (~S2TI);//S2TI = 0;
		cnt++;

	}
	ET0=1;//�򿪶�ʱ��0		
}

/***************************����1��ʼ������**********************/
/*����ԭ��:char Uart1Init(char smod,char brtx12,unsigned char reload)
/*��������:����1��ʼ��
/*�������:	smod	smodλ	0/1
			brtx12	brtx12λ  0/1
			reload	reload�Ĵ�����ֵ  0-255
   
/*�������:	-1		smodλ����û��д��0/1
			-2		brtx12λ����û��д��0/1
			0
/*��ע����������λ�����������ṩ�����ʣ��ʹ���2ʹ��ͬһ�������ʣ�Ҳ����ʹ�ö�ʱ��1���ṩ������
/********************************************************************/
char Uart1Init(char smod,char brtx12,unsigned char reload)
{
	SCON =  0X50;//8λ�ɱ䲨���ʣ�����żλ(SM0=0,SM1=1),ʹ�ܴ��ڽ���ģ��(REN=1)
	BRT	= reload;//���ö��������ʷ�����������

	if(smod == 1)
	{
		PCON |= SMOD;   //SMOD = 1;//�����ʱ���λ
	}
	else if(smod == 0)
	{
		PCON &= (~SMOD);   //SMOD = 0;//ȡ�������ʱ���λ
	}
	else
	{
		return -1;
	}
		
	if(brtx12 == 1)
	{
		AUXR |= BRTx12;//BRTx12 = 1;1Tģʽ	
	}
	else if(brtx12 == 0)
	{
		AUXR &= (~BRTx12);//BRTx12 = 0;12Tģʽ		
	}
	else
	{
		return -2;
	}
	
  	AUXR |= S1BRS;//����1����Ϊʹ�ö��������ʷ�����										
	AUXR |= BRTR;//���������ʷ�����
	
	//���ô���1�ж����ȼ�
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
											
	ES = 1;	   //�������ж�
	EA = 1;	   //�����ж�
	return 0;	
}

/***************************����1���ͺ���**********************/
/*����ԭ��:void Uart1SendHex(char i)
/*��������:����1���ͺ���
/*��������:����1���ͺ��������͵�һ����
/*�������:����
/*�������:��
/**************************************************************/
void Uart1SendHex(char i)
{
	TI = 0; //��շ������ж������־λ
	SBUF = i;  //�����ݷ���Ĵ�������
	while(TI == 0);//�ȴ�������ϣ�������� TI == 1
	TI = 0;	//��շ������ж������־λ
}

/***************************����1�����ַ�������**********************/
/*����ԭ��:void Uart1SendStr(char* data_at)
/*��������:����1���ͺ����������ַ���
/*�������:�ַ���ָ��
/*�������:��
/********************************************************************/
void Uart1SendStr(char* at)
{
	unsigned char cnt=0;
	ET0=0;//�رն�ʱ��0
	while(*(at+cnt))	//�ж�һ�������Ƿ����
	{
		SBUF=*(at+cnt);	//��������
		while(TI==0);	//��ѯ�����Ƿ����
		TI=0;	//�������һ��־λ
		cnt++;	//׼������һ������
	}
	ET0=1;//�򿪶�ʱ��0		
}

/***************************����1���ͽ����жϺ���********************/
/*����ԭ��:void Uart1InterruptReceive(void) interrupt 4
/*��������:����2���ͽ����жϺ���
/*�������:��
/*�������:��
/*��ע�����պ����ڲ������Է����������жϻ��߷��ͺ���������Ӱ���������
/********************************************************************/
void Uart1InterruptReceive(void) interrupt 4
{
	char tmp = 0;
	if(RI)
	{

		ES=0;//�ش��п��ж�
		RI=0;//�����ж��ź����㣬��ʾ����������
		tmp = SBUF;//��������

#ifdef UART1_DEBUG
		//�����յ������ݷ���DebugBuf_U1���ڶ�ʱ���ڣ�DebugBuf_U1�ᱻ����2���͸�GSMģ�顣
		//����ͨ������1���͵���Ƭ��������GSMģ��Ϳ����յ��ˣ���ҪΪ�˵��Եķ��㡣
		DebugBuf_U1[Debug1RcvCnt] = tmp;
		Debug1RcvCnt++;
		if(Debug1RcvCnt>=MAXBUF-1)
		{
			Debug1RcvCnt = 0;	
		}
#endif


	}	
}

/***************************����2���ͽ����жϺ���**********************/
/*����ԭ��:void Uart2InterruptReceive(void) interrupt 8
/*��������:����2���ͽ����жϺ���
/*�������:��
/*�������:��
/*��ע�����պ����ڲ������Է����������жϻ��߷��ͺ���������Ӱ���������
		���ڽ���GPRSģ�鴫���������ݣ������ݱ��浽GsmRcv[],�ں�����ʹ��strstr�ж����ݡ�
/********************************************************************/
void Uart2InterruptReceive(void) interrupt 8
{
	unsigned char tmp = 0;
	uchar i = 0;

	IE2 &= (~ES2);//�رմ���2�ж�//ES2 = 0;
	if(S2CON & S2RI)	//if(S2RI == 1)
	{

		S2CON &= (~S2RI);//S2RI = 0;
		tmp = S2BUF;

#ifdef UART1_DEBUG
		//�����յ������ݷ���DebugBuf_U2���ڶ�ʱ���ڣ�DebugBuf_U2�ᱻ����1���͸�PC����
		//����GSM���͸���Ƭ��������Ҳ�����ڵ����Ϲ۲쵽�ˡ�
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


/***************************GPRS���ջ������ݳ�ʼ������***************/
/*����ԭ��:void CleanGsmRcv(void)
/*��������:��GsmRcv[]�������������ʹ����ָ���Ƶ���һλ��
/*�������:��
/*�������:��
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
/***************************GPRS�����������ֺ���***************/
/*����ԭ��:bit Hand(unsigned char *a)
/*��������:�ж��յ��������Ƿ�����жϵ����ݡ�
/*�������:�жϳ��ֵ�����
/*�������:	1:�������жϵ�����
			0:û�г����жϵ�����
/********************************************************************/
bit Hand(unsigned char *a)
{ 
    if(strstr(GsmRcv,a)!=NULL)
	    return 1;
	else
		return 0;
}


