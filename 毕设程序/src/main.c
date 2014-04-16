
#include "systerm.h"
#include <intrins.h>

sbit KEY1   = P2^0;
sbit KEY2   = P2^1;
sbit KEY3   = P2^2;
sbit KEY4   = P2^3;


sbit BELL   = P1^5;

extern U8  U8T_data_H,U8T_data_L,U8RH_data_H,U8RH_data_L,U8checkdata;
bit disp_flag=0;

bit hold_flag = 0;

char timer_flagss=0;
unsigned char statu_count;
unsigned char new_msm_flag = 0;
unsigned char RcvNum[] = "15118104927";
unsigned char ChgNum[sizeof(RcvNum)+1] = "";
//�ڴ˴��޸ķ��ͺ��������
uchar n[13] = "15118104927";//���ź���


//�������������Ͷ��ŵĺ���
/***********************����˵��******************************************************
UDP���Ÿ�ʽ��ʾʱ��GPRSģ�鷢�ͳ��������ź�����б�����򣬱���ʹ�ú���1252015296811438���Ͷ���
��ô�յ��ĺ���ᱻ����λһ����������F������任������ǰ����λ��12��ô���21������λ��52�����25.
��ɺ���"2125102569184183"���������ż�������ü�F�����������Ҫ�����ͼ���F������λһ�������磺
�����ǣ�15296811438��11λ������ôҪ�ȼ�F��15296811438F,�ٱ�ɣ�5192861134F8.
**************************************************************************************/


  
unsigned char T_UP,T_DOWN,HR_UP,HR_DOWN;


/******************************************************************************
 * Delay15ms: 
 * ����ԭ��: - 
 * ��������: 
 * �������: 
 * �������: 
 * ��д���ڣ�   2014/04/13, ChenxianLong create this function
 ******************************************************************************/
void Delay15ms()		//@11.0592MHz
{
	unsigned char i, j, k;

	i = 1;
	j = 162;
	k = 89;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}



/******************************************************************************
 * bell_sound: 
 * ����ԭ��: -bell_sound() 
 * ��������: ��������������
 * �������: 
 * �������: 
 * ��д���ڣ�   2014/04/13, ChenxianLong create this function
 ******************************************************************************/
void bell_sound()
{
;
 BELL=0;
 DelaynMs(5);
 BELL=1;
}

   /***************���ڴ�ӡ��ʪ��ֵ�������************************/
/*����ԭ��:void UART2_SentWarningMessage(unsigned char T_DOWN,unsigned char  T_UP,unsigned char H_DOWN,unsigned char  H_UP,unsigned char num,unsigned char num1)
/*��������:����ָ���ֻ������������
/*�������:T_DOWN,char  T_UP,char H_DOWN,char  H_UP,char num,char num1
		   �¶�����ֵ���¶����ޣ�ʪ������ֵ��ʪ������ֵ���¶�ֵ��ʪ��ֵ��
/*�������:��
/********************************************************************/

/******************************************************************************
 * UART2_SentWarningMessage: 
 * ����ԭ��: - 
 * ��������: 
 * �������: 
 * �������: 
 * ��д���ڣ�   2014/04/13, ChenxianLong create this function
 ******************************************************************************/
