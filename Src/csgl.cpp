//****************************************************************************************************
//������������ ����������
//****************************************************************************************************
#include "csgl.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

//****************************************************************************************************
//���������� ����������
//****************************************************************************************************

//****************************************************************************************************
//���������
//****************************************************************************************************
static const float PI=3.1415926535897932384626433832795f;

//****************************************************************************************************
//����������������
//****************************************************************************************************

//****************************************************************************************************
//����������� � ����������
//****************************************************************************************************

//----------------------------------------------------------------------------------------------------
//�����������
//----------------------------------------------------------------------------------------------------
CSGL::CSGL(void)
{
 MatrixMode(SGL_MATRIX_TEXTURE);
 LoadIdentity();
 MatrixMode(SGL_MATRIX_PROJECTION);
 LoadIdentity();
 MatrixMode(SGL_MATRIX_MODELVIEW);
 LoadIdentity();
 ClearColor(0,0,0);
 DrawMode=false;
 PointArrayAmount=0;
 Color3f(1,1,1);
 Normal3f(1,0,0);
 TexCoordf(0,0);

 ScreenWidth=0;
 ScreenHeight=0;
 ImageMap=NULL;
 InvZBuffer=NULL;

 BindTexture(0,0,NULL);

 //DrawLineFunction_Ptr=&CSGL::DrawLine;
}
//----------------------------------------------------------------------------------------------------
//����������
//----------------------------------------------------------------------------------------------------
CSGL::~CSGL()
{
 if (ImageMap!=NULL) delete[](ImageMap);
 if (InvZBuffer!=NULL) delete[](InvZBuffer);
}

//****************************************************************************************************
//�������� �������
//****************************************************************************************************

//----------------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
//��������� ��������� ���������
//----------------------------------------------------------------------------------------------------
void CSGL::CreateFrustrumPlane(void)
{
 //��������� ����� ������� modelview-projection
 //��������!!!
 //��� ��� ���������� � ������ (Vertex3f) ��� ���������� � ������ ������� �������������, ������� ��������� ����� ������ �� ������� �������������!
 //��������!!!

 SGLMatrix4 projection_model_view_matrix=ProjectionMatrix;
 //��������� ������ ��������� ��������� �� �������� (����, ���, ����, �����)
 //�����
 FrustumPlane[0].X=projection_model_view_matrix.X.W+projection_model_view_matrix.X.X;
 FrustumPlane[0].Y=projection_model_view_matrix.Y.W+projection_model_view_matrix.Y.X;
 FrustumPlane[0].Z=projection_model_view_matrix.Z.W+projection_model_view_matrix.Z.X;
 FrustumPlane[0].W=projection_model_view_matrix.W.W+projection_model_view_matrix.W.X;
 NormaliseSGLVector4(FrustumPlane[0]);
 //������
 FrustumPlane[1].X=projection_model_view_matrix.X.W-projection_model_view_matrix.X.X;
 FrustumPlane[1].Y=projection_model_view_matrix.Y.W-projection_model_view_matrix.Y.X;
 FrustumPlane[1].Z=projection_model_view_matrix.Z.W-projection_model_view_matrix.Z.X;
 FrustumPlane[1].W=projection_model_view_matrix.W.W-projection_model_view_matrix.W.X;
 NormaliseSGLVector4(FrustumPlane[1]);
 //�������
 FrustumPlane[2].X=projection_model_view_matrix.X.W-projection_model_view_matrix.X.Y;
 FrustumPlane[2].Y=projection_model_view_matrix.Y.W-projection_model_view_matrix.Y.Y;
 FrustumPlane[2].Z=projection_model_view_matrix.Z.W-projection_model_view_matrix.Z.Y;
 FrustumPlane[2].W=projection_model_view_matrix.W.W-projection_model_view_matrix.W.Y;
 NormaliseSGLVector4(FrustumPlane[2]);
 //������
 FrustumPlane[3].X=projection_model_view_matrix.X.W+projection_model_view_matrix.X.Y;
 FrustumPlane[3].Y=projection_model_view_matrix.Y.W+projection_model_view_matrix.Y.Y;
 FrustumPlane[3].Z=projection_model_view_matrix.Z.W+projection_model_view_matrix.Z.Y;
 FrustumPlane[3].W=projection_model_view_matrix.W.W+projection_model_view_matrix.W.Y;
 NormaliseSGLVector4(FrustumPlane[3]);
 //��������
 FrustumPlane[4].X=projection_model_view_matrix.X.W+projection_model_view_matrix.X.Z;
 FrustumPlane[4].Y=projection_model_view_matrix.Y.W+projection_model_view_matrix.Y.Z;
 FrustumPlane[4].Z=projection_model_view_matrix.Z.W+projection_model_view_matrix.Z.Z;
 FrustumPlane[4].W=projection_model_view_matrix.W.W+projection_model_view_matrix.W.Z;
 NormaliseSGLVector4(FrustumPlane[4]);

 //������
 FrustumPlane[5].X=projection_model_view_matrix.X.W-projection_model_view_matrix.X.Z;
 FrustumPlane[5].Y=projection_model_view_matrix.Y.W-projection_model_view_matrix.Y.Z;
 FrustumPlane[5].Z=projection_model_view_matrix.Z.W-projection_model_view_matrix.Z.Z;
 FrustumPlane[5].W=projection_model_view_matrix.W.W-projection_model_view_matrix.W.Z;
 NormaliseSGLVector4(FrustumPlane[5]);

}
//----------------------------------------------------------------------------------------------------
//�������� ����� ����������� ������ � ���������
//----------------------------------------------------------------------------------------------------
void CSGL::GetIntersectionPlaneAndLine(const SGLNVCTPoint& A,const SGLNVCTPoint& B,SGLNVCTPoint& new_point,float nx,float ny,float nz,float w)
{
 new_point=A;
 float ax=A.sGLVertex.X;
 float ay=A.sGLVertex.Y;
 float az=A.sGLVertex.Z;
 float au=A.sGLTexture.U;
 float av=A.sGLTexture.V;
 float ar=A.sGLColor.R;
 float ag=A.sGLColor.G;
 float ab=A.sGLColor.B;

 float bx=B.sGLVertex.X;
 float by=B.sGLVertex.Y;
 float bz=B.sGLVertex.Z;
 float bu=B.sGLTexture.U;
 float bv=B.sGLTexture.V;
 float br=B.sGLColor.R;
 float bg=B.sGLColor.G;
 float bb=B.sGLColor.B;

 float dx=bx-ax;
 float dy=by-ay;
 float dz=bz-az;
 float du=bu-au;
 float dv=bv-av;
 float dr=br-ar;
 float dg=bg-ag;
 float db=bb-ab;

 float top=(nx*ax)+(ny*ay)+(nz*az)+w;
 float bottom=(nx*dx)+(ny*dy)+(nz*dz);
 float time=-top/bottom;

 float vx=ax+time*dx;
 float vy=ay+time*dy;
 float vz=az+time*dz;
 float vu=au+time*du;
 float vv=av+time*dv;
 float r=ar+time*dr;
 float g=ag+time*dg;
 float b=ab+time*db;
 //��������� ����� �����
 SetVertexCoord(new_point.sGLVertex,vx,vy,vz);
 SetTextureCoord(new_point.sGLTexture,vu,vv);
 SetColorValue(new_point.sGLColor,(uint8_t)r,(uint8_t)g,(uint8_t)b);
}

