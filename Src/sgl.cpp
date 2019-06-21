#include "sgl.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//-����������� ������--------------------------------------------------------
CSGL::CSGL(void)
{ 	
 ImageMap=NULL;
 ZBuffer=NULL;
 CurrentColor=0xffff;
 CurrentSelectedMatrix=2;
 LoadIdentity();
 CurrentSelectedMatrix=1;
 LoadIdentity();
 PointArrayAmount=0;
 DrawModeActive=false;
 EnableDepthText=false;
}
//-���������� ������---------------------------------------------------------
CSGL::~CSGL(void)
{
 if (ImageMap!=NULL) delete[](ImageMap);
 //if (ImageMap!=NULL) free(ImageMap);
 ImageMap=NULL;
 if (ZBuffer!=NULL) delete[](ZBuffer);
 //if (ZBuffer!=NULL) free(ZBuffer);
 ZBuffer=NULL;
}
//-������� ������------------------------------------------------------------
bool CSGL::Create(int screen_width,int screen_height)
{
 if (ImageMap!=NULL) delete[](ImageMap);
 if (ZBuffer!=NULL) delete[](ZBuffer);
 //if (ImageMap!=NULL) free(ImageMap);
 //if (ZBuffer!=NULL) free(ZBuffer);
 ImageMap=(unsigned short*)malloc((screen_width*screen_height+1)*sizeof(unsigned short));
 ZBuffer=(float*)malloc((screen_width*screen_height+1)*sizeof(float));
 ScreenWidth=screen_width;
 ScreenHeight=screen_height;
 return(true);
}
bool CSGL::MatrixProductfv(float* Matrix1,float* Matrix2,float* Matrix3)
{
 float matrix[16];
 for(int x=0;x<4;x++)
 {
  for(int y=0;y<4;y++)
  {
   float product=0;
   for(int i=0;i<4;i++) product+=Matrix1[i+y*4]*Matrix2[x+i*4];
   matrix[x+y*4]=product;
  }
 }
 memcpy(Matrix3,matrix,sizeof(float)*16);
 return(true);
}
bool CSGL::VectorProductMatrixfv(float *Vector,float* Matrix,float *ReturnVector)
{
 float vector[4];
 for(int y=0;y<4;y++)
 {
  float product=0;
  for(int x=0;x<4;x++) product+=Matrix[x*4+y]*Vector[x];
  vector[y]=product;
 }
 memcpy(ReturnVector,vector,sizeof(float)*4);
 return(true);
}
bool CSGL::MatrixProductVectorfv(float* Matrix,float *Vector,float *ReturnVector)
{
 float vector[4];
 for(int y=0;y<4;y++)
 {
  float product=0;
  for(int x=0;x<4;x++) product+=Matrix[y*4+x]*Vector[x];
  vector[y]=product;
 }
 memcpy(ReturnVector,vector,sizeof(float)*4);
 return(true);
}
bool CSGL::LoadIdentity(void)
{
 if (CurrentSelectedMatrix==1)
 {
  for(int m=0;m<16;m++) ProjectionMatrix[m]=0;
  for(int n=0;n<4;n++) ProjectionMatrix[n*5]=1;
 }
 if (CurrentSelectedMatrix==2)
 {
  for(int m=0;m<16;m++) ModelViewMatrix[m]=0;
  for(int n=0;n<4;n++) ModelViewMatrix[n*5]=1;
 }
 return(true);
}
bool CSGL::Frustrum(float Left,float Right,float Bottom,float Top,float Near,float Far)
{
 if (Right==Left) return(false);
 if (Top==Bottom) return(false);
 if (Near==Far) return(false);
 float Matrix[16];
 for(int n=0;n<16;n++) Matrix[n]=0;
 Matrix[0]=2*Near/(Right-Left);
 Matrix[2]=(Right+Left)/(Right-Left);
 Matrix[5]=2*Near/(Top-Bottom);
 Matrix[6]=(Top+Bottom)/(Top-Bottom);
 Matrix[10]=-(Far+Near)/(Far-Near);
 Matrix[11]=-2*Far*Near/(Far-Near);
 Matrix[14]=-1;
 MatrixProductfv(ProjectionMatrix,Matrix,ProjectionMatrix);
 return(true);
}
bool CSGL::SetViewport(float x,float y,float len,float hgt)
{
 Viewport[0]=x;
 Viewport[1]=y;
 Viewport[2]=len;
 Viewport[3]=hgt;
 return(true);
}
bool CSGL::Perspective(float Fovy,float Aspect,float Near,float Far)
{
 float Top=(float)tan(M_PI/180.0*Fovy/2)*Near;
 float Bottom=-Top;
 float Right=Top*Aspect;
 float Left=-Right;
 return(Frustrum(Left,Right,Bottom,Top,Near,Far));
}
bool CSGL::Rotatef(float angle,float nx,float ny,float nz)
{
 float norma=(float)sqrt(nx*nx+ny*ny+nz*nz);
 if (norma!=0)
 {
  nx=nx/norma;
  ny=ny/norma;
  nz=nz/norma;
 }
 float cf=(float)cos(M_PI/180*angle);
 float sf=(float)sin(M_PI/180*angle);
 float Matrix[16];
 Matrix[0]=nx*nx+cf*(1-nx*nx);
 Matrix[1]=nx*(1-cf)*ny+nz*sf;
 Matrix[2]=nx*(1-cf)*nz-ny*sf;
 Matrix[3]=0;
 Matrix[4]=nx*(1-cf)*ny-nz*sf;
 Matrix[5]=ny*ny+cf*(1-ny*ny);
 Matrix[6]=ny*(1-cf)*nz+nx*sf;
 Matrix[7]=0;
 Matrix[8]=nx*(1-cf)*nz+ny*sf;
 Matrix[9]=ny*(1-cf)*nz-nx*sf;
 Matrix[10]=nz*nz+cf*(1-nz*nz);
 Matrix[11]=0;
 Matrix[12]=0;
 Matrix[13]=0;
 Matrix[14]=0;
 Matrix[15]=1;
 if (CurrentSelectedMatrix==1) MatrixProductfv(Matrix,ProjectionMatrix,ProjectionMatrix);
 if (CurrentSelectedMatrix==2) MatrixProductfv(Matrix,ModelViewMatrix,ModelViewMatrix);
 return(true);
}
bool CSGL::Translatef(float nx,float ny,float nz)
{
 float Matrix[16];
 Matrix[0]=1;
 Matrix[1]=0;
 Matrix[2]=0;
 Matrix[3]=0;
 Matrix[4]=0;
 Matrix[5]=1;
 Matrix[6]=0;
 Matrix[7]=0;
 Matrix[8]=0;
 Matrix[9]=0;
 Matrix[10]=1;
 Matrix[11]=0;
 Matrix[12]=nx;
 Matrix[13]=ny;
 Matrix[14]=nz;
 Matrix[15]=1;
 if (CurrentSelectedMatrix==1) MatrixProductfv(Matrix,ProjectionMatrix,ProjectionMatrix);
 if (CurrentSelectedMatrix==2) MatrixProductfv(Matrix,ModelViewMatrix,ModelViewMatrix);
 return(true);
}
bool CSGL::MatrixMode(int matrix)
{
 if (matrix==SGL_MATRIX_PROJECTION) CurrentSelectedMatrix=1;
 if (matrix==SGL_MATRIX_MODELVIEW) CurrentSelectedMatrix=2;
 return(true);
}
bool CSGL::Color3i(unsigned char r,unsigned char g,unsigned char b)
{
 CurrentColor=r>>3;
 CurrentColor<<=6;
 CurrentColor|=g>>2;
 CurrentColor<<=5;
 CurrentColor|=b>>3;
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
 if (PointArrayAmount<3) return(false);//����� � ����� �� �� ������
 SSGLPoint TPoint[5];
 for(int n=1;n<PointArrayAmount-1;n++)
 {
  int i1=n;
  int i2=n+1;
  TPoint[0]=SSGLPointArray[0];
  TPoint[1]=SSGLPointArray[i1];
  TPoint[2]=SSGLPointArray[i2];
  PutTriangle(TPoint);
 }
 return(true);
}
bool CSGL::Vertex3f(float x,float y,float z)
{
 if (DrawModeActive==false) return(false);//����� ������ �� �����������
 if (PointArrayAmount>=100) return(false);//��� ����� � �������
 //�������� �������������� ����� ������������ � ������ ������� �������������
 float vector_in[4];
 float vector_out[4];
 vector_in[0]=x;
 vector_in[1]=y;
 vector_in[2]=z;
 vector_in[3]=1;
 VectorProductMatrixfv(vector_in,ModelViewMatrix,vector_out);
 //������ ��, ��� ���������� � ������
 SSGLPointArray[PointArrayAmount].X=vector_out[0]/vector_out[3];;
 SSGLPointArray[PointArrayAmount].Y=vector_out[1]/vector_out[3];
 SSGLPointArray[PointArrayAmount].Z=vector_out[2]/vector_out[3];
 SSGLPointArray[PointArrayAmount].Color=CurrentColor;
 PointArrayAmount++;
 return(true);
}
bool CSGL::PutTriangle(SSGLPoint *TVertex)
{
 if (ImageMap==NULL) return(false);
 float m1,m2;
 //��������, ��� ����������� ����� ������������
 if (TVertex[0].Z>-1 && TVertex[1].Z>-1 && TVertex[2].Z>-1) return(true);//���� ����������� �� ����� (�� ������)
 if (TVertex[0].Z<=-1 && TVertex[1].Z<=-1 && TVertex[2].Z<=-1) //���� ����������� ����� (����� �������)
  {
   DrawTriangle(TVertex[0],TVertex[1],TVertex[2]);
   return(true);
  }
 //��������� ��� ������: ����� ������� ���� ��� ��� ����� ������������.
 //����� ����� ��������� ���������� ����������� ������ ������������ � ���������� �������������
 //� ��������, ��� �������� ����� ������������
 //��������, ����� ����� ���� �����
 int si1=0;
 int si2=0;
 int si3=0;
 int mode=0;
 if (TVertex[0].Z>-1 && TVertex[1].Z>-1)
 {
  si1=0;
  si2=1;
  si3=2;
  mode=1;
 }
 if (TVertex[0].Z>-1 && TVertex[2].Z>-1)
 {
  si1=0;
  si2=2;
  si3=1;
  mode=2;
 }
 if (TVertex[1].Z>-1 && TVertex[2].Z>-1)
 {
  si1=2;
  si2=1;
  si3=0;
  mode=3;
 }
 if (mode!=0)
 {
  m1=(-1-TVertex[si1].Z)/(TVertex[si3].Z-TVertex[si1].Z);
  m2=(-1-TVertex[si2].Z)/(TVertex[si3].Z-TVertex[si2].Z);
  TVertex[3].X=TVertex[si1].X+(TVertex[si3].X-TVertex[si1].X)*m1;
  TVertex[3].Y=TVertex[si1].Y+(TVertex[si3].Y-TVertex[si1].Y)*m1;
  TVertex[3].Z=-1;
  TVertex[3].Color=TVertex[si1].Color;
  TVertex[4].X=TVertex[si2].X+(TVertex[si3].X-TVertex[si2].X)*m2;
  TVertex[4].Y=TVertex[si2].Y+(TVertex[si3].Y-TVertex[si2].Y)*m2;
  TVertex[4].Z=-1;
  TVertex[4].Color=TVertex[si2].Color;
  if (mode==1) DrawTriangle(TVertex[3],TVertex[4],TVertex[2]);
  if (mode==2) DrawTriangle(TVertex[3],TVertex[1],TVertex[4]);
  if (mode==3) DrawTriangle(TVertex[0],TVertex[4],TVertex[3]);   
  return(true);
 }
 //������ �������� ����� ����� ��� �����
 mode=0;
 if (TVertex[2].Z>-1)
 {
  si1=0;
  si2=1;
  si3=2;
  mode=1;
 }
 if (TVertex[0].Z>-1)
 {
  si1=2;
  si2=1;
  si3=0;
  mode=2;
 }
 if (TVertex[1].Z>-1)
 {
  si1=0;
  si2=2;
  si3=1;
  mode=3;
 }
 if (mode!=0)
 {
  m1=(-1-TVertex[si1].Z)/(TVertex[si3].Z-TVertex[si1].Z);
  m2=(-1-TVertex[si2].Z)/(TVertex[si3].Z-TVertex[si2].Z);
  TVertex[3].X=TVertex[si1].X+(TVertex[si3].X-TVertex[si1].X)*m1;
  TVertex[3].Y=TVertex[si1].Y+(TVertex[si3].Y-TVertex[si1].Y)*m1;
  TVertex[3].Z=-1;
  TVertex[3].Color=TVertex[si1].Color;
  TVertex[4].X=TVertex[si2].X+(TVertex[si3].X-TVertex[si2].X)*m2;
  TVertex[4].Y=TVertex[si2].Y+(TVertex[si3].Y-TVertex[si2].Y)*m2;
  TVertex[4].Z=-1;
  TVertex[3].Color=TVertex[si2].Color;
  if (mode==1)
  {
   DrawTriangle(TVertex[0],TVertex[1],TVertex[3]);
   DrawTriangle(TVertex[1],TVertex[3],TVertex[4]);
  }
  if (mode==2)
  {
   DrawTriangle(TVertex[2],TVertex[1],TVertex[3]);
   DrawTriangle(TVertex[1],TVertex[3],TVertex[4]);
  }
  if (mode==3)
  {
   DrawTriangle(TVertex[0],TVertex[2],TVertex[3]);
   DrawTriangle(TVertex[2],TVertex[3],TVertex[4]);
  }
  return(true);
 }
 return(true);
}
bool CSGL::DrawTriangle(SSGLPoint A,SSGLPoint B,SSGLPoint C)
{
 float vectorA[4],vectorB[4],vectorC[4];
 //�������� �������� ������ ������������
 vectorA[0]=A.X;
 vectorA[1]=A.Y;
 vectorA[2]=A.Z;
 vectorA[3]=1;
 vectorB[0]=B.X;
 vectorB[1]=B.Y;
 vectorB[2]=B.Z;
 vectorB[3]=1;
 vectorC[0]=C.X;
 vectorC[1]=C.Y;
 vectorC[2]=C.Z;
 vectorC[3]=1;
 MatrixProductVectorfv(ProjectionMatrix,vectorA,vectorA);
 MatrixProductVectorfv(ProjectionMatrix,vectorB,vectorB);
 MatrixProductVectorfv(ProjectionMatrix,vectorC,vectorC);
 A.X=(vectorA[0]/vectorA[3]+1)*Viewport[2]/2+Viewport[0];
 A.Y=(1-vectorA[1]/vectorA[3])*Viewport[3]/2+Viewport[1];
 B.X=(vectorB[0]/vectorB[3]+1)*Viewport[2]/2+Viewport[0];
 B.Y=(1-vectorB[1]/vectorB[3])*Viewport[3]/2+Viewport[1];
 C.X=(vectorC[0]/vectorC[3]+1)*Viewport[2]/2+Viewport[0];
 C.Y=(1-vectorC[1]/vectorC[3])*Viewport[3]/2+Viewport[1];
 SSGLPoint TemporaryPoint;
 if (A.Y>C.Y)
 {
  TemporaryPoint=A;
  A=C;
  C=TemporaryPoint;
 }
 if (A.Y>B.Y)
 {
  TemporaryPoint=A;
  A=B;
  B=TemporaryPoint;
 }
 if (C.Y<B.Y)
 {
  TemporaryPoint=C;
  C=B;
  B=TemporaryPoint;
 }
 //��������, �������� �� ����������� � ���� ���������
 float starty=A.Y;
 float endy=C.Y;
 if (starty==endy) return(true);//������ ��������
 if (starty>=Viewport[1]+Viewport[3]) return(true);//����������� �� �����
 if (endy<Viewport[0]) return(true);//����������� �� �����
 if (starty<Viewport[0]) starty=0;
 if (endy>=Viewport[1]+Viewport[3]) endy=Viewport[1]+Viewport[3]-1;
 float cx1=A.X+(starty-A.Y)*(C.X-A.X)/(C.Y-A.Y);
 float cx2=A.X+(starty-A.Y)*(B.X-A.X)/(B.Y-A.Y);
 float dcx1=(C.X-A.X)/(C.Y-A.Y);
 float dcx2=(B.X-A.X)/(B.Y-A.Y);

 float cz1=A.Z+(starty-A.Y)*(C.Z-A.Z)/(C.Y-A.Y);
 float cz2=A.Z+(starty-A.Y)*(B.Z-A.Z)/(B.Y-A.Y);
 float dcz1=(C.Z-A.Z)/(C.Y-A.Y);
 float dcz2=(B.Z-A.Z)/(B.Y-A.Y);
 int mode=0;
 for(float sy=starty;sy<=endy;sy++,cx1+=dcx1,cx2+=dcx2,cz1+=dcz1,cz2+=dcz2)
 {
  float x1,x2;
  float z1,z2;
  x1=cx1;
  z1=cz1;
  if (sy<B.Y)
  {
   x2=cx2;
   z2=cz2;
  }
  else
  {
   if (C.Y==B.Y)
   {
	  x2=B.X;
	  z2=B.Z;
   }
   else
   {
    if (mode==0)
	{
   cx2=B.X+(sy-B.Y)*(C.X-B.X)/(C.Y-B.Y);
	 dcx2=(C.X-B.X)/(C.Y-B.Y);
   cz2=B.Z+(sy-B.Y)*(C.Z-B.Z)/(C.Y-B.Y);
	 dcz2=(C.Z-B.Z)/(C.Y-B.Y);
	 mode=1;
	}
	x2=cx2;
	z2=cz2;
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
  }
  //������ ����� ������������
  float dz=0;
  if (x2!=x1) dz=(z2-z1)/(x2-x1);
  if (x2>=Viewport[0]+Viewport[2]) x2=Viewport[0]+Viewport[2]-1;
  if (x1<Viewport[0])
  {
   x1=Viewport[0];
   z1+=(Viewport[0]-x1)*dz;
  }
  unsigned short *vptr=ImageMap+((int)(x1)+(ScreenHeight-(int)(sy)-1)*ScreenWidth);
  if (EnableDepthText==false)//���� ������� �� ������������
  {
   float *depthptr=ZBuffer+(int)(x1-1)+(int)(ScreenHeight-(int)(sy)-1)*ScreenWidth;
   float z=z1;		
   for(float x=x1;x<=x2;x++,z+=dz,vptr++,depthptr++)
	 {
		*(vptr)=A.Color;
		*(depthptr)=z;
	 }
  }
  else//���� ������� ������������
  {
   float *depthptr=ZBuffer+(int)(x1-1)+(int)(ScreenHeight-(int)(sy)-1)*ScreenWidth;
   float z=z1;
   for(float x=x1;x<=x2;x++,z+=dz,vptr++,depthptr++)
   {
    if (*(depthptr)<z)
	  {
     *(vptr)=A.Color;
     *(depthptr)=z;
	  }
   }
  }
 }
 return(true);
}
bool CSGL::Clear(unsigned int mode)
{
 if (mode&SGL_COLOR_BUFFER_BIT && ImageMap!=NULL) memset(ImageMap,0,ScreenWidth*ScreenHeight*sizeof(unsigned short));
 if (mode&SGL_DEPTH_BUFFER_BIT && ZBuffer!=NULL) for(int n=0;n<ScreenWidth*ScreenHeight;n++) ZBuffer[n]=(float)(-10000000000000.0);
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
