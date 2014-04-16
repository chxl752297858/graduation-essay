
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
//在此处修改发送号码和内容
uchar n[13] = "15118104927";//短信号码


//以下是用来发送短信的号码
/***********************号码说明******************************************************
UDP短信格式显示时，GPRS模块发送出来的来信号码带有编码规则，比如使用号码1252015296811438发送短信
那么收到的号码会被“两位一反，奇数补F”规则变换，比如前面两位是12那么变成21，下两位是52，变成25.
完成后是"2125102569184183"号码个数是偶数，不用加F，如果是奇数要在最后就加上F后在两位一反。比如：
号码是：15296811438，11位数，那么要先加F：15296811438F,再变成：5192861134F8.
**************************************************************************************/


  
unsigned char T_UP,T_DOWN,HR_UP,HR_DOWN;


/******************************************************************************
 * Delay15ms: 
 * 函数原型: - 
 * 函数功能: 
 * 输入参数: 
 * 输出参数: 
 * 编写日期：   2014/04/13, ChenxianLong create this function
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
 * 函数原型: -bell_sound() 
 * 函数功能: 蜂鸣器发出响声
 * 输入参数: 
 * 输出参数: 
 * 编写日期：   2014/04/13, ChenxianLong create this function
 ******************************************************************************/
void bell_sound()
{
;
 BELL=0;
 DelaynMs(5);
 BELL=1;
}

   /***************串口打印温湿度值程序程序************************/
/*函数原型:void UART2_SentWarningMessage(unsigned char T_DOWN,unsigned char  T_UP,unsigned char H_DOWN,unsigned char  H_UP,unsigned char num,unsigned char num1)
/*函数功能:接收指定手机号码短信命令
/*输入参数:T_DOWN,char  T_UP,char H_DOWN,char  H_UP,char num,char num1
		   温度下限值，温度上限，湿度下限值，湿度上限值，温度值，湿度值。
/*输出参数:无
/********************************************************************/

/******************************************************************************
 * UART2_SentWarningMessage: 
 * 函数原型: - 
 * 函数功能: 
 * 输入参数: 
 * 输出参数: 
 * 编写日期：   2014/04/13, ChenxianLong create this function
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

//将gsm模块发送出来的来信号码转变成CS2编码

/******将gsm模块发送出来的来信号码转变成CS2编码************************/
/*函数原型:ChangeNum(uchar* num, uchar* des)
/*函数功能:将gsm模块发送出来的来信号码转变成CS2编码
/*输入参数:uchar* num,
           uchar* des
/*输出参数:无
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




/***************************显示温湿度值程序************************/
/*函数原型:void disp_T_HR(unsigned char T_data,unsigned char RH_data)
/*函数功能:显示温湿度值
/*输入参数:unsigned char T_data  温度值
           unsigned char RH_data 湿度值
		  
/*输出参数:无
/********************************************************************/

void Disp_T_HR(unsigned char T_data,unsigned char RH_data)
{


			 LCD_PutString(0,1,"温度");
			
			 Write_Data(0x30+T_data/10); 
			 Write_Data(0x30+T_data%10);
			 LCD_PutString(3,1,"℃");
			 LCD_PutString(4,1,"湿度");
			
			 Write_Data(0x30+RH_data/10); 
			 Write_Data(0x30+RH_data%10);
			 Write_Data('%');
}

/***************************显示温湿度上下限值函数************************/
/*函数原型:void disp_T_HR(unsigned char T_data,unsigned char RH_data)
/*函数功能:显示温湿度温湿度上下限值
/*输入参数:无
		  
/*输出参数:无
/********************************************************************/
 
