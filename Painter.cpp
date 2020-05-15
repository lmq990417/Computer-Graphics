#include<windows.h>
#include<glut.h>
#include<stdlib.h>
#include<math.h>
#include<stdio.h>


typedef struct point
{
	float x, y;
}point;

void glFillCircle(double x, double y, double z, double radius, int color[3]);
int ball1[] = {255,0,0};
int ball2[] = {240,128,128};
int ball3[] = {255,192,203};
float ball11[] = { 255/255,0.0,0.0 };
float ball22[] = { 240/255,128/255,128/255 };
float ball33[] = { 1.0,192/255,203/255 };
void BresenhamCircle(int x0, int y0, int z0, int r, float RGB[]);
void Bezier(point* ctrlPt, float z, int nCtrlPts, int nBezPts, float RGB[]);

void painter(int z1, int z2, int z3)
{
	int b[] = { 0,0,0 };
	int a[] = { 0,0,0 };
	a[0] = z1;
	a[1] = z2;
	a[2] = z3;
	b[0] = z1;
	b[1] = z2;
	b[2] = z3;

	//根据传入的z值进行冒泡排序，由近到远进行油画家算法的绘图
	for (int i = 0; i < 3; i++) {
		for (int j = i + 1; j < 3; j++) {
			if (a[i] > a[j]) {
				int temp = 0;
				temp = a[j];
				a[j] = a[i];
				a[i] = temp;
			}
		}
	}

	for (int k = 0; k < 3; k++) {
		if (a[k] == b[0]) {
			glFillCircle(-50, 380, a[k], 20, ball3);
			BresenhamCircle(-50,356,a[k],4,ball33);
		}
		if (a[k] = b[1]) {
			glFillCircle(-40, 400, a[k], 20, ball1);
			BresenhamCircle(-40, 376, a[k], 4, ball11);
		}
		if (a[k] = b[2]) {
			glFillCircle(-30, 380, a[k], 20, ball2);
			BresenhamCircle(-30, 356, a[k], 4, ball22);
		}
	}

}