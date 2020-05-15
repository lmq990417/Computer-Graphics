#include <stdlib.h>
#include<stdio.h>
#include <glut.h>
#include<iostream>
#include <math.h>
using namespace std;

typedef struct point
{
	float x, y;
}point;

//���λ��
struct Point
{
	int x;
	int y;
};

//DDAֱ���㷨
void lineDDA(int x0, int y0, int x1, int y1, int z, float RGB[]);
//Bresenham��Բ
void BresenhamCircle(int x0, int y0, int z0, int r, float RGB[]);
//�����ɨ������㷨
void PolyScan(int POINTNUM, int z, struct Point polypoint[100]);
//���Բ���㷨
void glFillCircle(double x, double y, double z, double radius, int color[3]);
//���Բ�Ǿ����㷨
void glFillRoundRec(int centerX, int centerY, int z, int width, int height, float cirR, int color[3]);
//�����������㷨
void Bezier(point* ctrlPt, float z, int nCtrlPts, int nBezPts, float RGB[]);
//�����㷨
void painter(int z1, int z2, int z3);


//���ڴ�Сֵ
GLfloat w = 1000;
GLfloat h = 1000;

double rotate_x = 0.0;
double rotate_y = 0.0;
double rotate_z = 0.0;
//��ɫ����
# define  white  1.0, 1.0, 1.0  
#define   yellow 1.0, 1.0, 0.0
#define   pink  255/255.0,182/255.0,193/255.0
# define  gray   0.502, 0.502,0.502
# define  hgray  0.117, 0.180,0.227
#define   purple1 138/255.0,43/255.0,226/255.0
#define   purple 147/255.0,112/255.0,219/255.0
# define  green 124/255.0,252.0/255.0,0.0
# define  brown  210.0/255.0, 105.0/255.0,  30.0/255.0
# define  nbrown 128.0/255.0, 64.0/255.0, 64.0/255.0
# define  water 141.0/255.0, 193.0/255.0, 241.0/255.0
# define  sunshine 255.0/255.0, 210.0/255.0, 166.0/255.0
# define  pi  3.1415926

 //���峤�����8������
double fang[8][3];    
// �������εĳ�����8������
double san[8][3];              

//�������������
GLuint texGround;
GLuint texPool;
GLuint texFloor;
GLuint texWall;

//��ת������ͼ
GLuint texMoon;
GLuint sign = 1;
GLUquadricObj* qobj;
GLfloat  corner = 0.0;
//ͼ���������ڴ���е�ƫ����
#define BMP_Header_Length 54  

//��ת�Ƕ�
static GLfloat angle = 0.0f;   


//�������Ų���
GLfloat sca[3] = { 1.5, 1.5, 1.5 };
//����ת������
GLfloat tra[3] = { -300, 0, -470 };


GLfloat AngleX;
GLfloat AngleY;

//Ӿ����������ƶ�����
GLfloat bx = 0; 
GLfloat by = 0;
GLfloat bz = 0;



GLfloat matiral_specular[4] = { 0.00, 0.00, 0.00, 1.00 };  //���ʲ��������淴�䣬���õ��Ĺ��ǿ��
GLfloat matiral_emission[4] = { 0.00, 0.00, 0.00, 1.00 };  //���ʱ���΢���ⷢ�����
const GLfloat matiral_shininess = 11.00;
void setMetiral(GLfloat matiral_diffuse_ambient[4])
{
	//������ɫ
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matiral_diffuse_ambient); //������ֵͬ������
	//���ò���
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matiral_specular);  //���淴�䣬���õ��Ĺ��ǿ�ȣ���ɫ��
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, matiral_emission);  //���ʱ����΢΢�����ⷢ����ߣ��������۾��о���������������ɫ 
	glMaterialf(GL_FRONT_AND_BACK, matiral_shininess, 0);   
	//����ָ����0~128�� ��ֵԽС������Խ�ֲڣ����Դ����Ĺ������䵽���棬Ҳ���Բ����ϴ�����㡣
	//��ֵԽ�󣬱�ʾ����Խ�����ھ��棬��Դ���䵽����󣬲�����С�����㡣

}

//���εĻ���
void cons(double x, double y, double z, double x1, double y1, double z1) {  
	san[0][0] = x;
	san[0][1] = y;
	san[0][2] = z;     //  ��0����

	san[1][0] = x;
	san[1][1] = y;
	san[1][2] = z + z1;   //  ��1����

	san[4][0] = x;
	san[4][1] = y + y1;
	san[4][2] = z;     //  ��4����

	san[5][0] = x;
	san[5][1] = y + y1;
	san[5][2] = z + z1 / 2;      //  ��5����
	for (int i = 0; i < 3; i++) {
		if (i == 0) {
			san[3][i] = san[0][i] + x1;
			san[2][i] = san[1][i] + x1;
			san[6][i] = san[4][i] + x1;
			san[7][i] = san[5][i] + x1;
		}
		else {
			san[3][i] = san[0][i];
			san[2][i] = san[1][i];
			san[6][i] = san[4][i];
			san[7][i] = san[5][i];
		}
	}
}

