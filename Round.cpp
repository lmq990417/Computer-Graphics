#include<Windows.h>
#include<glut.h>
#include<math.h>
#include<stdio.h>
#include<stdlib.h>
#define PI 3.1415926
//int border[3] = {};
int lineWidth = 1;



//画弧线，相对偏移量XY，开始的弧度，结束的弧度，半径
void glArc(double x, double y, double z, double start_angle, double end_angle, double radius, int MODE)
{
	//开始绘制曲线
	glBegin(MODE);
	//每次画增加的弧度
	double delta_angle = PI / 180;
	//画圆弧
	for (double i = start_angle; i <= end_angle; i += delta_angle)
	{
		//绝对定位加三角函数值
		double vx = x + radius * cos(i);
		double vy = y + radius * sin(i);
		glVertex3d(vx, vy,z);
	}
	//结束绘画
	glEnd();
}
//画圆
void glCircle(double x, double y, double z,double radius, int MODE)
{
	//画全圆
	glArc(x, y, z, 0, 2 * PI, radius, MODE);
}
//画填充的圆形
void glFillCircle(double x, double y, double z, double radius, int color[3]) {
	glColor3ub(color[0], color[1], color[2]);
	glCircle(x, y, z, radius - lineWidth / 2, GL_POLYGON);
}