#ifndef C_SGL_H
#define C_SGL_H

//****************************************************************************************************
//����� ����������� ������������
//****************************************************************************************************

//****************************************************************************************************
//������������ ����������
//****************************************************************************************************
#include <stdint.h>
#include <stdio.h>
#include "sglmatrix.h"
#include "cglscreencolor.h"

//****************************************************************************************************
//����������������
//****************************************************************************************************

//****************************************************************************************************
//���������
//****************************************************************************************************

//****************************************************************************************************
//��������������� ����������
//****************************************************************************************************

#pragma pack(1)
//���� ����� RGBA � ����� ������
struct SGLRGBAByteColor
{
 uint8_t R;
 uint8_t G;
 uint8_t B;
 uint8_t A;
};
#pragma pack()


//****************************************************************************************************
//����� ����������� ������������
//****************************************************************************************************
class CSGL
{
 public:
  //-������������---------------------------------------------------------------------------------------
  //�������� ������ ������
  enum MATRIX_MODE
  {
   SGL_MATRIX_MODELVIEW,//������� ������� �������������
   SGL_MATRIX_PROJECTION,//������� ������� ��������������
   SGL_MATRIX_TEXTURE//������� ������� ���������������
  };
  //�������� �������
  enum CLEAR_MODE
  {
   SGL_COLOR_BUFFER_BIT=(1<<0),
   SGL_DEPTH_BUFFER_BIT=(1<<1)
  };
  //-���������------------------------------------------------------------------------------------------
  static const uint8_t MAX_COLOR_VALUE=255;//������������ �������� �����
 public:
  //-���������------------------------------------------------------------------------------------------
  //���������� �����
  struct SGLVertex
  {
   float X;
   float Y;
   float Z;
  };
  //������� � �����
  struct SGLNormal
  {
   float Nx;
   float Ny;
   float Nz;
  };
  //���������� ����������
  struct SGLTexture
  {
   float U;
   float V;
  };
  //���� �����
  struct SGLColor
  {
   float R;
   float G;
   float B;
  };
  //����� � ���������, ������, ��������, ������������
  struct SGLNVCTPoint
  {
   SGLTexture sGLTexture;
   SGLColor sGLColor;
   SGLNormal sGLNormal;
   SGLVertex sGLVertex;
  };
  //����� �� ������
  struct SGLScreenPoint
  {
   int32_t X;
   int32_t Y;
  };
  //����� � ��������� � ������
  struct SGLNCTPoint
  {
   SGLTexture sGLTexture;
   SGLColor sGLColor;
  };
 private:
  struct SGLTextureObject
  {
   uint32_t Width;//������
   uint32_t Height;//������
   SGLRGBAByteColor *sGLRGBAByteColor_Ptr;//������ ��������
  };
 private:
  //-���������------------------------------------------------------------------------------------------
  static const uint32_t VERTEX_POINT_ARRAY=3;//������ ������ ������
  static const uint32_t FRUSTRUM_PLANE=6;//���������� ���������� ���������
  static const int32_t MIN_INV_Z_VALUE=0;//����������� �������� 1/Z
 private:
  //-����������-----------------------------------------------------------------------------------------
  SGLMatrix4 ProjectionMatrix;//������� ��������������
  SGLMatrix4 ModelViewMatrix;//������� �������������
  SGLMatrix4 TextureMatrix;//������� ���������������
  SGLVector4 ViewPort;//������ �������� �����
  SGLNVCTPoint sGLNVCTPointArray[VERTEX_POINT_ARRAY];//������ �������� ������
  size_t PointArrayAmount;//������ ������ � ������ �������� ������
  SGLVector4 FrustumPlane[FRUSTRUM_PLANE];//����� ���������� ��������� (������ �������� ����� ��������� ���������: ax+by+cz+d=0)
  SGLNVCTPoint sGLNVCTPoint_Current;//��������� ������� �����
  SGLColor sGLColor_Clear;//���� ������� ����

  SGLMatrix4 *sGLMatrix4_Ptr;//��������� �� �������, � ������� ������������ ������

  bool DrawMode;//������� �� ����� ���������

  SGLTextureObject sGLTextureObject_Current;//������� ��������

