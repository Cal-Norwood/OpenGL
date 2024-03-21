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
#include <cmath>
#define _USE_MATH_DEFINES
#define TO_RADIANS 3.141592/180.0
#include <math.h>
using namespace std;
using namespace chrono;

float cameraPosX = 0;
float cameraPosY = 0;
float cameraPosZ = 0;

float horizontal = 0;
float vertical = 0;
float sensitivity = 0.005;

float previousX = 0;
bool negativeX = false;

bool isWPressed = false;
bool isAPressed = false;
bool isSPressed = false;
bool isDPressed = false;

float rotation = 0;
float fx = 0.2;
float bx = -fx;

float ty = fx;
float by = -fx;

float angle;

bool moveRight = true;
bool moveUp = true;

void KeyboardInputDown(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
		isWPressed = true;
		break;

	case 'a':
		isAPressed = true;
		break;

	case 'd':
		isDPressed = true;
		break;

	case 's':
		isSPressed = true;
		break;
	}
}

void KeyboardInputUp(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
		isWPressed = false;
		break;

	case 'a':
		isAPressed = false;
		break;

	case 'd':
		isDPressed = false;
		break;

	case 's':
		isSPressed = false;
		break;
	}
}

void MouseInput(int x, int y) 
{
	if (x < previousX)
	{
		horizontal = 0;
		negativeX = true;
	} 
	else
	{
		horizontal = 0;
		negativeX = false;
	}

	if (negativeX == true)
	{
		horizontal = -x * sensitivity;
	}
	else 
	{
		horizontal = fabs(x) * sensitivity;
	}

	vertical = x * sensitivity;

	//glRotatef(horizontal, 0.0f, 1.0f, 0.0f);
	//glRotatef(vertical, 1.0f, 0.0f, 0.0f);

	previousX = x;
}

void Update(int time) 
{
	glutTimerFunc(time, Update, time);

	if (isWPressed == true) 
	{
		cameraPosZ = 0.1f;
	} 

	if (isAPressed == true)
	{
		cameraPosX = 0.2f;
	}

	if (isDPressed == true)
	{
		cameraPosX = -0.2f;
	}

	if (isSPressed == true)
	{
		cameraPosZ = -0.1f;
	}

	if (isDPressed == false && isAPressed == false)
	{
		cameraPosX = 0;
	}

	if (isWPressed == false && isSPressed == false)
	{
		cameraPosZ = 0;
	}

	glutPostRedisplay();
}

OpenGL::OpenGL(int argc, char* argv[])
{
	GLUTCallbacks::Init(this);
	glutInit(&argc, argv);
	glutInitWindowSize(1920, 1080);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH);
	glutCreateWindow("OpenGL Program");
	glutDisplayFunc(GLUTCallbacks::Display);

	gluPerspective(120, 1 / 1, 0.1, 100);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);

	glutKeyboardFunc(KeyboardInputDown);
	glutKeyboardUpFunc(KeyboardInputUp);
	glutPassiveMotionFunc(MouseInput);
	glutIgnoreKeyRepeat(1);
	Update(1000 / 60);
	glutMainLoop();
}

int main(int argc, char* argv[])
{
	random_device rd;
	uniform_real_distribution<float> dist(-0.5, 1);
	float offset = dist(rd);
	fx = offset;
	bx = offset - 0.5;

	offset = dist(rd);
	ty = offset;
	by = offset - 0.5;
	OpenGL* game = new OpenGL(argc, argv);
	return 0;
}

void OpenGL::Display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glTranslatef(cameraPosX, cameraPosY, cameraPosZ);

	DrawPolygon();
	DrawPolygon1();
	DrawPolygon2();
	DrawPolygon3();
	DrawPolygon4();
	DrawPolygon5();

	glFlush();

	rotation += 0.01;
}