void UART2_SentWarningMessage(unsigned char T_DOWN,unsigned char  T_UP,unsigned char H_DOWN,unsigned char  H_UP,unsigned char num,unsigned char num1)
  {
	unsigned char a[1],b[1];
	unsigned char TT_UP[1],TT_DOWN[1],HH_UP[1],HH_DOWN[1];;
//	Uart2SendStr("Danger warning!The current temperature or humidity is abnormal.");
  	Uart2SendStr("Danger warning!Detection is abnormal.  ");

  	Uart2SendStr("Temperature range is ");
	TT_DOWN[0]='0'+T_DOWN/10;
	TT_DOWN[1]='0'+T_DOWN%10;
	Uart2SendHex(TT_DOWN[0]);
	Uart2SendHex(TT_DOWN[1]);
	Uart2SendHex('c');
    Uart2SendHex('~');
	TT_UP[0]='0'+T_UP/10;
	TT_UP[1]='0'+T_UP%10;
	Uart2SendHex(TT_UP[0]);
	Uart2SendHex(TT_UP[1]);
		Uart2SendHex('c');
	Uart2SendHex('.');

	Uart2SendStr("Humidity range is ");
	HH_DOWN[0]='0'+H_DOWN/10;
	HH_DOWN[1]='0'+H_DOWN%10;
	Uart2SendHex(HH_DOWN[0]);
	Uart2SendHex(HH_DOWN[1]);
	Uart2SendHex('%');
    Uart2SendHex('~');
	HH_UP[0]='0'+H_UP/10;
	HH_UP[1]='0'+H_UP%10;
	Uart2SendHex(HH_UP[0]);
	Uart2SendHex(HH_UP[1]);
	Uart2SendHex('%');
	Uart2SendHex('.');

	Uart2SendStr(" But current temperature value is : ");
	a[0] = '0'+num/10;
	a[1] = '0'+num%10;

		Uart2SendHex(a[0]);
		Uart2SendHex(a[1]);
		Uart2SendHex('c');
		Uart2SendHex('.');

   Uart2SendStr("humidity value is : ");

	b[0] = '0'+num1/10;
	b[1] = '0'+num1%10;

		Uart2SendHex(b[0]);
		Uart2SendHex(b[1]);
		Uart2SendHex('%');
		Uart2SendHex('.');
	    //Uart1SendStr("\r\n");
  }

//��gsmģ�鷢�ͳ��������ź���ת���CS2����

/******��gsmģ�鷢�ͳ��������ź���ת���CS2����************************/
/*����ԭ��:ChangeNum(uchar* num, uchar* des)
/*��������:��gsmģ�鷢�ͳ��������ź���ת���CS2����
/*�������:uchar* num,
           uchar* des
/*�������:��
/********************************************************************/
void ChangeNum(uchar* num, uchar* des)
{
	char n=0,i=0;

	while(num[n]!=0)
	{
		n++;
	}

	if(n&0x01 == 1)
	{
		num[n] = 'F';	
	}

	for(i=0; i<=n; i=i+2)
	{
		des[i] = num[i+1];
		des[i+1] = num[i];  	
	}	
}




/***************************��ʾ��ʪ��ֵ����************************/
/*����ԭ��:void disp_T_HR(unsigned char T_data,unsigned char RH_data)
/*��������:��ʾ��ʪ��ֵ
/*�������:unsigned char T_data  �¶�ֵ
           unsigned char RH_data ʪ��ֵ
		  
/*�������:��
/********************************************************************/

void Disp_T_HR(unsigned char T_data,unsigned char RH_data)
{


			 LCD_PutString(0,1,"�¶�");
			
			 Write_Data(0x30+T_data/10); 
			 Write_Data(0x30+T_data%10);
			 LCD_PutString(3,1,"��");
			 LCD_PutString(4,1,"ʪ��");
			
			 Write_Data(0x30+RH_data/10); 
			 Write_Data(0x30+RH_data%10);
			 Write_Data('%');
}

/***************************��ʾ��ʪ��������ֵ����************************/
/*����ԭ��:void disp_T_HR(unsigned char T_data,unsigned char RH_data)
/*��������:��ʾ��ʪ����ʪ��������ֵ
/*�������:��
		  
/*�������:��
/********************************************************************/
 
void Disp_T_HR_updown()
{
         LCD_PutString(0,3,"T_U:");
		 Write_Data(0x30+T_UP/10); 
		 Write_Data(0x30+T_UP%10);
		 Write_Data(0x18);	 //���ϼ�ͷ
		 LCD_PutString(0,4,"T_D:");
		 Write_Data(0x30+T_DOWN/10); 
		 Write_Data(0x30+T_DOWN%10);
		 Write_Data(0x19);	 //���¼�ͷ
		
		 LCD_PutString(4,3,"H_U:");
		 Write_Data(0x30+HR_UP/10); 
		 Write_Data(0x30+HR_UP%10);
		 Write_Data(0x18);	 //���ϼ�ͷ
		 LCD_PutString(4,4,"H_D:");
		 Write_Data(0x30+HR_DOWN/10); 
		 Write_Data(0x30+HR_DOWN%10);
		 Write_Data(0x19);	 //���¼�ͷ


}


 /***************************����绰����************************/
