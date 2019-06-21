#ifndef SGL_H
#define SGL_H

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

#define SGL_MATRIX_PROJECTION 100
#define SGL_MATRIX_MODELVIEW  101

#define SGL_COLOR_BUFFER_BIT  1
#define SGL_DEPTH_BUFFER_BIT  2 

#define SGL_DEPTH_TEST		  1

struct SSGLPoint
{
 float X;
 float Y;
 float Z;

 unsigned short Color;	
};

class CSGL
{
 protected:
  //-Переменные класса-------------------------------------------------------
 float ProjectionMatrix[16];//матрица проектирования
 float ModelViewMatrix[16];//матрица моделирования
 float Viewport[4];//вектор видового порта
 int CurrentSelectedMatrix;//текущая выбранная матрица
 SSGLPoint SSGLPointArray[100];//список хранимых вершин
 int PointArrayAmount;//размер данных в списке
 bool DrawModeActive;//true-была выполнена команда Begin
  //-Функции класса----------------------------------------------------------
  //-Прочее------------------------------------------------------------------
 public:
  //-Конструктор класса------------------------------------------------------
  CSGL(void);
  //-Деструктор класса-------------------------------------------------------
  ~CSGL();
  //-Переменные класса-------------------------------------------------------
  //unsigned short ImageMap[160*120];
  //float ZBuffer[160*120];
 
  unsigned short* ImageMap;
  float *ZBuffer;
  int ScreenWidth;
  int ScreenHeight;
  //цвета
  unsigned short CurrentColor;//текущий цвет
  //char CurrentR;//текущий
  //char CurrentG;
  //char CurrentB;
  //параметры
  bool EnableDepthText;
  //-Функции класса----------------------------------------------------------
  bool Create(int screen_width,int screen_height);
  //функции работы с матрицами
  bool MatrixProductfv(float* Matrix1,float* Matrix2,float* Matrix3);
  bool VectorProductMatrixfv(float *Vector,float* Matrix,float *ReturnVector);
  bool MatrixProductVectorfv(float* Matrix,float *Vector,float *ReturnVector);
  bool LoadIdentity(void);
  bool Frustrum(float Left,float Right,float Bottom,float Top,float Near,float Far);
  bool SetViewport(float x,float y,float len,float hgt);
  bool Perspective(float Fovy,float Aspect,float Near,float Far);
  bool Rotatef(float angle,float nx,float ny,float nz);
  bool Translatef(float nx,float ny,float nz);
  bool MatrixMode(int matrix);
  //функции задания цвета
  bool Color3i(unsigned char r,unsigned char g,unsigned char b);
  //функции рисования примитивов
  bool Begin(void);
  bool End(void);
  bool Vertex3f(float x,float y,float z);
  bool PutTriangle(SSGLPoint *TVertex);
  bool DrawTriangle(SSGLPoint A,SSGLPoint B,SSGLPoint C);
  //функции очистки экранных массивов
  bool Clear(unsigned int mode);
  //функции включения/выключения режимов
  bool Enable(unsigned int mode);
  bool Disable(unsigned int mode);
  //-Прочее------------------------------------------------------------------
};

#endif

