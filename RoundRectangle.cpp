#include<Windows.h>
#include<glut.h>
#include<math.h>
#include<stdio.h>
#include<stdlib.h>
#define PI1 3.1415926
int border1[3] = { 0, 0, 0 };
int lineWidth1 = 2;



//��Բ�Ǿ��Σ�������ο�ߣ��ǰ뾶���������ĵ�����
void glRoundRec(int centerX, int centerY, int z, int width, int height, float cirR, int MODE) {
	//����֮PI��һ�����޵ĽǶ�
	float PI_HALF = PI1 / 2;
	//���̶ֳ�,ֵԽ�󻭵�Խ��ϸ
	float divide = 50.0;
	//Բ�Ǿ��ε�����
	float tx, ty;
	//���������
	glBegin(MODE);
	//�ĸ����޲�ͬ�Ĳ�����
	int opX[4] = { 1,-1,-1,1 };
	int opY[4] = { 1,1,-1,-1 };
	//�����������ӵ�һ���޵���������
	float x = 0;
	//x����ʱ�ӵ�ֵ
	float part = 1 / divide;
	//�����ھ��ο��һ�����ֵ
	int w = width / 2 - cirR;
	int h = height / 2 - cirR;
	//ѭ������
	for (x = 0; x < 4; x += part) {
		//�������
		float rad = PI_HALF * x;
		//��������ֵ
		tx = cirR * cos(rad) + opX[(int)x] * w + centerX;
		ty = cirR * sin(rad) + opY[(int)x] * h + centerY;
		//�������껭��
		glVertex3f(tx, ty, z);
	}
	//��������
	glEnd();
}


//������Բ�Ǿ���
void glFillRoundRec(int centerX, int centerY, int z, int width, int height, float cirR, int color[3]) {
	glColor3ub(border1[0], border1[1], border1[2]);
	glRoundRec(centerX, centerY, z,width, height, cirR, GL_LINE_LOOP);
	glColor3ub(color[0], color[1], color[2]);
	glRoundRec(centerX, centerY, z, width - lineWidth1 / 2, height - lineWidth1 / 2, cirR, GL_POLYGON);
}
