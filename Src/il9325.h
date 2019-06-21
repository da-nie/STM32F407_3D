#ifndef IL9325_H
#define IL9325_H

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
#define IL9325_BLACK     0x0000
#define IL9325_RED       0xF800
#define IL9325_ORANGE    0xFBE0
#define IL9325_YELLOW    0xFFE0
#define IL9325_GREEN     0x07E0
#define IL9325_CYAN      0x07FF
#define IL9325_BLUE      0x001F
#define IL9325_MAGENTA   0xF81F
#define IL9325_VIOLET    0x881F
#define IL9325_WHITE     0xFFFF


#define IL9325_WIDTH 240UL
#define IL9325_HEIGHT 320UL


//----------------------------------------------------------------------------------------------------
//��������� �������
//----------------------------------------------------------------------------------------------------
void IL9325_Init(void);//������������� LCD-������

void IL9325_CS_One(void);//��������� �� CS 1
void IL9325_CS_Zero(void);//��������� �� CS 0

void IL9325_RS_One(void);//��������� �� RS 1
void IL9325_RS_Zero(void);//��������� �� RS 0

void IL9325_WR_One(void);//��������� �� WR 1
void IL9325_WR_Zero(void);//��������� �� WR 0

void IL9325_RST_One(void);//��������� �� RST 1
void IL9325_RST_Zero(void);//��������� �� RST 0

void IL9325_RD_One(void);//��������� �� RD 1
void IL9325_RD_Zero(void);//��������� �� RD 0

void IL9325_SetData(unsigned char byte);//������ �������� �� ���� ������

void IL9325_Write8(unsigned char byte);//�������� ����
void IL9325_Write16(unsigned short value) ;//�������� 16 ��� 
void IL9325_WriteRegister16(unsigned short reg,unsigned short value) ;//�������� � ������� 16 ���
void IL9325_WriteRegister8(unsigned char reg,unsigned char value);//�������� � ������� 8 ���
void IL9325_Reset(void);//��������� �����
void IL9325_SetWindow(unsigned short x_left,unsigned short y_top,unsigned short x_right,unsigned short y_bottom);//������ ����

void IL9325_OutColor(unsigned short color);//�������� ���� �����
void IL9325_PutPixel(unsigned short x, unsigned short y, unsigned short color);//������� ���� �������
void IL9325_PutSymbol(long x,long y,char symbol,unsigned short color);//����� ������� � �������
void IL9325_PutString(long x,long y,const char *string,unsigned short color);//����� ������� � �������
void IL9325_Print(const char *string,unsigned short color);//������� ����� � ������� �������
void IL9325_Clear(void);//�������� ������� �������


#endif  
