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

//点的位置
struct Point
{
	int x;
	int y;
};

//DDA直线算法
void lineDDA(int x0, int y0, int x1, int y1, int z, float RGB[]);
//Bresenham画圆
void BresenhamCircle(int x0, int y0, int z0, int r, float RGB[]);
//多边形扫描填充算法
void PolyScan(int POINTNUM, int z, struct Point polypoint[100]);
//填充圆形算法
void glFillCircle(double x, double y, double z, double radius, int color[3]);
//填充圆角矩形算法
void glFillRoundRec(int centerX, int centerY, int z, int width, int height, float cirR, int color[3]);
//贝塞尔曲线算法
void Bezier(point* ctrlPt, float z, int nCtrlPts, int nBezPts, float RGB[]);
//消隐算法
void painter(int z1, int z2, int z3);


//窗口大小值
GLfloat w = 1000;
GLfloat h = 1000;

double rotate_x = 0.0;
double rotate_y = 0.0;
double rotate_z = 0.0;
//颜色定义
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

 //定义长方体的8个顶点
double fang[8][3];    
// 定义梯形的长方体8个顶点
double san[8][3];              

//定义纹理对象编号
GLuint texGround;
GLuint texPool;
GLuint texFloor;
GLuint texWall;

//旋转月亮贴图
GLuint texMoon;
GLuint sign = 1;
GLUquadricObj* qobj;
GLfloat  corner = 0.0;
//图像数据在内存块中的偏移量
#define BMP_Header_Length 54  

//旋转角度
static GLfloat angle = 0.0f;   


//场景缩放参数
GLfloat sca[3] = { 1.5, 1.5, 1.5 };
//场景转动参数
GLfloat tra[3] = { -300, 0, -470 };


GLfloat AngleX;
GLfloat AngleY;

//泳池中气球的移动坐标
GLfloat bx = 0; 
GLfloat by = 0;
GLfloat bz = 0;



GLfloat matiral_specular[4] = { 0.00, 0.00, 0.00, 1.00 };  //材质参数：镜面反射，所得到的光的强度
GLfloat matiral_emission[4] = { 0.00, 0.00, 0.00, 1.00 };  //材质本身微向外发射光线
const GLfloat matiral_shininess = 11.00;
void setMetiral(GLfloat matiral_diffuse_ambient[4])
{
	//设置颜色
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matiral_diffuse_ambient); //设置相同值更逼真
	//设置参数
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matiral_specular);  //镜面反射，所得到的光的强度（颜色）
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, matiral_emission);  //材质本身就微微的向外发射光线，以至于眼睛感觉到它有这样的颜色 
	glMaterialf(GL_FRONT_AND_BACK, matiral_shininess, 0);   
	//镜面指数（0~128） 该值越小，材质越粗糙，点光源发射的光线照射到上面，也可以产生较大的亮点。
	//该值越大，表示材质越类似于镜面，光源照射到上面后，产生较小的亮点。

}

