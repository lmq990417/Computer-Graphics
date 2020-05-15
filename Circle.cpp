#include<windows.h>
#include<glut.h>
#include<stdlib.h>
#include<math.h>
#include<stdio.h>

void setPixel(int x, int y, int z, float RGB[]);
//Bresenham»­Ô²
void BresenhamCircle(int x0, int y0, int z0, int r, float RGB[])
{
	int x = 0;
	int y = r;
	int d = 2 * (1 - r);
	int d1 = 0;
	int d2 = 0;
	int direction;
	while (y >= 0)
	{
		setPixel(x0 + x, y0 + y, z0,RGB);
		setPixel(x0 + x, y0 - y, z0,RGB);
		setPixel(x0 - x, y0 - y, z0, RGB);
		setPixel(x0 - x, y0 + y, z0, RGB);
		if (d < 0)
		{
			d1 = 2 * (d + y) - 1;
			if (d1 < 0)
			{
				direction = 1;
			}
			else
			{
				direction = 2;
			}
		}
		else if (d > 0)
		{
			d2 = 2 * (d - x) - 1;
			if (d < 0)
			{
				direction = 2;
			}
			else
			{
				direction = 3;
			}
		}
		else
			direction = 2;
		switch (direction)
		{
		case 1:
			x++;
			d += 2 * x + 1;
			break;
		case 2:
			x++;
			y--;
			d += 2 * (x - y + 1);
			break;
		case 3:
			y--;
			d += (-2 * y + 1);
			break;
		}
	}
}

