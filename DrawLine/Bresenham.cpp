#include "./../TestInterface.h"

#ifdef Bresenham

#include <iostream>
#include <math.h>

void drawPixel(int x, int y)
{
	std::cout << "draw pixed (" << x << ", " << y << ");" << std::endl;
}

void lineBresenham(int startX, int startY, int endX, int endY)
{
	int x = startX;
	int y = startY;
	if (x > endX) //±£Ö¤dx > 0
	{
		x = endX;
		endX = startX;

		y = endY;
		endY = startY;
	}

	int dx = endX - x;
	int dy = endY - y;
	int p = 2*dy - dx;

	drawPixel(x, y);

	while(x < endX)
	{
		x++;
		if (p > 0)
		{
			y++;
			p += 2 * (dy - dx);
		}
		else
		{
			p += 2 * dy;
		}
		drawPixel(x, y);
	}
}

int test_interface(int argc, char ** argv)
{
	lineBresenham(20, 10, 30, 18);
	system("pause");
	return 0;
}

#endif