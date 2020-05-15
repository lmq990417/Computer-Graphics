#include<windows.h>
#include<glut.h>
#include<stdlib.h>
#include<math.h>
#include<stdio.h>
using namespace std;

// ���Ա߱�AET���±߱�NET
typedef struct XET
{
	float x;
	float dx, ymax;
	XET* next;
}AET, NET;

//���λ��
struct Point
{
	int x;
	int y;
}polypoint;

void PolyScan(int POINTNUM, int z,struct Point polypoint[100])//�����ɨ��
{
	int MaxY = 0;//������ߵ��y����
	int i;
	for (i = 0; i < POINTNUM; i++)
		if (polypoint[i].y > MaxY)
			MaxY = polypoint[i].y;
	AET* pAET = new AET;//��ʼ��AET��
	pAET->next = NULL;
	NET* pNET[1024];//��ʼ��NET��
	for (i = 0; i <= MaxY; i++)
	{
		pNET[i] = new NET;
		pNET[i]->next = NULL;
	}
	glColor3f(0.5294f, 0.8078f, 0.9804f); //����ֱ�ߵ���ɫ
	glBegin(GL_POINTS);
	for (i = 0; i <= MaxY; i++)//ɨ�貢����NET��
	{
		for (int j = 0; j < POINTNUM; j++)
			if (polypoint[j].y == i)//һ�����ǰ���һ�����γ�һ���߶Σ�������ĵ�Ҳ�γ��߶�
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
	//*���������»��Ա߱�AET
	for (i = 0; i <= MaxY; i++)
	{
		NET* p = pAET->next;//�����µĽ���x,����AET
		while (p)
		{
			p->x = p->x + p->dx;
			p = p->next;
		}
		AET* tq = pAET;//���º���AET�����򣬶ϱ�����,���ٿ��ٿռ�
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
		AET* q = pAET;//(�Ľ��㷨)�ȴ�AET����ɾ��ymax==i�Ľ��
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
		p = pNET[i]->next;//��NET�е��µ����AET,���ò��뷨��Xֵ��������
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
		//��������ɫ
		p = pAET->next;
		while (p && p->next)
		{
			for (float j = p->x; j <= p->next->x; j++)
				glVertex3i(static_cast<int>(j), i,z);
			p = p->next->next;//���Ƕ˵����
		}
	}
}