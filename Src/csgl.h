#ifndef SGL_H
#define SGL_H

#include <stdint.h>

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

#pragma pack(1)
//���������� �����
struct SGuVertex
{
 float x;
 float y;
 float z;
};
//������� � �����
struct SGuNormal
{
 float nx;
 float ny;
 float nz;
};
//���������� ����������
struct SGuTexture
{
 float u;
 float v;
};
//���� �����
struct SGuColor
{ 
 uint8_t r;
 uint8_t g;	
 uint8_t b;	
};
//���� ����� ��������� ������
struct SGuScreenColor
{
 uint16_t Color;	
};
//����� � ���������, ������, ��������, ������������
struct SGuNVCTPoint
{
 SGuTexture sGuTexture;
 SGuColor sGuColor;
 SGuNormal sGuNormal;
 SGuVertex sGuVertex;
};
//������ 4
struct SGuVector4
{
 float x;
 float y;
 float z;
 float w;
};
//������ 3
struct SGuVector3
{
 float x;
 float y;
 float z;
};
//������� 4x4
struct SGuMatrix4
{
 SGuVector4 x;
 SGuVector4	y;
 SGuVector4 z;
 SGuVector4 w;
};
//������� 3x3
struct SGuMatrix3
{
 SGuVector3 x;
 SGuVector3	y;
 SGuVector3 z;
};

#pragma pack()



#define SGL_MATRIX_PROJECTION 100
#define SGL_MATRIX_MODELVIEW  101

#define SGL_COLOR_BUFFER_BIT  1
#define SGL_DEPTH_BUFFER_BIT  2 

#define SGL_DEPTH_TEST		  1

class CSGL
{	
 protected:
  enum MATRIX_MODE
  {
   MATRIX_MODE_MODEL_VIEW,
   MATRIX_MODE_PROJECTION		
  }; 
  //-���������� ������-------------------------------------------------------
  SGuMatrix4 ProjectionMatrix;//������� ��������������
  SGuMatrix4 ModelViewMatrix;//������� �������������
  SGuVector4 ViewPort;//������ �������� �����
	
  SGuNVCTPoint sGuNVCTPointArray[3];//������ �������� ������
  uint16_t PointArrayAmount;//������ ������ � ������
  MATRIX_MODE CurrentSelectedMatrix;//������� ��������� �������
  bool DrawModeActive;//true-���� ��������� ������� Begin
  SGuVector4 FrustumPlane[4];//������ ��������� ��������� (������ �������� ����� ��������� ���������: ax+by+cz+d=0)
  //-������� ������----------------------------------------------------------
  //-������------------------------------------------------------------------
 public:
  //-����������� ������------------------------------------------------------
  CSGL(void);
  //-���������� ������-------------------------------------------------------
  ~CSGL();
  //-���������� ������-------------------------------------------------------
  SGuScreenColor* ImageMap;
  float *ZBuffer;
  int ScreenWidth;
  int ScreenHeight;
  //�����
  SGuColor CurrentColor;//������� ����
  //char CurrentR;//�������
  //char CurrentG;
  //char CurrentB;
  //���������
  bool EnableDepthText;
  //-������� ������----------------------------------------------------------
  bool Create(int screen_width,int screen_height);
  //������� ������ � ���������
  bool LoadIdentity(void);
  bool Frustrum(float left,float right,float bottom,float top,float near,float far);
  bool SetViewport(float x,float y,float len,float hgt);
  bool Perspective(float fovy,float aspect,float near,float far);
  bool Rotatef(float angle,float nx,float ny,float nz);
  bool Translatef(float nx,float ny,float nz);
  bool MatrixMode(int matrix);
  //������� ������� �����
  bool Color3i(unsigned char r,unsigned char g,unsigned char b);
  //������� ��������� ����������
  bool Begin(void);
  bool End(void);
  bool Vertex3f(float x,float y,float z);
  //������� ������� �������� ��������
  bool Clear(unsigned int mode);
  //������� ���������/���������� �������
  bool Enable(unsigned int mode);
  bool Disable(unsigned int mode);	
	
  void SetVertexCoord(SGuVertex &sGuVertex,float x,float y,float z);//������ ���������� �������
  void SetNormalCoord(SGuNormal &sGuNormal,float nx,float ny,float nz);//������ ���������� �������
  void SetTextureCoord(SGuTexture &sGuTexture,float u,float v);//������ ���������� ��������
  void SetColorValue(SGuColor &sGuColor,uint8_t r,uint8_t g,uint8_t b);//������ ����
  void MultiplySGuVector4ToSGuMatrix4(const SGuVector4& v,const SGuMatrix4& m,SGuVector4& v_out);//��������� ������� ���� SGuVector4 �� ������� ���� SGuMatrix4
  void MultiplySGuMatrix4ToSGuVector4(const SGuMatrix4& m,const SGuVector4& v,SGuVector4& v_out);//��������� ������� ���� SGuMatrix4 �� ������ ���� SGuVector4
  void MultiplySGuMatrix4(const SGuMatrix4& a,const SGuMatrix4& b,SGuMatrix4& out);//��������� ���� ������ ���� SGuMatrix4
  void NormaliseSGuVector4(SGuVector4& v);//������������ ������� ���� SGuMatrix4
  double GetDeterminantSGuMatrix4(const SGuMatrix4& matrix);//��������� ������������ ������� ���� SGuMatrix4
  double GetDeterminantSGuMatrix3(const SGuMatrix3& matrix);//��������� ������������ ������� ���� SGuMatrix3
  void GetTruncatedMatrixSGuMatrix4(long y,long x,const SGuMatrix4& input_matrix,SGuMatrix3& output_matrix);//��������� ������� � ����������� ������� � �������� �� ����������� y � x ��� ������� ���� SGuMatrix4	
  bool CreateInvertMatrixSGuMatrix4(const SGuMatrix4& input_matrix,SGuMatrix4& output_matrix);//��������� �������� ������� ��� ������� ���� SGuMatrix4
  void CreateFrustrumPlane(void);//��������� ��������� ���������	
  void GetIntersectionPlaneAndLine(const SGuNVCTPoint& A,const SGuNVCTPoint& B,SGuNVCTPoint& new_point,float nx,float ny,float nz,float w);//�������� ����� ����������� ������ � ���������
  void Clip(const SGuNVCTPoint *point_array_input,uint16_t point_amount_input,SGuNVCTPoint *point_array_output,uint16_t &point_amount_output,float nx,float ny,float nz,float w);//��������� ��������� ���������  


  void OutputTriangle(SGuNVCTPoint A,SGuNVCTPoint B,SGuNVCTPoint C);//������� �����������
  bool DrawTriangle(SGuNVCTPoint A,SGuNVCTPoint B,SGuNVCTPoint C);


	//-������------------------------------------------------------------------
};

#endif

