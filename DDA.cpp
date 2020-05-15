#include<windows.h>
#include<glut.h>
#include<stdlib.h>
#include<math.h>
#include<stdio.h>

void setPixel(int x, int y, int z, float RGB[]);

inline int round(const float a) {
	return int(a + 0.5);
}
//DDAÖ±ÏßËã·¨
void lineDDA(int x0, int y0, int x1, int y1, int z, float RGB[])
{
	int dx = x1 - x0, dy = y1 - y0, step, k;
	float xIncrement, yIncrement, x = x0, y = y0;

	if (fabs(dx) > fabs(dy))
		step = fabs(dx);
	else
		step = fabs(dy);
	xIncrement = float(dx) / float(step);
	yIncrement = float(dy) / float(step);

	setPixel(round(x), round(y), z, RGB);
	for (k = 0; k < step; k++)
	{
		x = x + xIncrement;
		y = y + yIncrement;
		setPixel(round(x), round(y), z, RGB);
	}
}