//----------------------------------------------------------------------------------------------------
//��������� ��������� ���������
//----------------------------------------------------------------------------------------------------
void CSGL::Clip(const SGLNVCTPoint *point_array_input,uint16_t point_amount_input,SGLNVCTPoint *point_array_output,uint16_t &point_amount_output,float nx,float ny,float nz,float w)
{
 point_amount_output=0;
 for(uint16_t n=0;n<point_amount_input;n++)
 {
  long next_p=n+1;
  if (next_p>=point_amount_input) next_p-=point_amount_input;

  const SGLNVCTPoint *sGLNVCTPoint_Current_Ptr=&(point_array_input[n]);
  float current_vx=sGLNVCTPoint_Current_Ptr->sGLVertex.X;
  float current_vy=sGLNVCTPoint_Current_Ptr->sGLVertex.Y;
  float current_vz=sGLNVCTPoint_Current_Ptr->sGLVertex.Z;
  //���������� ��������� ������������ ��������� ���������
  float current_ret=current_vx*nx+current_vy*ny+current_vz*nz+w;

  const SGLNVCTPoint *sGLNVCTPoint_Next_Ptr=&(point_array_input[next_p]);
  float next_vx=sGLNVCTPoint_Next_Ptr->sGLVertex.X;
  float next_vy=sGLNVCTPoint_Next_Ptr->sGLVertex.Y;
  float next_vz=sGLNVCTPoint_Next_Ptr->sGLVertex.Z;
  //���������� ��������� ������������ ��������� ���������
  float next_ret=next_vx*nx+next_vy*ny+next_vz*nz+w;

  if (current_ret>0)//������� ����� ������
  {
   if (next_ret>0)//��������� ����� ������
   {
    point_array_output[point_amount_output]=*sGLNVCTPoint_Next_Ptr;
    point_amount_output++;
   }
   else
   {
    //��������� ����� ����� �����������
    SGLNVCTPoint sGLNVCTPoint_New;
    GetIntersectionPlaneAndLine(*sGLNVCTPoint_Current_Ptr,*sGLNVCTPoint_Next_Ptr,sGLNVCTPoint_New,nx,ny,nz,w);
    point_array_output[point_amount_output]=sGLNVCTPoint_New;
    point_amount_output++;
   }
  }
  else//������� ����� �� �����
  {
   if (next_ret>0)//��������� ����� �����
   {
    //��������� ����� ����� �����������
    SGLNVCTPoint sGLNVCTPoint_New;
    GetIntersectionPlaneAndLine(*sGLNVCTPoint_Current_Ptr,*sGLNVCTPoint_Next_Ptr,sGLNVCTPoint_New,nx,ny,nz,w);
    point_array_output[point_amount_output]=sGLNVCTPoint_New;
    point_amount_output++;
    //��������� ��������� �����
    point_array_output[point_amount_output]=*sGLNVCTPoint_Next_Ptr;
    point_amount_output++;
   }
  }
 }
}

