#pragma once
#include "OpenGL.h"
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include "GL\freeglut.h"
#include <chrono>
#include <thread>
#include <iostream>
#include <random>
using namespace std;
using namespace chrono;

float fx = 0.2;
float bx = -fx;

float ty = fx;
float by = -fx;

bool moveRight = true;
bool moveUp = true;

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
	random_device rd;
	uniform_real_distribution<float> dist(-0.5, 1);
	float offset = dist(rd);
	cout << offset;
	fx = offset;
	bx = offset - 0.5;

	offset = dist(rd);
	cout << offset;
	ty = offset;
	by = offset - 0.5;
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
	if (moveRight == true)
	{
		fx += 0.0001;
		bx += 0.0001;
	}
	else
	{
		fx -= 0.0001;
		bx -= 0.0001;
	}

	if (moveUp == true)
	{
		ty += 0.00009;
		by += 0.00009;
	}
	else
	{
		ty -= 0.00009;
		by -= 0.00009;
	}

	if (fx >= 1 )
	{
		moveRight = false;
	}

	if (bx < -1)
	{
		moveRight = true;
	}

	if (ty >= 1)
	{
		moveUp = false;
	}

	if (by <= -1)
	{
		moveUp = true;
	}

	glutPostRedisplay();
}

void OpenGL::DrawPolygon()
{
	glBegin(GL_POLYGON);
	{
		glColor4f(1, 0, 0, 0);
		glVertex2f(bx, ty);
		glColor4f(0, 1, 0, 0);
		glVertex2f(fx, ty);
		glColor4f(0, 0, 1, 0);
		glVertex2f(fx, by);
		glColor4f(0, 0, 0, 1);
		glVertex2f(bx, by);
		glEnd();
	}
}