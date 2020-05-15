#include<Windows.h>
#include<glut.h>
#include<math.h>
#include<stdio.h>
#include<stdlib.h>
#define PI 3.1415926
//int border[3] = {};
int lineWidth = 1;



//�����ߣ����ƫ����XY����ʼ�Ļ��ȣ������Ļ��ȣ��뾶
void glArc(double x, double y, double z, double start_angle, double end_angle, double radius, int MODE)
{
	//��ʼ��������
	glBegin(MODE);
	//ÿ�λ����ӵĻ���
	double delta_angle = PI / 180;
	//��Բ��
	for (double i = start_angle; i <= end_angle; i += delta_angle)
	{
		//���Զ�λ�����Ǻ���ֵ
		double vx = x + radius * cos(i);
		double vy = y + radius * sin(i);
		glVertex3d(vx, vy,z);
	}
	//�����滭
	glEnd();
}
//��Բ
void glCircle(double x, double y, double z,double radius, int MODE)
{
	//��ȫԲ
	glArc(x, y, z, 0, 2 * PI, radius, MODE);
}
//������Բ��
void glFillCircle(double x, double y, double z, double radius, int color[3]) {
	glColor3ub(color[0], color[1], color[2]);
	glCircle(x, y, z, radius - lineWidth / 2, GL_POLYGON);
}