//----------------------------------------------------------------------------------------------------
//������� �����������
//----------------------------------------------------------------------------------------------------
void CSGL::OutputTriangle(SGLNVCTPoint A,SGLNVCTPoint B,SGLNVCTPoint C)
{
 //��������� ��������� ��������� ��� ���� �������������� ����������
 SGLNVCTPoint sGLNVCTPoint[2][3*2*4];//������ �������� ������

 sGLNVCTPoint[0][0]=A;
 sGLNVCTPoint[0][1]=B;
 sGLNVCTPoint[0][2]=C;

 uint16_t point_amount=3;
 uint8_t input_index=0;
 uint8_t output_index=1;

 for(uint8_t n=0;n<FRUSTRUM_PLANE;n++)
 {
  float nx=FrustumPlane[n].X;
  float ny=FrustumPlane[n].Y;
  float nz=FrustumPlane[n].Z;
  float w=FrustumPlane[n].W;
  Clip(sGLNVCTPoint[input_index],point_amount,sGLNVCTPoint[output_index],point_amount,nx,ny,nz,w);
  //������������ ������ �����
  uint8_t tmp=input_index;
  input_index=output_index;
  output_index=tmp;
 }
 if (point_amount<3) return;//������ ��������
 //��������� ��������� �������������
 for(uint8_t n=1;n<point_amount-1;n++)
 {
  uint16_t i1=n;
  uint16_t i2=n+1;
  DrawTriangle(sGLNVCTPoint[input_index][0],sGLNVCTPoint[input_index][i1],sGLNVCTPoint[input_index][i2]);
 }
}
//----------------------------------------------------------------------------------------------------
//��������� ������������
//----------------------------------------------------------------------------------------------------
void CSGL::DrawTriangle(SGLNVCTPoint A,SGLNVCTPoint B,SGLNVCTPoint C)
{
 SGLVector4 sGLVector4_A;
 SGLVector4 sGLVector4_B;
 SGLVector4 sGLVector4_C;

 //�������� �������� ������ ������������
 sGLVector4_A.X=A.sGLVertex.X;
 sGLVector4_A.Y=A.sGLVertex.Y;
 sGLVector4_A.Z=A.sGLVertex.Z;
 sGLVector4_A.W=1;

 sGLVector4_B.X=B.sGLVertex.X;
 sGLVector4_B.Y=B.sGLVertex.Y;
 sGLVector4_B.Z=B.sGLVertex.Z;
 sGLVector4_B.W=1;

 sGLVector4_C.X=C.sGLVertex.X;
 sGLVector4_C.Y=C.sGLVertex.Y;
 sGLVector4_C.Z=C.sGLVertex.Z;
 sGLVector4_C.W=1;

 SGLVector4 sGLVector4_A_New;
 SGLVector4 sGLVector4_B_New;
 SGLVector4 sGLVector4_C_New;

 MultiplySGLMatrix4ToSGLVector4(ProjectionMatrix,sGLVector4_A,sGLVector4_A_New);
 MultiplySGLMatrix4ToSGLVector4(ProjectionMatrix,sGLVector4_B,sGLVector4_B_New);
 MultiplySGLMatrix4ToSGLVector4(ProjectionMatrix,sGLVector4_C,sGLVector4_C_New);

 SGLScreenPoint ap;
 SGLScreenPoint bp;
 SGLScreenPoint cp;

 ap.X=(int32_t)((sGLVector4_A_New.X/sGLVector4_A_New.W+1)*ViewPort.Z/2+ViewPort.X);
 ap.Y=(int32_t)((1-sGLVector4_A_New.Y/sGLVector4_A_New.W)*ViewPort.W/2+ViewPort.Y);

 bp.X=(int32_t)((sGLVector4_B_New.X/sGLVector4_B_New.W+1)*ViewPort.Z/2+ViewPort.X);
 bp.Y=(int32_t)((1-sGLVector4_B_New.Y/sGLVector4_B_New.W)*ViewPort.W/2+ViewPort.Y);

 cp.X=(int32_t)((sGLVector4_C_New.X/sGLVector4_C_New.W+1)*ViewPort.Z/2+ViewPort.X);
 cp.Y=(int32_t)((1-sGLVector4_C_New.Y/sGLVector4_C_New.W)*ViewPort.W/2+ViewPort.Y);

 RenderTriangle(A,B,C,ap,bp,cp);
}
//----------------------------------------------------------------------------------------------------
//������������ ������������ �� ������
//----------------------------------------------------------------------------------------------------
void CSGL::RenderTriangle(SGLNVCTPoint &a,SGLNVCTPoint &b,SGLNVCTPoint &c,SGLScreenPoint &ap,SGLScreenPoint &bp,SGLScreenPoint &cp)
{
 SGLNVCTPoint tmp_point;
 SGLScreenPoint tmp_screen;
 if (ap.Y>cp.Y)
 {
  tmp_point=a;
  a=c;
  c=tmp_point;

  tmp_screen=ap;
  ap=cp;
  cp=tmp_screen;
 }
 if (ap.Y>bp.Y)
 {
  tmp_point=a;
  a=b;
  b=tmp_point;

  tmp_screen=ap;
  ap=bp;
  bp=tmp_screen;
 }
 if (cp.Y<bp.Y)
 {
  tmp_point=c;
  c=b;
  b=tmp_point;

  tmp_screen=cp;
  cp=bp;
  bp=tmp_screen;
 }

 //��������, �������� �� ����������� � ���� ���������
 int32_t starty=ap.Y;
 int32_t endy=cp.Y;
 if (starty==endy) return;//������ ��������
 if (starty>=ViewPort.Y+ViewPort.W) return;//����������� �� �����
 if (endy<ViewPort.X) return;//����������� �� �����
 if (starty<ViewPort.X) starty=ViewPort.X;
 if (endy>=ViewPort.Y+ViewPort.W) endy=ViewPort.Y+ViewPort.W-1;

 //������� ������� � ������ ��������� �� �������� �����
 float lyca=(cp.Y-ap.Y);
 float lyba=(bp.Y-ap.Y);
 float offset=starty-ap.Y;
 //x
 float dcx1=(cp.X-ap.X)/lyca;
 float dcx2=(bp.X-ap.X)/lyba;
 float cx1=ap.X+offset*dcx1;
 float cx2=ap.X+offset*dcx2;
 //z
 float dcz1=(1.0f/c.sGLVertex.Z-1.0f/a.sGLVertex.Z)/lyca;
 float dcz2=(1.0f/b.sGLVertex.Z-1.0f/a.sGLVertex.Z)/lyba;
 float cz1=1.0f/a.sGLVertex.Z+offset*dcz1;
 float cz2=1.0f/a.sGLVertex.Z+offset*dcz2;
 //r
 float dcr1=(c.sGLColor.R-a.sGLColor.R)/lyca;
 float dcr2=(b.sGLColor.R-a.sGLColor.R)/lyba;
 float cr1=a.sGLColor.R+offset*dcr1;
 float cr2=a.sGLColor.R+offset*dcr2;
 //g
 float dcg1=(c.sGLColor.G-a.sGLColor.G)/lyca;
 float dcg2=(b.sGLColor.G-a.sGLColor.G)/lyba;
 float cg1=a.sGLColor.G+offset*dcg1;
 float cg2=a.sGLColor.G+offset*dcg2;
 //b
 float dcb1=(c.sGLColor.B-a.sGLColor.B)/lyca;
 float dcb2=(b.sGLColor.B-a.sGLColor.B)/lyba;
 float cb1=a.sGLColor.B+offset*dcb1;
 float cb2=a.sGLColor.B+offset*dcb2;

 //u
 float dcu1=(c.sGLTexture.U/c.sGLVertex.Z-a.sGLTexture.U/a.sGLVertex.Z)/lyca;
 float dcu2=(b.sGLTexture.U/b.sGLVertex.Z-a.sGLTexture.U/a.sGLVertex.Z)/lyba;
 float cu1=a.sGLTexture.U/a.sGLVertex.Z+offset*dcu1;
 float cu2=a.sGLTexture.U/a.sGLVertex.Z+offset*dcu2;

 //v
 float dcv1=(c.sGLTexture.V/c.sGLVertex.Z-a.sGLTexture.V/a.sGLVertex.Z)/lyca;
 float dcv2=(b.sGLTexture.V/b.sGLVertex.Z-a.sGLTexture.V/a.sGLVertex.Z)/lyba;
 float cv1=a.sGLTexture.V/a.sGLVertex.Z+offset*dcv1;
 float cv2=a.sGLTexture.V/a.sGLVertex.Z+offset*dcv2;

 bool first_half=true;
 SGLNCTPoint sGLNCTPoint_1;
 SGLNCTPoint sGLNCTPoint_2;
 for(int32_t sy=starty;sy<=endy;sy++,cx1+=dcx1,cx2+=dcx2,cz1+=dcz1,cz2+=dcz2, cr1+=dcr1,cr2+=dcr2, cg1+=dcg1,cg2+=dcg2, cb1+=dcb1,cb2+=dcb2, cu1+=dcu1,cu2+=dcu2, cv1+=dcv1,cv2+=dcv2)
 {
  float x1,x2;
  float z1,z2;

  x1=cx1;
  z1=cz1;
  sGLNCTPoint_1.sGLColor.R=cr1;
  sGLNCTPoint_1.sGLColor.G=cg1;
  sGLNCTPoint_1.sGLColor.B=cb1;
  sGLNCTPoint_1.sGLTexture.U=cu1;
  sGLNCTPoint_1.sGLTexture.V=cv1;

  if (sy<bp.Y)
  {
   x2=cx2;
   z2=cz2;

   sGLNCTPoint_2.sGLColor.R=cr2;
   sGLNCTPoint_2.sGLColor.G=cg2;
   sGLNCTPoint_2.sGLColor.B=cb2;
   sGLNCTPoint_2.sGLTexture.U=cu2;
   sGLNCTPoint_2.sGLTexture.V=cv2;
  }
  else
  {
   if (cp.Y==bp.Y)
   {
    x2=bp.X;
    z2=1.0f/b.sGLVertex.Z;

    sGLNCTPoint_2.sGLColor=b.sGLColor;
    sGLNCTPoint_2.sGLTexture.U=b.sGLTexture.U/b.sGLVertex.Z;
    sGLNCTPoint_2.sGLTexture.V=b.sGLTexture.V/b.sGLVertex.Z;
   }
   else
   {
    if (first_half==true)
       {
     float lycb=(cp.Y-bp.Y);
     float offset=sy-bp.Y;

     dcx2=(cp.X-bp.X)/lycb;
     dcz2=(1.0f/c.sGLVertex.Z-1.0f/b.sGLVertex.Z)/lycb;
     cx2=bp.X+offset*dcx2;
     cz2=1.0f/b.sGLVertex.Z+offset*dcz2;

     dcr2=(c.sGLColor.R-b.sGLColor.R)/lycb;
     cr2=b.sGLColor.R+offset*dcr2;

     dcg2=(c.sGLColor.G-b.sGLColor.G)/lycb;
     cg2=b.sGLColor.G+offset*dcg2;

     dcb2=(c.sGLColor.B-b.sGLColor.B)/lycb;
     cb2=b.sGLColor.B+offset*dcb2;

     dcu2=(c.sGLTexture.U/c.sGLVertex.Z-b.sGLTexture.U/b.sGLVertex.Z)/lycb;
     cu2=b.sGLTexture.U/b.sGLVertex.Z+offset*dcu2;

     dcv2=(c.sGLTexture.V/c.sGLVertex.Z-b.sGLTexture.V/b.sGLVertex.Z)/lycb;
     cv2=b.sGLTexture.V/b.sGLVertex.Z+offset*dcv2;

     first_half=false;
    }
    x2=cx2;
    z2=cz2;

    sGLNCTPoint_2.sGLColor.R=cr2;
    sGLNCTPoint_2.sGLColor.G=cg2;
    sGLNCTPoint_2.sGLColor.B=cb2;

    sGLNCTPoint_2.sGLTexture.U=cu2;
    sGLNCTPoint_2.sGLTexture.V=cv2;
   }
  }
  if (x1>x2)
  {
   float tmp=x1;
   x1=x2;
   x2=tmp;
   tmp=z1;
   z1=z2;
   z2=tmp;

   SGLNCTPoint sGLNCTPoint_tmp=sGLNCTPoint_1;
   sGLNCTPoint_1=sGLNCTPoint_2;
   sGLNCTPoint_2=sGLNCTPoint_tmp;
  }
  //������ ����� ������������
  //(this->*DrawLineFunction_Ptr)(sy,x1,x2,z1,z2,r1,r2,g1,g2,b1,b2,u1,u2,v1,v2);
  DrawLine(sy,x1,x2,z1,z2,sGLNCTPoint_1,sGLNCTPoint_2);
 }
}
//----------------------------------------------------------------------------------------------------
//��������� �����
//----------------------------------------------------------------------------------------------------
void CSGL::DrawLine(int32_t y,int32_t x1,int32_t x2,float z1,float z2,const SGLNCTPoint &sGLNCTPoint_1,const SGLNCTPoint &sGLNCTPoint_2)
{
 float u1=sGLNCTPoint_1.sGLTexture.U;
 float v1=sGLNCTPoint_1.sGLTexture.V;

 float r1=sGLNCTPoint_1.sGLColor.R;
 float g1=sGLNCTPoint_1.sGLColor.G;
 float b1=sGLNCTPoint_1.sGLColor.B;

 float u2=sGLNCTPoint_2.sGLTexture.U;
 float v2=sGLNCTPoint_2.sGLTexture.V;

 float r2=sGLNCTPoint_2.sGLColor.R;
 float g2=sGLNCTPoint_2.sGLColor.G;
 float b2=sGLNCTPoint_2.sGLColor.B;

 //������������� ���������� �������� - ��� �������� � ������ �����������
 //������� ��������� �� ��������� - ���������� ������ ��������� ����

 u1*=sGLTextureObject_Current.Width;
 v1*=sGLTextureObject_Current.Height;

 u2*=sGLTextureObject_Current.Width;
 v2*=sGLTextureObject_Current.Height;

 uint32_t mask_texture_width=sGLTextureObject_Current.Width-1;
 uint32_t mask_texture_height=sGLTextureObject_Current.Height-1;

 float dz=0;
 float dr=0;
 float dg=0;
 float db=0;
 float du=0;
 float dv=0;
 if (x2!=x1)
 {
  float dx=x2-x1;
  dz=(z2-z1)/dx;
  dr=(r2-r1)/dx;
  dg=(g2-g1)/dx;
  db=(b2-b1)/dx;
  du=(u2-u1)/dx;
  dv=(v2-v1)/dx;
 }
 if (x2>=ViewPort.X+ViewPort.Z) x2=ViewPort.X+ViewPort.Z-1;
 if (x1<ViewPort.X)
 {
  float offset=(ViewPort.X-x1);
  x1=ViewPort.X;
  z1+=offset*dz;
  r1+=offset*dr;
  g1+=offset*dg;
  b1+=offset*db;
  u1+=offset*du;
  v1+=offset*dv;
 }
 CGLScreenColor *vptr=ImageMap+(x1+(ScreenHeight-y-1)*ScreenWidth);
 float *invdepthptr=InvZBuffer+x1+(int32_t)(ScreenHeight-y-1)*ScreenWidth;

 float z=z1;
 float r=r1;
 float g=g1;
 float b=b1;
 float u=u1;
 float v=v1;

 //���������� �������� ������������ �������� ����� �������� ���������� ��������
 int32_t step=16;//��� ������������
 if ((x2-x1)<step) step=x2-x1;
 float dz_step=dz*step;
 float du_step=du*step;
 float dv_step=dv*step;

 float tu1=u/z;
 float tv1=v/z;
 float tu2=(u+du_step)/(z+dz_step);
 float tv2=(v+dv_step)/(z+dz_step);
 float dtu=(tu2-tu1)/step;
 float dtv=(tv2-tv1)/step;

 float tu=tu1;
 float tv=tv1;

 int32_t counter=step;
 for(float x=x1;x<=x2;x++,z+=dz,r+=dr,g+=dg,b+=db,u+=du,v+=dv,vptr++,invdepthptr++,counter--)
 {
  if (counter==0)
  {
   counter=step;
   if ((x2-counter)<step) step=x2-counter;

   dz_step=dz*step;
   du_step=du*step;
   dv_step=dv*step;

   tu1=u/z;
   tv1=v/z;
   tu2=(u+du_step)/(z+dz_step);
   tv2=(v+dv_step)/(z+dz_step);
   dtu=(tu2-tu1)/step;
   dtv=(tv2-tv1)/step;
  }
  tu+=dtu;
  tv+=dtv;

  if (*(invdepthptr)>z)//������� ����� ������� �����������
  {
   uint32_t tui=static_cast<uint32_t>(tu);
   uint32_t tvi=static_cast<uint32_t>(tv);

   tui&=mask_texture_width;
   tvi&=mask_texture_height;

   SGLRGBAByteColor &sGLRGBAByteColor=sGLTextureObject_Current.sGLRGBAByteColor_Ptr[tui+tvi*sGLTextureObject_Current.Width];

   uint8_t cr=(int8_t)(r*sGLRGBAByteColor.R);
   uint8_t cg=(int8_t)(g*sGLRGBAByteColor.G);
   uint8_t cb=(int8_t)(b*sGLRGBAByteColor.B);

   vptr->SetColor((int8_t)(cr)&0xff,(int8_t)(cg)&0xff,(int8_t)(cb)&0xff);
   *(invdepthptr)=z;   
  }
 }
}