//������Ļ���
void constract(double x, double y, double z, double x1, double y1, double z1) { 
	fang[0][0] = x;
	fang[0][1] = y;
	fang[0][2] = z;        //  ��0����

	fang[1][0] = x;
	fang[1][1] = y;
	fang[1][2] = z + z1;     //  ��һ����

	fang[2][0] = x + x1;
	fang[2][1] = y;
	fang[2][2] = z + z1;     //  �ڶ�����

	fang[3][0] = x + x1;
	fang[3][1] = y;
	fang[3][2] = z;        //  ��������
	for (int i = 0; i < 4; i++) {  
		for (int j = 0; j < 3; j++) {
			if (j == 1)
				fang[i + 4][j] = fang[i][j] + y1;
			else
				fang[i + 4][j] = fang[i][j];
		}
	}
}

void build1() {
	glBegin(GL_POLYGON);
	glNormal3f(0.0, -1.0, 0.0);
	glVertex3f(san[0][0], san[0][1], san[0][2]);
	glVertex3f(san[1][0], san[1][1], san[1][2]);
	glVertex3f(san[2][0], san[2][1], san[2][2]);
	glVertex3f(san[3][0], san[3][1], san[3][2]);
	glEnd();   //     ����

	glBegin(GL_POLYGON);
	glNormal3f(-1.0, 0.0, 0.0);
	glVertex3f(san[1][0], san[1][1], san[1][2]);
	glVertex3f(san[0][0], san[0][1], san[0][2]);
	glVertex3f(san[4][0], san[4][1], san[4][2]);
	glVertex3f(san[5][0], san[5][1], san[5][2]);
	glEnd();   //     ����

	glBegin(GL_POLYGON);
	glNormal3f(1.0, 0.0, 0.0);
	glVertex3f(san[7][0], san[7][1], san[7][2]);
	glVertex3f(san[6][0], san[6][1], san[6][2]);
	glVertex3f(san[3][0], san[3][1], san[3][2]);
	glVertex3f(san[2][0], san[2][1], san[2][2]);
	glEnd();   //     ����

	glBegin(GL_POLYGON);
	glNormal3f(0.0, 0.0, 1.0);
	glVertex3f(san[5][0], san[5][1], san[5][2]);
	glVertex3f(san[6][0], san[6][1], san[6][2]);
	glVertex3f(san[2][0], san[2][1], san[2][2]);
	glVertex3f(san[1][0], san[1][1], san[1][2]);
	glEnd();   //     ǰ��

	glBegin(GL_POLYGON);
	glNormal3f(0.0, 0.0, -1.0);
	glVertex3f(san[0][0], san[0][1], san[0][2]);
	glVertex3f(san[3][0], san[3][1], san[3][2]);
	glVertex3f(san[7][0], san[7][1], san[7][2]);
	glVertex3f(san[4][0], san[4][1], san[4][2]);
	glEnd();   //     ����

	glBegin(GL_POLYGON);
	glNormal3f(0.0, 1.0, 0.0);
	glVertex3f(san[4][0], san[4][1], san[4][2]);
	glVertex3f(san[7][0], san[7][1], san[7][2]);
	glVertex3f(san[6][0], san[6][1], san[6][2]);
	glVertex3f(san[5][0], san[5][1], san[5][2]);
	glEnd();   //     ����
}

