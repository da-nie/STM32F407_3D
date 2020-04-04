#ifndef SGL_MATRIX_H
#define SGL_MATRIX_H

//****************************************************************************************************
//�������� � �������� ��� ����������� ������������
//****************************************************************************************************

//****************************************************************************************************
//������������ ����������
//****************************************************************************************************
#include <stdint.h>

//****************************************************************************************************
//���������
//****************************************************************************************************

//������ 4
struct SGLVector4
{
 float X;
 float Y;
 float Z;
 float W;
};
//������ 3
struct SGLVector3
{
 float X;
 float Y;
 float Z;
};
//������� 4x4
struct SGLMatrix4
{
 SGLVector4 X;
 SGLVector4 Y;
 SGLVector4 Z;
 SGLVector4 W;
};
//������� 3x3
struct SGLMatrix3
{
 SGLVector3 X;
 SGLVector3 Y;
 SGLVector3 Z;
};


//****************************************************************************************************
//��������� �������
//****************************************************************************************************

void MultiplySGLVector4ToSGLMatrix4(const SGLVector4& v,const SGLMatrix4& m,SGLVector4& v_out);//��������� ������� ���� SGLVector4 �� ������� ���� SGLMatrix4
void MultiplySGLMatrix4ToSGLVector4(const SGLMatrix4& m,const SGLVector4& v,SGLVector4& v_out);//��������� ������� ���� SGLMatrix4 �� ������ ���� SGLVector4
void MultiplySGLMatrix4(const SGLMatrix4& a,const SGLMatrix4& b,SGLMatrix4& out);//��������� ���� ������ ���� SGLMatrix4
double GetNorma(SGLVector4& v);//�������� ����� ������� ���� SGLVector4
void NormaliseSGLVector4(SGLVector4& v);//������������ ������� ���� SGLVector4
double GetDeterminantSGLMatrix4(const SGLMatrix4& matrix);//��������� ������������ ������� ���� SGLMatrix4
double GetDeterminantSGLMatrix3(const SGLMatrix3& matrix);//��������� ������������ ������� ���� SGLMatrix3
void GetTruncatedMatrixSGLMatrix4(long y,long x,const SGLMatrix4& input_matrix,SGLMatrix3& output_matrix);//��������� ������� � ����������� ������� � �������� �� ����������� y � x ��� ������� ���� SGLMatrix4
bool CreateInvertMatrixSGLMatrix4(const SGLMatrix4& input_matrix,SGLMatrix4& output_matrix);//��������� �������� ������� ��� ������� ���� SGLMatrix4

#endif