//梯形的绘制
void cons(double x, double y, double z, double x1, double y1, double z1) {  
	san[0][0] = x;
	san[0][1] = y;
	san[0][2] = z;     //  第0个点

	san[1][0] = x;
	san[1][1] = y;
	san[1][2] = z + z1;   //  第1个点

	san[4][0] = x;
	san[4][1] = y + y1;
	san[4][2] = z;     //  第4个点

	san[5][0] = x;
	san[5][1] = y + y1;
	san[5][2] = z + z1 / 2;      //  第5个点
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

//长方体的绘制
void constract(double x, double y, double z, double x1, double y1, double z1) { 
	fang[0][0] = x;
	fang[0][1] = y;
	fang[0][2] = z;        //  第0个点

	fang[1][0] = x;
	fang[1][1] = y;
	fang[1][2] = z + z1;     //  第一个点

	fang[2][0] = x + x1;
	fang[2][1] = y;
	fang[2][2] = z + z1;     //  第二个点

	fang[3][0] = x + x1;
	fang[3][1] = y;
	fang[3][2] = z;        //  第三个点
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
	glEnd();   //     下面

	glBegin(GL_POLYGON);
	glNormal3f(-1.0, 0.0, 0.0);
	glVertex3f(san[1][0], san[1][1], san[1][2]);
	glVertex3f(san[0][0], san[0][1], san[0][2]);
	glVertex3f(san[4][0], san[4][1], san[4][2]);
	glVertex3f(san[5][0], san[5][1], san[5][2]);
	glEnd();   //     左面

	glBegin(GL_POLYGON);
	glNormal3f(1.0, 0.0, 0.0);
	glVertex3f(san[7][0], san[7][1], san[7][2]);
	glVertex3f(san[6][0], san[6][1], san[6][2]);
	glVertex3f(san[3][0], san[3][1], san[3][2]);
	glVertex3f(san[2][0], san[2][1], san[2][2]);
	glEnd();   //     右面

	glBegin(GL_POLYGON);
	glNormal3f(0.0, 0.0, 1.0);
	glVertex3f(san[5][0], san[5][1], san[5][2]);
	glVertex3f(san[6][0], san[6][1], san[6][2]);
	glVertex3f(san[2][0], san[2][1], san[2][2]);
	glVertex3f(san[1][0], san[1][1], san[1][2]);
	glEnd();   //     前面

	glBegin(GL_POLYGON);
	glNormal3f(0.0, 0.0, -1.0);
	glVertex3f(san[0][0], san[0][1], san[0][2]);
	glVertex3f(san[3][0], san[3][1], san[3][2]);
	glVertex3f(san[7][0], san[7][1], san[7][2]);
	glVertex3f(san[4][0], san[4][1], san[4][2]);
	glEnd();   //     后面

	glBegin(GL_POLYGON);
	glNormal3f(0.0, 1.0, 0.0);
	glVertex3f(san[4][0], san[4][1], san[4][2]);
	glVertex3f(san[7][0], san[7][1], san[7][2]);
	glVertex3f(san[6][0], san[6][1], san[6][2]);
	glVertex3f(san[5][0], san[5][1], san[5][2]);
	glEnd();   //     上面
}

void build() {
	glBegin(GL_POLYGON);
	glNormal3f(0.0, -1.0, 0.0);
	glVertex3f(fang[0][0], fang[0][1], fang[0][2]);
	glVertex3f(fang[1][0], fang[1][1], fang[1][2]);
	glVertex3f(fang[2][0], fang[2][1], fang[2][2]);
	glVertex3f(fang[3][0], fang[3][1], fang[3][2]);
	glEnd();   //     下面

	glBegin(GL_POLYGON);
	glNormal3f(-1.0, 0.0, 0.0);
	glVertex3f(fang[1][0], fang[1][1], fang[1][2]);
	glVertex3f(fang[0][0], fang[0][1], fang[0][2]);
	glVertex3f(fang[4][0], fang[4][1], fang[4][2]);
	glVertex3f(fang[5][0], fang[5][1], fang[5][2]);
	glEnd();   //     左面

	glBegin(GL_POLYGON);
	glNormal3f(1.0, 0.0, 0.0);
	glVertex3f(fang[7][0], fang[7][1], fang[7][2]);
	glVertex3f(fang[6][0], fang[6][1], fang[6][2]);
	glVertex3f(fang[2][0], fang[2][1], fang[2][2]);
	glVertex3f(fang[3][0], fang[3][1], fang[3][2]);
	glEnd();   //     右面

	glBegin(GL_POLYGON);
	glNormal3f(0.0, 0.0, 1.0);
	glVertex3f(fang[5][0], fang[5][1], fang[5][2]);
	glVertex3f(fang[6][0], fang[6][1], fang[6][2]);
	glVertex3f(fang[2][0], fang[2][1], fang[2][2]);
	glVertex3f(fang[1][0], fang[1][1], fang[1][2]);
	glEnd();   //     前面

	glBegin(GL_POLYGON);
	glNormal3f(0.0, 0.0, -1.0);
	glVertex3f(fang[0][0], fang[0][1], fang[0][2]);
	glVertex3f(fang[3][0], fang[3][1], fang[3][2]);
	glVertex3f(fang[7][0], fang[7][1], fang[7][2]);
	glVertex3f(fang[4][0], fang[4][1], fang[4][2]);
	glEnd();   //     后面

	glBegin(GL_POLYGON);
	glNormal3f(0.0, 1.0, 0.0);
	glVertex3f(fang[4][0], fang[4][1], fang[4][2]);
	glVertex3f(fang[7][0], fang[7][1], fang[7][2]);
	glVertex3f(fang[6][0], fang[6][1], fang[6][2]);
	glVertex3f(fang[5][0], fang[5][1], fang[5][2]);
	glEnd();   //     上面
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();         
	//增加M矩阵
	glRotatef(rotate_x, 1.0, 0.0, 0.0);
	glRotatef(rotate_y, 0.0, 1.0, 0.0);
	glRotatef(rotate_z, 0.0, 0.0, 1.0);
	glScalef(sca[0], sca[1], sca[2]);
	glTranslatef(tra[0], tra[1], tra[2]);

	//紫色地板绘制
	glBegin(GL_POLYGON);
	constract(0, 0, -100, 720, 10, 700);
	glColor3f(purple);
	build();

	//棕色大地层绘制
	glBegin(GL_POLYGON);
	constract(0, -5, -100, 720, 5, 800);
	glColor3f(nbrown);
	build();

	//深灰色小路绘制
	glBegin(GL_POLYGON);
	constract(0, 0, 600, 720, 10, 100);
	glColor3f(gray);
	build();

	//绘制车库底板
	glBegin(GL_POLYGON);
	constract(330, 20, 250, 140, 1, 250);
	glColor3f(purple);
	build();

	//设置材质
	GLfloat matiral_diffused[4] = { 0.20, 0.32, 0.80, 1.00 };
	setMetiral(matiral_diffused);

	//地板纹理
	glEnable(GL_TEXTURE_2D);    
	glBindTexture(GL_TEXTURE_2D, texGround);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0, 11, -100);
	glTexCoord2f(0.0f, 5.0f); glVertex3f(0, 11, 600);
	glTexCoord2f(5.0f, 5.0f); glVertex3f(720, 11, 600);
	glTexCoord2f(5.0f, 0.0f); glVertex3f(720, 11, -100);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	//绘制白色斑马线
	for (int i = 0; i < 8; i++) {
		glBegin(GL_POLYGON);
		constract(i * 97, 10, 645, 39, 1, 10);
		glColor3f(white);
		build();
	}
	//绘制栅栏基座
	glBegin(GL_POLYGON);
	constract(10, 10, 10, 20, 20, 480);
	glColor3f(white);
	build();       //    对应于A点
	constract(30, 10, 10, 650, 20, 20);
	glColor3f(white);
	build();       //    对应于B点
	constract(680, 10, 10, 20, 20, 480);
	glColor3f(white);
	build();       //    对应于C点
	constract(30, 10, 470, 100, 20, 20);
	glColor3f(white);
	build();       //    对应于D点
	constract(230, 10, 470, 100, 20, 20);
	glColor3f(white);
	build();       //    对应于E点
	constract(470, 10, 470, 210, 20, 20);
	glColor3f(white);
	build();       //    对应于F点

	//绘制栅栏柱子
	glBegin(GL_POLYGON);
	constract(10, 10, 10, 20, 90, 20);
	glColor3f(white);
	build();       //    对应于A点
	glBegin(GL_POLYGON);
	constract(680, 10, 10, 20, 90, 20);
	glColor3f(white);
	build();       //    对应于B点
	glBegin(GL_POLYGON);
	constract(230, 10, 470, 20, 90, 20);
	glColor3f(white);
	build();       //    对应于C点
	glBegin(GL_POLYGON);
	constract(10, 10, 470, 20, 90, 20);
	glColor3f(white);
	build();       //    对应于D点
	glBegin(GL_POLYGON);
	glBegin(GL_POLYGON);
	constract(470, 10, 470, 20, 90, 20);
	glColor3f(white);
	build();       //    对应于E点
	glBegin(GL_POLYGON);
	constract(310, 10, 470, 20, 90, 20);
	glColor3f(white);
	build();       //    对应于F点
	constract(680, 10, 470, 20, 90, 20);
	glColor3f(white);
	build();       //    对应于G点
	glBegin(GL_POLYGON);
	constract(110, 10, 470, 20, 90, 20);
	glColor3f(white);
	build();       //    对应于H点
	
	//绘制棕色栅栏
	glColor3f(hgray);
	for (int i = 35; i < 470; i += 25)
	{
		constract(15, 20, i, 6, 70, 6);
		build();         //左面竖条
	}
	for (int i = 35; i < 470; i += 25)
	{
		constract(685, 20, i, 6, 70, 6);
		build();          // 右面竖条
	}
	for (int i = 35; i < 680; i += 25)
	{
		constract(i, 20, 15, 6, 70, 6);
		build();          // 后面横条
	}
	for (int i = 35; i < 110; i += 25)
	{
		constract(i, 20, 480, 6, 70, 6);
		build();          // 前面横条
	}
	for (int i = 235; i < 310; i += 25)
	{
		constract(i, 20, 480, 6, 70, 6);
		build();          // 前面横条
	}
	for (int i = 475; i < 680; i += 25)
	{
		constract(i, 20, 480, 6, 70, 6);
		build();          // 前面横条
	}

	//绘制泳池
	glColor3f(water);
	constract(490, 10, 75, 150, 5, 350);
	build();

	glColor3f(white);
	constract(490, 10, 75, 8, 12, 350);
	build();     //左墙壁
	glColor3f(white);
	constract(632, 10, 75, 8, 12, 350);
	build();     //右墙壁
	glColor3f(white);
	constract(498, 10, 75, 134, 12, 8);
	build();     //上墙壁
	glColor3f(white);
	constract(498, 10, 417, 134, 12, 8);
	build();     //下墙壁

	//绘制泳池纹理
	glEnable(GL_TEXTURE_2D);   
	glBindTexture(GL_TEXTURE_2D, texPool);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(490, 16, 75);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(490, 16, 425);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(640, 16, 425);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(640, 16, 75);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	//绘制飘动的旗子
	//贝塞尔曲面
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
	//绘制曲面
	glEnable(GL_MAP2_VERTEX_3);
	glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 5, 0, 1, 15, 5, &ctrlpoints[0][0][0]);
	glMapGrid2f(10.0, 0.0, 1.0, 10.0, 0.0, 1.0);
	glEvalMesh2(GL_FILL, 0, 10.0, 0, 10.0);
	glPopMatrix();
	
	//绘制路灯
	glColor3f(hgray);
	constract(480, 10, 450, 5, 250, 5);
	build();
	float brown1[] = {0.2431f,0.1412f,0.0157f};
	float blue1[] = { 0.0f,0.0f,0.8039f };//深蓝色
	int blue2[] = {30,144,255};//中蓝色
	//int blue3[] = {135,206,250};//浅蓝色

	//贝塞尔曲线
	point ctrlPt[4] = { {485.0,240.0},{497.0,240.0},{510.0,227.0},{510.0,215.0} };
	int nCtrlPts = 4, nBezPts = 1000;
	Bezier(ctrlPt, 452, nCtrlPts, nBezPts,blue1);
	lineDDA(485, 179, 498, 179, 452, brown1);
	lineDDA(498, 185, 522, 185, 452, brown1);
	lineDDA(498, 185, 498, 173, 452, brown1);
	lineDDA(498, 173, 522, 173, 452, brown1);
	lineDDA(522, 185, 522, 173, 452, brown1);
	//BresenhamCircle(510, 205, 452, 10, brown1);
	//圆形填充灯泡
	glFillCircle(510, 205, 452, 10, blue2);
	//多边形扫描填充灯板
	struct Point polypoint[4];
	polypoint[0].x = 498; polypoint[0].y = 173;
	polypoint[1].x = 498; polypoint[1].y = 185;
	polypoint[2].x = 522; polypoint[2].y = 185;
	polypoint[3].x = 522; polypoint[3].y = 173;
	PolyScan(4, 452, polypoint);
	glEnd();

	//第二盏灯
	glColor3f(hgray);
	constract(652, 10, 50, 5, 250, 5);
	build();
	//贝塞尔曲线
	point ctrlPt1[4] = { {657.0,240.0},{669.0,240.0},{682.0,227.0},{682.0,215.0} };
	int nCtrlPts1 = 4, nBezPts1 = 1000;
	Bezier(ctrlPt1, 52, nCtrlPts1, nBezPts1, blue1);
	lineDDA(657, 179, 670, 179, 52, brown1);
	lineDDA(670, 185, 694, 185, 52, brown1);
	lineDDA(670, 185, 670, 173, 52, brown1);
	lineDDA(670, 173, 694, 173, 52, brown1);
	lineDDA(694, 185, 694, 173, 52, brown1);
	//BresenhamCircle(682, 205, 52, 10, brown1);
	//圆形填充灯泡
	glFillCircle(682, 205, 52, 10, blue2);
	//多边形扫描填充灯板
	struct Point polypoint1[4];
	polypoint1[0].x = 670; polypoint1[0].y = 173;
	polypoint1[1].x = 670; polypoint1[1].y = 185;
	polypoint1[2].x = 694; polypoint1[2].y = 185;
	polypoint1[3].x = 694; polypoint1[3].y = 173;
	PolyScan(4, 52, polypoint1);
	glEnd();

	//油画家消隐算法绘气球
	painter(471,469,470);
	//绘制绳子
	float blue4[] = { 0.5294f, 0.8078f, 0.9804f };//浅蓝色
	point ctrlPt2[3] = { {-40.0,372.0},{-35.0,200.0},{10.0,100.0} };
	int nCtrlPts2 = 3, nBezPts2 = 1000;
	Bezier(ctrlPt2, 469, nCtrlPts2, nBezPts2, blue4);

	point ctrlPt3[3] = { {-30.0,352.0},{-25.0,200.0},{10.0,100.0} };
	int nCtrlPts3 = 3, nBezPts3 = 1000;
	Bezier(ctrlPt3, 470, nCtrlPts3, nBezPts3, blue4);

	point ctrlPt4[3] = { {-50.0,352.0},{-45.0,200.0},{10.0,100.0} };
	int nCtrlPts4 = 3, nBezPts4 = 1000;
	Bezier(ctrlPt4, 471, nCtrlPts4, nBezPts4, blue4);

	//绘制白色车库墙
	glColor3f(white);
	constract(320, 10, 250, 10, 100, 220);
	build();
	constract(460, 10, 250, 10, 100, 220);
	build();
	//绘制深灰色车库顶棚
	glColor3f(purple);
	constract(320, 110, 250, 150, 10, 220);
	build();
	
	//第一层
	//房屋墙壁
	glColor3f(white);
	constract(50, 10, 50, 12, 150, 197);
	build();     //左墙壁
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
	build();     //右墙壁
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
	build();     //后墙壁
	//间隔板
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

	//第二层
	//房屋墙壁
	glColor3f(white);
	constract(50, 166, 50, 12, 150, 197);
	build();     //左墙壁
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
	build();     //右墙壁
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
	build();     //后墙壁
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texWall);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(450, 316, 49);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(450, 166, 49);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(50, 166, 49);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(50, 316, 49);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	//间隔板
	glColor3f(purple);
	constract(40, 160, 40, 420, 6, 217);
	build();
	glColor3f(pink);
	constract(451, 10, 100,5, 110, 50);
	build();     //右门
	glColor3f(hgray);
	constract(456, 60, 140, 5, 5, 5);
	build();              //门把手
	//第一层
	glColor3f(white);
	constract(62, 10, 235, 376, 35, 12);
	build();     //前墙壁1
	glColor3f(white);
	constract(62, 45, 235, 30, 75, 12);
	build();     //前墙壁2
	glColor3f(white);
	constract(184, 45, 235, 254, 75, 12);
	build();     //前墙壁3
	glColor3f(white);
	constract(62, 120, 235, 376, 40, 12);
	build();     //前墙壁4
	//第二层
	glColor3f(white);
	constract(62, 166, 235, 376, 35, 12);
	build();     //前墙壁1
	glColor3f(white);
	constract(62, 201, 235, 25, 85, 12);
	build();     //前墙壁2
	glColor3f(white);
	constract(179, 201, 235, 25, 85, 12);
	build();     //前墙壁3
	glColor3f(white);
	constract(296, 201, 235, 25, 85, 12);
	build();     //前墙壁4
	glColor3f(white);
	constract(413, 201, 235, 25, 85, 12);
	build();     //前墙壁5
	glColor3f(white);
	constract(62, 286, 235, 376, 30, 12);
	build();     //前墙壁6
	//绘制房内地板
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texFloor);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(62, 15, 62);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(62, 15, 235);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(438, 15, 235);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(438, 15, 62);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	//绘制房内地板
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texFloor);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(62, 171, 62);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(62, 171, 235);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(438, 171, 235);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(438, 171, 62);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	
	//绘制第一层窗户
	glColor4f(pink, 0.4);
	constract(92, 45, 235, 44, 75, 12);
	build();
	glColor4f(pink, 0.4);
	constract(140, 45, 235, 44, 75, 12);
	build();
	//绘制窗户板子
	glColor3f(hgray);
	constract(136, 45, 235, 4, 75, 12);
	build();              
	//绘制第二层窗户
	glColor4f(pink, 0.4);
	constract(87, 201, 235, 44, 85, 12);
	build();
	glColor3f(hgray);
	constract(131, 201, 235, 4, 85, 12);
	build();
	glColor4f(pink, 0.4);
	constract(135, 201, 235, 44, 85, 12);
	build();
	//绘制第二层窗户
	glColor4f(pink, 0.4);
	constract(204, 201, 235, 44, 85, 12);
	build();
	glColor3f(hgray);
	constract(248, 201, 235, 4, 85, 12);
	build();
	glColor4f(pink, 0.4);
	constract(252, 201, 235, 44, 85, 12);
	build();
	//绘制第二层窗户
	glColor4f(pink, 0.4);
	constract(321, 201, 235, 44, 85, 12);
	build();
	glColor3f(hgray);
	constract(365, 201, 235, 4, 85, 12);
	build();
	glColor4f(pink, 0.4);
	constract(369, 201, 235, 44, 85, 12);
	build();
	//绘制车库玻璃
	glColor4f(pink, 0.4);
	constract(330, 22, 450, 130, 90, 10);
	build();


	//绘制紫色房顶
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
	
	//绘制门
	glColor3f(pink);
	constract(260, 10, 250, 40, 110, 5);
	build();              //门
	glColor3f(pink);
	constract(220, 10, 250, 40, 110, 5);
	build();              //门
	glColor3f(hgray);
	constract(265, 60, 255, 5, 5, 5);
	build();              //门把手
	glColor3f(hgray);
	constract(257, 10, 255,6,110,5);
	build();              
	glColor3f(hgray);
	constract(250, 60, 255, 5, 5, 5);
	build();              //门把手
	//绘制小路
	for (int i = 0; i < 6; i++)
	{
		glColor3f(purple1);
		constract(235 - i * 15, 10, 250 + i * 40, 40, 2, 20);
		build();
	}
	//绘制月亮
	glTranslatef(700, 500, 300);
	glPushMatrix();
	qobj = gluNewQuadric();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texMoon);
	gluQuadricTexture(qobj, GL_TRUE);//纹理函数
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	glRotatef(66.3f, 0.0f, 0.0f, 1.0f);
	glRotatef(-corner, 0.0, -0.261, 0.595);
	gluSphere(qobj, 35.0, 40, 40);//二次曲面qobj
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();


	glTranslatef(-700, -500, -300);
	//绘制棕色树干
	glColor3f(brown);
	constract(60, 10, 390, 15, 70, 15);
	build();
	//绘制绿色树叶
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


	//绘制水池中的球
	glColor4f(yellow, 0.8);
	glTranslatef(565 - bx, 25 - by, 250 - bz);
	glutSolidSphere(15, 20, 20);
	glTranslatef(-565, -25, -250);
	
	//刷新
	glFlush();  
	glutSwapBuffers();
}

