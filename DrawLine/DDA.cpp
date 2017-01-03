#include "./../TestInterface.h"

#ifdef DDA

#include <iostream>
#include <math.h>

void drawPixel(int x, int y)
{
	std::cout << "draw pixed (" << x << ", " << y << ");" << std::endl;
}

//浮点数转整数的宏  
inline int round(float a) {return int(a+0.5f);}  

void lineDDA(int startX, int startY, int endX, int endY)
{
	float dx = endX - startX;
	float dy = endY - startY;
	float steps = fabs(dx) > fabs(dy) ? fabs(dx) : fabs(dy);
	float incX = dx / steps;
	float incY = dy / steps;

	for (int i = 0; i <= steps; i++)
	{
		float newX = (float)startX + (float)(i) * incX;
		float newY = (float)startY + (float)(i) * incY;
		drawPixel(round(newX), round(newY));
	}
}


int test_interface(int argc, char ** argv)
{
	lineDDA(2, 0, 6, 2);
	return 0;
}

#endif