#include "spfd5408.h"

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//����������������
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//�������� �������
#define SPFD5408_GPIO_PIN_CS		GPIO_PIN_12
#define SPFD5408_GPIO_CS				GPIOE

#define SPFD5408_GPIO_PIN_RST	GPIO_PIN_2
#define SPFD5408_GPIO_RST			GPIOE

#define SPFD5408_GPIO_PIN_RS		GPIO_PIN_15
#define SPFD5408_GPIO_RS				GPIOE

#define SPFD5408_GPIO_PIN_WR		GPIO_PIN_14
#define SPFD5408_GPIO_WR				GPIOE

#define SPFD5408_GPIO_PIN_RD		GPIO_PIN_13
#define SPFD5408_GPIO_RD				GPIOE


#define SPFD5408_GPIO_PIN_D0		GPIO_PIN_4
#define SPFD5408_GPIO_D0				GPIOE

#define SPFD5408_GPIO_PIN_D1		GPIO_PIN_5
#define SPFD5408_GPIO_D1				GPIOE

#define SPFD5408_GPIO_PIN_D2		GPIO_PIN_6
#define SPFD5408_GPIO_D2				GPIOE

#define SPFD5408_GPIO_PIN_D3		GPIO_PIN_7
#define SPFD5408_GPIO_D3				GPIOE

#define SPFD5408_GPIO_PIN_D4		GPIO_PIN_8
#define SPFD5408_GPIO_D4				GPIOE

#define SPFD5408_GPIO_PIN_D5		GPIO_PIN_9
#define SPFD5408_GPIO_D5				GPIOE

#define SPFD5408_GPIO_PIN_D6		GPIO_PIN_10
#define SPFD5408_GPIO_D6				GPIOE

#define SPFD5408_GPIO_PIN_D7		GPIO_PIN_11
#define SPFD5408_GPIO_D7				GPIOE

//��� ������� ����� � �������������
#define DELAY_CODE 0xFFFF

#define TFTLCD_DELAY DELAY_CODE

#define ILI9341_SOFTRESET          0x01
#define ILI9341_SLEEPIN            0x10
#define ILI9341_SLEEPOUT           0x11
#define ILI9341_NORMALDISP         0x13
#define ILI9341_INVERTOFF          0x20
#define ILI9341_INVERTON           0x21
#define ILI9341_GAMMASET           0x26
#define ILI9341_DISPLAYOFF         0x28
#define ILI9341_DISPLAYON          0x29
#define ILI9341_COLADDRSET         0x2A
#define ILI9341_PAGEADDRSET        0x2B
#define ILI9341_MEMORYWRITE        0x2C
#define ILI9341_PIXELFORMAT        0x3A
#define ILI9341_FRAMECONTROL       0xB1
#define ILI9341_DISPLAYFUNC        0xB6
#define ILI9341_ENTRYMODE          0xB7
#define ILI9341_POWERCONTROL1      0xC0
#define ILI9341_POWERCONTROL2      0xC1
#define ILI9341_VCOMCONTROL1       0xC5
#define ILI9341_VCOMCONTROL2       0xC7
#define ILI9341_MEMCONTROL         0x36
#define ILI9341_MADCTL             0x36

#define ILI9341_MADCTL_MY  0x80
#define ILI9341_MADCTL_MX  0x40
#define ILI9341_MADCTL_MV  0x20
#define ILI9341_MADCTL_ML  0x10
#define ILI9341_MADCTL_RGB 0x00
#define ILI9341_MADCTL_BGR 0x08
#define ILI9341_MADCTL_MH  0x04

#define FONT_HEIGHT 14
#define FONT_WIDTH 8

//----------------------------------------------------------------------------------------------------
//���������� ����������
//----------------------------------------------------------------------------------------------------

volatile static long PrintYPosition=0;//������� ������ ������

const unsigned char Font8x14[224][14]=
{
 { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},//' '=0x20 
 { 0x00, 0x10, 0x38, 0x38, 0x38, 0x10, 0x10, 0x10, 0x00, 0x10, 0x10, 0x00, 0x00, 0x00},//'!'=0x21 
 { 0x00, 0x66, 0x66, 0x22, 0x44, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},//'"'=0x22 
 { 0x00, 0x44, 0x44, 0xFE, 0xFE, 0x44, 0x44, 0xFE, 0xFE, 0x44, 0x44, 0x00, 0x00, 0x00},//'#'=0x23 
 { 0x18, 0x18, 0x7C, 0xC6, 0xC2, 0xC0, 0x7C, 0x06, 0x06, 0x86, 0xC6, 0x7C, 0x18, 0x18}, 
 { 0x00, 0x00, 0x00, 0xC2, 0xC6, 0x0C, 0x18, 0x30, 0x60, 0xC6, 0x86, 0x00, 0x00, 0x00}, 
 { 0x00, 0x38, 0x6C, 0x6C, 0x38, 0x76, 0xDC, 0xCC, 0xCC, 0xCC, 0x76, 0x00, 0x00, 0x00}, 
 { 0x00, 0x30, 0x30, 0x30, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
 { 0x00, 0x0C, 0x18, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x18, 0x0C, 0x00, 0x00, 0x00}, 
 { 0x00, 0x30, 0x18, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x18, 0x30, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0x66, 0x3C, 0xFF, 0x3C, 0x66, 0x00, 0x00, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x10, 0x10, 0x10, 0x7C, 0x7C, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x18, 0x30, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7C, 0x7C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x18, 0x00, 0x00, 0x00}, 
 { 0x02, 0x06, 0x04, 0x0C, 0x08, 0x18, 0x10, 0x30, 0x20, 0x60, 0x40, 0x00, 0x00, 0x00}, 
 { 0x00, 0x7C, 0xC6, 0xC6, 0xCE, 0xDE, 0xF6, 0xE6, 0xC6, 0xC6, 0x7C, 0x00, 0x00, 0x00}, 
 { 0x00, 0x18, 0x38, 0x78, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x7E, 0x00, 0x00, 0x00}, 
 { 0x00, 0x7C, 0xC6, 0x06, 0x0C, 0x18, 0x30, 0x60, 0xC0, 0xC6, 0xFE, 0x00, 0x00, 0x00}, 
 { 0x00, 0x7C, 0xC6, 0x06, 0x06, 0x3C, 0x06, 0x06, 0x06, 0xC6, 0x7C, 0x00, 0x00, 0x00}, 
 { 0x00, 0x0C, 0x1C, 0x3C, 0x6C, 0xCC, 0xFE, 0x0C, 0x0C, 0x0C, 0x1E, 0x00, 0x00, 0x00}, 
 { 0x00, 0xFE, 0xC0, 0xC0, 0xC0, 0xFC, 0x06, 0x06, 0x06, 0xC6, 0x7C, 0x00, 0x00, 0x00}, 
 { 0x00, 0x38, 0x60, 0xC0, 0xC0, 0xFC, 0xC6, 0xC6, 0xC6, 0xC6, 0x7C, 0x00, 0x00, 0x00}, 
 { 0x00, 0xFE, 0xC6, 0x06, 0x06, 0x0C, 0x18, 0x30, 0x30, 0x30, 0x30, 0x00, 0x00, 0x00}, 
 { 0x00, 0x7C, 0xC6, 0xC6, 0xC6, 0x7C, 0xC6, 0xC6, 0xC6, 0xC6, 0x7C, 0x00, 0x00, 0x00}, 
 { 0x00, 0x7C, 0xC6, 0xC6, 0xC6, 0x7E, 0x06, 0x06, 0x06, 0x0C, 0x78, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x18, 0x18, 0x30, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x06, 0x0C, 0x18, 0x30, 0x60, 0x30, 0x18, 0x0C, 0x06, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0x7E, 0x7E, 0x00, 0x00, 0x7E, 0x7E, 0x00, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x60, 0x30, 0x18, 0x0C, 0x06, 0x0C, 0x18, 0x30, 0x60, 0x00, 0x00, 0x00}, 
 { 0x00, 0x7C, 0xC6, 0xC6, 0x0C, 0x18, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x7C, 0xC6, 0xC6, 0xDE, 0xDE, 0xDE, 0xDC, 0xC0, 0x7C, 0x00, 0x00, 0x00}, 
 { 0x00, 0x10, 0x38, 0x6C, 0xC6, 0xC6, 0xFE, 0xC6, 0xC6, 0xC6, 0xC6, 0x00, 0x00, 0x00}, 
 { 0x00, 0xFC, 0x66, 0x66, 0x66, 0x7C, 0x66, 0x66, 0x66, 0x66, 0xFC, 0x00, 0x00, 0x00}, 
 { 0x00, 0x3C, 0x66, 0xC2, 0xC0, 0xC0, 0xC0, 0xC0, 0xC2, 0x66, 0x3C, 0x00, 0x00, 0x00}, 
 { 0x00, 0xF8, 0x6C, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x6C, 0xF8, 0x00, 0x00, 0x00}, 
 { 0x00, 0xFE, 0x66, 0x62, 0x68, 0x78, 0x68, 0x60, 0x62, 0x66, 0xFE, 0x00, 0x00, 0x00}, 
 { 0x00, 0xFE, 0x66, 0x62, 0x68, 0x78, 0x68, 0x60, 0x60, 0x60, 0xF0, 0x00, 0x00, 0x00}, 
 { 0x00, 0x3C, 0x66, 0xC2, 0xC0, 0xC0, 0xDE, 0xC6, 0xC6, 0x66, 0x3A, 0x00, 0x00, 0x00}, 
 { 0x00, 0xC6, 0xC6, 0xC6, 0xC6, 0xFE, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0x00, 0x00, 0x00}, 
 { 0x00, 0x3C, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3C, 0x00, 0x00, 0x00}, 
 { 0x00, 0x1E, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0xCC, 0xCC, 0xCC, 0x78, 0x00, 0x00, 0x00}, 
 { 0x00, 0xE6, 0x66, 0x66, 0x6C, 0x78, 0x78, 0x6C, 0x66, 0x66, 0xE6, 0x00, 0x00, 0x00}, 
 { 0x00, 0xF0, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x62, 0x66, 0xFE, 0x00, 0x00, 0x00}, 
 { 0x00, 0xC6, 0xEE, 0xFE, 0xFE, 0xD6, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0x00, 0x00, 0x00}, 
 { 0x00, 0xC6, 0xE6, 0xF6, 0xFE, 0xDE, 0xCE, 0xC6, 0xC6, 0xC6, 0xC6, 0x00, 0x00, 0x00}, 
 { 0x00, 0x7C, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0x7C, 0x00, 0x00, 0x00}, 
 { 0x00, 0xFC, 0x66, 0x66, 0x66, 0x7C, 0x60, 0x60, 0x60, 0x60, 0xF0, 0x00, 0x00, 0x00}, 
 { 0x00, 0x7C, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xD6, 0xDE, 0x7C, 0x0C, 0x0E, 0x00}, 
 { 0x00, 0xFC, 0x66, 0x66, 0x66, 0x7C, 0x6C, 0x66, 0x66, 0x66, 0xE6, 0x00, 0x00, 0x00}, 
 { 0x00, 0x7C, 0xC6, 0xC6, 0x60, 0x38, 0x0C, 0x06, 0xC6, 0xC6, 0x7C, 0x00, 0x00, 0x00}, 
 { 0x00, 0x7E, 0x7E, 0x5A, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3C, 0x00, 0x00, 0x00}, 
 { 0x00, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0x7C, 0x00, 0x00, 0x00}, 
 { 0x00, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0x6C, 0x38, 0x10, 0x00, 0x00, 0x00}, 
 { 0x00, 0xC6, 0xC6, 0xC6, 0xC6, 0xD6, 0xD6, 0xD6, 0xFE, 0xEE, 0x6C, 0x00, 0x00, 0x00}, 
 { 0x00, 0xC6, 0xC6, 0x6C, 0x7C, 0x38, 0x38, 0x7C, 0x6C, 0xC6, 0xC6, 0x00, 0x00, 0x00}, 
 { 0x00, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x18, 0x18, 0x18, 0x18, 0x3C, 0x00, 0x00, 0x00}, 
 { 0x00, 0xFE, 0xC6, 0x86, 0x0C, 0x18, 0x30, 0x60, 0xC2, 0xC6, 0xFE, 0x00, 0x00, 0x00}, 
 { 0x00, 0x3C, 0x3C, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x3C, 0x3C, 0x00, 0x00, 0x00}, 
 { 0x40, 0x60, 0x20, 0x30, 0x10, 0x18, 0x08, 0x0C, 0x04, 0x06, 0x02, 0x00, 0x00, 0x00}, 
 { 0x00, 0x3C, 0x3C, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x3C, 0x3C, 0x00, 0x00, 0x00}, 
 { 0x10, 0x38, 0x6C, 0xC6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00}, 
 { 0x00, 0x20, 0x20, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0x78, 0x0C, 0x7C, 0xCC, 0xCC, 0xCC, 0x76, 0x00, 0x00, 0x00}, 
 { 0x00, 0xE0, 0x60, 0x60, 0x78, 0x6C, 0x66, 0x66, 0x66, 0x66, 0x7C, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0x7C, 0xC6, 0xC0, 0xC0, 0xC0, 0xC6, 0x7C, 0x00, 0x00, 0x00}, 
 { 0x00, 0x1C, 0x0C, 0x0C, 0x3C, 0x6C, 0xCC, 0xCC, 0xCC, 0xCC, 0x76, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0x7C, 0xC6, 0xFE, 0xC0, 0xC0, 0xC6, 0x7C, 0x00, 0x00, 0x00}, 
 { 0x00, 0x38, 0x6C, 0x64, 0x60, 0xF0, 0x60, 0x60, 0x60, 0x60, 0xF0, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0x76, 0xCC, 0xCC, 0xCC, 0xCC, 0x7C, 0x0C, 0xCC, 0x78, 0x00}, 
 { 0x00, 0xE0, 0x60, 0x60, 0x6C, 0x76, 0x66, 0x66, 0x66, 0x66, 0xE6, 0x00, 0x00, 0x00}, 
 { 0x00, 0x18, 0x18, 0x00, 0x38, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3C, 0x00, 0x00, 0x00}, 
 { 0x00, 0x06, 0x06, 0x00, 0x0E, 0x06, 0x06, 0x06, 0x06, 0x06, 0x66, 0x66, 0x3C, 0x00}, 
 { 0x00, 0xE0, 0x60, 0x60, 0x66, 0x6C, 0x78, 0x78, 0x6C, 0x66, 0xE6, 0x00, 0x00, 0x00}, 
 { 0x00, 0x38, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3C, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0xEC, 0xFE, 0xD6, 0xD6, 0xD6, 0xD6, 0xC6, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0xDC, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0x7C, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0x7C, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0xDC, 0x66, 0x66, 0x66, 0x66, 0x66, 0x7C, 0x60, 0xF0, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0x76, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0x7C, 0x0C, 0x1E, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0xDC, 0x76, 0x66, 0x60, 0x60, 0x60, 0xF0, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0x7C, 0xC6, 0x60, 0x38, 0x0C, 0xC6, 0x7C, 0x00, 0x00, 0x00}, 
 { 0x00, 0x10, 0x30, 0x30, 0xFC, 0x30, 0x30, 0x30, 0x30, 0x36, 0x1C, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0x76, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0x66, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x18, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0xC6, 0xC6, 0xD6, 0xD6, 0xD6, 0xFE, 0x6C, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0xC6, 0x6C, 0x38, 0x38, 0x38, 0x6C, 0xC6, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0x7E, 0x06, 0xC6, 0x7C, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0xFE, 0xCC, 0x18, 0x30, 0x60, 0xC6, 0xFE, 0x00, 0x00, 0x00}, 
 { 0x00, 0x0E, 0x18, 0x18, 0x18, 0x70, 0x18, 0x18, 0x18, 0x18, 0x0E, 0x00, 0x00, 0x00}, 
 { 0x00, 0x10, 0x10, 0x10, 0x10, 0x00, 0x00, 0x10, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00}, 
 { 0x00, 0x70, 0x18, 0x18, 0x18, 0x0E, 0x18, 0x18, 0x18, 0x18, 0x70, 0x00, 0x00, 0x00}, 
 { 0x00, 0x76, 0xDC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x10, 0x38, 0x6C, 0xC6, 0xC6, 0xC6, 0xFE, 0x00, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x03, 0x03}, 
 { 0x00, 0x00, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, 
 { 0x00, 0x18, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, 
 { 0x00, 0x00, 0xC0, 0xE1, 0xF1, 0xF9, 0xF9, 0xF9, 0xFD, 0xFC, 0xFC, 0xFE, 0xFE, 0xFE}, 
 { 0x00, 0x00, 0x00, 0xC0, 0x20, 0xA0, 0xE0, 0xA0, 0xE0, 0x20, 0xC0, 0x00, 0x00, 0x00}, 
 { 0x07, 0x0F, 0x0F, 0x0F, 0x1F, 0x1F, 0x1F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x7F, 0x7F}, 
 { 0xFB, 0xF9, 0xF1, 0xF0, 0xE0, 0xE0, 0xE0, 0xC0, 0xC0, 0xC0, 0x80, 0x80, 0x80, 0xC0}, 
 { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x7F, 0x7E, 0x3E, 0x3E, 0x3C, 0x1C, 0x1E}, 
 { 0xFF, 0xDF, 0xCF, 0x8F, 0x87, 0x87, 0x07, 0x03, 0x03, 0x03, 0x01, 0x01, 0x01, 0x00}, 
 { 0x00, 0x80, 0x80, 0x80, 0xC0, 0xC0, 0xC0, 0xE0, 0xE0, 0xE0, 0xF0, 0xF0, 0xF0, 0xF8}, 
 { 0x7F, 0x7F, 0x7C, 0x7C, 0x7C, 0x78, 0x78, 0x78, 0x70, 0x70, 0x70, 0x60, 0x60, 0x60}, 
 { 0xE0, 0xE0, 0xE0, 0xF0, 0xF0, 0xF0, 0x78, 0x78, 0x78, 0x3C, 0x3C, 0x3C, 0x1E, 0x1E}, 
 { 0x1F, 0x0F, 0x0F, 0x0F, 0x07, 0x07, 0x07, 0x03, 0x03, 0x03, 0x01, 0x01, 0x01, 0x00}, 
 { 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0xC0, 0xC0, 0xC0, 0xE0, 0xE0, 0xE0, 0xF0, 0xF0}, 
 { 0x78, 0x78, 0x3C, 0x3C, 0x3C, 0x1E, 0x1E, 0x1E, 0x1F, 0x0F, 0x0F, 0x0F, 0x07, 0x07}, 
 { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
 { 0x1F, 0x1F, 0x3F, 0x3F, 0x3F, 0x7F, 0x7F, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, 
 { 0x00, 0x00, 0x80, 0x80, 0x80, 0xC0, 0xC1, 0xC1, 0xE1, 0xE3, 0xE3, 0xF3, 0xF7, 0xFF}, 
 { 0x78, 0x78, 0x78, 0x7C, 0xFC, 0xFC, 0xFE, 0xFE, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, 
 { 0x04, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80}, 
 { 0x03, 0x03, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
 { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x3F, 0x1F, 0x0F, 0x07, 0x03, 0x00, 0x00, 0x00}, 
 { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x18, 0x00, 0x00}, 
 { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFC, 0xF8, 0xF0, 0xC0, 0x00, 0x00, 0x00}, 
 { 0x80, 0xC0, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
 { 0x18, 0x3C, 0x24, 0x24, 0x24, 0x24, 0x3C, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
 { 0x00, 0x10, 0x10, 0x7C, 0x7C, 0x10, 0x10, 0x00, 0x7C, 0x7C, 0x00, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
 { 0x28, 0x00, 0xFE, 0x66, 0x62, 0x68, 0x78, 0x68, 0x62, 0x66, 0xFE, 0x00, 0x00, 0x00}, 
 { 0x3C, 0x66, 0x42, 0x81, 0x99, 0xBD, 0xA1, 0xA1, 0xBD, 0x99, 0x81, 0x42, 0x66, 0x3C}, 
 { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0x00, 0x33, 0x66, 0xCC, 0x66, 0x33, 0x00, 0x00, 0x00, 0x00}, 
 { 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA}, 
 { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, 
 { 0x3C, 0x66, 0x42, 0x81, 0xB9, 0xBD, 0xA5, 0xB9, 0xBD, 0xA5, 0x81, 0x42, 0x66, 0x3C}, 
 { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x1F, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10}, 
 { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0xF0, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10}, 
 { 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x1F, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
 { 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0xF0, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
 { 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10}, 
 { 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x1F, 0x1F, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10}, 
 { 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0xF0, 0xF0, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10}, 
 { 0x00, 0x28, 0x28, 0x00, 0x7C, 0xC6, 0xFE, 0xC0, 0xC0, 0xC6, 0x7C, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10}, 
 { 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0x00, 0xCC, 0x66, 0x33, 0x66, 0xCC, 0x00, 0x00, 0x00, 0x00}, 
 { 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0xFF, 0xFF, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10}, 
 { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, 
 { 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0}, 
 { 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F}, 
 { 0x00, 0x1E, 0x36, 0x66, 0xC6, 0xC6, 0xFE, 0xC6, 0xC6, 0xC6, 0xC6, 0x00, 0x00, 0x00}, 
 { 0x00, 0xFE, 0x62, 0x62, 0x60, 0x7C, 0x66, 0x66, 0x66, 0x66, 0xFC, 0x00, 0x00, 0x00}, 
 { 0x00, 0xFC, 0x66, 0x66, 0x66, 0x7C, 0x66, 0x66, 0x66, 0x66, 0xFC, 0x00, 0x00, 0x00}, 
 { 0x00, 0xFE, 0x62, 0x62, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0xF0, 0x00, 0x00, 0x00}, 
 { 0x00, 0x1E, 0x36, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0xFF, 0xC3, 0x81, 0x00}, 
 { 0x00, 0xFE, 0x66, 0x62, 0x68, 0x78, 0x68, 0x60, 0x62, 0x66, 0xFE, 0x00, 0x00, 0x00}, 
 { 0x00, 0xD6, 0xD6, 0x54, 0x54, 0x7C, 0x7C, 0x54, 0xD6, 0xD6, 0xD6, 0x00, 0x00, 0x00}, 
 { 0x00, 0x7C, 0xC6, 0x06, 0x06, 0x3C, 0x06, 0x06, 0x06, 0xC6, 0x7C, 0x00, 0x00, 0x00}, 
 { 0x00, 0xC6, 0xC6, 0xCE, 0xCE, 0xD6, 0xE6, 0xE6, 0xC6, 0xC6, 0xC6, 0x00, 0x00, 0x00}, 
 { 0x28, 0x10, 0xC6, 0xC6, 0xCE, 0xCE, 0xD6, 0xE6, 0xE6, 0xC6, 0xC6, 0x00, 0x00, 0x00}, 
 { 0x00, 0xE6, 0x66, 0x6C, 0x6C, 0x78, 0x78, 0x6C, 0x6C, 0x66, 0xE6, 0x00, 0x00, 0x00}, 
 { 0x00, 0x1E, 0x36, 0x66, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0x00, 0x00, 0x00}, 
 { 0x00, 0xC6, 0xEE, 0xFE, 0xFE, 0xD6, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0x00, 0x00, 0x00}, 
 { 0x00, 0xC6, 0xC6, 0xC6, 0xC6, 0xFE, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0x00, 0x00, 0x00}, 
 { 0x00, 0x7C, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0x7C, 0x00, 0x00, 0x00}, 
 { 0x00, 0xFE, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0x00, 0x00, 0x00}, 
 { 0x00, 0xFC, 0x66, 0x66, 0x66, 0x7C, 0x60, 0x60, 0x60, 0x60, 0xF0, 0x00, 0x00, 0x00}, 
 { 0x00, 0x3C, 0x66, 0xC2, 0xC0, 0xC0, 0xC0, 0xC0, 0xC2, 0x66, 0x3C, 0x00, 0x00, 0x00}, 
 { 0x00, 0x7E, 0x5A, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3C, 0x00, 0x00, 0x00}, 
 { 0x00, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0x7E, 0x06, 0x06, 0xC6, 0x7C, 0x00, 0x00, 0x00}, 
 { 0x3C, 0x18, 0x7E, 0xDB, 0xDB, 0xDB, 0xDB, 0xDB, 0x7E, 0x18, 0x3C, 0x00, 0x00, 0x00}, 
 { 0x00, 0xC6, 0xC6, 0x6C, 0x7C, 0x38, 0x38, 0x7C, 0x6C, 0xC6, 0xC6, 0x00, 0x00, 0x00}, 
 { 0x00, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xFE, 0x06, 0x06, 0x00}, 
 { 0x00, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0x7E, 0x06, 0x06, 0x06, 0x06, 0x00, 0x00, 0x00}, 
 { 0x00, 0xDB, 0xDB, 0xDB, 0xDB, 0xDB, 0xDB, 0xDB, 0xDB, 0xDB, 0xFF, 0x00, 0x00, 0x00}, 
 { 0x00, 0xDB, 0xDB, 0xDB, 0xDB, 0xDB, 0xDB, 0xDB, 0xDB, 0xDB, 0xFF, 0x03, 0x03, 0x00}, 
 { 0x00, 0xF8, 0xB0, 0x30, 0x30, 0x3C, 0x36, 0x36, 0x36, 0x36, 0x7C, 0x00, 0x00, 0x00}, 
 { 0x00, 0xC3, 0xC3, 0xC3, 0xC3, 0xF3, 0xDB, 0xDB, 0xDB, 0xDB, 0xF3, 0x00, 0x00, 0x00}, 
 { 0x00, 0xF0, 0x60, 0x60, 0x60, 0x7C, 0x66, 0x66, 0x66, 0x66, 0xFC, 0x00, 0x00, 0x00}, 
 { 0x00, 0x7C, 0xC6, 0x06, 0x26, 0x3E, 0x26, 0x06, 0x06, 0xC6, 0x7C, 0x00, 0x00, 0x00}, 
 { 0x00, 0xCE, 0xDB, 0xDB, 0xDB, 0xFB, 0xDB, 0xDB, 0xDB, 0xDB, 0xCE, 0x00, 0x00, 0x00}, 
 { 0x00, 0x3F, 0x66, 0x66, 0x66, 0x3E, 0x3E, 0x66, 0x66, 0x66, 0xE7, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0x78, 0x0C, 0x7C, 0xCC, 0xCC, 0xCC, 0x76, 0x00, 0x00, 0x00}, 
 { 0x02, 0x06, 0x3C, 0x60, 0x60, 0x7C, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0xFC, 0x66, 0x66, 0x7C, 0x66, 0x66, 0xFC, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0x7E, 0x32, 0x32, 0x30, 0x30, 0x30, 0x78, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0x1E, 0x36, 0x36, 0x66, 0x66, 0x66, 0xFF, 0xC3, 0xC3, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0x7C, 0xC6, 0xFE, 0xC0, 0xC0, 0xC6, 0x7C, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0xD6, 0xD6, 0x54, 0x7C, 0x54, 0xD6, 0xD6, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0x3C, 0x66, 0x06, 0x0C, 0x06, 0x66, 0x3C, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0xC6, 0xC6, 0xCE, 0xD6, 0xE6, 0xC6, 0xC6, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x28, 0x10, 0xC6, 0xC6, 0xCE, 0xD6, 0xE6, 0xC6, 0xC6, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0xE6, 0x6C, 0x78, 0x78, 0x6C, 0x66, 0xE6, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0x1E, 0x36, 0x66, 0x66, 0x66, 0x66, 0x66, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0xC6, 0xEE, 0xFE, 0xFE, 0xD6, 0xD6, 0xC6, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0xC6, 0xC6, 0xC6, 0xFE, 0xC6, 0xC6, 0xC6, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0x7C, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0x7C, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0xFE, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0xDC, 0x66, 0x66, 0x66, 0x66, 0x66, 0x7C, 0x60, 0xF0, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0x7C, 0xC6, 0xC0, 0xC0, 0xC0, 0xC6, 0x7C, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0x7E, 0x5A, 0x18, 0x18, 0x18, 0x18, 0x3C, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0x7E, 0x06, 0xC6, 0x7C, 0x00}, 
 { 0x00, 0x00, 0x00, 0x3C, 0x18, 0x7E, 0xDB, 0xDB, 0xDB, 0xDB, 0x7E, 0x18, 0x3C, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0xC6, 0x6C, 0x38, 0x38, 0x38, 0x6C, 0xC6, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xFE, 0x06, 0x06, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0xC6, 0xC6, 0xC6, 0xC6, 0x7E, 0x06, 0x06, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0xD6, 0xD6, 0xD6, 0xD6, 0xD6, 0xD6, 0xFE, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0xD6, 0xD6, 0xD6, 0xD6, 0xD6, 0xD6, 0xFE, 0x03, 0x03, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0xF8, 0xB0, 0x30, 0x3E, 0x33, 0x33, 0x7E, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0xC6, 0xC6, 0xC6, 0xF6, 0xDE, 0xDE, 0xF6, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0xF0, 0x60, 0x60, 0x7C, 0x66, 0x66, 0xFC, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0x3C, 0x66, 0x06, 0x1E, 0x06, 0x66, 0x3C, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0xCE, 0xDB, 0xDB, 0xFB, 0xDB, 0xDB, 0xCE, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0x7E, 0xCC, 0xCC, 0xFC, 0x6C, 0xCC, 0xCE, 0x00, 0x00, 0x00}
};

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//����� �������
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//----------------------------------------------------------------------------------------------------
//������������� LCD-������
//----------------------------------------------------------------------------------------------------
void SPFD5408_Init(void)
{
 //����������� �����

 __HAL_RCC_GPIOD_CLK_ENABLE();
 __HAL_RCC_GPIOE_CLK_ENABLE();
	
 //GPIO_Pin � ������ �������, ������� ���������������. ������ ��� ���������� �������:
 //GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
 //GPIO_Speed � ������ �������� ��� ��������� �������.
 //GPIO_Mode � ������ ����� ������ �������. 

 //����������� ��������� ������
 GPIO_InitTypeDef SPFD5408_GPIO_Init;

 SPFD5408_GPIO_Init.Mode=GPIO_MODE_OUTPUT_PP;
 SPFD5408_GPIO_Init.Pull=GPIO_NOPULL;
 SPFD5408_GPIO_Init.Speed=GPIO_SPEED_FREQ_VERY_HIGH;


 SPFD5408_GPIO_Init.Pin=SPFD5408_GPIO_PIN_CS;
 HAL_GPIO_Init(SPFD5408_GPIO_CS,&SPFD5408_GPIO_Init);

 SPFD5408_GPIO_Init.Pin=SPFD5408_GPIO_PIN_RS;
 HAL_GPIO_Init(SPFD5408_GPIO_RS,&SPFD5408_GPIO_Init);

 SPFD5408_GPIO_Init.Pin=SPFD5408_GPIO_PIN_RST;
 HAL_GPIO_Init(SPFD5408_GPIO_RST,&SPFD5408_GPIO_Init);

 SPFD5408_GPIO_Init.Pin=SPFD5408_GPIO_PIN_RD;
 HAL_GPIO_Init(SPFD5408_GPIO_RD,&SPFD5408_GPIO_Init);

 SPFD5408_GPIO_Init.Pin=SPFD5408_GPIO_PIN_WR;
 HAL_GPIO_Init(SPFD5408_GPIO_WR,&SPFD5408_GPIO_Init);
 

 SPFD5408_GPIO_Init.Pin=SPFD5408_GPIO_PIN_D0;
 HAL_GPIO_Init(SPFD5408_GPIO_D0,&SPFD5408_GPIO_Init);

 SPFD5408_GPIO_Init.Pin=SPFD5408_GPIO_PIN_D1;
 HAL_GPIO_Init(SPFD5408_GPIO_D1,&SPFD5408_GPIO_Init);

 SPFD5408_GPIO_Init.Pin=SPFD5408_GPIO_PIN_D2;
 HAL_GPIO_Init(SPFD5408_GPIO_D2,&SPFD5408_GPIO_Init);

 SPFD5408_GPIO_Init.Pin=SPFD5408_GPIO_PIN_D3;
 HAL_GPIO_Init(SPFD5408_GPIO_D3,&SPFD5408_GPIO_Init);

 SPFD5408_GPIO_Init.Pin=SPFD5408_GPIO_PIN_D4;
 HAL_GPIO_Init(SPFD5408_GPIO_D4,&SPFD5408_GPIO_Init);

 SPFD5408_GPIO_Init.Pin=SPFD5408_GPIO_PIN_D5;
 HAL_GPIO_Init(SPFD5408_GPIO_D5,&SPFD5408_GPIO_Init);

 SPFD5408_GPIO_Init.Pin=SPFD5408_GPIO_PIN_D6;
 HAL_GPIO_Init(SPFD5408_GPIO_D6,&SPFD5408_GPIO_Init);

 SPFD5408_GPIO_Init.Pin=SPFD5408_GPIO_PIN_D7;
 HAL_GPIO_Init(SPFD5408_GPIO_D7,&SPFD5408_GPIO_Init);

 //�������������� �������
 SPFD5408_Reset();
 SPFD5408_CS_Zero();
 
 SPFD5408_WriteRegister8(ILI9341_SOFTRESET,0);
 HAL_Delay(50);
 SPFD5408_WriteRegister8(ILI9341_DISPLAYOFF,0);
 SPFD5408_WriteRegister8(ILI9341_POWERCONTROL1,0x23);
 SPFD5408_WriteRegister8(ILI9341_POWERCONTROL2,0x10);
 SPFD5408_WriteRegister16(ILI9341_VCOMCONTROL1,0x2B2B);
 SPFD5408_WriteRegister8(ILI9341_VCOMCONTROL2,0xC0);
 SPFD5408_WriteRegister8(ILI9341_MEMCONTROL,ILI9341_MADCTL_MY|ILI9341_MADCTL_BGR);
 SPFD5408_WriteRegister8(ILI9341_PIXELFORMAT,0x55);
 SPFD5408_WriteRegister16(ILI9341_FRAMECONTROL,0x001B);    
 SPFD5408_WriteRegister8(ILI9341_ENTRYMODE,0x07);
 SPFD5408_WriteRegister8(ILI9341_SLEEPOUT,0);
 HAL_Delay(150);
 SPFD5408_WriteRegister8(ILI9341_DISPLAYON,0);
 HAL_Delay(500);
}
//----------------------------------------------------------------------------------------------------
//��������� �� CS 1
//----------------------------------------------------------------------------------------------------
inline void SPFD5408_CS_One(void)
{
 HAL_GPIO_WritePin(SPFD5408_GPIO_CS,SPFD5408_GPIO_PIN_CS,GPIO_PIN_SET);
}
//----------------------------------------------------------------------------------------------------
//��������� �� CS 0
//----------------------------------------------------------------------------------------------------
inline void SPFD5408_CS_Zero(void)
{
 HAL_GPIO_WritePin(SPFD5408_GPIO_CS,SPFD5408_GPIO_PIN_CS,GPIO_PIN_RESET);
}
//----------------------------------------------------------------------------------------------------
//��������� �� RS 1
//----------------------------------------------------------------------------------------------------
inline void SPFD5408_RS_One(void)
{
 HAL_GPIO_WritePin(SPFD5408_GPIO_RS,SPFD5408_GPIO_PIN_RS,GPIO_PIN_SET);
}
//----------------------------------------------------------------------------------------------------
//��������� �� RS 0
//----------------------------------------------------------------------------------------------------
inline void SPFD5408_RS_Zero(void)
{
  HAL_GPIO_WritePin(SPFD5408_GPIO_RS,SPFD5408_GPIO_PIN_RS,GPIO_PIN_RESET);
}
//----------------------------------------------------------------------------------------------------
//��������� �� WR 1
//----------------------------------------------------------------------------------------------------
inline void SPFD5408_WR_One(void)
{
 HAL_GPIO_WritePin(SPFD5408_GPIO_WR,SPFD5408_GPIO_PIN_WR,GPIO_PIN_SET);
}
//----------------------------------------------------------------------------------------------------
//��������� �� WR 0
//----------------------------------------------------------------------------------------------------
inline void SPFD5408_WR_Zero(void)
{
 HAL_GPIO_WritePin(SPFD5408_GPIO_WR,SPFD5408_GPIO_PIN_WR,GPIO_PIN_RESET);
}
//----------------------------------------------------------------------------------------------------
//��������� �� RST 1
//----------------------------------------------------------------------------------------------------
inline void SPFD5408_RST_One(void)
{
 HAL_GPIO_WritePin(SPFD5408_GPIO_RST,SPFD5408_GPIO_PIN_RST,GPIO_PIN_SET);
}
//----------------------------------------------------------------------------------------------------
//��������� �� RST 0
//----------------------------------------------------------------------------------------------------
inline void SPFD5408_RST_Zero(void)
{
 HAL_GPIO_WritePin(SPFD5408_GPIO_RST,SPFD5408_GPIO_PIN_RST,GPIO_PIN_RESET);
}
//----------------------------------------------------------------------------------------------------
//��������� �� RD 1
//----------------------------------------------------------------------------------------------------
inline void SPFD5408_RD_One(void)
{
 HAL_GPIO_WritePin(SPFD5408_GPIO_RD,SPFD5408_GPIO_PIN_RD,GPIO_PIN_SET);
}
//----------------------------------------------------------------------------------------------------
//��������� �� RD 0
//----------------------------------------------------------------------------------------------------
inline void SPFD5408_RD_Zero(void)
{
 HAL_GPIO_WritePin(SPFD5408_GPIO_RD,SPFD5408_GPIO_PIN_RD,GPIO_PIN_RESET);
}
//----------------------------------------------------------------------------------------------------
//������ �������� �� ���� ������
//----------------------------------------------------------------------------------------------------
inline void SPFD5408_SetData(unsigned char byte)
{
 unsigned long inv_byte=0xFF^byte;
 unsigned long pr_byte=byte;
 SPFD5408_GPIO_D0->BSRR=(pr_byte<<4UL)|(inv_byte<<(4UL+16UL));

}

//----------------------------------------------------------------------------------------------------
//��������� �����
//----------------------------------------------------------------------------------------------------
void SPFD5408_Reset(void)
{
 SPFD5408_CS_One();
 SPFD5408_RS_One();
 SPFD5408_RD_One();
 SPFD5408_WR_One();
 SPFD5408_RST_One();    
 HAL_Delay(10);
 SPFD5408_RST_Zero();
 HAL_Delay(10);
 SPFD5408_RST_One();
 HAL_Delay(250);
}
//----------------------------------------------------------------------------------------------------
//�������� ����
//----------------------------------------------------------------------------------------------------
inline void SPFD5408_Write8(unsigned char byte)
{ 
 SPFD5408_SetData(byte);
 SPFD5408_WR_Zero();
 SPFD5408_WR_One();
}
//----------------------------------------------------------------------------------------------------
//�������� 16 ��� 
//----------------------------------------------------------------------------------------------------
inline void SPFD5408_Write16(unsigned short value) 
{ 
 SPFD5408_Write8((value>>8)&0xFF);
 SPFD5408_Write8(value&0xFF);
}
//----------------------------------------------------------------------------------------------------
//�������� 24 ����
//----------------------------------------------------------------------------------------------------
inline void SPFD5408_Write24(unsigned long value) 
{ 
 SPFD5408_Write8((value>>16)&0xFF);
 SPFD5408_Write8((value>>8)&0xFF);
 SPFD5408_Write8(value&0xFF);
}
//----------------------------------------------------------------------------------------------------
//�������� 32 ����
//----------------------------------------------------------------------------------------------------
inline void SPFD5408_Write32(unsigned long value) 
{ 
 SPFD5408_Write8((value>>24)&0xFF);
 SPFD5408_Write8((value>>16)&0xFF);
 SPFD5408_Write8((value>>8)&0xFF);
 SPFD5408_Write8(value&0xFF);
}
//----------------------------------------------------------------------------------------------------
//�������� � ������� 16 ���
//----------------------------------------------------------------------------------------------------
inline void SPFD5408_WriteRegister16(unsigned short reg,unsigned short value) 
{ 
 SPFD5408_RS_Zero();
 SPFD5408_Write16(reg);
 SPFD5408_RS_One();
 SPFD5408_Write16(value); 
}
//----------------------------------------------------------------------------------------------------
//�������� � ������� 8 ���
//----------------------------------------------------------------------------------------------------
inline void SPFD5408_WriteRegister8(unsigned char reg,unsigned char value) 
{ 
 SPFD5408_RS_Zero();
 SPFD5408_Write8(reg);
 SPFD5408_RS_One();
 SPFD5408_Write8(value); 
}

//----------------------------------------------------------------------------------------------------
//������ ����
//----------------------------------------------------------------------------------------------------
void SPFD5408_SetWindow(unsigned short x_left,unsigned short y_top,unsigned short x_right,unsigned short y_bottom)
{
 SPFD5408_RS_Zero();
 SPFD5408_Write8(ILI9341_COLADDRSET);
 SPFD5408_RS_One();
 SPFD5408_Write16(x_left);	
 SPFD5408_Write16(x_right);	

 SPFD5408_RS_Zero();
 SPFD5408_Write8(ILI9341_PAGEADDRSET);
 SPFD5408_RS_One();
 SPFD5408_Write16(y_top);	
 SPFD5408_Write16(y_bottom);	

 SPFD5408_RS_Zero();
 SPFD5408_Write8(ILI9341_MEMORYWRITE);
 SPFD5408_RS_One();	
}
//------------------------------------------------------------------------------
//�������� ���� �����
//------------------------------------------------------------------------------
void SPFD5408_OutColor(unsigned short color)
{
 SPFD5408_Write16(color);
}
//------------------------------------------------------------------------------
//������� ���� �������
//------------------------------------------------------------------------------
void SPFD5408_PutPixel(unsigned short x, unsigned short y, unsigned short color)
{
 SPFD5408_SetWindow(x,y,x,y);
 SPFD5408_OutColor(color);
}

//----------------------------------------------------------------------------------------------------
//����� ������� � �������
//----------------------------------------------------------------------------------------------------
void SPFD5408_PutSymbol(long x,long y,char symbol,unsigned short color)
{
 unsigned char s=(unsigned char)(symbol);
 if (s<=32) return;
 s-=32;
 if (s>223) return;
 //������ ������
 for(long ys=0;ys<FONT_HEIGHT;ys++)
 {
  unsigned char byte=Font8x14[s][ys];
  unsigned char mask=128;
  for(long xs=0;xs<FONT_WIDTH;xs++,mask>>=1)
  {   
   if (x+xs<0 || x+xs>=SPFD5408_WIDTH) continue;
   if (y+ys<0 || y+ys>=SPFD5408_HEIGHT) continue;
   if (byte&mask) SPFD5408_PutPixel(x+xs,y+ys,color);
  }
 }	
}
//----------------------------------------------------------------------------------------------------
//����� ������� � �������
//----------------------------------------------------------------------------------------------------
void SPFD5408_PutString(long x,long y,const char *string,unsigned short color)
{
 long s=0;	
 while((*string)!=0)
 {
  SPFD5408_PutSymbol(x,y,*string,color);
	x+=FONT_WIDTH;  
	string++;
	s++;
	if (s>20) break;
 }	
}
//----------------------------------------------------------------------------------------------------
//������� ����� � ������� �������
//----------------------------------------------------------------------------------------------------
void SPFD5408_Print(const char *string,unsigned short color)
{
 if (PrintYPosition+FONT_HEIGHT>=SPFD5408_HEIGHT) SPFD5408_Clear();
 SPFD5408_PutString(0,PrintYPosition,string,color);
 PrintYPosition+=FONT_HEIGHT;	
}
//----------------------------------------------------------------------------------------------------
//�������� ������� �������
//----------------------------------------------------------------------------------------------------
void SPFD5408_Clear(void)
{
 PrintYPosition=0;	
 SPFD5408_SetWindow(0,0,SPFD5408_WIDTH-1,SPFD5408_HEIGHT-1);
 //�������� ������� ������ �����������
 SPFD5408_RS_Zero();
 SPFD5408_Write8(ILI9341_MEMORYWRITE);
 //������� ������ ��������
 SPFD5408_RS_One();
 for(long y=0;y<SPFD5408_HEIGHT;y++)
 {
  for(long x=0;x<SPFD5408_WIDTH;x++)
  {
		SPFD5408_Write16(SPFD5408_BLACK);
  }
 }
}