void Moving()     //月亮旋转（只是演示，实际不存在）
{
	if (sign == 1)
	{
		corner += 0.25f;
		glutPostRedisplay();
	}
}


void init(void)
{	
	//初始化操作
	GLfloat sun_direction[] = { 700.0, 400.0, 100.0, 1.0 };
	GLfloat sun_intensity[] = { sunshine, 1.0 };
	GLfloat ambient_intensity[] = { 0.5, 0.5, 0.5, 1.0 };//设置环境光参数

	//环境光
	glEnable(GL_LIGHTING);              
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient_intensity);

	//漫反射
	glEnable(GL_LIGHT0);                
	glLightfv(GL_LIGHT0, GL_POSITION, sun_direction);//设置光源位置
	glLightfv(GL_LIGHT0, GL_DIFFUSE, sun_intensity);

	//设置背景颜色
	glEnable(GL_COLOR_MATERIAL);       
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	cout << "The OpenGL version is: " << glGetString(GL_VERSION) << "\n";
	cout << glGetString(GL_VENDOR) << "\n";
	glLineWidth(5);
	//定义背景颜色
	glClearColor(0.0, 0.0, 0.0, 0.0);      

	glEnable(GL_BLEND); //打开混合
	glDisable(GL_DEPTH_TEST); //关闭深度测试
	glBlendFunc(GL_SRC_ALPHA, GL_ONE); //基于源象素alpha通道值的半透明混合函数
	glEnable(GL_DEPTH_TEST); //打开深度测试

	glMatrixMode(GL_PROJECTION);     //视角有关的定义，将摄像机放在（0,0,0)，面对着是(0,0,-1).
	glOrtho(-w, w, -h, h, -w, w); // 正交平行视野
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

