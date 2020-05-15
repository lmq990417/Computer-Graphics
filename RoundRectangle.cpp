#include<Windows.h>
#include<glut.h>
#include<math.h>
#include<stdio.h>
#include<stdlib.h>
#define PI1 3.1415926
int border1[3] = { 0, 0, 0 };
int lineWidth1 = 2;



//画圆角矩形，传入矩形宽高，角半径，矩形中心点坐标
void glRoundRec(int centerX, int centerY, int z, int width, int height, float cirR, int MODE) {
	//二分之PI，一个象限的角度
	float PI_HALF = PI1 / 2;
	//划分程度,值越大画得越精细
	float divide = 50.0;
	//圆角矩形的坐标
	float tx, ty;
	//画封闭曲线
	glBegin(MODE);
	//四个象限不同的操作符
	int opX[4] = { 1,-1,-1,1 };
	int opY[4] = { 1,1,-1,-1 };
	//用来计数，从第一象限到第四象限
	float x = 0;
	//x自增时加的值
	float part = 1 / divide;
	//计算内矩形宽高一半的数值
	int w = width / 2 - cirR;
	int h = height / 2 - cirR;
	//循环画线
	for (x = 0; x < 4; x += part) {
		//求出弧度
		float rad = PI_HALF * x;
		//计算坐标值
		tx = cirR * cos(rad) + opX[(int)x] * w + centerX;
		ty = cirR * sin(rad) + opY[(int)x] * h + centerY;
		//传入坐标画线
		glVertex3f(tx, ty, z);
	}
	//结束画线
	glEnd();
}


//画填充的圆角矩形
void glFillRoundRec(int centerX, int centerY, int z, int width, int height, float cirR, int color[3]) {
	glColor3ub(border1[0], border1[1], border1[2]);
	glRoundRec(centerX, centerY, z,width, height, cirR, GL_LINE_LOOP);
	glColor3ub(color[0], color[1], color[2]);
	glRoundRec(centerX, centerY, z, width - lineWidth1 / 2, height - lineWidth1 / 2, cirR, GL_POLYGON);
}
