#ifndef _GPRS_H_
#define _GPRS_H_


#define MAXRCV 200
#define MAXBUF 100

extern uchar GsmRcv[];
extern uchar DebugBuf_U2[];
extern uchar DebugBuf_U1[];

extern uchar PrintBuf_U1[];

extern uchar GsmRcvCnt;
extern uchar GsmAtFlag;
extern uchar Debug2RcvCnt;
extern uchar Debug1RcvCnt;

//ÉùÃ÷º¯Êý

char Uart2Init(char s2smod,char brtx12,unsigned char reload);
void Uart2SendHex(uchar i);
void Uart2SendStr(char* data_at);
char Uart1Init(char smod,char brtx12,unsigned char reload);
void Uart1SendHex(char i);
void Uart1SendStr(char* at);
void CleanGsmRcv(void);
bit Hand(unsigned char *);


#endif

