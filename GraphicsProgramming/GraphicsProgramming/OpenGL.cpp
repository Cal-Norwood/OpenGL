#pragma once
#include "OpenGL.h"
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include "GL\freeglut.h"
#include <chrono>
#include <thread>
using namespace std;
using namespace chrono;

float x = 0.5;
float y = 0.5;
bool zoomIn = true;

OpenGL::OpenGL(int argc, char* argv[])
{
	GLUTCallbacks::Init(this);
	glutInit(&argc, argv);
	glutInitWindowSize(800, 800);
	glutCreateWindow("OpenGL Program");
	glutDisplayFunc(GLUTCallbacks::Display);
	glutMainLoop();
}

int main(int argc, char* argv[])
{
	OpenGL* game = new OpenGL(argc, argv);

	return 0;

	glutMainLoop();
}

OpenGL::~OpenGL(void)
{

}

void OpenGL::Display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	DrawPolygon();
	glFlush();

	if (zoomIn == true)
	{
		x += 0.0001;
		y += 0.0001;
	}
	else
	{
		x -= 0.0001;
		y -= 0.0001;
	}

	if (x >= 1 || y >= 1)
	{
		zoomIn = false;
	}

	if (x <= 0 || y <= 0)
	{
		zoomIn = true;
	}

	glutPostRedisplay();
}

void OpenGL::DrawPolygon()
{
	glBegin(GL_POLYGON);
	{
		glColor4f(1, 0, 0, 0);
		glVertex2f(-x, y);
		glColor4f(0, 1, 0, 0);
		glVertex2f(x, y);
		glColor4f(0, 0, 1, 0);
		glVertex2f(x, -y);
		glColor4f(0.75, 0.5, 0.5, 1);
		glVertex2f(-x, -y);
		glEnd();
	}
}