//****************************************************************************************************
//�������� �������
//****************************************************************************************************

//----------------------------------------------------------------------------------------------------
//����������������
//----------------------------------------------------------------------------------------------------
void CSGL::Init(uint32_t screen_width,uint32_t screen_height)
{
 ScreenWidth=screen_width;
 ScreenHeight=screen_height;
 ImageMap=new CGLScreenColor[screen_width*screen_height];
 InvZBuffer=new float[screen_width*screen_height];
}


//----------------------------------------------------------------------------------------------------
//������� ������� ���������
//----------------------------------------------------------------------------------------------------
void CSGL::LoadIdentity(void)
{
 sGLMatrix4_Ptr->X.X=1.0f;
 sGLMatrix4_Ptr->X.Y=0;
 sGLMatrix4_Ptr->X.Z=0;
 sGLMatrix4_Ptr->X.W=0;

 sGLMatrix4_Ptr->Y.X=0;
 sGLMatrix4_Ptr->Y.Y=1.0f;
 sGLMatrix4_Ptr->Y.Z=0;
 sGLMatrix4_Ptr->Y.W=0;

 sGLMatrix4_Ptr->Z.X=0;
 sGLMatrix4_Ptr->Z.Y=0;
 sGLMatrix4_Ptr->Z.Z=1.0f;
 sGLMatrix4_Ptr->Z.W=0;

 sGLMatrix4_Ptr->W.X=0;
 sGLMatrix4_Ptr->W.Y=0;
 sGLMatrix4_Ptr->W.Z=0;
 sGLMatrix4_Ptr->W.W=1.0f;
}
//----------------------------------------------------------------------------------------------------
//�������� ������� ������� �� ������� �������� ������ �������
//----------------------------------------------------------------------------------------------------
void CSGL::Rotatef(float angle,float nx,float ny,float nz)
{
 SGLVector4 vector;
 vector.X=nx;
 vector.Y=ny;
 vector.Z=nz;
 vector.W=0;
 NormaliseSGLVector4(vector);
 nx=vector.X;
 ny=vector.Y;
 nz=vector.Z;

 float cf=cosf(PI/180.0f*angle);
 float sf=sinf(PI/180.0f*angle);

 SGLMatrix4 matrix;

 matrix.X.X=nx*nx+cf*(1.0f-nx*nx);
 matrix.X.Y=nx*(1.0f-cf)*ny+nz*sf;
 matrix.X.Z=nx*(1.0f-cf)*nz-ny*sf;
 matrix.X.W=0;

 matrix.Y.X=nx*(1.0f-cf)*ny-nz*sf;
 matrix.Y.Y=ny*ny+cf*(1.0f-ny*ny);
 matrix.Y.Z=ny*(1.0f-cf)*nz+nx*sf;
 matrix.Y.W=0;

 matrix.Z.X=nx*(1.0f-cf)*nz+ny*sf;
 matrix.Z.Y=ny*(1.0f-cf)*nz-nx*sf;
 matrix.Z.Z=nz*nz+cf*(1.0f-nz*nz);
 matrix.Z.W=0;

 matrix.W.X=0;
 matrix.W.Y=0;
 matrix.W.Z=0;
 matrix.W.W=1.0f;

 SGLMatrix4 tmp_matrix=*sGLMatrix4_Ptr;
 MultiplySGLMatrix4(matrix,tmp_matrix,*sGLMatrix4_Ptr);
}
//----------------------------------------------------------------------------------------------------
//�������� ������� ������� �� ������� ��������
//----------------------------------------------------------------------------------------------------
void CSGL::Translatef(float nx,float ny,float nz)
{
 SGLMatrix4 matrix;
 matrix.X.X=1.0f;
 matrix.X.Y=0;
 matrix.X.Z=0;
 matrix.X.W=0;

 matrix.Y.X=0;
 matrix.Y.Y=1.0f;
 matrix.Y.Z=0;
 matrix.Y.W=0;

 matrix.Z.X=0;
 matrix.Z.Y=0;
 matrix.Z.Z=1.0f;
 matrix.Z.W=0;

 matrix.W.X=nx;
 matrix.W.Y=ny;
 matrix.W.Z=nz;
 matrix.W.W=1.0f;

 SGLMatrix4 tmp_matrix=*sGLMatrix4_Ptr;
 MultiplySGLMatrix4(matrix,tmp_matrix,*sGLMatrix4_Ptr);
}
//----------------------------------------------------------------------------------------------------
//������� �������
//----------------------------------------------------------------------------------------------------
void CSGL::MatrixMode(MATRIX_MODE matrix)
{
 if (matrix==SGL_MATRIX_MODELVIEW) sGLMatrix4_Ptr=&ModelViewMatrix;
 if (matrix==SGL_MATRIX_PROJECTION) sGLMatrix4_Ptr=&ProjectionMatrix;
 if (matrix==SGL_MATRIX_TEXTURE) sGLMatrix4_Ptr=&TextureMatrix;
}

