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

//цвета
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
//прототипы функций
//----------------------------------------------------------------------------------------------------
void SPFD5408_Init(void);//инициализация LCD-экрана

void SPFD5408_CS_One(void);//выставить на CS 1
void SPFD5408_CS_Zero(void);//выставить на CS 0

void SPFD5408_RS_One(void);//выставить на RS 1
void SPFD5408_RS_Zero(void);//выставить на RS 0

void SPFD5408_WR_One(void);//выставить на WR 1
void SPFD5408_WR_Zero(void);//выставить на WR 0

void SPFD5408_RST_One(void);//выставить на RST 1
void SPFD5408_RST_Zero(void);//выставить на RST 0

void SPFD5408_RD_One(void);//выставить на RD 1
void SPFD5408_RD_Zero(void);//выставить на RD 0

void SPFD5408_SetData(unsigned char byte);//задать значение на шине данных

void SPFD5408_Write8(unsigned char byte);//передать байт
void SPFD5408_Write16(unsigned short value) ;//передать 16 бит 
void SPFD5408_Write24(unsigned long value);//передать 24 бита
void SPFD5408_Write32(unsigned long value);//передать 32 бита
void SPFD5408_WriteRegister16(unsigned short reg,unsigned short value) ;//записать в регистр 16 бит
void SPFD5408_WriteRegister8(unsigned char reg,unsigned char value);//записать в регистр 8 бит
void SPFD5408_Reset(void);//выполнить сброс
void SPFD5408_SetWindow(unsigned short x_left,unsigned short y_top,unsigned short x_right,unsigned short y_bottom);//задать окно

void SPFD5408_OutColor(unsigned short color);//передать цвет точки
void SPFD5408_PutPixel(unsigned short x, unsigned short y, unsigned short color);//вывести один пиксель
void SPFD5408_PutSymbol(long x,long y,char symbol,unsigned short color);//вывод символа в позицию
void SPFD5408_PutString(long x,long y,const char *string,unsigned short color);//вывод строчки в позицию
void SPFD5408_Print(const char *string,unsigned short color);//вывести текст в текущую позицию
void SPFD5408_Clear(void);//сбросить текущую позицию


#endif  
