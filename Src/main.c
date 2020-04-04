//****************************************************************************************************
//подключаемые библиотеки
//****************************************************************************************************
#include "main.h"
#include "stm32f4xx_hal.h"
//#include "cdisplayhx8347d.h"
//#include "cdisplayspfd5408.h"
#include "cdisplayil9325.h"
#include "cdisplaystandardlibrary.h"
#include "csgl.h"

#include <math.h>
#include <stdlib.h>

//****************************************************************************************************
//макроопределения
//****************************************************************************************************

//****************************************************************************************************
//константы
//****************************************************************************************************

//****************************************************************************************************
//глобальные переменные
//****************************************************************************************************
CSGL cSGL;//графическая библиотека
//CDisplayHX8347D cDisplay;
//CDisplaySPFD5408 cDisplay;
CDisplayIL9325 cDisplay;//дисплей
IDisplay *iDisplay_Ptr=&cDisplay;//указатель на дисплей
CDisplayStandardLibrary cDisplayStandardLibrary(iDisplay_Ptr,false);//стандартная библиотека дисплея


//****************************************************************************************************
//прототипы функций
//****************************************************************************************************

void VectorProduct(float *xv1,float *yv1,float *zv1,float xv2,float yv2,float zv2);//векторное произведение с нормировкой
void Octahedron(CSGL &cSGL,float height);//рисование октаэдра
void CreateTexture(void);//создание текстуры
void RCC_Init(void);//инициализация RCC
void ErrorHandler(void);//локальный обработчик ошибок
void _Error_Handler(char *file,int line);//глобальный обработчик ошибок

//****************************************************************************************************
//реализация функций
//****************************************************************************************************

//----------------------------------------------------------------------------------------------------
//главная функция программы
//----------------------------------------------------------------------------------------------------
int main(void)
{ 	
 HAL_Init();
 RCC_Init();
 iDisplay_Ptr->Init();
 cDisplayStandardLibrary.Clear(IDisplay::COLOR_BLACK);
	
 const int32_t WIDTH=160;
 const int32_t HEIGHT=120;
 const float VISIBLE_ANGLE=60;
 const float NEAR_PLANE=1;
 const float FAR_PLANE=1000;	
 float aspect=static_cast<float>(WIDTH)/static_cast<float>(HEIGHT);
 		
 cSGL.Init(WIDTH,HEIGHT);
 cSGL.Perspective(VISIBLE_ANGLE,aspect,NEAR_PLANE,FAR_PLANE);
 cSGL.SetViewport(0,0,WIDTH,HEIGHT);
 float angle=0;
 float dist=0;
 float d_dist=0.1;	
 //создадим текстуру
 CreateTexture();	
 //создадим источник света 
 cSGL.MatrixMode(CSGL::SGL_MATRIX_MODELVIEW);
 cSGL.LoadIdentity();
 float l0_position[]={0,0,0};
 float l0_ambient[]={0.1,0.1,0.1};
 float l0_diffuse[]={0.7,0.7,0.7};
 float l0_specular[]={1,1,1};
 float l0_shininess[]={1};
 cSGL.Lightfv(CSGL::SGL_LIGHT0,CSGL::SGL_POSITION,l0_position);
 cSGL.Lightfv(CSGL::SGL_LIGHT0,CSGL::SGL_AMBIENT,l0_ambient);
 cSGL.Lightfv(CSGL::SGL_LIGHT0,CSGL::SGL_DIFFUSE,l0_diffuse);
 cSGL.Lightfv(CSGL::SGL_LIGHT0,CSGL::SGL_SPECULAR,l0_specular);
 cSGL.Lightfv(CSGL::SGL_LIGHT0,CSGL::SGL_SHININESS,l0_shininess);
 //зададим материал
 float m0_ambient[]={0.1,0.1,0.1};
 float m0_diffuse[]={0.5,0.5,0.5};
 float m0_specular[]={0.5,0.5,0.5};
 float m0_emission[]={0.1,0.1,0.1};
 cSGL.Materialfv(CSGL::SGL_AMBIENT,m0_ambient);
 cSGL.Materialfv(CSGL::SGL_DIFFUSE,m0_diffuse);
 cSGL.Materialfv(CSGL::SGL_SPECULAR,m0_specular); 
 cSGL.Materialfv(CSGL::SGL_EMISSION,m0_emission); 
 
 //включим расчет освещенности и источники света	 
 cSGL.Enable(CSGL::SGL_LIGHTING);
 cSGL.Enable(CSGL::SGL_LIGHT0); 
 
 while (1)
 {	 
  //рисуем	 
	cSGL.Clear(CSGL::SGL_COLOR_BUFFER_BIT|CSGL::SGL_DEPTH_BUFFER_BIT);	 
 
  uint32_t begin=HAL_GetTick();
  cSGL.MatrixMode(CSGL::SGL_MATRIX_TEXTURE);
  cSGL.LoadIdentity();
  cSGL.Rotatef(angle,0,0,1);
  cSGL.Translatef(-0.5,-0.5,0);
	 
  cSGL.MatrixMode(CSGL::SGL_MATRIX_MODELVIEW);
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
   uint16_t *raw14_color_ptr=raw14_local_ptr+WIDTH*(IDisplay::DISPLAY_WIDTH/2-1);
   for(int16_t x=0;x<(IDisplay::DISPLAY_WIDTH>>1);x++,raw14_color_ptr-=WIDTH)//0..239
   {
    iDisplay_Ptr->OutColor(*raw14_color_ptr);
    iDisplay_Ptr->OutColor(*raw14_color_ptr);
   }
  }
 }
 
 /*  
  //выводим FPS (без учета переброски буфера на дисплей) 
	float frame=0;
  if (end!=begin) frame=1000.0f/(end-begin);	
	char str[30];
  sprintf(str,"%.1f",frame);	 		
  cDisplayStandardLibrary.PutString(0,0,str,IDisplay::COLOR_YELLOW);
 */
 }
}