void Disp_T_HR_updown()
{
         LCD_PutString(0,3,"T_U:");
		 Write_Data(0x30+T_UP/10); 
		 Write_Data(0x30+T_UP%10);
		 Write_Data(0x18);	 //向上箭头
		 LCD_PutString(0,4,"T_D:");
		 Write_Data(0x30+T_DOWN/10); 
		 Write_Data(0x30+T_DOWN%10);
		 Write_Data(0x19);	 //向下箭头
		
		 LCD_PutString(4,3,"H_U:");
		 Write_Data(0x30+HR_UP/10); 
		 Write_Data(0x30+HR_UP%10);
		 Write_Data(0x18);	 //向上箭头
		 LCD_PutString(4,4,"H_D:");
		 Write_Data(0x30+HR_DOWN/10); 
		 Write_Data(0x30+HR_DOWN%10);
		 Write_Data(0x19);	 //向下箭头


}


 /***************************拨打电话程序************************/
/*函数原型:void call_phone()
/*函数功能:拨打电话
/*输入参数:无
		  
/*输出参数:无
/********************************************************************/

 void call_phone()//拨打电话函数
 {
 	ClrScreen();
 	DelaySec(2);	 	
   
 	while(!Hand("OK"))//与GSM模块握手，判断是否收到GSM模块返回的OK
	{
		Uart2SendStr("ATE1\r");//打开回显命令
		LCD_PutString(0,2,"连接中...");
	    DelaySec(1);
		ClrScreen();
		
	}
	CleanGsmRcv();//清除接收缓存

	ClrScreen();

	while(!(Hand("+CREG: 1,1") || Hand("+CREG: 1,5")))//检测是否以注册到服务商
	{
		Uart2SendStr("AT+CREG=1\r");//设置服务商状态为数据显示
		LCD_PutString(0,2,"注册到服务商");
		DelaySec(1);
		ClrScreen();
		Uart2SendStr("AT+CREG?\r");//检测服务商状态
		LCD_PutString(0,2,"服务商状态OK");
		DelaySec(1);
		ClrScreen();		
	}
	CleanGsmRcv();//清除接收缓存



	Uart2SendStr("ATH\r");//拨打前先把电话挂断。
	bell_sound();
	LCD_PutString(0,2,"拨号");
	DelaySec(2);
	ClrScreen();
	Uart2SendStr("ATD15118104927;\r");//拨打15577304201
	LCD_PutString(0,2,"电话15118104927");
	DelaySec(2);
	ClrScreen();
	while(!Hand("+CLCC: 1,0,0")&&(timer_flagss==1))//检查是否已打通，收到+CLCC: 1,0,0，证明已经打通电话了。
	{
		CleanGsmRcv();
		Uart2SendStr("AT+CLCC\r");//测试接通状态
		bell_sound();
		LCD_PutString(0,2,"拨号中.");
		DelaySec(2);
		LCD_PutString(0,2,"拨号中..");
		DelaySec(2);
		LCD_PutString(0,2,"拨号中...");
		DelaySec(2);
		LCD_PutString(0,2,"拨号中....");
		DelaySec(2);
		LCD_PutString(0,2,"拨号中.....");
		DelaySec(2);
		LCD_PutString(0,2,"拨号中......");
		DelaySec(2);
		ClrScreen();
		bell_sound();
		LCD_PutString(0,2,"挂断电话");
		Uart2SendStr("ATH\r");//测试接通状态
		DelaySec(5);
		ClrScreen();
		timer_flagss=0;
			
	}
	

 }