//----------------------------------------------------------------------------------------------------
//������ ��������� ��������� � ������� �������������
//----------------------------------------------------------------------------------------------------
void CSGL::Frustrum(float left,float right,float bottom,float top,float near,float far)
{
 if (right==left) return;
 if (top==bottom) return;
 if (near==far) return;

 MatrixMode(SGL_MATRIX_PROJECTION);
 LoadIdentity();

 SGLMatrix4 matrix;
 matrix.X.X=2.0f*near/(right-left);
 matrix.X.Y=0;
 matrix.X.Z=(right+left)/(right-left);
 matrix.X.W=0;

 matrix.Y.X=0;
 matrix.Y.Y=2.0f*near/(top-bottom);
 matrix.Y.Z=(top+bottom)/(top-bottom);
 matrix.Y.W=0;

 matrix.Z.X=0;
 matrix.Z.Y=0;
 matrix.Z.Z=-(far+near)/(far-near);
 matrix.Z.W=-2.0f*far*near/(far-near);

 matrix.W.X=0;
 matrix.W.Y=0;
 matrix.W.Z=-1.0f;
 matrix.W.W=0;

 SGLMatrix4 tmp_matrix=ProjectionMatrix;
 MultiplySGLMatrix4(tmp_matrix,matrix,ProjectionMatrix);
}
//----------------------------------------------------------------------------------------------------
//������ ������� ����
//----------------------------------------------------------------------------------------------------
void CSGL::SetViewport(float x,float y,float len,float hgt)
{
 ViewPort.X=x;
 ViewPort.Y=y;
 ViewPort.Z=len;
 ViewPort.W=hgt;
}
//----------------------------------------------------------------------------------------------------
//������ ������� �������������
//----------------------------------------------------------------------------------------------------
void CSGL::Perspective(float fovy,float aspect,float near,float far)
{
 float top=tanf(PI/180.0f*fovy/2.0f)*near;
 float bottom=-top;
 float right=top*aspect;
 float left=-right;
 Frustrum(left,right,bottom,top,near,far);
}
//----------------------------------------------------------------------------------------------------
//������ ���� �����
//----------------------------------------------------------------------------------------------------
void CSGL::Color3f(float r,float g,float b)
{
 sGLNVCTPoint_Current.sGLColor.R=r;
 sGLNVCTPoint_Current.sGLColor.G=g;
 sGLNVCTPoint_Current.sGLColor.B=b;
}
//----------------------------------------------------------------------------------------------------
//������ ���������� ���������� �����
//----------------------------------------------------------------------------------------------------
void CSGL::TexCoordf(float u,float v)
{
 //��������� ������� ���������������
 SGLVector4 vector_in;
 SGLVector4 vector_out;
 vector_in.X=u;
 vector_in.Y=v;
 vector_in.Z=0;
 vector_in.W=1;
 MultiplySGLVector4ToSGLMatrix4(vector_in,TextureMatrix,vector_out);

 sGLNVCTPoint_Current.sGLTexture.U=vector_out.X/vector_out.W;
 sGLNVCTPoint_Current.sGLTexture.V=vector_out.Y/vector_out.W;
}
//----------------------------------------------------------------------------------------------------
//������ ������� � �����
//----------------------------------------------------------------------------------------------------
void CSGL::Normal3f(float nx,float ny,float nz)
{
 //��������� ������� ������������� � �������
 SGLVector4 vector_in;
 SGLVector4 vector_out;

 vector_in.X=nx;
 vector_in.Y=ny;
 vector_in.Z=nz;
 vector_in.W=1;

 MultiplySGLVector4ToSGLMatrix4(vector_in,ModelViewMatrix,vector_out);
 //�������� ��������
 sGLNVCTPoint_Current.sGLNormal.Nx=(vector_out.X-ModelViewMatrix.W.X)/vector_out.W;
 sGLNVCTPoint_Current.sGLNormal.Ny=(vector_out.Y-ModelViewMatrix.W.Y)/vector_out.W;
 sGLNVCTPoint_Current.sGLNormal.Nz=(vector_out.Z-ModelViewMatrix.W.Z)/vector_out.W;
}
//----------------------------------------------------------------------------------------------------
//������ ���������
//----------------------------------------------------------------------------------------------------
void CSGL::Begin(void)
{
 PointArrayAmount=0;
 DrawMode=true;
 CreateFrustrumPlane();
}
//----------------------------------------------------------------------------------------------------
//��������� ���������
//----------------------------------------------------------------------------------------------------
void CSGL::End(void)
{
 DrawMode=false;
}
//----------------------------------------------------------------------------------------------------
//������ ���������� �����
//----------------------------------------------------------------------------------------------------
void CSGL::Vertex3f(float x,float y,float z)
{
 if (DrawMode==false) return;//����� ��������� �� �����������
 //�������� �������������� ����� ������������ � ������ ������� �������������
 SGLVector4 vector_in;
 SGLVector4 vector_out;

 vector_in.X=x;
 vector_in.Y=y;
 vector_in.Z=z;
 vector_in.W=1.0f;

 MultiplySGLVector4ToSGLMatrix4(vector_in,ModelViewMatrix,vector_out);
 //������ ��, ��� ���������� � ������ �����
 sGLNVCTPoint_Current.sGLVertex.X=vector_out.X/vector_out.W;
 sGLNVCTPoint_Current.sGLVertex.Y=vector_out.Y/vector_out.W;
 sGLNVCTPoint_Current.sGLVertex.Z=vector_out.Z/vector_out.W;
 sGLNVCTPointArray[PointArrayAmount]=sGLNVCTPoint_Current;
 PointArrayAmount++;
 //������������ �������
 if (PointArrayAmount==VERTEX_POINT_ARRAY)//��� ���� ����� �� �����������
 {
  OutputTriangle(sGLNVCTPointArray[0],sGLNVCTPointArray[1],sGLNVCTPointArray[2]);
  sGLNVCTPointArray[VERTEX_POINT_ARRAY-2]=sGLNVCTPointArray[VERTEX_POINT_ARRAY-1];//������� �������
  PointArrayAmount=VERTEX_POINT_ARRAY-1;//�������� ��������� �������
 }
}
//----------------------------------------------------------------------------------------------------
//������ ���� ������� ����
//----------------------------------------------------------------------------------------------------
void CSGL::ClearColor(float r,float g,float b)
{
 SetColorValue(sGLColor_Clear,r,g,b);
}
//----------------------------------------------------------------------------------------------------
//�������� �����
//----------------------------------------------------------------------------------------------------
void CSGL::Clear(uint32_t mode)
{
 if (mode&SGL_COLOR_BUFFER_BIT)
 {
  CGLScreenColor *ptr=ImageMap;
  uint8_t r=static_cast<uint8_t>(sGLColor_Clear.R*MAX_COLOR_VALUE);
  uint8_t g=static_cast<uint8_t>(sGLColor_Clear.G*MAX_COLOR_VALUE);
  uint8_t b=static_cast<uint8_t>(sGLColor_Clear.B*MAX_COLOR_VALUE);
  for(uint32_t n=0;n<ScreenWidth*ScreenHeight;n++,ptr++) ptr->SetColor(r,g,b);
 }
 if (mode&SGL_DEPTH_BUFFER_BIT)
 {
  float *ptr=InvZBuffer;
  for(uint32_t n=0;n<ScreenWidth*ScreenHeight;n++,ptr++) *ptr=static_cast<float>(MIN_INV_Z_VALUE);
 }
}
//----------------------------------------------------------------------------------------------------
//������ ��������
//----------------------------------------------------------------------------------------------------
void CSGL::BindTexture(uint32_t width,uint32_t height,SGLRGBAByteColor *sGLRGBAByteColor_Ptr_Set)
{
 sGLTextureObject_Current.Width=width;
 sGLTextureObject_Current.Height=height;
 sGLTextureObject_Current.sGLRGBAByteColor_Ptr=sGLRGBAByteColor_Ptr_Set;
}