/*����ԭ��:void call_phone()
/*��������:����绰
/*�������:��
		  
/*�������:��
/********************************************************************/

 void call_phone()//����绰����
 {
 	ClrScreen();
 	DelaySec(2);	 	
   
 	while(!Hand("OK"))//��GSMģ�����֣��ж��Ƿ��յ�GSMģ�鷵�ص�OK
	{
		Uart2SendStr("ATE1\r");//�򿪻�������
		LCD_PutString(0,2,"������...");
	    DelaySec(1);
		ClrScreen();
		
	}
	CleanGsmRcv();//������ջ���

	ClrScreen();

	while(!(Hand("+CREG: 1,1") || Hand("+CREG: 1,5")))//����Ƿ���ע�ᵽ������
	{
		Uart2SendStr("AT+CREG=1\r");//���÷�����״̬Ϊ������ʾ
		LCD_PutString(0,2,"ע�ᵽ������");
		DelaySec(1);
		ClrScreen();
		Uart2SendStr("AT+CREG?\r");//��������״̬
		LCD_PutString(0,2,"������״̬OK");
		DelaySec(1);
		ClrScreen();		
	}
	CleanGsmRcv();//������ջ���



	Uart2SendStr("ATH\r");//����ǰ�Ȱѵ绰�Ҷϡ�
	bell_sound();
	LCD_PutString(0,2,"����");
	DelaySec(2);
	ClrScreen();
	Uart2SendStr("ATD15118104927;\r");//����15577304201
	LCD_PutString(0,2,"�绰15118104927");
	DelaySec(2);
	ClrScreen();
	while(!Hand("+CLCC: 1,0,0")&&(timer_flagss==1))//����Ƿ��Ѵ�ͨ���յ�+CLCC: 1,0,0��֤���Ѿ���ͨ�绰�ˡ�
	{
		CleanGsmRcv();
		Uart2SendStr("AT+CLCC\r");//���Խ�ͨ״̬
		bell_sound();
		LCD_PutString(0,2,"������.");
		DelaySec(2);
		LCD_PutString(0,2,"������..");
		DelaySec(2);
		LCD_PutString(0,2,"������...");
		DelaySec(2);
		LCD_PutString(0,2,"������....");
		DelaySec(2);
		LCD_PutString(0,2,"������.....");
		DelaySec(2);
		LCD_PutString(0,2,"������......");
		DelaySec(2);
		ClrScreen();
		bell_sound();
		LCD_PutString(0,2,"�Ҷϵ绰");
		Uart2SendStr("ATH\r");//���Խ�ͨ״̬
		DelaySec(5);
		ClrScreen();
		timer_flagss=0;
			
	}
	

 }