/***************************发送英文短信程序************************/
/*函数原型:sent_EnMessage()
/*函数功能:发送短信
/*输入参数:无
		  
/*输出参数:无
/********************************************************************/

 void sent_EnMessage()
 {	  
	while(!Hand("OK"))//与GSM模块握手，判断是否收到GSM模块返回的OK
	{   ClrScreen();
	    bell_sound();
		LCD_PutString(0,2,"发送短信.");
		Uart2SendStr("ATE1\r");//打开回显命令
		DelaySec(1);
	
	}
	CleanGsmRcv();//清除接收缓存

	while(!(Hand("+CREG: 1,1") || Hand("+CREG: 1,5")))//检测是否以注册到服务商
	{	LCD_PutString(0,2,"发送短信..");
		Uart2SendStr("AT+CREG=1\r");//设置服务商状态为数据显示
		DelaySec(1);
		Uart2SendStr("AT+CREG?\r");//检测服务商状态
		DelaySec(1);		
	}
	CleanGsmRcv();//清除接收缓存


	while(!Hand("OK"))//与GSM模块握手，判断是否收到GSM模块返回的OK
	{

	   	LCD_PutString(0,2,"发送短信...");
		Uart2SendStr("AT+CMGD=1\r");//清除第一个位置的短信
		DelaySec(2);
	//	ClrScreen();		
	}
	CleanGsmRcv();//清除接收缓存

	
	Uart2SendStr("AT+CMGF=1\r");//设置英文方式发送，当然也可以用中文的AT+CMGF=0，不过发送方式不太一样，具体请看相关程序
	LCD_PutString(0,2,"发送短信....");
	DelaySec(2);
	Uart2SendStr("AT+CMGS=\"15118104927\"\r");//发送指令设置10086电话是短信发送的号码
	while(!Hand(">"));//与GSM模块握手，判断是否收到GSM模块返回的OK					
//	Uart2SendStr("temperature is higt");
//	UART2_PutStringAndNumH("Danger warning!The current temperature or humidity is abnormal.  temperature is:",U8T_data_H," humidity is:",U8RH_data_H);
    UART2_SentWarningMessage(T_DOWN,T_UP,HR_DOWN,HR_UP,U8T_data_H,U8RH_data_H);
//	Uart2SendHex('0'+1);
//	Uart2SendStr("103");//103 为发送的内容
	CleanGsmRcv();//清除接收缓存
	Uart2SendHex(0x1a);//以0x1a来结束，表示内容结束，发送短信
	while(!Hand("+CMGS"));
	bell_sound();
	LCD_PutString(0,2,"短信发送成功.");
	DelaySec(2);
	ClrScreen();
	timer_flagss=0;		
}


