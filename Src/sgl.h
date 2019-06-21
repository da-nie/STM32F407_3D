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
  //-���������� ������-------------------------------------------------------
 float ProjectionMatrix[16];//������� ��������������
 float ModelViewMatrix[16];//������� �������������
 float Viewport[4];//������ �������� �����
 int CurrentSelectedMatrix;//������� ��������� �������
 SSGLPoint SSGLPointArray[100];//������ �������� ������
 int PointArrayAmount;//������ ������ � ������
 bool DrawModeActive;//true-���� ��������� ������� Begin
  //-������� ������----------------------------------------------------------
  //-������------------------------------------------------------------------
 public:
  //-����������� ������------------------------------------------------------
  CSGL(void);
  //-���������� ������-------------------------------------------------------
  ~CSGL();
  //-���������� ������-------------------------------------------------------
  //unsigned short ImageMap[160*120];
  //float ZBuffer[160*120];
 
  unsigned short* ImageMap;
  float *ZBuffer;
  int ScreenWidth;
  int ScreenHeight;
  //�����
  unsigned short CurrentColor;//������� ����
  //char CurrentR;//�������
  //char CurrentG;
  //char CurrentB;
  //���������
  bool EnableDepthText;
  //-������� ������----------------------------------------------------------
  bool Create(int screen_width,int screen_height);
  //������� ������ � ���������
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
  //������� ������� �����
  bool Color3i(unsigned char r,unsigned char g,unsigned char b);
  //������� ��������� ����������
  bool Begin(void);
  bool End(void);
  bool Vertex3f(float x,float y,float z);
  bool PutTriangle(SSGLPoint *TVertex);
  bool DrawTriangle(SSGLPoint A,SSGLPoint B,SSGLPoint C);
  //������� ������� �������� ��������
  bool Clear(unsigned int mode);
  //������� ���������/���������� �������
  bool Enable(unsigned int mode);
  bool Disable(unsigned int mode);
  //-������------------------------------------------------------------------
};

#endif

