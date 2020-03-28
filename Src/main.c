#include "main.h"
#include "stm32f4xx_hal.h"
#include "cdisplayhx8347d.h"
//#include "cdisplayspfd5408.h"

#include "csgl.h"
#include <math.h>

void RCC_Init(void);
void GPIO_Init(void);
void SPI_Init(void);

long MODE_Led1_On(long state_pos);//включение светодиода 1
long MODE_Led2_On(long state_pos);//включение светодиода 2
long MODE_Led3_On(long state_pos);//включение светодиода 3
long MODE_Led4_On(long state_pos);//включение светодиода 4
long MODE_Led1_Off(long state_pos);//отключение светодиода 1
long MODE_Led2_Off(long state_pos);//отключение светодиода 2
long MODE_Led3_Off(long state_pos);//отключение светодиода 3
long MODE_Led4_Off(long state_pos);//отключение светодиода 4

//----------------------------------------------------------------------------------------------------
//локальный обработчик ошибок
//----------------------------------------------------------------------------------------------------
void ErrorHandler(void)
{
 while(1) 
 {
 }
}
//----------------------------------------------------------------------------------------------------
//глобальный обработчик ошибок
//----------------------------------------------------------------------------------------------------
void _Error_Handler(char *file,int line)
{
 ErrorHandler();
}

//----------------------------------------------------------------------------------------------------
//инициализация GPIO
//----------------------------------------------------------------------------------------------------
void GPIO_Init(void)
{
 //включаем тактирование портов  
 __HAL_RCC_GPIOC_CLK_ENABLE();
 __HAL_RCC_GPIOH_CLK_ENABLE();
 __HAL_RCC_GPIOA_CLK_ENABLE();
 __HAL_RCC_GPIOD_CLK_ENABLE();
  	
 //GPIO_Pin – номера выводов, которые конфигурируются. Пример для нескольких выводов:
 //GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
 //GPIO_Speed – задает скорость для выбранных выводов.
 //GPIO_Mode – задает режим работы выводов. Может принимать следующие значения:

 //настраиваем параметры порта D
 GPIO_InitTypeDef LED_GPIO_Init;
 LED_GPIO_Init.Mode=GPIO_MODE_OUTPUT_PP;
 LED_GPIO_Init.Pull=GPIO_NOPULL;
 LED_GPIO_Init.Pin=GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
 LED_GPIO_Init.Speed=GPIO_SPEED_HIGH;
 HAL_GPIO_Init(GPIOD,&LED_GPIO_Init);
}

//----------------------------------------------------------------------------------------------------
//инициализация SPI
//----------------------------------------------------------------------------------------------------
void SPI_Init(void)
{
 SPI_HandleTypeDef hspi1;  
  
 hspi1.Instance=SPI1;
 hspi1.Init.Mode=SPI_MODE_MASTER;
 hspi1.Init.Direction=SPI_DIRECTION_2LINES;
 hspi1.Init.DataSize=SPI_DATASIZE_8BIT;
 hspi1.Init.CLKPolarity=SPI_POLARITY_LOW;
 hspi1.Init.CLKPhase=SPI_PHASE_1EDGE;
 hspi1.Init.NSS=SPI_NSS_SOFT;
 hspi1.Init.BaudRatePrescaler=SPI_BAUDRATEPRESCALER_2;
 hspi1.Init.FirstBit=SPI_FIRSTBIT_MSB;
 hspi1.Init.TIMode=SPI_TIMODE_DISABLE;
 hspi1.Init.CRCCalculation=SPI_CRCCALCULATION_DISABLE;
 hspi1.Init.CRCPolynomial=10;
 if (HAL_SPI_Init(&hspi1)!=HAL_OK) ErrorHandler();
}

