#include "csgl.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//-����������� ������--------------------------------------------------------
CSGL::CSGL(void)
{ 	
 ImageMap=NULL;
 ZBuffer=NULL;
 //CurrentColor.Color=0xffff;
 CurrentColor.r=0xff;
 CurrentColor.g=0xff;
 CurrentColor.b=0xff;
 CurrentSelectedMatrix=MATRIX_MODE_PROJECTION;
 LoadIdentity();
 CurrentSelectedMatrix=MATRIX_MODE_MODEL_VIEW;
 LoadIdentity();
 PointArrayAmount=0;
 DrawModeActive=false;
 EnableDepthText=false;
 CreateFrustrumPlane();
}
//-���������� ������---------------------------------------------------------
CSGL::~CSGL(void)
{
 if (ImageMap!=NULL) delete[](ImageMap);
 ImageMap=NULL;
 if (ZBuffer!=NULL) delete[](ZBuffer);
 ZBuffer=NULL;
}
//-������� ������------------------------------------------------------------
bool CSGL::Create(int screen_width,int screen_height)
{
 if (ImageMap!=NULL) delete[](ImageMap);
 if (ZBuffer!=NULL) delete[](ZBuffer);
 ImageMap=new SGuScreenColor[screen_width*screen_height+1];
 ZBuffer=new float[screen_width*screen_height+1];
 ScreenWidth=screen_width;
 ScreenHeight=screen_height;
 return(true);
}
bool CSGL::LoadIdentity(void)
{
 if (CurrentSelectedMatrix==MATRIX_MODE_PROJECTION)
 {
  ProjectionMatrix.x.x=1; 
  ProjectionMatrix.x.y=0;
  ProjectionMatrix.x.z=0;
  ProjectionMatrix.x.w=0;	 
	 
  ProjectionMatrix.y.x=0; 
  ProjectionMatrix.y.y=1;
  ProjectionMatrix.y.z=0;
  ProjectionMatrix.y.w=0;	 

  ProjectionMatrix.z.x=0; 
  ProjectionMatrix.z.y=0;
  ProjectionMatrix.z.z=1;
  ProjectionMatrix.z.w=0;	 

  ProjectionMatrix.w.x=0; 
  ProjectionMatrix.w.y=0;
  ProjectionMatrix.w.z=0;
  ProjectionMatrix.w.w=1;	 
 }
 if (CurrentSelectedMatrix==MATRIX_MODE_MODEL_VIEW)
 {
  ModelViewMatrix.x.x=1; 
  ModelViewMatrix.x.y=0;
  ModelViewMatrix.x.z=0;
  ModelViewMatrix.x.w=0;	 
	 
  ModelViewMatrix.y.x=0; 
  ModelViewMatrix.y.y=1;
  ModelViewMatrix.y.z=0;
  ModelViewMatrix.y.w=0;	 

  ModelViewMatrix.z.x=0; 
  ModelViewMatrix.z.y=0;
  ModelViewMatrix.z.z=1;
  ModelViewMatrix.z.w=0;	 

  ModelViewMatrix.w.x=0; 
  ModelViewMatrix.w.y=0;
  ModelViewMatrix.w.z=0;
  ModelViewMatrix.w.w=1;	 
 }
 CreateFrustrumPlane();
 return(true);
}
bool CSGL::Frustrum(float left,float right,float bottom,float top,float near,float far)
{
 if (right==left) return(false);
 if (top==bottom) return(false);
 if (near==far) return(false);
	
 SGuMatrix4	matrix;
 matrix.x.x=2*near/(right-left);
 matrix.x.y=0;	
 matrix.x.z=(right+left)/(right-left);	
 matrix.x.w=0;	

 matrix.y.x=0;	
 matrix.y.y=2*near/(top-bottom);	
 matrix.y.z=(top+bottom)/(top-bottom);	
 matrix.y.w=0;	

 matrix.z.x=0;	
 matrix.z.y=0;	
 matrix.z.z=-(far+near)/(far-near);	
 matrix.z.w=-2*far*near/(far-near);	
	
 matrix.w.x=0;	
 matrix.w.y=0;	
 matrix.w.z=-1;	
 matrix.w.w=0;	
	
 SGuMatrix4 tmp_matrix=ProjectionMatrix;	
 MultiplySGuMatrix4(tmp_matrix,matrix,ProjectionMatrix);
 CreateFrustrumPlane();
 return(true);
}
bool CSGL::SetViewport(float x,float y,float len,float hgt)
{  
 ViewPort.x=x;
 ViewPort.y=y;
 ViewPort.z=len;
 ViewPort.w=hgt;
 return(true);
}
bool CSGL::Perspective(float fovy,float aspect,float near,float far)
{
 float top=(float)tan(M_PI/180.0*fovy/2)*near;
 float bottom=-top;
 float right=top*aspect;
 float left=-right;
 return(Frustrum(left,right,bottom,top,near,far));
}
bool CSGL::Rotatef(float angle,float nx,float ny,float nz)
{
 SGuVector4 vector;	
 vector.x=nx;
 vector.y=ny;
 vector.z=nz;
 vector.w=0;	
 NormaliseSGuVector4(vector);
 nx=vector.x;	
 ny=vector.y;	
 nz=vector.z;	
	
 float cf=(float)cos(M_PI/180*angle);
 float sf=(float)sin(M_PI/180*angle);
 SGuMatrix4	matrix;
 
 matrix.x.x=nx*nx+cf*(1-nx*nx);
 matrix.x.y=nx*(1-cf)*ny+nz*sf;
 matrix.x.z=nx*(1-cf)*nz-ny*sf;
 matrix.x.w=0;

 matrix.y.x=nx*(1-cf)*ny-nz*sf;
 matrix.y.y=ny*ny+cf*(1-ny*ny);
 matrix.y.z=ny*(1-cf)*nz+nx*sf;
 matrix.y.w=0;

 matrix.z.x=nx*(1-cf)*nz+ny*sf;
 matrix.z.y=ny*(1-cf)*nz-nx*sf;
 matrix.z.z=nz*nz+cf*(1-nz*nz);
 matrix.z.w=0;

 matrix.w.x=0;
 matrix.w.y=0;
 matrix.w.z=0;
 matrix.w.w=1;

 if (CurrentSelectedMatrix==MATRIX_MODE_PROJECTION) 
 {
  SGuMatrix4 tmp_matrix=ProjectionMatrix;
  MultiplySGuMatrix4(matrix,tmp_matrix,ProjectionMatrix);
 }
 if (CurrentSelectedMatrix==MATRIX_MODE_MODEL_VIEW) 
 {
  SGuMatrix4 tmp_matrix=ModelViewMatrix;		 
  MultiplySGuMatrix4(matrix,tmp_matrix,ModelViewMatrix);
 }
 CreateFrustrumPlane();
 return(true);
}
bool CSGL::Translatef(float nx,float ny,float nz)
{
 SGuMatrix4	matrix;
 matrix.x.x=1;	
 matrix.x.y=0;	
 matrix.x.z=0;	
 matrix.x.w=0;	

 matrix.y.x=0;	
 matrix.y.y=1;	
 matrix.y.z=0;	
 matrix.y.w=0;	

 matrix.z.x=0;	
 matrix.z.y=0;	
 matrix.z.z=1;	
 matrix.z.w=0;	
	
 matrix.w.x=nx;	
 matrix.w.y=ny;	
 matrix.w.z=nz;	
 matrix.w.w=1;	
	
 if (CurrentSelectedMatrix==MATRIX_MODE_PROJECTION) 
 {
  SGuMatrix4 tmp_matrix=ProjectionMatrix;
  MultiplySGuMatrix4(matrix,tmp_matrix,ProjectionMatrix);
 }
 if (CurrentSelectedMatrix==MATRIX_MODE_MODEL_VIEW) 
 {
  SGuMatrix4 tmp_matrix=ModelViewMatrix;		 
  MultiplySGuMatrix4(matrix,tmp_matrix,ModelViewMatrix);
 }
 CreateFrustrumPlane();
 return(true);
}
bool CSGL::MatrixMode(int matrix)
{
 if (matrix==SGL_MATRIX_PROJECTION) CurrentSelectedMatrix=MATRIX_MODE_PROJECTION;
 if (matrix==SGL_MATRIX_MODELVIEW) CurrentSelectedMatrix=MATRIX_MODE_MODEL_VIEW;
 return(true);
}
bool CSGL::Color3i(unsigned char r,unsigned char g,unsigned char b)
{
 uint16_t color;	  
 color=r>>3;
 color<<=6;
 color|=g>>2;
 color<<=5;
 color|=b>>3;
// CurrentColor.Color=color;
 CurrentColor.r=r;
 CurrentColor.g=g;
 CurrentColor.b=b;
 return(true);
}
bool CSGL::Begin(void)
{
 PointArrayAmount=0;
 DrawModeActive=true;
 return(true);
}
bool CSGL::End(void)
{
 if (DrawModeActive==false) return(false);//����� ������ �� ����������� 
 DrawModeActive=false;
 PointArrayAmount=0;
 return(true);
}
bool CSGL::Vertex3f(float x,float y,float z)
{
 if (DrawModeActive==false) return(false);//����� ������ �� ����������� 
 //�������� �������������� ����� ������������ � ������ ������� �������������
 SGuVector4 vector_in;		
 SGuVector4 vector_out;	
	
 vector_in.x=x;
 vector_in.y=y;
 vector_in.z=z;
 vector_in.w=1;
	
 MultiplySGuVector4ToSGuMatrix4(vector_in,ModelViewMatrix,vector_out);
 //������ ��, ��� ���������� � ������
 sGuNVCTPointArray[PointArrayAmount].sGuVertex.x=vector_out.x/vector_out.w;
 sGuNVCTPointArray[PointArrayAmount].sGuVertex.y=vector_out.y/vector_out.w;
 sGuNVCTPointArray[PointArrayAmount].sGuVertex.z=vector_out.z/vector_out.w;
 sGuNVCTPointArray[PointArrayAmount].sGuColor=CurrentColor;
 PointArrayAmount++;

 //������������ �������
 if (PointArrayAmount==3)//��� ���� ����� �� �����������
 {  
  OutputTriangle(sGuNVCTPointArray[0],sGuNVCTPointArray[1],sGuNVCTPointArray[2]);
  sGuNVCTPointArray[1]=sGuNVCTPointArray[2];//������� �������
  PointArrayAmount=2;//�������� ��������� �������  
 }
 return(true);
}
bool CSGL::DrawTriangle(SGuNVCTPoint A,SGuNVCTPoint B,SGuNVCTPoint C)
{
 SGuVector4 sGuVector4_A;
 SGuVector4 sGuVector4_B;
 SGuVector4 sGuVector4_C;
	
 //�������� �������� ������ ������������
 sGuVector4_A.x=A.sGuVertex.x;
 sGuVector4_A.y=A.sGuVertex.y;
 sGuVector4_A.z=A.sGuVertex.z;
 sGuVector4_A.w=1;

 sGuVector4_B.x=B.sGuVertex.x;
 sGuVector4_B.y=B.sGuVertex.y;
 sGuVector4_B.z=B.sGuVertex.z;
 sGuVector4_B.w=1;

 sGuVector4_C.x=C.sGuVertex.x;
 sGuVector4_C.y=C.sGuVertex.y;
 sGuVector4_C.z=C.sGuVertex.z;
 sGuVector4_C.w=1;

 SGuVector4 sGuVector4_A_New;
 SGuVector4 sGuVector4_B_New;
 SGuVector4 sGuVector4_C_New;

 MultiplySGuMatrix4ToSGuVector4(ProjectionMatrix,sGuVector4_A,sGuVector4_A_New);
 MultiplySGuMatrix4ToSGuVector4(ProjectionMatrix,sGuVector4_B,sGuVector4_B_New);
 MultiplySGuMatrix4ToSGuVector4(ProjectionMatrix,sGuVector4_C,sGuVector4_C_New);

 A.sGuVertex.x=(sGuVector4_A_New.x/sGuVector4_A_New.w+1)*ViewPort.z/2+ViewPort.x;
 A.sGuVertex.y=(1-sGuVector4_A_New.y/sGuVector4_A_New.w)*ViewPort.w/2+ViewPort.y;

 B.sGuVertex.x=(sGuVector4_B_New.x/sGuVector4_B_New.w+1)*ViewPort.z/2+ViewPort.x;
 B.sGuVertex.y=(1-sGuVector4_B_New.y/sGuVector4_B_New.w)*ViewPort.w/2+ViewPort.y;

 C.sGuVertex.x=(sGuVector4_C_New.x/sGuVector4_C_New.w+1)*ViewPort.z/2+ViewPort.x;
 C.sGuVertex.y=(1-sGuVector4_C_New.y/sGuVector4_C_New.w)*ViewPort.w/2+ViewPort.y;

 SGuNVCTPoint TemporaryPoint;
 if (A.sGuVertex.y>C.sGuVertex.y)
 {
  TemporaryPoint=A;
  A=C;
  C=TemporaryPoint;
 }
 if (A.sGuVertex.y>B.sGuVertex.y)
 {
  TemporaryPoint=A;
  A=B;
  B=TemporaryPoint;
 }
 if (C.sGuVertex.y<B.sGuVertex.y)
 {
  TemporaryPoint=C;
  C=B;
  B=TemporaryPoint;
 }

 //��������, �������� �� ����������� � ���� ���������
 float starty=A.sGuVertex.y;
 float endy=C.sGuVertex.y;
 if (starty==endy) return(true);//������ ��������
 if (starty>=ViewPort.y+ViewPort.w) return(true);//����������� �� �����
 if (endy<ViewPort.x) return(true);//����������� �� �����
 if (starty<ViewPort.x) starty=0;
 if (endy>=ViewPort.y+ViewPort.w) endy=ViewPort.y+ViewPort.w-1;

 float cx1=A.sGuVertex.x+(starty-A.sGuVertex.y)*(C.sGuVertex.x-A.sGuVertex.x)/(C.sGuVertex.y-A.sGuVertex.y);
 float cx2=A.sGuVertex.x+(starty-A.sGuVertex.y)*(B.sGuVertex.x-A.sGuVertex.x)/(B.sGuVertex.y-A.sGuVertex.y);
 float dcx1=(C.sGuVertex.x-A.sGuVertex.x)/(C.sGuVertex.y-A.sGuVertex.y);
 float dcx2=(B.sGuVertex.x-A.sGuVertex.x)/(B.sGuVertex.y-A.sGuVertex.y);
 
 float cz1=A.sGuVertex.z+(starty-A.sGuVertex.y)*(C.sGuVertex.z-A.sGuVertex.z)/(C.sGuVertex.y-A.sGuVertex.y);
 float cz2=A.sGuVertex.z+(starty-A.sGuVertex.y)*(B.sGuVertex.z-A.sGuVertex.z)/(B.sGuVertex.y-A.sGuVertex.y);
 float dcz1=(C.sGuVertex.z-A.sGuVertex.z)/(C.sGuVertex.y-A.sGuVertex.y);
 float dcz2=(B.sGuVertex.z-A.sGuVertex.z)/(B.sGuVertex.y-A.sGuVertex.y);

 float cr1=A.sGuColor.r+(starty-A.sGuVertex.y)*(C.sGuColor.r-A.sGuColor.r)/(C.sGuVertex.y-A.sGuVertex.y);
 float cr2=A.sGuColor.r+(starty-A.sGuVertex.y)*(B.sGuColor.r-A.sGuColor.r)/(B.sGuVertex.y-A.sGuVertex.y);
 float dcr1=(C.sGuColor.r-A.sGuColor.r)/(C.sGuVertex.y-A.sGuVertex.y);
 float dcr2=(B.sGuColor.r-A.sGuColor.r)/(B.sGuVertex.y-A.sGuVertex.y);

 float cg1=A.sGuColor.g+(starty-A.sGuVertex.y)*(C.sGuColor.g-A.sGuColor.g)/(C.sGuVertex.y-A.sGuVertex.y);
 float cg2=A.sGuColor.g+(starty-A.sGuVertex.y)*(B.sGuColor.g-A.sGuColor.g)/(B.sGuVertex.y-A.sGuVertex.y);
 float dcg1=(C.sGuColor.g-A.sGuColor.g)/(C.sGuVertex.y-A.sGuVertex.y);
 float dcg2=(B.sGuColor.g-A.sGuColor.g)/(B.sGuVertex.y-A.sGuVertex.y);

 float cb1=A.sGuColor.b+(starty-A.sGuVertex.y)*(C.sGuColor.b-A.sGuColor.b)/(C.sGuVertex.y-A.sGuVertex.y);
 float cb2=A.sGuColor.b+(starty-A.sGuVertex.y)*(B.sGuColor.b-A.sGuColor.b)/(B.sGuVertex.y-A.sGuVertex.y);
 float dcb1=(C.sGuColor.b-A.sGuColor.b)/(C.sGuVertex.y-A.sGuVertex.y);
 float dcb2=(B.sGuColor.b-A.sGuColor.b)/(B.sGuVertex.y-A.sGuVertex.y);

 int mode=0;
 for(float sy=starty;sy<=endy;sy++,cx1+=dcx1,cx2+=dcx2,cz1+=dcz1,cz2+=dcz2, cr1+=dcr1,cr2+=dcr2, cg1+=dcg1,cg2+=dcg2, cb1+=dcb1,cb2+=dcb2)
 {
  float x1,x2;
  float z1,z2;
  float r1,r2;
  float g1,g2;
  float b1,b2;
  x1=cx1;
  z1=cz1;
  r1=cr1;
  g1=cg1;
  b1=cb1;

  if (sy<B.sGuVertex.y)
  {
   x2=cx2;
   z2=cz2;

   r2=cr2;
   g2=cg2;
   b2=cb2;
  }
  else
  {
   if (C.sGuVertex.y==B.sGuVertex.y)
   {
	x2=B.sGuVertex.x;
	z2=B.sGuVertex.z;

	r2=B.sGuColor.r;
	g2=B.sGuColor.g;
	b2=B.sGuColor.b;
   }
   else
   {
    if (mode==0)
   	{
     cx2=B.sGuVertex.x+(sy-B.sGuVertex.y)*(C.sGuVertex.x-B.sGuVertex.x)/(C.sGuVertex.y-B.sGuVertex.y);
	   dcx2=(C.sGuVertex.x-B.sGuVertex.x)/(C.sGuVertex.y-B.sGuVertex.y);
     cz2=B.sGuVertex.z+(sy-B.sGuVertex.y)*(C.sGuVertex.z-B.sGuVertex.z)/(C.sGuVertex.y-B.sGuVertex.y);
	   dcz2=(C.sGuVertex.z-B.sGuVertex.z)/(C.sGuVertex.y-B.sGuVertex.y);

     cr2=B.sGuColor.r+(sy-B.sGuVertex.y)*(C.sGuColor.r-B.sGuColor.r)/(C.sGuVertex.y-B.sGuVertex.y);
	   dcr2=(C.sGuColor.r-B.sGuColor.r)/(C.sGuVertex.y-B.sGuVertex.y);

     cg2=B.sGuColor.g+(sy-B.sGuVertex.y)*(C.sGuColor.g-B.sGuColor.g)/(C.sGuVertex.y-B.sGuVertex.y);
	   dcg2=(C.sGuColor.g-B.sGuColor.g)/(C.sGuVertex.y-B.sGuVertex.y);

     cb2=B.sGuColor.b+(sy-B.sGuVertex.y)*(C.sGuColor.b-B.sGuColor.b)/(C.sGuVertex.y-B.sGuVertex.y);
	   dcb2=(C.sGuColor.b-B.sGuColor.b)/(C.sGuVertex.y-B.sGuVertex.y);

	   mode=1;
	  }
	  x2=cx2;
	  z2=cz2;

    r2=cr2;
    g2=cg2;
    b2=cb2;
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

   tmp=r1;
   r1=r2;
   r2=tmp;

   tmp=g1;
   g1=g2;
   g2=tmp;

   tmp=b1;
   b1=b2;
   b2=tmp;
  }
  //������ ����� ������������
  float dz=0;
  float dr=0;
  float dg=0;
  float db=0;
  if (x2!=x1)
  {
   dz=(z2-z1)/(x2-x1);
   dr=(r2-r1)/(x2-x1);
   dg=(g2-g1)/(x2-x1);
   db=(b2-b1)/(x2-x1);
  }
  if (x2>=ViewPort.x+ViewPort.z) x2=ViewPort.x+ViewPort.z-1;
  if (x1<ViewPort.x)
  {
   x1=ViewPort.x;
   z1+=(ViewPort.x-x1)*dz;
   r1+=(ViewPort.x-x1)*dr;
   g1+=(ViewPort.x-x1)*dg;
   b1+=(ViewPort.x-x1)*db;
  }
  SGuScreenColor *vptr=ImageMap+((int32_t)(x1)+(ScreenHeight-(int32_t)(sy)-1)*ScreenWidth);
  if (EnableDepthText==false)//���� ������� �� ������������
  {
   float *depthptr=ZBuffer+(int32_t)(x1-1)+(int)(ScreenHeight-(int32_t)(sy)-1)*ScreenWidth;
   float z=z1;
   float r=r1;
   float g=g1;
   float b=b1;
   for(float x=x1;x<=x2;x++,z+=dz,r+=dr,g+=dg,b+=db,vptr++,depthptr++)
   {
    //*((SGuScreenColor*)(&A.sGuColor));
    uint16_t color;	  
    color=(uint8_t)(r)>>3;
    color<<=6;
    color|=(uint8_t)(g)>>2;
    color<<=5;
    color|=(uint8_t)(b)>>3;
		 
  	vptr->Color=color;
   	*(depthptr)=z;
   }
  }
  else//���� ������� ������������
  {
   float *depthptr=ZBuffer+(int32_t)(x1-1)+(int32_t)(ScreenHeight-(int32_t)(sy)-1)*ScreenWidth;
   float z=z1;
   float r=r1;
   float g=g1;
   float b=b1;
   for(float x=x1;x<=x2;x++,z+=dz,r+=dr,g+=dg,b+=db,vptr++,depthptr++)
   {
    if (*(depthptr)<z)
	{
	 //*(vptr)=*((SGuScreenColor*)(&A.sGuColor));
    uint16_t color;	  
    color=(uint8_t)(r)>>3;
    color<<=6;
    color|=(uint8_t)(g)>>2;
    color<<=5;
    color|=(uint8_t)(b)>>3;
		 
  	vptr->Color=color;
   	*(depthptr)=z;
     *(depthptr)=z;
	  }
   }
  }
 }
 return(true);
}
bool CSGL::Clear(unsigned int mode)
{
 if (mode&SGL_COLOR_BUFFER_BIT && ImageMap!=NULL) memset(ImageMap,0,ScreenWidth*ScreenHeight*sizeof(SGuScreenColor));
 if (mode&SGL_DEPTH_BUFFER_BIT && ZBuffer!=NULL) for(int n=0;n<ScreenWidth*ScreenHeight;n++) ZBuffer[n]=(float)(-100000.0);
 return(true);
}
bool CSGL::Enable(unsigned int mode)
{
 if (mode==SGL_DEPTH_TEST) EnableDepthText=true;
 return(true);
}
bool CSGL::Disable(unsigned int mode)
{
 if (mode==SGL_DEPTH_TEST) EnableDepthText=false;
 return(true);
}










//----------------------------------------------------------------------------------------------------
//������ ���������� �������
//----------------------------------------------------------------------------------------------------
void CSGL::SetVertexCoord(SGuVertex &sGuVertex,float x,float y,float z)
{
 sGuVertex.x=x;
 sGuVertex.y=y;
 sGuVertex.z=z;
}
//----------------------------------------------------------------------------------------------------
//������ ���������� �������
//----------------------------------------------------------------------------------------------------
void CSGL::SetNormalCoord(SGuNormal &sGuNormal,float nx,float ny,float nz)
{
 sGuNormal.nx=nx;
 sGuNormal.ny=ny;
 sGuNormal.nz=nz;
}
//----------------------------------------------------------------------------------------------------
//������ ���������� ��������
//----------------------------------------------------------------------------------------------------
void CSGL::SetTextureCoord(SGuTexture &sGuTexture,float u,float v)
{
 sGuTexture.u=u;
 sGuTexture.v=v;
}
//----------------------------------------------------------------------------------------------------
//������ ����
//----------------------------------------------------------------------------------------------------
void CSGL::SetColorValue(SGuColor &sGuColor,uint8_t r,uint8_t g,uint8_t b)
{
 uint16_t color;	  
 color=r>>3;
 color<<=6;
 color|=g>>2;
 color<<=5;
 color|=b>>3;
// sGuColor.Color=color;
 sGuColor.r=r;
 sGuColor.g=g;
 sGuColor.b=b;
}
//----------------------------------------------------------------------------------------------------
//��������� ������� ���� SGuVector4 �� ������� ���� SGuMatrix4
//----------------------------------------------------------------------------------------------------
void CSGL::MultiplySGuVector4ToSGuMatrix4(const SGuVector4& v,const SGuMatrix4& m,SGuVector4& v_out)
{
 //������ ���������� � ������� - y, ������ x
 v_out.x=v.x*m.x.x+v.y*m.y.x+v.z*m.z.x+v.w*m.w.x;
 v_out.y=v.x*m.x.y+v.y*m.y.y+v.z*m.z.y+v.w*m.w.y;
 v_out.z=v.x*m.x.z+v.y*m.y.z+v.z*m.z.z+v.w*m.w.z;
 v_out.w=v.x*m.x.w+v.y*m.y.w+v.z*m.z.w+v.w*m.w.w;
}
//----------------------------------------------------------------------------------------------------
//��������� ������� ���� SGuMatrix4 �� ������ ���� SGuVector4
//----------------------------------------------------------------------------------------------------
void CSGL::MultiplySGuMatrix4ToSGuVector4(const SGuMatrix4& m,const SGuVector4& v,SGuVector4& v_out)
{
 //������ ���������� � ������� - y, ������ x
 v_out.x=v.x*m.x.x+v.y*m.x.y+v.z*m.x.z+v.w*m.x.w;
 v_out.y=v.x*m.y.x+v.y*m.y.y+v.z*m.y.z+v.w*m.y.w;
 v_out.z=v.x*m.z.x+v.y*m.z.y+v.z*m.z.z+v.w*m.z.w;
 v_out.w=v.x*m.w.x+v.y*m.w.y+v.z*m.w.z+v.w*m.w.w;
}
//----------------------------------------------------------------------------------------------------
//��������� ���� ������ ���� SGuMatrix4
//----------------------------------------------------------------------------------------------------
void CSGL::MultiplySGuMatrix4(const SGuMatrix4& a,const SGuMatrix4& b,SGuMatrix4& out)
{
 out.x.x=a.x.x*b.x.x+a.x.y*b.y.x+a.x.z*b.z.x+a.x.w*b.w.x;
 out.x.y=a.x.x*b.x.y+a.x.y*b.y.y+a.x.z*b.z.y+a.x.w*b.w.y;
 out.x.z=a.x.x*b.x.z+a.x.y*b.y.z+a.x.z*b.z.z+a.x.w*b.w.z;
 out.x.w=a.x.x*b.x.w+a.x.y*b.y.w+a.x.z*b.z.w+a.x.w*b.w.w;

 out.y.x=a.y.x*b.x.x+a.y.y*b.y.x+a.y.z*b.z.x+a.y.w*b.w.x;
 out.y.y=a.y.x*b.x.y+a.y.y*b.y.y+a.y.z*b.z.y+a.y.w*b.w.y;
 out.y.z=a.y.x*b.x.z+a.y.y*b.y.z+a.y.z*b.z.z+a.y.w*b.w.z;
 out.y.w=a.y.x*b.x.w+a.y.y*b.y.w+a.y.z*b.z.w+a.y.w*b.w.w;

 out.z.x=a.z.x*b.x.x+a.z.y*b.y.x+a.z.z*b.z.x+a.z.w*b.w.x;
 out.z.y=a.z.x*b.x.y+a.z.y*b.y.y+a.z.z*b.z.y+a.z.w*b.w.y;
 out.z.z=a.z.x*b.x.z+a.z.y*b.y.z+a.z.z*b.z.z+a.z.w*b.w.z;
 out.z.w=a.z.x*b.x.w+a.z.y*b.y.w+a.z.z*b.z.w+a.z.w*b.w.w;

 out.w.x=a.w.x*b.x.x+a.w.y*b.y.x+a.w.z*b.z.x+a.w.w*b.w.x;
 out.w.y=a.w.x*b.x.y+a.w.y*b.y.y+a.w.z*b.z.y+a.w.w*b.w.y;
 out.w.z=a.w.x*b.x.z+a.w.y*b.y.z+a.w.z*b.z.z+a.w.w*b.w.z;
 out.w.w=a.w.x*b.x.w+a.w.y*b.y.w+a.w.z*b.z.w+a.w.w*b.w.w;
}
//----------------------------------------------------------------------------------------------------
//������������ ������� ���� SGuMatrix4
//----------------------------------------------------------------------------------------------------
void CSGL::NormaliseSGuVector4(SGuVector4& v)
{
 const float norma=sqrtf(v.x*v.x+v.y*v.y+v.z*v.z);
 const float scale=1.0f/norma;
 v.x*=scale;
 v.y*=scale;
 v.z*=scale;
 v.w*=scale;
}
//----------------------------------------------------------------------------------------------------
//��������� ������������ ������� ���� SGuMatrix4
//----------------------------------------------------------------------------------------------------
double CSGL::GetDeterminantSGuMatrix4(const SGuMatrix4& matrix)
{
 double a=matrix.x.x;
 double b=matrix.x.y;
 double c=matrix.x.z;
 double d=matrix.x.w;

 double e=matrix.y.x;
 double f=matrix.y.y;
 double g=matrix.y.z;
 double h=matrix.y.w;

 double i=matrix.z.x;
 double j=matrix.z.y;
 double k=matrix.z.z;
 double l=matrix.z.w;

 double m=matrix.w.x;
 double n=matrix.w.y;
 double o=matrix.w.z;
 double p=matrix.w.w;

 double det=a*(f*(k*p-l*o)+g*(l*n-j*p)+h*(j*o-k*n));
 det+=-b*(e*(k*p-l*o)+g*(l*m-i*p)+h*(i*o-k*m));
 det+=c*(e*(j*p-l*n)+f*(l*m-i*p)+h*(i*n-j*m));
 det+=-d*(e*(j*o-k*n)+f*(k*m-i*o)+g*(i*n-j*m));
 return(det);
}
//----------------------------------------------------------------------------------------------------
//��������� ������������ ������� ���� SGuMatrix3
//----------------------------------------------------------------------------------------------------
double CSGL::GetDeterminantSGuMatrix3(const SGuMatrix3& matrix)
{
 double a=matrix.x.x;
 double b=matrix.x.y;
 double c=matrix.x.z;

 double d=matrix.y.x;
 double e=matrix.y.y;
 double f=matrix.y.z;

 double g=matrix.z.x;
 double h=matrix.z.y;
 double i=matrix.z.z;

 double det=b*(f*g-d*i)+c*(d*h-e*g)+a*(e*i-f*h);
 return(det);
}

//----------------------------------------------------------------------------------------------------
//��������� ������� � ����������� ������� � �������� �� ����������� y � x ��� ������� ���� SGuMatrix4
//----------------------------------------------------------------------------------------------------
void CSGL::GetTruncatedMatrixSGuMatrix4(long y,long x,const SGuMatrix4& input_matrix,SGuMatrix3& output_matrix)
{
 float i_matrix[4][4];
 i_matrix[0][0]=input_matrix.x.x;
 i_matrix[0][1]=input_matrix.x.y;
 i_matrix[0][2]=input_matrix.x.z;
 i_matrix[0][3]=input_matrix.x.w;

 i_matrix[1][0]=input_matrix.y.x;
 i_matrix[1][1]=input_matrix.y.y;
 i_matrix[1][2]=input_matrix.y.z;
 i_matrix[1][3]=input_matrix.y.w;

 i_matrix[2][0]=input_matrix.z.x;
 i_matrix[2][1]=input_matrix.z.y;
 i_matrix[2][2]=input_matrix.z.z;
 i_matrix[2][3]=input_matrix.z.w;

 i_matrix[3][0]=input_matrix.w.x;
 i_matrix[3][1]=input_matrix.w.y;
 i_matrix[3][2]=input_matrix.w.z;
 i_matrix[3][3]=input_matrix.w.w;

 float* o_matrix_ptr[3][3];
 o_matrix_ptr[0][0]=&output_matrix.x.x;
 o_matrix_ptr[0][1]=&output_matrix.x.y;
 o_matrix_ptr[0][2]=&output_matrix.x.z;

 o_matrix_ptr[1][0]=&output_matrix.y.x;
 o_matrix_ptr[1][1]=&output_matrix.y.y;
 o_matrix_ptr[1][2]=&output_matrix.y.z;

 o_matrix_ptr[2][0]=&output_matrix.z.x;
 o_matrix_ptr[2][1]=&output_matrix.z.y;
 o_matrix_ptr[2][2]=&output_matrix.z.z;

 long n_i=0;
 for(long i=0;i<4;i++)
 {
  if (i==y) continue;
  long n_j=0;
  for(long j=0;j<4;j++)
  {
   if (j==x) continue;
   float element=i_matrix[i][j];
   *(o_matrix_ptr[n_i][n_j])=element;
   n_j++;
  }
  n_i++;
 }
}
//----------------------------------------------------------------------------------------------------
//��������� �������� ������� ��� ������� ���� SGuMatrix4
//----------------------------------------------------------------------------------------------------
bool CSGL::CreateInvertMatrixSGuMatrix4(const SGuMatrix4& input_matrix,SGuMatrix4& output_matrix)
{
 float *o_matrix_ptr[4][4];
 o_matrix_ptr[0][0]=&output_matrix.x.x;
 o_matrix_ptr[0][1]=&output_matrix.x.y;
 o_matrix_ptr[0][2]=&output_matrix.x.z;
 o_matrix_ptr[0][3]=&output_matrix.x.w;

 o_matrix_ptr[1][0]=&output_matrix.y.x;
 o_matrix_ptr[1][1]=&output_matrix.y.y;
 o_matrix_ptr[1][2]=&output_matrix.y.z;
 o_matrix_ptr[1][3]=&output_matrix.y.w;

 o_matrix_ptr[2][0]=&output_matrix.z.x;
 o_matrix_ptr[2][1]=&output_matrix.z.y;
 o_matrix_ptr[2][2]=&output_matrix.z.z;
 o_matrix_ptr[2][3]=&output_matrix.z.w;

 o_matrix_ptr[3][0]=&output_matrix.w.x;
 o_matrix_ptr[3][1]=&output_matrix.w.y;
 o_matrix_ptr[3][2]=&output_matrix.w.z;
 o_matrix_ptr[3][3]=&output_matrix.w.w;

 double determinant=GetDeterminantSGuMatrix4(input_matrix);
 if (determinant==0) return(false);
 SGuMatrix3 matrix3;
 for(long y=0;y<4;y++)
 {
  for(long x=0;x<4;x++)
  {
   GetTruncatedMatrixSGuMatrix4(y,x,input_matrix,matrix3);
   double detsubmatrix=GetDeterminantSGuMatrix3(matrix3);
   double value=detsubmatrix/determinant;
   if ((x+y)%2!=0) value=-value;
   *(o_matrix_ptr[x][y])=value;
  }
 }
 return(true);
}
//----------------------------------------------------------------------------------------------------
//��������� ��������� ���������
//----------------------------------------------------------------------------------------------------
void CSGL::CreateFrustrumPlane(void)
{
 //��������� ����� ������� modelview-projection
 //��������!!!
 //��� ��� ���������� � ������ (Vertex3f) ��� ���������� � ������ ������� �������������, ������� ��������� ����� ������ �� ������� �������������!
 //��������!!!

 SGuMatrix4 projection_model_view_matrix=ProjectionMatrix;
 //MultiplySGuMatrix4(ModelViewMatrix,ProjectionMatrix,projection_model_view_matrix);
 //��������� ������ ��������� ��������� �� �������� (����, ���, ����, �����)
 //�����
 FrustumPlane[0].x=projection_model_view_matrix.x.w+projection_model_view_matrix.x.x;
 FrustumPlane[0].y=projection_model_view_matrix.y.w+projection_model_view_matrix.y.x;
 FrustumPlane[0].z=projection_model_view_matrix.z.w+projection_model_view_matrix.z.x;
 FrustumPlane[0].w=projection_model_view_matrix.w.w+projection_model_view_matrix.w.x;
 NormaliseSGuVector4(FrustumPlane[0]);
 //������
 FrustumPlane[1].x=projection_model_view_matrix.x.w-projection_model_view_matrix.x.x;
 FrustumPlane[1].y=projection_model_view_matrix.y.w-projection_model_view_matrix.y.x;
 FrustumPlane[1].z=projection_model_view_matrix.z.w-projection_model_view_matrix.z.x;
 FrustumPlane[1].w=projection_model_view_matrix.w.w-projection_model_view_matrix.w.x;
 NormaliseSGuVector4(FrustumPlane[1]);
 //�������
 FrustumPlane[2].x=projection_model_view_matrix.x.w-projection_model_view_matrix.x.y;
 FrustumPlane[2].y=projection_model_view_matrix.y.w-projection_model_view_matrix.y.y;
 FrustumPlane[2].z=projection_model_view_matrix.z.w-projection_model_view_matrix.z.y;
 FrustumPlane[2].w=projection_model_view_matrix.w.w-projection_model_view_matrix.w.y;
 NormaliseSGuVector4(FrustumPlane[2]);
 //������
 FrustumPlane[3].x=projection_model_view_matrix.x.w+projection_model_view_matrix.x.y;
 FrustumPlane[3].y=projection_model_view_matrix.y.w+projection_model_view_matrix.y.y;
 FrustumPlane[3].z=projection_model_view_matrix.z.w+projection_model_view_matrix.z.y;
 FrustumPlane[3].w=projection_model_view_matrix.w.w+projection_model_view_matrix.w.y;
 NormaliseSGuVector4(FrustumPlane[3]);
}
//----------------------------------------------------------------------------------------------------
//�������� ����� ����������� ������ � ���������
//----------------------------------------------------------------------------------------------------
void CSGL::GetIntersectionPlaneAndLine(const SGuNVCTPoint& A,const SGuNVCTPoint& B,SGuNVCTPoint& new_point,float nx,float ny,float nz,float w)
{
 new_point=A;
 float ax=A.sGuVertex.x;
 float ay=A.sGuVertex.y;
 float az=A.sGuVertex.z;
 float au=A.sGuTexture.u;
 float av=A.sGuTexture.v;
 float ar=A.sGuColor.r;
 float ag=A.sGuColor.g;
 float ab=A.sGuColor.b;

 float bx=B.sGuVertex.x;
 float by=B.sGuVertex.y;
 float bz=B.sGuVertex.z;
 float bu=B.sGuTexture.u;
 float bv=B.sGuTexture.v;
 float br=B.sGuColor.r;
 float bg=B.sGuColor.g;
 float bb=B.sGuColor.b;

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
 SetVertexCoord(new_point.sGuVertex,vx,vy,vz);
 SetTextureCoord(new_point.sGuTexture,vu,vv);
 SetColorValue(new_point.sGuColor,(uint8_t)r,(uint8_t)g,(uint8_t)b);
}

//----------------------------------------------------------------------------------------------------
//��������� ��������� ���������
//----------------------------------------------------------------------------------------------------
void CSGL::Clip(const SGuNVCTPoint *point_array_input,uint16_t point_amount_input,SGuNVCTPoint *point_array_output,uint16_t &point_amount_output,float nx,float ny,float nz,float w)
{
 point_amount_output=0; 
 for(uint16_t n=0;n<point_amount_input;n++)
 {
  long next_p=n+1;
  if (next_p>=point_amount_input) next_p-=point_amount_input;

  const SGuNVCTPoint *sGuNVCTPoint_Current_Ptr=&(point_array_input[n]);
  float current_vx=sGuNVCTPoint_Current_Ptr->sGuVertex.x;
  float current_vy=sGuNVCTPoint_Current_Ptr->sGuVertex.y;
  float current_vz=sGuNVCTPoint_Current_Ptr->sGuVertex.z;
  //���������� ��������� ������������ ��������� ���������
  float current_ret=current_vx*nx+current_vy*ny+current_vz*nz+w;

  const SGuNVCTPoint *sGuNVCTPoint_Next_Ptr=&(point_array_input[next_p]);
  float next_vx=sGuNVCTPoint_Next_Ptr->sGuVertex.x;
  float next_vy=sGuNVCTPoint_Next_Ptr->sGuVertex.y;
  float next_vz=sGuNVCTPoint_Next_Ptr->sGuVertex.z;
  //���������� ��������� ������������ ��������� ���������
  float next_ret=next_vx*nx+next_vy*ny+next_vz*nz+w;

  if (current_ret>0)//������� ����� ������
  {
   if (next_ret>0)//��������� ����� ������
   {
    point_array_output[point_amount_output]=*sGuNVCTPoint_Next_Ptr;
	point_amount_output++;
   }
   else
   {
    //��������� ����� ����� �����������
    SGuNVCTPoint sGuNVCTPoint_New;
    GetIntersectionPlaneAndLine(*sGuNVCTPoint_Current_Ptr,*sGuNVCTPoint_Next_Ptr,sGuNVCTPoint_New,nx,ny,nz,w);
    point_array_output[point_amount_output]=sGuNVCTPoint_New;
	point_amount_output++;
   }
  }
  else//������� ����� �� �����
  {
   if (next_ret>0)//��������� ����� �����
   {
    //��������� ����� ����� �����������
    SGuNVCTPoint sGuNVCTPoint_New;
    GetIntersectionPlaneAndLine(*sGuNVCTPoint_Current_Ptr,*sGuNVCTPoint_Next_Ptr,sGuNVCTPoint_New,nx,ny,nz,w);
    point_array_output[point_amount_output]=sGuNVCTPoint_New;
	point_amount_output++;
    //��������� ��������� �����
    point_array_output[point_amount_output]=*sGuNVCTPoint_Next_Ptr;
	point_amount_output++;
   }
  }
 }
}

//----------------------------------------------------------------------------------------------------
//������� �����������
//----------------------------------------------------------------------------------------------------
void CSGL::OutputTriangle(SGuNVCTPoint A,SGuNVCTPoint B,SGuNVCTPoint C)
{
 //��������� ��������� ��������� ��� ���� �������������� ���������� 
 SGuNVCTPoint sGuNVCTPoint[2][3*2*4];//������ �������� ������
 
 sGuNVCTPoint[0][0]=A;
 sGuNVCTPoint[0][1]=B;
 sGuNVCTPoint[0][2]=C;
 
 uint16_t point_amount=3;
 uint8_t input_index=0;
 uint8_t output_index=1;
 
 for(uint8_t n=0;n<4;n++)
 {
  float nx=FrustumPlane[n].x;
  float ny=FrustumPlane[n].y;
  float nz=FrustumPlane[n].z;
  float w=FrustumPlane[n].w;  
  Clip(sGuNVCTPoint[input_index],point_amount,sGuNVCTPoint[output_index],point_amount,nx,ny,nz,w);
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
  DrawTriangle(sGuNVCTPoint[input_index][0],sGuNVCTPoint[input_index][i1],sGuNVCTPoint[input_index][i2]);
 } 
}