void build() {
	glBegin(GL_POLYGON);
	glNormal3f(0.0, -1.0, 0.0);
	glVertex3f(fang[0][0], fang[0][1], fang[0][2]);
	glVertex3f(fang[1][0], fang[1][1], fang[1][2]);
	glVertex3f(fang[2][0], fang[2][1], fang[2][2]);
	glVertex3f(fang[3][0], fang[3][1], fang[3][2]);
	glEnd();   //     ����

	glBegin(GL_POLYGON);
	glNormal3f(-1.0, 0.0, 0.0);
	glVertex3f(fang[1][0], fang[1][1], fang[1][2]);
	glVertex3f(fang[0][0], fang[0][1], fang[0][2]);
	glVertex3f(fang[4][0], fang[4][1], fang[4][2]);
	glVertex3f(fang[5][0], fang[5][1], fang[5][2]);
	glEnd();   //     ����

	glBegin(GL_POLYGON);
	glNormal3f(1.0, 0.0, 0.0);
	glVertex3f(fang[7][0], fang[7][1], fang[7][2]);
	glVertex3f(fang[6][0], fang[6][1], fang[6][2]);
	glVertex3f(fang[2][0], fang[2][1], fang[2][2]);
	glVertex3f(fang[3][0], fang[3][1], fang[3][2]);
	glEnd();   //     ����

	glBegin(GL_POLYGON);
	glNormal3f(0.0, 0.0, 1.0);
	glVertex3f(fang[5][0], fang[5][1], fang[5][2]);
	glVertex3f(fang[6][0], fang[6][1], fang[6][2]);
	glVertex3f(fang[2][0], fang[2][1], fang[2][2]);
	glVertex3f(fang[1][0], fang[1][1], fang[1][2]);
	glEnd();   //     ǰ��

	glBegin(GL_POLYGON);
	glNormal3f(0.0, 0.0, -1.0);
	glVertex3f(fang[0][0], fang[0][1], fang[0][2]);
	glVertex3f(fang[3][0], fang[3][1], fang[3][2]);
	glVertex3f(fang[7][0], fang[7][1], fang[7][2]);
	glVertex3f(fang[4][0], fang[4][1], fang[4][2]);
	glEnd();   //     ����

	glBegin(GL_POLYGON);
	glNormal3f(0.0, 1.0, 0.0);
	glVertex3f(fang[4][0], fang[4][1], fang[4][2]);
	glVertex3f(fang[7][0], fang[7][1], fang[7][2]);
	glVertex3f(fang[6][0], fang[6][1], fang[6][2]);
	glVertex3f(fang[5][0], fang[5][1], fang[5][2]);
	glEnd();   //     ����
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();         
	//����M����
	glRotatef(rotate_x, 1.0, 0.0, 0.0);
	glRotatef(rotate_y, 0.0, 1.0, 0.0);
	glRotatef(rotate_z, 0.0, 0.0, 1.0);
	glScalef(sca[0], sca[1], sca[2]);
	glTranslatef(tra[0], tra[1], tra[2]);

	//��ɫ�ذ����
	glBegin(GL_POLYGON);
	constract(0, 0, -100, 720, 10, 700);
	glColor3f(purple);
	build();

	//��ɫ��ز����
	glBegin(GL_POLYGON);
	constract(0, -5, -100, 720, 5, 800);
	glColor3f(nbrown);
	build();

	//���ɫС·����
	glBegin(GL_POLYGON);
	constract(0, 0, 600, 720, 10, 100);
	glColor3f(gray);
	build();

	//���Ƴ���װ�
	glBegin(GL_POLYGON);
	constract(330, 20, 250, 140, 1, 250);
	glColor3f(purple);
	build();

	//���ò���
	GLfloat matiral_diffused[4] = { 0.20, 0.32, 0.80, 1.00 };
	setMetiral(matiral_diffused);

	//�ذ�����
	glEnable(GL_TEXTURE_2D);    
	glBindTexture(GL_TEXTURE_2D, texGround);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0, 11, -100);
	glTexCoord2f(0.0f, 5.0f); glVertex3f(0, 11, 600);
	glTexCoord2f(5.0f, 5.0f); glVertex3f(720, 11, 600);
	glTexCoord2f(5.0f, 0.0f); glVertex3f(720, 11, -100);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	//���ư�ɫ������
	for (int i = 0; i < 8; i++) {
		glBegin(GL_POLYGON);
		constract(i * 97, 10, 645, 39, 1, 10);
		glColor3f(white);
		build();
	}
	//����դ������
	glBegin(GL_POLYGON);
	constract(10, 10, 10, 20, 20, 480);
	glColor3f(white);
	build();       //    ��Ӧ��A��
	constract(30, 10, 10, 650, 20, 20);
	glColor3f(white);
	build();       //    ��Ӧ��B��
	constract(680, 10, 10, 20, 20, 480);
	glColor3f(white);
	build();       //    ��Ӧ��C��
	constract(30, 10, 470, 100, 20, 20);
	glColor3f(white);
	build();       //    ��Ӧ��D��
	constract(230, 10, 470, 100, 20, 20);
	glColor3f(white);
	build();       //    ��Ӧ��E��
	constract(470, 10, 470, 210, 20, 20);
	glColor3f(white);
	build();       //    ��Ӧ��F��

	//����դ������
	glBegin(GL_POLYGON);
	constract(10, 10, 10, 20, 90, 20);
	glColor3f(white);
	build();       //    ��Ӧ��A��
	glBegin(GL_POLYGON);
	constract(680, 10, 10, 20, 90, 20);
	glColor3f(white);
	build();       //    ��Ӧ��B��
	glBegin(GL_POLYGON);
	constract(230, 10, 470, 20, 90, 20);
	glColor3f(white);
	build();       //    ��Ӧ��C��
	glBegin(GL_POLYGON);
	constract(10, 10, 470, 20, 90, 20);
	glColor3f(white);
	build();       //    ��Ӧ��D��
	glBegin(GL_POLYGON);
	glBegin(GL_POLYGON);
	constract(470, 10, 470, 20, 90, 20);
	glColor3f(white);
	build();       //    ��Ӧ��E��
	glBegin(GL_POLYGON);
	constract(310, 10, 470, 20, 90, 20);
	glColor3f(white);
	build();       //    ��Ӧ��F��
	constract(680, 10, 470, 20, 90, 20);
	glColor3f(white);
	build();       //    ��Ӧ��G��
	glBegin(GL_POLYGON);
	constract(110, 10, 470, 20, 90, 20);
	glColor3f(white);
	build();       //    ��Ӧ��H��
	
	//������ɫդ��
	glColor3f(hgray);
	for (int i = 35; i < 470; i += 25)
	{
		constract(15, 20, i, 6, 70, 6);
		build();         //��������
	}
	for (int i = 35; i < 470; i += 25)
	{
		constract(685, 20, i, 6, 70, 6);
		build();          // ��������
	}
	for (int i = 35; i < 680; i += 25)
	{
		constract(i, 20, 15, 6, 70, 6);
		build();          // �������
	}
	for (int i = 35; i < 110; i += 25)
	{
		constract(i, 20, 480, 6, 70, 6);
		build();          // ǰ�����
	}
	for (int i = 235; i < 310; i += 25)
	{
		constract(i, 20, 480, 6, 70, 6);
		build();          // ǰ�����
	}
	for (int i = 475; i < 680; i += 25)
	{
		constract(i, 20, 480, 6, 70, 6);
		build();          // ǰ�����
	}

	//����Ӿ��
	glColor3f(water);
	constract(490, 10, 75, 150, 5, 350);
	build();

	glColor3f(white);
	constract(490, 10, 75, 8, 12, 350);
	build();     //��ǽ��
	glColor3f(white);
	constract(632, 10, 75, 8, 12, 350);
	build();     //��ǽ��
	glColor3f(white);
	constract(498, 10, 75, 134, 12, 8);
	build();     //��ǽ��
	glColor3f(white);
	constract(498, 10, 417, 134, 12, 8);
	build();     //��ǽ��

	//����Ӿ������
	glEnable(GL_TEXTURE_2D);   
	glBindTexture(GL_TEXTURE_2D, texPool);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(490, 16, 75);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(490, 16, 425);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(640, 16, 425);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(640, 16, 75);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	//����Ʈ��������
	//����������
	glColor3f(hgray);
	constract(652, 10, 417, 5, 400, 5);
	build();  
	GLfloat ctrlpoints[5][5][3] = { {{657,410,419.5},{672,410,439.5},{687,410,419.5},{702,410,399.5},{717,410,419.5}}, 
	{{657,400,419.5},{672,400,439.5},{687,400,419.5},{702,400,399.5},{717,400,419.5}} ,
	{{657,390,419.5},{672,390,439.5},{687,390,419.5},{702,390,399.5},{717,390,419.5}} ,
	{{657,380,419.5},{672,380,439.5},{687,380,419.5},{702,380,399.5},{717,380,419.5}} ,
	{{657,370,419.5},{672,370,439.5},{687,370,419.5},{702,370,399.5},{717,370,419.5}} };
	glColor3f(yellow);
	glPushMatrix();
	//��������
	glEnable(GL_MAP2_VERTEX_3);
	glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 5, 0, 1, 15, 5, &ctrlpoints[0][0][0]);
	glMapGrid2f(10.0, 0.0, 1.0, 10.0, 0.0, 1.0);
	glEvalMesh2(GL_FILL, 0, 10.0, 0, 10.0);
	glPopMatrix();
	
	//����·��
	glColor3f(hgray);
	constract(480, 10, 450, 5, 250, 5);
	build();
	float brown1[] = {0.2431f,0.1412f,0.0157f};
	float blue1[] = { 0.0f,0.0f,0.8039f };//����ɫ
	int blue2[] = {30,144,255};//����ɫ
	//int blue3[] = {135,206,250};//ǳ��ɫ

	//����������
	point ctrlPt[4] = { {485.0,240.0},{497.0,240.0},{510.0,227.0},{510.0,215.0} };
	int nCtrlPts = 4, nBezPts = 1000;
	Bezier(ctrlPt, 452, nCtrlPts, nBezPts,blue1);
	lineDDA(485, 179, 498, 179, 452, brown1);
	lineDDA(498, 185, 522, 185, 452, brown1);
	lineDDA(498, 185, 498, 173, 452, brown1);
	lineDDA(498, 173, 522, 173, 452, brown1);
	lineDDA(522, 185, 522, 173, 452, brown1);
	//BresenhamCircle(510, 205, 452, 10, brown1);
	//Բ��������
	glFillCircle(510, 205, 452, 10, blue2);
	//�����ɨ�����ư�
	struct Point polypoint[4];
	polypoint[0].x = 498; polypoint[0].y = 173;
	polypoint[1].x = 498; polypoint[1].y = 185;
	polypoint[2].x = 522; polypoint[2].y = 185;
	polypoint[3].x = 522; polypoint[3].y = 173;
	PolyScan(4, 452, polypoint);
	glEnd();

	//�ڶ�յ��
	glColor3f(hgray);
	constract(652, 10, 50, 5, 250, 5);
	build();
	//����������
	point ctrlPt1[4] = { {657.0,240.0},{669.0,240.0},{682.0,227.0},{682.0,215.0} };
	int nCtrlPts1 = 4, nBezPts1 = 1000;
	Bezier(ctrlPt1, 52, nCtrlPts1, nBezPts1, blue1);
	lineDDA(657, 179, 670, 179, 52, brown1);
	lineDDA(670, 185, 694, 185, 52, brown1);
	lineDDA(670, 185, 670, 173, 52, brown1);
	lineDDA(670, 173, 694, 173, 52, brown1);
	lineDDA(694, 185, 694, 173, 52, brown1);
	//BresenhamCircle(682, 205, 52, 10, brown1);
	//Բ��������
	glFillCircle(682, 205, 52, 10, blue2);
	//�����ɨ�����ư�
	struct Point polypoint1[4];
	polypoint1[0].x = 670; polypoint1[0].y = 173;
	polypoint1[1].x = 670; polypoint1[1].y = 185;
	polypoint1[2].x = 694; polypoint1[2].y = 185;
	polypoint1[3].x = 694; polypoint1[3].y = 173;
	PolyScan(4, 52, polypoint1);
	glEnd();

	//�ͻ��������㷨������
	painter(471,469,470);
	//��������
	float blue4[] = { 0.5294f, 0.8078f, 0.9804f };//ǳ��ɫ
	point ctrlPt2[3] = { {-40.0,372.0},{-35.0,200.0},{10.0,100.0} };
	int nCtrlPts2 = 3, nBezPts2 = 1000;
	Bezier(ctrlPt2, 469, nCtrlPts2, nBezPts2, blue4);

	point ctrlPt3[3] = { {-30.0,352.0},{-25.0,200.0},{10.0,100.0} };
	int nCtrlPts3 = 3, nBezPts3 = 1000;
	Bezier(ctrlPt3, 470, nCtrlPts3, nBezPts3, blue4);

	point ctrlPt4[3] = { {-50.0,352.0},{-45.0,200.0},{10.0,100.0} };
	int nCtrlPts4 = 3, nBezPts4 = 1000;
	Bezier(ctrlPt4, 471, nCtrlPts4, nBezPts4, blue4);

	//���ư�ɫ����ǽ
	glColor3f(white);
	constract(320, 10, 250, 10, 100, 220);
	build();
	constract(460, 10, 250, 10, 100, 220);
	build();
	//�������ɫ���ⶥ��
	glColor3f(purple);
	constract(320, 110, 250, 150, 10, 220);
	build();
	
	//��һ��
	//����ǽ��
	glColor3f(white);
	constract(50, 10, 50, 12, 150, 197);
	build();     //��ǽ��
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texWall);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(49, 160, 50);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(49, 10, 50);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(49, 10, 247);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(49, 160, 247);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glColor3f(white);
	constract(438, 10, 50, 12, 150, 197);
	build();     //��ǽ��
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texWall);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(451, 160, 50);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(451, 10, 50);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(451, 10, 247);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(451, 160, 247);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glColor3f(white);
	constract(62, 10, 50, 376, 150, 12);
	build();     //��ǽ��
	//�����
	glColor3f(purple);
	constract(40, 160, 40, 420, 6, 217);
	build(); 
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texWall);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(450, 160, 49);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(450, 10, 49);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(50, 10, 49);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(50, 160, 49);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	//�ڶ���
	//����ǽ��
	glColor3f(white);
	constract(50, 166, 50, 12, 150, 197);
	build();     //��ǽ��
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texWall);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(49, 316, 50);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(49, 166, 50);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(49, 166, 247);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(49, 316, 247);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glColor3f(white);
	constract(438, 166, 50, 12, 150, 197);
	build();     //��ǽ��
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texWall);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(451, 316, 50);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(451, 166, 50);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(451, 166, 247);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(451, 316, 247);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glColor3f(white);
	constract(62, 166, 50, 376, 150, 12);
	build();     //��ǽ��
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texWall);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(450, 316, 49);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(450, 166, 49);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(50, 166, 49);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(50, 316, 49);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	//�����
	glColor3f(purple);
	constract(40, 160, 40, 420, 6, 217);
	build();
	glColor3f(pink);
	constract(451, 10, 100,5, 110, 50);
	build();     //����
	glColor3f(hgray);
	constract(456, 60, 140, 5, 5, 5);
	build();              //�Ű���
	//��һ��
	glColor3f(white);
	constract(62, 10, 235, 376, 35, 12);
	build();     //ǰǽ��1
	glColor3f(white);
	constract(62, 45, 235, 30, 75, 12);
	build();     //ǰǽ��2
	glColor3f(white);
	constract(184, 45, 235, 254, 75, 12);
	build();     //ǰǽ��3
	glColor3f(white);
	constract(62, 120, 235, 376, 40, 12);
	build();     //ǰǽ��4
	//�ڶ���
	glColor3f(white);
	constract(62, 166, 235, 376, 35, 12);
	build();     //ǰǽ��1
	glColor3f(white);
	constract(62, 201, 235, 25, 85, 12);
	build();     //ǰǽ��2
	glColor3f(white);
	constract(179, 201, 235, 25, 85, 12);
	build();     //ǰǽ��3
	glColor3f(white);
	constract(296, 201, 235, 25, 85, 12);
	build();     //ǰǽ��4
	glColor3f(white);
	constract(413, 201, 235, 25, 85, 12);
	build();     //ǰǽ��5
	glColor3f(white);
	constract(62, 286, 235, 376, 30, 12);
	build();     //ǰǽ��6
	//���Ʒ��ڵذ�
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texFloor);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(62, 15, 62);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(62, 15, 235);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(438, 15, 235);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(438, 15, 62);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	//���Ʒ��ڵذ�
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texFloor);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(62, 171, 62);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(62, 171, 235);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(438, 171, 235);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(438, 171, 62);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	
	//���Ƶ�һ�㴰��
	glColor4f(pink, 0.4);
	constract(92, 45, 235, 44, 75, 12);
	build();
	glColor4f(pink, 0.4);
	constract(140, 45, 235, 44, 75, 12);
	build();
	//���ƴ�������
	glColor3f(hgray);
	constract(136, 45, 235, 4, 75, 12);
	build();              
	//���Ƶڶ��㴰��
	glColor4f(pink, 0.4);
	constract(87, 201, 235, 44, 85, 12);
	build();
	glColor3f(hgray);
	constract(131, 201, 235, 4, 85, 12);
	build();
	glColor4f(pink, 0.4);
	constract(135, 201, 235, 44, 85, 12);
	build();
	//���Ƶڶ��㴰��
	glColor4f(pink, 0.4);
	constract(204, 201, 235, 44, 85, 12);
	build();
	glColor3f(hgray);
	constract(248, 201, 235, 4, 85, 12);
	build();
	glColor4f(pink, 0.4);
	constract(252, 201, 235, 44, 85, 12);
	build();
	//���Ƶڶ��㴰��
	glColor4f(pink, 0.4);
	constract(321, 201, 235, 44, 85, 12);
	build();
	glColor3f(hgray);
	constract(365, 201, 235, 4, 85, 12);
	build();
	glColor4f(pink, 0.4);
	constract(369, 201, 235, 44, 85, 12);
	build();
	//���Ƴ��ⲣ��
	glColor4f(pink, 0.4);
	constract(330, 22, 450, 130, 90, 10);
	build();


	//������ɫ����
	glColor3f(purple);
	cons(50, 316, 148.5, 400, 27, 110);
	build1();
	cons(50, 343, 148.5, 400, 27, 90);
	build1();
	cons(50, 370, 148.5, 400, 27, 70);
	build1();
	cons(50, 397, 148.5, 400, 15, 50);
	build1();
	cons(50, 412, 148.5, 400, 15, 30);
	build1();
	glColor3f(purple);
	cons(50, 316, 148.5, 400, 27, -110);
	build1();
	cons(50, 343, 148.5, 400, 27, -90);
	build1();
	cons(50, 370, 148.5, 400, 27, -70);
	build1();
	cons(50, 397, 148.5, 400, 15, -50);
	build1();
	cons(50, 412, 148.5, 400, 15, -30);
	build1();
	
	//������
	glColor3f(pink);
	constract(260, 10, 250, 40, 110, 5);
	build();              //��
	glColor3f(pink);
	constract(220, 10, 250, 40, 110, 5);
	build();              //��
	glColor3f(hgray);
	constract(265, 60, 255, 5, 5, 5);
	build();              //�Ű���
	glColor3f(hgray);
	constract(257, 10, 255,6,110,5);
	build();              
	glColor3f(hgray);
	constract(250, 60, 255, 5, 5, 5);
	build();              //�Ű���
	//����С·
	for (int i = 0; i < 6; i++)
	{
		glColor3f(purple1);
		constract(235 - i * 15, 10, 250 + i * 40, 40, 2, 20);
		build();
	}
	//��������
	glTranslatef(700, 500, 300);
	glPushMatrix();
	qobj = gluNewQuadric();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texMoon);
	gluQuadricTexture(qobj, GL_TRUE);//������
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	glRotatef(66.3f, 0.0f, 0.0f, 1.0f);
	glRotatef(-corner, 0.0, -0.261, 0.595);
	gluSphere(qobj, 35.0, 40, 40);//��������qobj
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();


	glTranslatef(-700, -500, -300);
	//������ɫ����
	glColor3f(brown);
	constract(60, 10, 390, 15, 70, 15);
	build();
	//������ɫ��Ҷ
	glColor3f(green);
	glTranslatef(60, 70, 400);
	glutSolidSphere(25.0, 20, 20);  

	glTranslatef(20, 0, 0);
	glutSolidSphere(25.0, 20, 20);  

	glTranslatef(-10, 0, -10);
	glutSolidSphere(25.0, 20, 20);  

	glTranslatef(0, 0, 20);
	glutSolidSphere(25.0, 20, 20);  

	glTranslatef(0, 10, -10);
	glutSolidSphere(25.0, 20, 20); 
	glTranslatef(-70, -80, -400);


	//����ˮ���е���
	glColor4f(yellow, 0.8);
	glTranslatef(565 - bx, 25 - by, 250 - bz);
	glutSolidSphere(15, 20, 20);
	glTranslatef(-565, -25, -250);
	
	//ˢ��
	glFlush();  
	glutSwapBuffers();
}