//键盘交互函数
void specialkeys(int key, int x, int y) {   

	//场景旋转
	if (key == GLUT_KEY_RIGHT)
		rotate_y -= 1;
	else if (key == GLUT_KEY_LEFT)
		rotate_y += 1;
	else if (key == GLUT_KEY_DOWN)
		rotate_x -= 1;
	else if (key == GLUT_KEY_UP)
		rotate_x += 1;

	//场景放缩
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
	//水池球移动
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
// 判断一个整数是不是2的整数次幂
int power_of_two(int n)
{
	if (n <= 0)
		return 0;
	return (n & (n - 1)) == 0;
}

/* 函数load_texture
* 读取一个BMP文件作为纹理
* 如果失败，返回0，如果成功，返回纹理编号
*/
GLuint load_texture(const char* file_name)
{
	GLint width, height, total_bytes;
	GLubyte* pixels = 0;
	GLuint last_texture_ID = 0, texture_ID = 0;
	FILE* pFile;
	errno_t err;

	//打开文件，如果失败，返回
	if ((err = fopen_s(&pFile, file_name, "rb")) != 0)
	{
		printf("无法打开此文件\n");            //如果打不开，就输出打不开
		exit(0);                               //终止程序
	}

	//读取文件中图象的宽度和高度
	fseek(pFile, 0x0012, SEEK_SET);
	fread(&width, 4, 1, pFile);
	fread(&height, 4, 1, pFile);
	fseek(pFile, BMP_Header_Length, SEEK_SET);

	//计算每行像素所占字节数，并根据此数据计算总像素字节数
	{
		GLint line_bytes = width * 3;
		while (line_bytes % 4 != 0)
			++line_bytes;
		total_bytes = line_bytes * height;
	}

	//根据总像素字节数分配内存
	pixels = (GLubyte*)malloc(total_bytes);
	if (pixels == 0)
	{
		fclose(pFile);
		return 0;
	}

	//读取像素数据
	if (fread(pixels, total_bytes, 1, pFile) <= 0)
	{
		free(pixels);
		fclose(pFile);
		return 0;
	}

	//若图象的宽度和高度不是的整数次方，缩放
	//若图像宽高超过了OpenGL规定的最大值，也缩放
	{
		GLint max;
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max);
		if (!power_of_two(width)
			|| !power_of_two(height)
			|| width > max
			|| height > max)
		{
			const GLint new_width = 256;
			const GLint new_height = 256; // 规定缩放后新的大小为边长的正方形
			GLint new_line_bytes, new_total_bytes;
			GLubyte* new_pixels = 0;

			//计算每行需要的字节数和总字节数
			new_line_bytes = new_width * 3;
			while (new_line_bytes % 4 != 0)
				++new_line_bytes;
			new_total_bytes = new_line_bytes * new_height;

			//分配内存
			new_pixels = (GLubyte*)malloc(new_total_bytes);
			if (new_pixels == 0)
			{
				free(pixels);
				fclose(pFile);
				return 0;
			}

			//进行像素缩放
			gluScaleImage(GL_RGB,
				width, height, GL_UNSIGNED_BYTE, pixels,
				new_width, new_height, GL_UNSIGNED_BYTE, new_pixels);

			//释放原来的像素数据，把pixels指向新的像素数据，并重新设置width和height
			free(pixels);
			pixels = new_pixels;
			width = new_width;
			height = new_height;
		}
	}

	// 分配一个新的纹理编号
	glGenTextures(1, &texture_ID);
	if (texture_ID == 0)
	{
		free(pixels);
		fclose(pFile);
		return 0;
	}

	// 绑定新的纹理，载入纹理并设置纹理参数
	// 绑定前，先获得原来绑定的纹理编号，以便在最后进行恢复
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
	//恢复之前的纹理绑定
	glBindTexture(GL_TEXTURE_2D, lastTextureID);  
	free(pixels);
	return texture_ID;
}

int main(int argc, char** argv)   
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);//定义窗口显示模式
	glutInitWindowSize(w, h);//窗口大小
	glutInitWindowPosition(100, 100);//窗口位置
	glutCreateWindow("My Home");//窗口标题
	init();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);//允许深度测试

	//启用纹理
	glEnable(GL_TEXTURE_2D);    
	//加载纹理
	texGround = load_texture("ground.bmp");  
	texPool = load_texture("pool.bmp");
	texFloor = load_texture("floor.bmp");
	texMoon = load_texture("moon.bmp");
	texWall = load_texture("wall.bmp");
	
	glutDisplayFunc(display);
	glutIdleFunc(Moving);
	//调用键盘控制函数	
	glutSpecialFunc(specialkeys); 
	glutKeyboardFunc(NormalKeys);
	//定时器
	glutTimerFunc(1000, timerFunction, 1);  
	glutMainLoop();
	system("pause");
	return 0;
}