/******************************************************************************
 * key_processing: 
 * 函数原型: - key_processing() 
 * 函数功能: 按键处理函数
 * 输入参数: 无
 * 输出参数: 无
 * 编写日期：   2014/04/13, ChenxianLong create this function
 ******************************************************************************/
  void key_processing()
 {
 
  if(!KEY1)  //如果检测到低电平，说明按键按下
    {
	 Delay15ms(); //延时去抖，一般10-20ms
     if(!KEY1)     //再次确认按键是否按下，没有按下则退出
	   {
        while(!KEY1);//如果确认按下按键等待按键释放，没有释放则一直等待
	       {
		   bell_sound();
		   disp_flag=1;	   //关闭显示
		   statu_count++;
		   if(statu_count>=5)
		   {
		   statu_count=0;
		   disp_flag=0;
		   Write_Cmd(0x0C);       //开显示(无游标、不反白)
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
	  	    if(!KEY2)  //如果检测到低电平，说明按键按下
    {
	 Delay15ms(); //延时去抖，一般10-20ms
     if(!KEY2)     //再次确认按键是否按下，没有按下则退出
	   {
        while(!KEY2);//如果确认按下按键等待按键释放，没有释放则一直等待
	        {
		    bell_sound();

		   disp_flag=0;
		   Write_Cmd(0x0C);    

	 		}
	   }
	  
  	  }
	  	  if(!KEY3)  //如果检测到低电平，说明按键按下
    {
	 Delay15ms(); //延时去抖，一般10-20ms
     if(!KEY3)     //再次确认按键是否按下，没有按下则退出
	   {
        while(!KEY3);//如果确认按下按键等待按键释放，没有释放则一直等待
	        {
		     
			  bell_sound();
			if((disp_flag==1)&&(statu_count==1))
			{
			
			        T_UP++;
					Write_Cmd(0x0C);       //开显示(无游标、不反白)
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
					Write_Cmd(0x0C);       //开显示(无游标、不反白)
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
					Write_Cmd(0x0C);       //开显示(无游标、不反白)
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
					Write_Cmd(0x0C);       //开显示(无游标、不反白)
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
	  if(!KEY4)  //如果检测到低电平，说明按键按下
    {
	 Delay15ms(); //延时去抖，一般10-20ms
     if(!KEY4)     //再次确认按键是否按下，没有按下则退出
	   {
        while(!KEY4);//如果确认按下按键等待按键释放，没有释放则一直等待
	        {    bell_sound();
			 	if((disp_flag==1)&&(statu_count==1))
			{
			
			 	    T_UP--;
		        	Write_Cmd(0x0C);       //开显示(无游标、不反白)
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
					Write_Cmd(0x0C);       //开显示(无游标、不反白)
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
					Write_Cmd(0x0C);       //开显示(无游标、不反白)
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
					Write_Cmd(0x0C);       //开显示(无游标、不反白)
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
 * 函数原型: -  receve_message()
 * 函数功能: 短信接收函数
 * 输入参数: 
 * 输出参数: 
 * 编写日期：   2014/04/13, ChenxianLong create this function
 ******************************************************************************/
 void receve_message() //接收短信
 {
 
 if(Hand("+CMTI"))//收到短信，GSM模块会发来+CMTI字符
		{
				ClrScreen();
				bell_sound();
				LCD_PutString(0,2,"收到短信...");
				
				Uart2SendStr("AT+CMGF=1\r");//设置为UDP短信格式
				DelaySec(1);
				CleanGsmRcv();
				Uart2SendStr("AT+CMGR=1\r");//读出第一个位置的短信
				DelaySec(3);
				new_msm_flag = 1;
				ClrScreen();
				
		}
			if(Hand(ChgNum) && new_msm_flag == 1)
		     {
				bell_sound();
				LCD_PutString(0,2,"指定号码");
				DelaySec(2);
				ClrScreen();
			
			if(Hand("ledon"))
			{
			    bell_sound();
		    	LCD_PutString(0,2,"开灯");
				DelaySec(2);
				ClrScreen();					
			}
		
			else if(Hand("ledoff"))
			{
			    bell_sound();
				LCD_PutString(0,2,"关灯");
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
				LCD_PutString(0,2,"无效指令");
				DelaySec(2);
				ClrScreen();
						
			}
		
			
				bell_sound();
				Uart2SendStr("AT+CMGD=1\r");//清除第一个位置的短信
				LCD_PutString(0,2,"DEL MESSAGE");//LCD显示
				DelaySec(2);
		        CleanGsmRcv();
				new_msm_flag = 0;
				ClrScreen();
		
			
							
		}
		else if(!Hand(ChgNum) && new_msm_flag == 1)
		{
			bell_sound();
		   	LCD_PutString(0,2,"陌生号码");//LCD显示
			DelaySec(2);
			
			ClrScreen();
		 	Uart2SendStr("AT+CMGD=1\r");//清除第一个位置的短信
			bell_sound();	
			LCD_PutString(0,2,"DEL MESSAGE");//LCD显示
			DelaySec(2);
			
			CleanGsmRcv();
			new_msm_flag = 0;
			ClrScreen();
			
		
		}


 
 }

/******************************************************************************
 * main: 
 * 函数原型: - 
 * 函数功能: 
 * 输入参数: 
 * 输出参数: 
 * 编写日期：   2014/04/13, ChenxianLong create this function
 ******************************************************************************/
void main()
{   
 
    T_DOWN=20;
	T_UP=43;
	HR_DOWN=30;
	HR_UP=80;

    Init_Timer1();
	Timer0Init(); 
//  调整输入函数的加载值可以改变串口波特率
//	Uart1Init(0,1,184);//初始化串口，设置波特率4800
//	Uart2Init(0,1,220);//初始化串口，设置波特率9600
//	Uart2Init(0,1,232);//初始化串口，设置波特率14400
//	Uart1Init(0,1,253);//初始化串口，设置波特率115200
	Uart1Init(0,1,220);//初始化串口，设置波特率9600
	Uart2Init(0,1,220);//初始化串口，设置波特率9600
//	Uart1Init(0,0,232);//初始化串口，设置波特率1200
//	Uart2Init(0,0,232);//初始化串口，设置波特率1200
	CleanGsmRcv();
	CGRAM();     //写入自定义字符
    Init_ST7920();   //初始化 
	LCD_PutString(0,1,"液晶初始化...");
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