//----------------------------------------------------------------------------------------------------
//инициализация тактового генератора
//----------------------------------------------------------------------------------------------------
void RCC_Init(void)
{
 RCC_OscInitTypeDef RCC_OscInitStruct;
 RCC_ClkInitTypeDef RCC_ClkInitStruct;
 __HAL_RCC_PWR_CLK_ENABLE();
 __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  
 RCC_OscInitStruct.OscillatorType=RCC_OSCILLATORTYPE_HSE;
 RCC_OscInitStruct.HSEState=RCC_HSE_ON;
 RCC_OscInitStruct.PLL.PLLState=RCC_PLL_ON;
 RCC_OscInitStruct.PLL.PLLSource=RCC_PLLSOURCE_HSE;
 RCC_OscInitStruct.PLL.PLLM=4;
 RCC_OscInitStruct.PLL.PLLN=168;
 RCC_OscInitStruct.PLL.PLLP=RCC_PLLP_DIV2;
 RCC_OscInitStruct.PLL.PLLQ=7;
 if (HAL_RCC_OscConfig(&RCC_OscInitStruct)!=HAL_OK) ErrorHandler();
 
 RCC_ClkInitStruct.ClockType=RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
 RCC_ClkInitStruct.SYSCLKSource=RCC_SYSCLKSOURCE_PLLCLK;
 RCC_ClkInitStruct.AHBCLKDivider=RCC_SYSCLK_DIV1;
 RCC_ClkInitStruct.APB1CLKDivider=RCC_HCLK_DIV4;
 RCC_ClkInitStruct.APB2CLKDivider=RCC_HCLK_DIV2;

 if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct,FLASH_LATENCY_5)!=HAL_OK) ErrorHandler();
 HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
 HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
 HAL_NVIC_SetPriority(SysTick_IRQn,0,0);
}

//----------------------------------------------------------------------------------------------------
//включение светодиода 1
//----------------------------------------------------------------------------------------------------
long MODE_Led1_On(long state_pos)
{
 HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_SET);//включаем светодиод
 return(state_pos+1);
}
//----------------------------------------------------------------------------------------------------
//включение светодиода 2
//----------------------------------------------------------------------------------------------------
long MODE_Led2_On(long state_pos)
{
 HAL_GPIO_WritePin(GPIOD,GPIO_PIN_13,GPIO_PIN_SET);//включаем светодиод
 return(state_pos+1);
}
//----------------------------------------------------------------------------------------------------
//включение светодиода 3
//----------------------------------------------------------------------------------------------------
long MODE_Led3_On(long state_pos)
{
 HAL_GPIO_WritePin(GPIOD,GPIO_PIN_14,GPIO_PIN_SET);//включаем светодиод
 return(state_pos+1);
}
//----------------------------------------------------------------------------------------------------
//включение светодиода 4
//----------------------------------------------------------------------------------------------------
long MODE_Led4_On(long state_pos)
{
 HAL_GPIO_WritePin(GPIOD,GPIO_PIN_15,GPIO_PIN_SET);//включаем светодиод
 return(state_pos+1);
}

//----------------------------------------------------------------------------------------------------
//отключение светодиода 1
//----------------------------------------------------------------------------------------------------
long MODE_Led1_Off(long state_pos)
{
 HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_RESET);//включаем светодиод
 return(state_pos+1);
}
//----------------------------------------------------------------------------------------------------
//отключение светодиода 2
//----------------------------------------------------------------------------------------------------
long MODE_Led2_Off(long state_pos)
{
 HAL_GPIO_WritePin(GPIOD,GPIO_PIN_13,GPIO_PIN_RESET);//включаем светодиод
 return(state_pos+1);
}
//----------------------------------------------------------------------------------------------------
//отключение светодиода 3
//----------------------------------------------------------------------------------------------------
long MODE_Led3_Off(long state_pos)
{
 HAL_GPIO_WritePin(GPIOD,GPIO_PIN_14,GPIO_PIN_RESET);//включаем светодиод
 return(state_pos+1);
}
//----------------------------------------------------------------------------------------------------
//отключение светодиода 4
//----------------------------------------------------------------------------------------------------
long MODE_Led4_Off(long state_pos)
{
 HAL_GPIO_WritePin(GPIOD,GPIO_PIN_15,GPIO_PIN_RESET);//включаем светодиод
 return(state_pos+1);
}

