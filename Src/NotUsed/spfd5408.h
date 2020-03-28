#ifndef SPFD5408_H
#define SPFD5408_H

#include "stm32f4xx_hal.h"

#ifndef bool 
#define bool unsigned char
#endif

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif

//�����
#define SPFD5408_BLACK     0x0000
#define SPFD5408_RED       0xF800
#define SPFD5408_ORANGE    0xFBE0
#define SPFD5408_YELLOW    0xFFE0
#define SPFD5408_GREEN     0x07E0
#define SPFD5408_CYAN      0x07FF
#define SPFD5408_BLUE      0x001F
#define SPFD5408_MAGENTA   0xF81F
#define SPFD5408_VIOLET    0x881F
#define SPFD5408_WHITE     0xFFFF


#define SPFD5408_WIDTH 240UL
#define SPFD5408_HEIGHT 320UL


//----------------------------------------------------------------------------------------------------
//��������� �������
//----------------------------------------------------------------------------------------------------
void SPFD5408_Init(void);//������������� LCD-������

void SPFD5408_CS_One(void);//��������� �� CS 1
void SPFD5408_CS_Zero(void);//��������� �� CS 0

void SPFD5408_RS_One(void);//��������� �� RS 1
void SPFD5408_RS_Zero(void);//��������� �� RS 0

void SPFD5408_WR_One(void);//��������� �� WR 1
void SPFD5408_WR_Zero(void);//��������� �� WR 0

void SPFD5408_RST_One(void);//��������� �� RST 1
void SPFD5408_RST_Zero(void);//��������� �� RST 0

void SPFD5408_RD_One(void);//��������� �� RD 1
void SPFD5408_RD_Zero(void);//��������� �� RD 0

void SPFD5408_SetData(unsigned char byte);//������ �������� �� ���� ������

void SPFD5408_Write8(unsigned char byte);//�������� ����
void SPFD5408_Write16(unsigned short value) ;//�������� 16 ��� 
void SPFD5408_Write24(unsigned long value);//�������� 24 ����
void SPFD5408_Write32(unsigned long value);//�������� 32 ����
void SPFD5408_WriteRegister16(unsigned short reg,unsigned short value) ;//�������� � ������� 16 ���
void SPFD5408_WriteRegister8(unsigned char reg,unsigned char value);//�������� � ������� 8 ���
void SPFD5408_Reset(void);//��������� �����
void SPFD5408_SetWindow(unsigned short x_left,unsigned short y_top,unsigned short x_right,unsigned short y_bottom);//������ ����

void SPFD5408_OutColor(unsigned short color);//�������� ���� �����
void SPFD5408_PutPixel(unsigned short x, unsigned short y, unsigned short color);//������� ���� �������
void SPFD5408_PutSymbol(long x,long y,char symbol,unsigned short color);//����� ������� � �������
void SPFD5408_PutString(long x,long y,const char *string,unsigned short color);//����� ������� � �������
void SPFD5408_Print(const char *string,unsigned short color);//������� ����� � ������� �������
void SPFD5408_Clear(void);//�������� ������� �������


#endif  