//****************************************************************************************************
//����������� �������
//****************************************************************************************************

//----------------------------------------------------------------------------------------------------
//������ ���������� �������
//----------------------------------------------------------------------------------------------------
void CSGL::SetVertexCoord(SGLVertex &sGLVertex,float x,float y,float z)
{
 sGLVertex.X=x;
 sGLVertex.Y=y;
 sGLVertex.Z=z;
}
//----------------------------------------------------------------------------------------------------
//������ ���������� �������
//----------------------------------------------------------------------------------------------------
void CSGL::SetNormalCoord(SGLNormal &sGLNormal,float nx,float ny,float nz)
{
 sGLNormal.Nx=nx;
 sGLNormal.Ny=ny;
 sGLNormal.Nz=nz;
}
//----------------------------------------------------------------------------------------------------
//������ ���������� ��������
//----------------------------------------------------------------------------------------------------
void CSGL::SetTextureCoord(SGLTexture &sGLTexture,float u,float v)
{
 sGLTexture.U=u;
 sGLTexture.V=v;
}
//----------------------------------------------------------------------------------------------------
//������ ����
//----------------------------------------------------------------------------------------------------
void CSGL::SetColorValue(SGLColor &sGLColor,float r,float g,float b)
{
 sGLColor.R=r;
 sGLColor.G=g;
 sGLColor.B=b;
}

