
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
                   显示字符串
x:横坐标值，范围0~8
y:纵坐标值，范围1~4
------------------------------------------------*/
void LCD_PutString(unsigned char x,unsigned char y,unsigned char code *s);
/*------------------------------------------------
                      清屏
------------------------------------------------*/
void ClrScreen();
/*------------------------------------------------
                   显示图片
------------------------------------------------*/
void LCD_PutGraphic(unsigned char code *img);
/*------------------------------------------------
                 设置到绘图模式
------------------------------------------------*/
void SetGraphicMode();
#endif