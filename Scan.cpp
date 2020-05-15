#include<windows.h>
#include<glut.h>
#include<stdlib.h>
#include<math.h>
#include<stdio.h>
using namespace std;

// 活性边表AET和新边表NET
typedef struct XET
{
	float x;
	float dx, ymax;
	XET* next;
}AET, NET;

//点的位置
struct Point
{
	int x;
	int y;
}polypoint;

void PolyScan(int POINTNUM, int z,struct Point polypoint[100])//多边形扫描
{
	int MaxY = 0;//计算最高点的y坐标
	int i;
	for (i = 0; i < POINTNUM; i++)
		if (polypoint[i].y > MaxY)
			MaxY = polypoint[i].y;
	AET* pAET = new AET;//初始化AET表
	pAET->next = NULL;
	NET* pNET[1024];//初始化NET表
	for (i = 0; i <= MaxY; i++)
	{
		pNET[i] = new NET;
		pNET[i]->next = NULL;
	}
	glColor3f(0.5294f, 0.8078f, 0.9804f); //设置直线的颜色
	glBegin(GL_POINTS);
	for (i = 0; i <= MaxY; i++)//扫描并建立NET表
	{
		for (int j = 0; j < POINTNUM; j++)
			if (polypoint[j].y == i)//一个点跟前面的一个点形成一条线段，跟后面的点也形成线段
			{
				if (polypoint[(j - 1 + POINTNUM) % POINTNUM].y > polypoint[j].y)
				{
					NET* p = new NET;
					p->x = polypoint[j].x;
					p->ymax = polypoint[(j - 1 + POINTNUM) % POINTNUM].y;
					p->dx = (polypoint[(j - 1 + POINTNUM) % POINTNUM].x - polypoint[j].x) / (polypoint[(j - 1 + POINTNUM) % POINTNUM].y - polypoint[j].y);
					p->next = pNET[i]->next;
					pNET[i]->next = p;
				}
				if (polypoint[(j + 1 + POINTNUM) % POINTNUM].y > polypoint[j].y)
				{
					NET* p = new NET;
					p->x = polypoint[j].x;
					p->ymax = polypoint[(j + 1 + POINTNUM) % POINTNUM].y;
					p->dx = (polypoint[(j + 1 + POINTNUM) % POINTNUM].x - polypoint[j].x) / (polypoint[(j + 1 + POINTNUM) % POINTNUM].y - polypoint[j].y);
					p->next = pNET[i]->next;
					pNET[i]->next = p;
				}
			}
	}
	//*建立并更新活性边表AET
	for (i = 0; i <= MaxY; i++)
	{
		NET* p = pAET->next;//计算新的交点x,更新AET
		while (p)
		{
			p->x = p->x + p->dx;
			p = p->next;
		}
		AET* tq = pAET;//更新后新AET先排序，断表排序,不再开辟空间
		p = pAET->next;
		tq->next = NULL;
		while (p)
		{
			while (tq->next && p->x >= tq->next->x)
				tq = tq->next;
			NET* s = p->next;
			p->next = tq->next;
			tq->next = p;
			p = s;
			tq = pAET;
		}
		AET* q = pAET;//(改进算法)先从AET表中删除ymax==i的结点
		p = q->next;
		while (p)
		{
			if (p->ymax == i)
			{
				q->next = p->next;
				delete p;
				p = q->next;
			}
			else
			{
				q = q->next;
				p = q->next;
			}
		}
		p = pNET[i]->next;//将NET中的新点加入AET,并用插入法按X值递增排序
		q = pAET;
		while (p)
		{
			while (q->next && p->x >= q->next->x)
				q = q->next;
			NET* s = p->next;
			p->next = q->next;
			q->next = p;
			p = s;
			q = pAET;
		}
		//配对填充颜色
		p = pAET->next;
		while (p && p->next)
		{
			for (float j = p->x; j <= p->next->x; j++)
				glVertex3i(static_cast<int>(j), i,z);
			p = p->next->next;//考虑端点情况
		}
	}
}