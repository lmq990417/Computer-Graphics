#include<windows.h>
#include<glut.h>
#include<stdlib.h>
#include<math.h>
#include<stdio.h>

void setPixel(int x, int y, int z,float RGB[]) {
	glColor3f(RGB[0], RGB[1], RGB[2]);
	glBegin(GL_POINTS);
	glVertex3i(x, y, z);
	glEnd();
}