
#ifndef __LCD12864_H__
#define __LCD12864_H__


#define DataPort P0        //MCU P0<------> LCM
sbit RS = P2^4;
sbit RW = P2^5;
sbit E  = P2^6;


void DelayUs2x(unsigned char t);
void DelaynMs(unsigned char t);
void Delays(unsigned int t);
void Check_Busy();
void Write_Cmd(unsigned char Cmd);
void Write_Data(unsigned char Data);
void Init_ST7920();
void CGRAM();
void DisplayCGRAM(unsigned char x,unsigned char y);
/*------------------------------------------------
                   ��ʾ�ַ���
x:������ֵ����Χ0~8
y:������ֵ����Χ1~4
------------------------------------------------*/
void LCD_PutString(unsigned char x,unsigned char y,unsigned char code *s);
/*------------------------------------------------
                      ����
------------------------------------------------*/
void ClrScreen();
/*------------------------------------------------
                   ��ʾͼƬ
------------------------------------------------*/
void LCD_PutGraphic(unsigned char code *img);
/*------------------------------------------------
                 ���õ���ͼģʽ
------------------------------------------------*/
void SetGraphicMode();
#endif