/***************************����Ӣ�Ķ��ų���************************/
/*����ԭ��:sent_EnMessage()
/*��������:���Ͷ���
/*�������:��
		  
/*�������:��
/********************************************************************/

 void sent_EnMessage()
 {	  
	while(!Hand("OK"))//��GSMģ�����֣��ж��Ƿ��յ�GSMģ�鷵�ص�OK
	{   ClrScreen();
	    bell_sound();
		LCD_PutString(0,2,"���Ͷ���.");
		Uart2SendStr("ATE1\r");//�򿪻�������
		DelaySec(1);
	
	}
	CleanGsmRcv();//������ջ���

	while(!(Hand("+CREG: 1,1") || Hand("+CREG: 1,5")))//����Ƿ���ע�ᵽ������
	{	LCD_PutString(0,2,"���Ͷ���..");
		Uart2SendStr("AT+CREG=1\r");//���÷�����״̬Ϊ������ʾ
		DelaySec(1);
		Uart2SendStr("AT+CREG?\r");//��������״̬
		DelaySec(1);		
	}
	CleanGsmRcv();//������ջ���


	while(!Hand("OK"))//��GSMģ�����֣��ж��Ƿ��յ�GSMģ�鷵�ص�OK
	{

	   	LCD_PutString(0,2,"���Ͷ���...");
		Uart2SendStr("AT+CMGD=1\r");//�����һ��λ�õĶ���
		DelaySec(2);
	//	ClrScreen();		
	}
	CleanGsmRcv();//������ջ���

	
	Uart2SendStr("AT+CMGF=1\r");//����Ӣ�ķ�ʽ���ͣ���ȻҲ���������ĵ�AT+CMGF=0���������ͷ�ʽ��̫һ���������뿴��س���
	LCD_PutString(0,2,"���Ͷ���....");
	DelaySec(2);
	Uart2SendStr("AT+CMGS=\"15118104927\"\r");//����ָ������10086�绰�Ƕ��ŷ��͵ĺ���
	while(!Hand(">"));//��GSMģ�����֣��ж��Ƿ��յ�GSMģ�鷵�ص�OK					
//	Uart2SendStr("temperature is higt");
//	UART2_PutStringAndNumH("Danger warning!The current temperature or humidity is abnormal.  temperature is:",U8T_data_H," humidity is:",U8RH_data_H);
    UART2_SentWarningMessage(T_DOWN,T_UP,HR_DOWN,HR_UP,U8T_data_H,U8RH_data_H);
//	Uart2SendHex('0'+1);
//	Uart2SendStr("103");//103 Ϊ���͵�����
	CleanGsmRcv();//������ջ���
	Uart2SendHex(0x1a);//��0x1a����������ʾ���ݽ��������Ͷ���
	while(!Hand("+CMGS"));
	bell_sound();
	LCD_PutString(0,2,"���ŷ��ͳɹ�.");
	DelaySec(2);
	ClrScreen();
	timer_flagss=0;		
}


