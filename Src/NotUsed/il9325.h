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

//цвета
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
//прототипы функций
//----------------------------------------------------------------------------------------------------
void IL9325_Init(void);//инициализация LCD-экрана

void IL9325_CS_One(void);//выставить на CS 1
void IL9325_CS_Zero(void);//выставить на CS 0

void IL9325_RS_One(void);//выставить на RS 1
void IL9325_RS_Zero(void);//выставить на RS 0

void IL9325_WR_One(void);//выставить на WR 1
void IL9325_WR_Zero(void);//выставить на WR 0

void IL9325_RST_One(void);//выставить на RST 1
void IL9325_RST_Zero(void);//выставить на RST 0

void IL9325_RD_One(void);//выставить на RD 1
void IL9325_RD_Zero(void);//выставить на RD 0

void IL9325_SetData(unsigned char byte);//задать значение на шине данных

void IL9325_Write8(unsigned char byte);//передать байт
void IL9325_Write16(unsigned short value) ;//передать 16 бит 
void IL9325_WriteRegister16(unsigned short reg,unsigned short value) ;//записать в регистр 16 бит
void IL9325_WriteRegister8(unsigned char reg,unsigned char value);//записать в регистр 8 бит
void IL9325_Reset(void);//выполнить сброс
void IL9325_SetWindow(unsigned short x_left,unsigned short y_top,unsigned short x_right,unsigned short y_bottom);//задать окно

void IL9325_OutColor(unsigned short color);//передать цвет точки
void IL9325_PutPixel(unsigned short x, unsigned short y, unsigned short color);//вывести один пиксель
void IL9325_PutSymbol(long x,long y,char symbol,unsigned short color);//вывод символа в позицию
void IL9325_PutString(long x,long y,const char *string,unsigned short color);//вывод строчки в позицию
void IL9325_Print(const char *string,unsigned short color);//вывести текст в текущую позицию
void IL9325_Clear(void);//сбросить текущую позицию


#endif  