void Moving()     //������ת��ֻ����ʾ��ʵ�ʲ����ڣ�
{
	if (sign == 1)
	{
		corner += 0.25f;
		glutPostRedisplay();
	}
}


void init(void)
{	
	//��ʼ������
	GLfloat sun_direction[] = { 700.0, 400.0, 100.0, 1.0 };
	GLfloat sun_intensity[] = { sunshine, 1.0 };
	GLfloat ambient_intensity[] = { 0.5, 0.5, 0.5, 1.0 };//���û��������

	//������
	glEnable(GL_LIGHTING);              
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient_intensity);

	//������
	glEnable(GL_LIGHT0);                
	glLightfv(GL_LIGHT0, GL_POSITION, sun_direction);//���ù�Դλ��
	glLightfv(GL_LIGHT0, GL_DIFFUSE, sun_intensity);

	//���ñ�����ɫ
	glEnable(GL_COLOR_MATERIAL);       
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	cout << "The OpenGL version is: " << glGetString(GL_VERSION) << "\n";
	cout << glGetString(GL_VENDOR) << "\n";
	glLineWidth(5);
	//���屳����ɫ
	glClearColor(0.0, 0.0, 0.0, 0.0);      

	glEnable(GL_BLEND); //�򿪻��
	glDisable(GL_DEPTH_TEST); //�ر���Ȳ���
	glBlendFunc(GL_SRC_ALPHA, GL_ONE); //����Դ����alphaͨ��ֵ�İ�͸����Ϻ���
	glEnable(GL_DEPTH_TEST); //����Ȳ���

	glMatrixMode(GL_PROJECTION);     //�ӽ��йصĶ��壬����������ڣ�0,0,0)���������(0,0,-1).
	glOrtho(-w, w, -h, h, -w, w); // ����ƽ����Ұ
}