void VectorProduct(float *xv1,float *yv1,float *zv1,float xv2,float yv2,float zv2)
{
 float x1=*xv1;
 float y1=*yv1;
 float z1=*zv1;
 float x2=xv2;
 float y2=yv2;
 float z2=zv2;
 float xn=y1*z2-z1*y2;
 float yn=-(x1*z2-z1*x2);
 float zn=x1*y2-y1*x2;
 float len=(float)sqrt(xn*xn+yn*yn+zn*zn);
 if (len!=0)
 {
  xn=xn/len;
  yn=yn/len;
  zn=zn/len;
 }
 *xv1=xn;
 *yv1=yn;
 *zv1=zn;
}
void Octahedron(CSGL &cSGL,float height)
{
 float a=height/2;
 float x1=0;
 float z1=0;
 float y1=0;
 float x2=0;
 float z2=0;
 float y2=height;
 float x3=a;
 float z3=0;
 float y3=a;
 float x4=-a;
 float z4=0;
 float y4=a;
 float x5=0;
 float z5=a;
 float y5=a;
 float x6=0;
 float z6=-a;
 float y6=a;
 float nx,ny,nz;
  nx=x3-x1;
  ny=y3-y1;
  nz=z3-z1;
  VectorProduct(&nx,&ny,&nz,x5-x1,y5-y1,z5-z1);
  cSGL.Color3i(255,255,255);
  cSGL.Begin();
   cSGL.TexCoord(0,0);
   cSGL.Vertex3f(x3,y3,z3);
	 cSGL.Color3i(255,0,255);
	 cSGL.TexCoord(1,0);
   cSGL.Vertex3f(x5,y5,z5);
	 cSGL.Color3i(0,255,0);
	 cSGL.TexCoord(0,1);
   cSGL.Vertex3f(x1,y1,z1);
  cSGL.End();
  //========================================
  nx=x5-x1;
  ny=y5-y1;
  nz=z5-z1;
  VectorProduct(&nx,&ny,&nz,x4-x1,y4-y1,z4-z1);
  cSGL.Color3i(255,255,0);
  cSGL.Begin();
   cSGL.TexCoord(0,0);	
   cSGL.Vertex3f(x5,y5,z5);
	 cSGL.TexCoord(0,1);
   cSGL.Vertex3f(x4,y4,z4);
   cSGL.TexCoord(1,0);	 
   cSGL.Vertex3f(x1,y1,z1);
  cSGL.End();
  //========================================
  nx=x4-x1;
  ny=y4-y1;
  nz=z4-z1;
  VectorProduct(&nx,&ny,&nz,x6-x1,y6-y1,z6-z1);
  cSGL.Color3i(255,0,255);
  cSGL.Begin();
   cSGL.TexCoord(0,0);	
   cSGL.Vertex3f(x4,y4,z4);
   cSGL.TexCoord(1,1);	 
   cSGL.Vertex3f(x6,y6,z6);
   cSGL.TexCoord(0,1);	 
   cSGL.Vertex3f(x1,y1,z1);
  cSGL.End();
  //========================================
  nx=x6-x1;
  ny=y6-y1;
  nz=z6-z1;
  VectorProduct(&nx,&ny,&nz,x3-x1,y3-y1,z3-z1);
  cSGL.Color3i(0,255,255);
  /*cSGL.Begin();
   cSGL.Vertex3f(x6,y6,z6);
   cSGL.Vertex3f(x3,y3,z3);
   cSGL.Vertex3f(x1,y1,z1);
  cSGL.End();*/
  //========================================
  nx=x4-x2;
  ny=y4-y2;
  nz=z4-z2;
  VectorProduct(&nx,&ny,&nz,x5-x2,y5-y2,z5-z2);
  cSGL.Color3i(255,0,0);
  cSGL.Begin();
   cSGL.TexCoord(1,1);	
   cSGL.Vertex3f(x4,y4,z4);
   cSGL.TexCoord(0,0);	 
   cSGL.Vertex3f(x5,y5,z5);
   cSGL.TexCoord(1,0);	 
   cSGL.Vertex3f(x2,y2,z2);
  cSGL.End();
  //========================================
  nx=x6-x2;
  ny=y6-y2;
  nz=z6-z2;
  VectorProduct(&nx,&ny,&nz,x4-x2,y4-y2,z4-z2);
  cSGL.Color3i(0,255,0);
  cSGL.Begin();
   cSGL.TexCoord(0,0);	
   cSGL.Vertex3f(x6,y6,z6);
   cSGL.TexCoord(0,1);	 
   cSGL.Vertex3f(x4,y4,z4);
   cSGL.TexCoord(1,0);	 
   cSGL.Vertex3f(x2,y2,z2);
  cSGL.End();
  //========================================
  nx=x3-x2;
  ny=y3-y2;
  nz=z3-z2;
  VectorProduct(&nx,&ny,&nz,x6-x2,y6-y2,z6-z2);
  cSGL.Color3i(0,0,255);
  cSGL.Begin();
   cSGL.TexCoord(0,0);	
   cSGL.Vertex3f(x3,y3,z3);
   cSGL.TexCoord(0,1);	 
   cSGL.Vertex3f(x6,y6,z6);
   cSGL.TexCoord(1,1);	 
   cSGL.Vertex3f(x2,y2,z2);
  cSGL.End();
  //========================================
  nx=x5-x2;
  ny=y5-y2;
  nz=z5-z2;
  VectorProduct(&nx,&ny,&nz,x3-x2,y3-y2,z3-z2);
  cSGL.Color3i(127,127,255);
  cSGL.Begin();
   cSGL.TexCoord(0,0);	
   cSGL.Vertex3f(x5,y5,z5);
   cSGL.TexCoord(0,1);	 
   cSGL.Vertex3f(x3,y3,z3);
   cSGL.TexCoord(1,0);	 
   cSGL.Vertex3f(x2,y2,z2);
  cSGL.End();
  //========================================
}