  typedef void(CSGL::*draw_line_ptr_t)(int32_t y,int32_t x1,int32_t x2,float z1,float z2,float r1,float r2,float g1,float g2,float b1,float b2,float u1,float u2,float v1,float v2);//��� ��������� �� ������� ��������� �������������� �����

  draw_line_ptr_t DrawLineFunction_Ptr;//��������� �� ������� ��������� �����
 public:
  CGLScreenColor* ImageMap;
  uint32_t ScreenWidth;//������� ������
  uint32_t ScreenHeight;
  float *InvZBuffer;//����� 1/z

 public:
  //-�����������----------------------------------------------------------------------------------------
  CSGL(void);
  //-����������-----------------------------------------------------------------------------------------
  ~CSGL();
 public:
  //-�������� �������-----------------------------------------------------------------------------------
  void Init(uint32_t screen_width,uint32_t screen_height);//����������������

  void LoadIdentity(void);//������� ������� ���������
  void Rotatef(float angle,float nx,float ny,float nz);//�������� ������� ������� �� ������� �������� ������ �������
  void Translatef(float nx,float ny,float nz);//�������� ������� ������� �� ������� ��������
  void MatrixMode(MATRIX_MODE matrix);//������� �������

  void Frustrum(float left,float right,float bottom,float top,float near,float far);//������ ��������� ��������� � ������� �������������
  void SetViewport(float x,float y,float len,float hgt);//������ ������� ����
  void Perspective(float fovy,float aspect,float near,float far);//������ ������� �������������

  void Color3f(float r,float g,float b);//������ ���� �����
  void TexCoordf(float u,float v);//������ ���������� ���������� �����
  void Normal3f(float nx,float ny,float nz);//������ ������� � �����
  void Vertex3f(float x,float y,float z);//������ ���������� �����
  void Begin(void);//������ ���������
  void End(void);//��������� ���������
  void ClearColor(float r,float g,float b);//������ ���� ������� ����
  void Clear(uint32_t mode);//�������� �����
  void BindTexture(uint32_t width,uint32_t height,SGLRGBAByteColor *sGLRGBAByteColor_Ptr_Set);//������ ��������

  //-����������� �������--------------------------------------------------------------------------------
  static void SetVertexCoord(SGLVertex &sGLVertex,float x,float y,float z);//������ ���������� �������
  static void SetNormalCoord(SGLNormal &sGLNormal,float nx,float ny,float nz);//������ ���������� �������
  static void SetTextureCoord(SGLTexture &sGLTexture,float u,float v);//������ ���������� ��������
  static void SetColorValue(SGLColor &sGLColor,float r,float g,float b);//������ ����
 private:
  //-�������� �������-----------------------------------------------------------------------------------
  void CreateFrustrumPlane(void);//��������� ��������� ���������
  void GetIntersectionPlaneAndLine(const SGLNVCTPoint& A,const SGLNVCTPoint& B,SGLNVCTPoint& new_point,float nx,float ny,float nz,float w);//�������� ����� ����������� ������ � ���������
  void Clip(const SGLNVCTPoint *point_array_input,uint16_t point_amount_input,SGLNVCTPoint *point_array_output,uint16_t &point_amount_output,float nx,float ny,float nz,float w);//��������� ��������� ���������

  void OutputTriangle(SGLNVCTPoint A,SGLNVCTPoint B,SGLNVCTPoint C);//������� �����������
  void DrawTriangle(SGLNVCTPoint A,SGLNVCTPoint B,SGLNVCTPoint C);//��������� ������������
  void RenderTriangle(SGLNVCTPoint &a,SGLNVCTPoint &b,SGLNVCTPoint &c,SGLScreenPoint &ap,SGLScreenPoint &bp,SGLScreenPoint &cp);//������������ ������������ �� ������
  //void DrawLine(int32_t y,int32_t x1,int32_t x2,float z1,float z2,float r1,float r2,float g1,float g2,float b1,float b2,float u1,float u2,float v1,float v2);//��������� ���������������� �������������� �����
  void DrawLine(int32_t y,int32_t x1,int32_t x2,float z1,float z2,const SGLNCTPoint &sGLNCTPoint_1,const SGLNCTPoint &sGLNCTPoint_2);//��������� ���������������� �������������� �����
};

#endif