int flag = 1;
void timerFunction(int value)
{
	if (flag == 1) {
		by = by + 4;
		flag++;
	}
	else if (flag == 2) {
		by = by - 4;
		flag--;
	}
	else {
		cout << "Wrong!\n";
	}
	glutPostRedisplay();
	glutTimerFunc(1000, timerFunction, 1);
}

//���̽�������
void specialkeys(int key, int x, int y) {   

	//������ת
	if (key == GLUT_KEY_RIGHT)
		rotate_y -= 1;
	else if (key == GLUT_KEY_LEFT)
		rotate_y += 1;
	else if (key == GLUT_KEY_DOWN)
		rotate_x -= 1;
	else if (key == GLUT_KEY_UP)
		rotate_x += 1;

	//��������
	if (key == GLUT_KEY_F1) {
		for (int i = 0; i < 3; i++)
			sca[i] = sca[i] + 0.1;
	}
	else if (key == GLUT_KEY_F2) {
		for (int i = 0; i < 3; i++)
			sca[i] = sca[i] - 0.1;
	}
	glutPostRedisplay();
}

void NormalKeys(unsigned char key, int x, int y) {
	//ˮ�����ƶ�
	if (key == 'd')
		bx -= 1;
	else if (key == 'a')
		bx += 1;
	else if (key == 'w')
		bz += 1;
	else if (key == 's')
		bz -= 1;
	glutPostRedisplay();

}
// �ж�һ�������ǲ���2����������
int power_of_two(int n)
{
	if (n <= 0)
		return 0;
	return (n & (n - 1)) == 0;
}

