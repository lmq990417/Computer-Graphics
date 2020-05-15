#include <glut.h>
#include <stdlib.h>
#include <math.h>

typedef struct point
{
	float x, y;
}point;

void setPixel(point bezierPt, float z,float RGB[])
{
	glColor3f(RGB[0], RGB[1], RGB[2]);
	glPointSize(2.0);
	glBegin(GL_POINTS);
	glVertex3f(bezierPt.x, bezierPt.y,z);
	glEnd();
}

int binomialCoeffs(int k, int n)
{
	int i, c = 1;
	if (k == 0)
		return c;
	else
	{
		for (i = n - k + 1; i <= n; i++)
			c *= i;
		for (i = 1; i <= k; i++)
			c /= i;
		return c;
	}
}

void computeBezierPt(float u, point* bezierPt, point* ctrlPt, int nCtrlPts)
{
	int k = 0;
	float bernstein;
	bezierPt->x = 0;
	bezierPt->y = 0;
	for (k = 0; k <= nCtrlPts - 1; k++)
	{
		bernstein = binomialCoeffs(k, nCtrlPts - 1) * pow(u, k) * pow(1 - u, nCtrlPts - 1 - k);
		bezierPt->x += ctrlPt[k].x * bernstein;
		bezierPt->y += ctrlPt[k].y * bernstein;
	}
}

void Bezier(point* ctrlPt,float z, int nCtrlPts, int nBezPts, float RGB[])
{
	point bezierPt;
	float u;
	int k;
	for (k = 0; k <= nBezPts; k++)
	{
		u = float(k) / float(nBezPts);
		computeBezierPt(u, &bezierPt, ctrlPt, nCtrlPts);
		setPixel(bezierPt,z,RGB);
	}
}