CSGL cSGL;
CDisplayHX8347D cDisplayHX8347D;
//CDisplaySPFD5408 cDisplaySPFD5408;
IDisplay *iDisplay_Ptr=&cDisplayHX8347D;

#include <stdlib.h>

//----------------------------------------------------------------------------------------------------
//главная функция программы
//----------------------------------------------------------------------------------------------------
int main(void)
{ 	
 HAL_Init();
 RCC_Init();
 GPIO_Init();
 SPI_Init();
 iDisplay_Ptr->Init();
 iDisplay_Ptr->Clear(IDisplay::COLOR_BLACK);
	/*
 char str[20];
	
 for(unsigned long size=130000;size>0;size--)
 {
 if (new char[size]==NULL) continue; 	
	sprintf(str,"%i",size);
	SPFD5408_Print(str,SPFD5408_YELLOW);	 
 }
 while(1);
 */
		
 const int width=160;
 const int height=120;	
	
 cSGL.Create(width,height);
 cSGL.Perspective(60,(float)((float)(width)/(float)(height)),1,10000);
 cSGL.SetViewport(0,0,width,height);
 float angle=0;
 float dist=0;
 float d_dist=0.1;
 while (1)
 {	 
  //рисуем	 
  cSGL.Clear(SGL_COLOR_BUFFER_BIT|SGL_DEPTH_BUFFER_BIT);
  uint32_t begin=HAL_GetTick();
  cSGL.Enable(SGL_DEPTH_TEST);
  cSGL.MatrixMode(SGL_MATRIX_MODELVIEW);
  cSGL.LoadIdentity();
  cSGL.Translatef(0,0,-10-dist);
  cSGL.Rotatef(angle,1,1,0);
  cSGL.Translatef(0,-5,0);
  //нарисуем фигуру
  Octahedron(cSGL,10);
	 
	angle+=5.0f;
	dist+=d_dist;
	if (dist<0) d_dist=-d_dist;
	if (dist>5) d_dist=-d_dist;
  if (angle>=360.0f) angle-=360.0f;	 
  //отображаем на экране  	 
	 iDisplay_Ptr->SetWindow(0,0,IDisplay::DISPLAY_WIDTH-1,IDisplay::DISPLAY_HEIGHT-1); 
	uint32_t end=HAL_GetTick();

  uint16_t *raw14_local_ptr=(uint16_t *)cSGL.ImageMap;
  
 for(int16_t y=0;y<(IDisplay::DISPLAY_HEIGHT>>1);y++,raw14_local_ptr++)//0...319
 {
  for(int16_t n=0;n<2;n++)
  {	 
   uint16_t *raw14_color_ptr=raw14_local_ptr+width*(IDisplay::DISPLAY_WIDTH/2-1);
   for(int16_t x=0;x<(IDisplay::DISPLAY_WIDTH>>1);x++,raw14_color_ptr-=width)//0..239
   {
    iDisplay_Ptr->OutColor(*raw14_color_ptr);
    iDisplay_Ptr->OutColor(*raw14_color_ptr);
   }
  }
 }
 
	float frame=0;
  if (end!=begin) frame=1000.0f/(end-begin);
	 
	char str[30];
  sprintf(str,"%.1f",frame);	 	
 
	
 iDisplay_Ptr->PutString(0,0,str,IDisplay::COLOR_YELLOW);
	 /*
  //идем по экрану
  unsigned short *raw_local_ptr=(unsigned short*)cSGL.ImageMap;
	if (raw_local_ptr==NULL)
	{
	 SPFD5408_Print("No memory!",SPFD5408_YELLOW);
	 continue;
	}
	 
  for(int y=0;y<height;y++,raw_local_ptr+=width)
  {
   unsigned short *raw_color_ptr=raw_local_ptr;
   for(int x=0;x<width;x++,raw_color_ptr++)
   {
 	  SPFD5408_Write16(*raw_color_ptr);
   }
  }	 
	*/
 }
}