/* ����load_texture
* ��ȡһ��BMP�ļ���Ϊ����
* ���ʧ�ܣ�����0������ɹ�������������
*/
GLuint load_texture(const char* file_name)
{
	GLint width, height, total_bytes;
	GLubyte* pixels = 0;
	GLuint last_texture_ID = 0, texture_ID = 0;
	FILE* pFile;
	errno_t err;

	//���ļ������ʧ�ܣ�����
	if ((err = fopen_s(&pFile, file_name, "rb")) != 0)
	{
		printf("�޷��򿪴��ļ�\n");            //����򲻿���������򲻿�
		exit(0);                               //��ֹ����
	}

	//��ȡ�ļ���ͼ��Ŀ�Ⱥ͸߶�
	fseek(pFile, 0x0012, SEEK_SET);
	fread(&width, 4, 1, pFile);
	fread(&height, 4, 1, pFile);
	fseek(pFile, BMP_Header_Length, SEEK_SET);

	//����ÿ��������ռ�ֽ����������ݴ����ݼ����������ֽ���
	{
		GLint line_bytes = width * 3;
		while (line_bytes % 4 != 0)
			++line_bytes;
		total_bytes = line_bytes * height;
	}

	//�����������ֽ��������ڴ�
	pixels = (GLubyte*)malloc(total_bytes);
	if (pixels == 0)
	{
		fclose(pFile);
		return 0;
	}

	//��ȡ��������
	if (fread(pixels, total_bytes, 1, pFile) <= 0)
	{
		free(pixels);
		fclose(pFile);
		return 0;
	}

	//��ͼ��Ŀ�Ⱥ͸߶Ȳ��ǵ������η�������
	//��ͼ���߳�����OpenGL�涨�����ֵ��Ҳ����
	{
		GLint max;
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max);
		if (!power_of_two(width)
			|| !power_of_two(height)
			|| width > max
			|| height > max)
		{
			const GLint new_width = 256;
			const GLint new_height = 256; // �涨���ź��µĴ�СΪ�߳���������
			GLint new_line_bytes, new_total_bytes;
			GLubyte* new_pixels = 0;

			//����ÿ����Ҫ���ֽ��������ֽ���
			new_line_bytes = new_width * 3;
			while (new_line_bytes % 4 != 0)
				++new_line_bytes;
			new_total_bytes = new_line_bytes * new_height;

			//�����ڴ�
			new_pixels = (GLubyte*)malloc(new_total_bytes);
			if (new_pixels == 0)
			{
				free(pixels);
				fclose(pFile);
				return 0;
			}

			//������������
			gluScaleImage(GL_RGB,
				width, height, GL_UNSIGNED_BYTE, pixels,
				new_width, new_height, GL_UNSIGNED_BYTE, new_pixels);

			//�ͷ�ԭ�����������ݣ���pixelsָ���µ��������ݣ�����������width��height
			free(pixels);
			pixels = new_pixels;
			width = new_width;
			height = new_height;
		}
	}

	// ����һ���µ�������
	glGenTextures(1, &texture_ID);
	if (texture_ID == 0)
	{
		free(pixels);
		fclose(pFile);
		return 0;
	}

	// ���µ������������������������
	// ��ǰ���Ȼ��ԭ���󶨵������ţ��Ա��������лָ�
	GLint lastTextureID = last_texture_ID;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &lastTextureID);
	glBindTexture(GL_TEXTURE_2D, texture_ID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
		GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);
	//�ָ�֮ǰ�������
	glBindTexture(GL_TEXTURE_2D, lastTextureID);  
	free(pixels);
	return texture_ID;
}

int main(int argc, char** argv)   
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);//���崰����ʾģʽ
	glutInitWindowSize(w, h);//���ڴ�С
	glutInitWindowPosition(100, 100);//����λ��
	glutCreateWindow("My Home");//���ڱ���
	init();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);//������Ȳ���

	//��������
	glEnable(GL_TEXTURE_2D);    
	//��������
	texGround = load_texture("ground.bmp");  
	texPool = load_texture("pool.bmp");
	texFloor = load_texture("floor.bmp");
	texMoon = load_texture("moon.bmp");
	texWall = load_texture("wall.bmp");
	
	glutDisplayFunc(display);
	glutIdleFunc(Moving);
	//���ü��̿��ƺ���	
	glutSpecialFunc(specialkeys); 
	glutKeyboardFunc(NormalKeys);
	//��ʱ��
	glutTimerFunc(1000, timerFunction, 1);  
	glutMainLoop();
	system("pause");
	return 0;
}