void OpenGL::DrawPolygon()
{
	glPushMatrix();
	glRotatef(rotation, 0.1, 0, 3);

	glPointSize(10);

	glBegin(GL_LINE_LOOP);
	{
		glColor4f(1, 0, 0, 0);
		glVertex3f(0.05, 0.05, -1);
		glVertex3f(-0.05, 0.05, -1);
		glVertex3f(-0.05, -0.05, -1);
		glVertex3f(0.05, -0.05, -1);

		glEnd();
	}

	glBegin(GL_LINE_LOOP);
	{
		glColor4f(0, 1, 0, 0);
		glVertex3f(0.05, -0.05, -2);
		glVertex3f(-0.05, -0.05, -2);
		glVertex3f(-0.05, 0.05, -2);
		glVertex3f(0.05, 0.05, -2);

		glEnd();
	}

	glBegin(GL_LINE_LOOP);
	{
		glColor4f(0, 0, 1, 0);
		glVertex3f(0.05, 0.05, -1);
		glVertex3f(0.05, 0.05, -2);
		glVertex3f(0.05, -0.05, -2);
		glVertex3f(0.05, -0.05, -1);

		glEnd();
	}

	glBegin(GL_LINE_LOOP);
	{
		glColor4f(1, 0, 1, 0);
		glVertex3f(-0.05, 0.05, -1);
		glVertex3f(-0.05, 0.05, -2);
		glVertex3f(-0.05, -0.05, -2);
		glVertex3f(-0.05, -0.05, -1);

		glEnd();
	}

	glBegin(GL_LINE_LOOP);
	{
		glColor4f(1, 1, 1, 0);
		glVertex3f(0.05, -0.05, -1);
		glVertex3f(-0.05, -0.05, -1);
		glVertex3f(-0.05, -0.05, -2);
		glVertex3f(0.05, -0.05, -2);

		glEnd();
	}

	glBegin(GL_LINE_LOOP);
	{
		glColor4f(1, 1, 0, 0);
		glVertex3f(0.05, 0.05, -1);
		glVertex3f(-0.05, 0.05, -1);
		glVertex3f(-0.05, 0.05, -2);
		glVertex3f(0.05, 0.05, -2);

		glEnd();
	}

	glPopMatrix();
}

void OpenGL::DrawPolygon1() 
{
	glPushMatrix();

	glBegin(GL_POLYGON);
	{
		glColor4f(1, 0, 0, 0);
		glVertex3f(15, 4, -10);
		glVertex3f(-15, 4, -10);
		glVertex3f(-15, -4, -10);
		glVertex3f(15, -4, -10);

		glEnd();
	}

	glPopMatrix();
}

void OpenGL::DrawPolygon2()
{
	glPushMatrix();

	glBegin(GL_POLYGON);
	{
		glColor4f(0.9, 0, 0, 0);
		glVertex3f(15, 4, -10);
		glVertex3f(15, 4, 10);
		glVertex3f(15, -4, 10);
		glVertex3f(15, -4, -10);

		glEnd();
	}

	glPopMatrix();
}

void OpenGL::DrawPolygon3()
{
	glPushMatrix();

	glBegin(GL_POLYGON);
	{
		glColor4f(0.8, 0, 0, 0);
		glVertex3f(-15, 4, -10);
		glVertex3f(-15, 4, 10);
		glVertex3f(-15, -4, 10);
		glVertex3f(-15, -4, -10);

		glEnd();
	}

	glPopMatrix();
}

void OpenGL::DrawPolygon4()
{
	glPushMatrix();

	glBegin(GL_POLYGON);
	{
		glColor4f(0.7, 0, 0, 0);
		glVertex3f(15, 4, 10);
		glVertex3f(-15, 4, 10);
		glVertex3f(-15, -4, 10);
		glVertex3f(15, -4, 10);

		glEnd();
	}

	glPopMatrix();
}

void OpenGL::DrawPolygon5()
{
	glPushMatrix();

	glBegin(GL_POLYGON);
	{
		glColor4f(0, 1, 0, 0);
		glVertex3f(15, -4, -10);
		glVertex3f(15, -4, 10);
		glVertex3f(-15, -4, 10);
		glVertex3f(-15, -4, -10);

		glEnd();
	}

	glPopMatrix();
}