/******************************************************************************
 * key_processing: 
 * ����ԭ��: - key_processing() 
 * ��������: ����������
 * �������: ��
 * �������: ��
 * ��д���ڣ�   2014/04/13, ChenxianLong create this function
 ******************************************************************************/
  void key_processing()
 {
 
  if(!KEY1)  //�����⵽�͵�ƽ��˵����������
    {
	 Delay15ms(); //��ʱȥ����һ��10-20ms
     if(!KEY1)     //�ٴ�ȷ�ϰ����Ƿ��£�û�а������˳�
	   {
        while(!KEY1);//���ȷ�ϰ��°����ȴ������ͷţ�û���ͷ���һֱ�ȴ�
	       {
		   bell_sound();
		   disp_flag=1;	   //�ر���ʾ
		   statu_count++;
		   if(statu_count>=5)
		   {
		   statu_count=0;
		   disp_flag=0;
		   Write_Cmd(0x0C);       //����ʾ(���αꡢ������)
		   }
		   switch(statu_count)
		   {
		           case 1: Write_Cmd(0x0f);
		           Write_Cmd(0x88+2);


				   break;
				   case 2: Write_Cmd(0x0f);
		           Write_Cmd(0x98+2);

				   break;
				   case 3: Write_Cmd(0x0f);
		           Write_Cmd(0x88+6);

				   break;
				   case 4: Write_Cmd(0x0f);
		           Write_Cmd(0x98+6);

				   break;
				   default:break;
		   
		   
		   }

	 		}
	   }
	  
  	  }
	  	    if(!KEY2)  //�����⵽�͵�ƽ��˵����������
    {
	 Delay15ms(); //��ʱȥ����һ��10-20ms
     if(!KEY2)     //�ٴ�ȷ�ϰ����Ƿ��£�û�а������˳�
	   {
        while(!KEY2);//���ȷ�ϰ��°����ȴ������ͷţ�û���ͷ���һֱ�ȴ�
	        {
		    bell_sound();

		   disp_flag=0;
		   Write_Cmd(0x0C);    

	 		}
	   }
	  
  	  }
	  	  if(!KEY3)  //�����⵽�͵�ƽ��˵����������
    {
	 Delay15ms(); //��ʱȥ����һ��10-20ms
     if(!KEY3)     //�ٴ�ȷ�ϰ����Ƿ��£�û�а������˳�
	   {
        while(!KEY3);//���ȷ�ϰ��°����ȴ������ͷţ�û���ͷ���һֱ�ȴ�
	        {
		     
			  bell_sound();
			if((disp_flag==1)&&(statu_count==1))
			{
			
			        T_UP++;
					Write_Cmd(0x0C);       //����ʾ(���αꡢ������)
					Disp_T_HR(U8T_data_H,U8RH_data_H); 
					Disp_T_HR_updown();
			  if(T_UP==100)
			  {
			  T_UP=99;
			  
			  }
			
			} 
				
			if((disp_flag==1)&&(statu_count==2))
			{
			
			        T_DOWN++;
					Write_Cmd(0x0C);       //����ʾ(���αꡢ������)
					Disp_T_HR(U8T_data_H,U8RH_data_H); 
					Disp_T_HR_updown();
			  if(T_DOWN==100)
			  {
			  T_DOWN=99;
			  
			  }
			
			} 

					if((disp_flag==1)&&(statu_count==3))
			{
			
			        HR_UP++;
					Write_Cmd(0x0C);       //����ʾ(���αꡢ������)
					Disp_T_HR(U8T_data_H,U8RH_data_H); 
					Disp_T_HR_updown();
			  if(HR_UP==100)
			  {
			  HR_UP=99;
			  
			  }
			
			} 

						if((disp_flag==1)&&(statu_count==4))
			{
			
			        HR_DOWN++;
					Write_Cmd(0x0C);       //����ʾ(���αꡢ������)
					Disp_T_HR(U8T_data_H,U8RH_data_H); 
					Disp_T_HR_updown();
			  if(HR_DOWN==100)
			  {
			  HR_DOWN=99;
			  }
			
			} 
	
	 		}
	   }
	  
  	  }
	  if(!KEY4)  //�����⵽�͵�ƽ��˵����������
    {
	 Delay15ms(); //��ʱȥ����һ��10-20ms
     if(!KEY4)     //�ٴ�ȷ�ϰ����Ƿ��£�û�а������˳�
	   {
        while(!KEY4);//���ȷ�ϰ��°����ȴ������ͷţ�û���ͷ���һֱ�ȴ�
	        {    bell_sound();
			 	if((disp_flag==1)&&(statu_count==1))
			{
			
			 	    T_UP--;
		        	Write_Cmd(0x0C);       //����ʾ(���αꡢ������)
					Disp_T_HR(U8T_data_H,U8RH_data_H); 
					Disp_T_HR_updown();
			  if(T_UP==255)
			  {
			  T_UP=0;
			  
			  }
			
			} 
				if((disp_flag==1)&&(statu_count==2))
			{
			
			        T_DOWN--;
					Write_Cmd(0x0C);       //����ʾ(���αꡢ������)
					Disp_T_HR(U8T_data_H,U8RH_data_H); 
					Disp_T_HR_updown();
			  if(T_DOWN==255)
			  {
			  T_DOWN=0;
			  
			  }
			
			}
				if((disp_flag==1)&&(statu_count==3))
			{
			
			        HR_UP--;
					Write_Cmd(0x0C);       //����ʾ(���αꡢ������)
					Disp_T_HR(U8T_data_H,U8RH_data_H); 
					Disp_T_HR_updown();
			  if(HR_UP==255)
			  {
			  HR_UP=0;
			  
			  }
			
			} 
			
						if((disp_flag==1)&&(statu_count==4))
			{
			
			        HR_DOWN--;
					Write_Cmd(0x0C);       //����ʾ(���αꡢ������)
					Disp_T_HR(U8T_data_H,U8RH_data_H); 
					Disp_T_HR_updown();
			  if(HR_DOWN==255)
			  {
			  HR_DOWN=0;
			  
			  }
			
			}  
		   
	 		}
	   }
	  
  	  }
  	 
 }