//----------------------------------------------------------------------------------------------------
//векторное произведение с нормировкой
//----------------------------------------------------------------------------------------------------
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

//----------------------------------------------------------------------------------------------------
//рисование октаэдра
//----------------------------------------------------------------------------------------------------
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
  cSGL.Color3f(1,1,1);
	cSGL.Normal3f(nx,ny,nz);	
  cSGL.Begin();
   cSGL.TexCoordf(0,0);
   cSGL.Vertex3f(x3,y3,z3);
	 cSGL.Color3f(1,0,1);
	 cSGL.TexCoordf(1,0);
   cSGL.Vertex3f(x5,y5,z5);
	 cSGL.Color3f(0,1,0);
	 cSGL.TexCoordf(0,1);
   cSGL.Vertex3f(x1,y1,z1);
  cSGL.End();
  //========================================
  nx=x5-x1;
  ny=y5-y1;
  nz=z5-z1;
  VectorProduct(&nx,&ny,&nz,x4-x1,y4-y1,z4-z1);
	cSGL.Normal3f(nx,ny,nz);	
  cSGL.Color3f(1,1,0);
  cSGL.Begin();
   cSGL.TexCoordf(0,0);	
   cSGL.Vertex3f(x5,y5,z5);
	 cSGL.TexCoordf(0,1);
   cSGL.Vertex3f(x4,y4,z4);
   cSGL.TexCoordf(1,0);	 
   cSGL.Vertex3f(x1,y1,z1);
  cSGL.End();
  //========================================
  nx=x4-x1;
  ny=y4-y1;
  nz=z4-z1;
  VectorProduct(&nx,&ny,&nz,x6-x1,y6-y1,z6-z1);
	cSGL.Normal3f(nx,ny,nz);	
  cSGL.Color3f(1,0,1);
  cSGL.Begin();
   cSGL.TexCoordf(0,0);	
   cSGL.Vertex3f(x4,y4,z4);
   cSGL.TexCoordf(1,1);	 
   cSGL.Vertex3f(x6,y6,z6);
   cSGL.TexCoordf(0,1);	 
   cSGL.Vertex3f(x1,y1,z1);
  cSGL.End();
  //========================================
  nx=x6-x1;
  ny=y6-y1;
  nz=z6-z1;
  VectorProduct(&nx,&ny,&nz,x3-x1,y3-y1,z3-z1);
	cSGL.Normal3f(nx,ny,nz);	
  cSGL.Color3f(0,1,1);
	//откроем одну стенку
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
	cSGL.Normal3f(nx,ny,nz);	
  cSGL.Color3f(1,0,0);
  cSGL.Begin();
   cSGL.TexCoordf(1,1);	
   cSGL.Vertex3f(x4,y4,z4);
   cSGL.TexCoordf(0,0);	 
   cSGL.Vertex3f(x5,y5,z5);
   cSGL.TexCoordf(1,0);	 
   cSGL.Vertex3f(x2,y2,z2);
  cSGL.End();
  //========================================
  nx=x6-x2;
  ny=y6-y2;
  nz=z6-z2;
  VectorProduct(&nx,&ny,&nz,x4-x2,y4-y2,z4-z2);
	cSGL.Normal3f(nx,ny,nz);	
  cSGL.Color3f(0,1,0);
  cSGL.Begin();
   cSGL.TexCoordf(0,0);	
   cSGL.Vertex3f(x6,y6,z6);
   cSGL.TexCoordf(0,1);	 
   cSGL.Vertex3f(x4,y4,z4);
   cSGL.TexCoordf(1,0);	 
   cSGL.Vertex3f(x2,y2,z2);
  cSGL.End();
  //========================================
  nx=x3-x2;
  ny=y3-y2;
  nz=z3-z2;
  VectorProduct(&nx,&ny,&nz,x6-x2,y6-y2,z6-z2);
	cSGL.Normal3f(nx,ny,nz);	
  cSGL.Color3f(0,0,1);
  cSGL.Begin();
   cSGL.TexCoordf(0,0);	
   cSGL.Vertex3f(x3,y3,z3);
   cSGL.TexCoordf(0,1);	 
   cSGL.Vertex3f(x6,y6,z6);
   cSGL.TexCoordf(1,1);	 
   cSGL.Vertex3f(x2,y2,z2);
  cSGL.End();
  //========================================
  nx=x5-x2;
  ny=y5-y2;
  nz=z5-z2;
  VectorProduct(&nx,&ny,&nz,x3-x2,y3-y2,z3-z2);
	cSGL.Normal3f(nx,ny,nz);	
  cSGL.Color3f(0.5,0.5,1);
  cSGL.Begin();
   cSGL.TexCoordf(0,0);	
   cSGL.Vertex3f(x5,y5,z5);
   cSGL.TexCoordf(0,1);	 
   cSGL.Vertex3f(x3,y3,z3);
   cSGL.TexCoordf(1,0);	 
   cSGL.Vertex3f(x2,y2,z2);
  cSGL.End();
  //========================================
}
//----------------------------------------------------------------------------------------------------
//создание текстуры
//----------------------------------------------------------------------------------------------------
void CreateTexture(void)
{
 const uint16_t TESTURE_SIZE_X=8;
 const uint16_t TESTURE_SIZE_Y=8;
 const uint16_t COLOR_SIZE=4; 
	
 static uint8_t texture[TESTURE_SIZE_X*TESTURE_SIZE_Y*COLOR_SIZE];
 int16_t index;
 for(uint32_t x=0;x<TESTURE_SIZE_X;x++)
 {
	for(uint32_t y=0;y<TESTURE_SIZE_Y;y++)
  {
	 uint8_t r=63;
	 uint8_t g=64;
	 uint8_t b=127;
	 if ((x+y)%2)
	 {
		if (index%3==0)  
		{
     r=255;
     g=127;
     b=255;		 
		}
		if (index%3==1)  
		{
     r=255;
     g=127;
     b=127;		 
		}
		if (index%3==2)  
		{
     r=127;
     g=255;
     b=127;		 
		}
    index++;		
	 }
	 uint32_t offset=(x+y*TESTURE_SIZE_X)*COLOR_SIZE;
	 texture[offset+0]=r;
	 texture[offset+1]=g;
	 texture[offset+2]=b;
	 texture[offset+3]=255;
  }	
 }
 cSGL.BindTexture(8,8,reinterpret_cast<SGLRGBAByteColor*>(texture));	 
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

