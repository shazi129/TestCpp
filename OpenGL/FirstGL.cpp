#include "./../TestInterface.h"

#ifdef FirstGL

#include <GL/glut.h>
#include <iostream>

void displayFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex2i(185, 15);
	glVertex2i(10, 145);
	glEnd();
	glFlush();
}

void init()
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, 200.0, 0.0, 150.0);
}

int test_interface(int argc, char ** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(400, 200);
	glutInitWindowSize(1000, 600);
	glutCreateWindow("openGL test");
	init();
	glutDisplayFunc(displayFunc);
	glutMainLoop();
	return 0;
}

#endif