/******************************************************************************
 * receve_message: 
 * ����ԭ��: -  receve_message()
 * ��������: ���Ž��պ���
 * �������: 
 * �������: 
 * ��д���ڣ�   2014/04/13, ChenxianLong create this function
 ******************************************************************************/
 void receve_message() //���ն���
 {
 
 if(Hand("+CMTI"))//�յ����ţ�GSMģ��ᷢ��+CMTI�ַ�
		{
				ClrScreen();
				bell_sound();
				LCD_PutString(0,2,"�յ�����...");
				
				Uart2SendStr("AT+CMGF=1\r");//����ΪUDP���Ÿ�ʽ
				DelaySec(1);
				CleanGsmRcv();
				Uart2SendStr("AT+CMGR=1\r");//������һ��λ�õĶ���
				DelaySec(3);
				new_msm_flag = 1;
				ClrScreen();
				
		}
			if(Hand(ChgNum) && new_msm_flag == 1)
		     {
				bell_sound();
				LCD_PutString(0,2,"ָ������");
				DelaySec(2);
				ClrScreen();
			
			if(Hand("ledon"))
			{
			    bell_sound();
		    	LCD_PutString(0,2,"����");
				DelaySec(2);
				ClrScreen();					
			}
		
			else if(Hand("ledoff"))
			{
			    bell_sound();
				LCD_PutString(0,2,"�ص�");
				DelaySec(2);			
			}

				else if(Hand("message"))
			{
			    bell_sound();
				LCD_PutString(0,2,"message");
				DelaySec(2);	
			    sent_EnMessage(); 	
			}
			
			else
			{
			    bell_sound();
				LCD_PutString(0,2,"��Чָ��");
				DelaySec(2);
				ClrScreen();
						
			}
		
			
				bell_sound();
				Uart2SendStr("AT+CMGD=1\r");//�����һ��λ�õĶ���
				LCD_PutString(0,2,"DEL MESSAGE");//LCD��ʾ
				DelaySec(2);
		        CleanGsmRcv();
				new_msm_flag = 0;
				ClrScreen();
		
			
							
		}
		else if(!Hand(ChgNum) && new_msm_flag == 1)
		{
			bell_sound();
		   	LCD_PutString(0,2,"İ������");//LCD��ʾ
			DelaySec(2);
			
			ClrScreen();
		 	Uart2SendStr("AT+CMGD=1\r");//�����һ��λ�õĶ���
			bell_sound();	
			LCD_PutString(0,2,"DEL MESSAGE");//LCD��ʾ
			DelaySec(2);
			
			CleanGsmRcv();
			new_msm_flag = 0;
			ClrScreen();
			
		
		}


 
 }

/******************************************************************************
 * main: 
 * ����ԭ��: - 
 * ��������: 
 * �������: 
 * �������: 
 * ��д���ڣ�   2014/04/13, ChenxianLong create this function
 ******************************************************************************/
void main()
{   
 
    T_DOWN=20;
	T_UP=43;
	HR_DOWN=30;
	HR_UP=80;

    Init_Timer1();
	Timer0Init(); 
//  �������뺯���ļ���ֵ���Ըı䴮�ڲ�����
//	Uart1Init(0,1,184);//��ʼ�����ڣ����ò�����4800
//	Uart2Init(0,1,220);//��ʼ�����ڣ����ò�����9600
//	Uart2Init(0,1,232);//��ʼ�����ڣ����ò�����14400
//	Uart1Init(0,1,253);//��ʼ�����ڣ����ò�����115200
	Uart1Init(0,1,220);//��ʼ�����ڣ����ò�����9600
	Uart2Init(0,1,220);//��ʼ�����ڣ����ò�����9600
//	Uart1Init(0,0,232);//��ʼ�����ڣ����ò�����1200
//	Uart2Init(0,0,232);//��ʼ�����ڣ����ò�����1200
	CleanGsmRcv();
	CGRAM();     //д���Զ����ַ�
    Init_ST7920();   //��ʼ�� 
	LCD_PutString(0,1,"Һ����ʼ��...");
	DelaySec(2);
	ClrScreen();
	bell_sound();
 	timer_flagss=1;
	disp_flag=0;

   
    

 while(1)
   {

	 receve_message();

 if(!disp_flag)
 {
 	
	Disp_T_HR(U8T_data_H,U8RH_data_H); 
	Disp_T_HR_updown();
	if(((U8T_data_H>T_UP)||(U8T_data_H<T_DOWN)||(U8RH_data_H>HR_UP)||(U8RH_data_H<HR_DOWN))&&(timer_flagss==1))
	{
   // call_phone(); 
      sent_EnMessage();

	}

	   key_processing();
	}
	

	           
		
   key_processing();
} 

}


