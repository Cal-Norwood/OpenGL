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
#include <math.h>
#define ToRadians (M_PI/180.0f)
using namespace std;
using namespace chrono;

bool bExit = false;

float cameraSpeedX = 0;
float cameraPosY = 0;
float cameraSpeedZ = 0;

float cameraOffsetX = 0;
float cameraOffsetY = 0;
float cameraOffsetZ = 0;

float cameraRotationY = 0;

float horizontal = 0;
float vertical = 0;
float sensitivity = 0.0025;

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

	case 'u':
		bExit = true;
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
	
	cameraRotationY += horizontal;

	vertical = x * sensitivity;

	//cout << horizontal << endl;
	glTranslatef(-cameraOffsetX, 0, -cameraOffsetZ);
	glRotatef(horizontal, 0.0f, 1.0f, 0.0f);
	glTranslatef(cameraOffsetX, 0, cameraOffsetZ);
	//glRotatef(vertical, 1.0f, 0.0f, 0.0f);

	previousX = x;
}

void Update(int time) 
{
	glutTimerFunc(time, Update, time);

	if (isWPressed == true) 
	{
		cameraSpeedZ = 0.1f;
		//cameraOffsetZ += 0.1f;
	} 

	if (isAPressed == true)
	{
		cameraSpeedX = 0.2f;
		//cameraOffsetX += 0.2f;
	}

	if (isDPressed == true)
	{
		cameraSpeedX = -0.2f;
		//cameraOffsetX -= 0.2f;
	}

	if (isSPressed == true)
	{
		cameraSpeedZ = -0.1f;
		//cameraOffsetZ -= 0.1f;
	}

	if (isDPressed == false && isAPressed == false)
	{
		cameraSpeedX = 0;
	}

	if (isWPressed == false && isSPressed == false)
	{
		cameraSpeedZ = 0;
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
	if (bExit == true)
	{
		exit(0);
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	float cosAngle = cosf(cameraRotationY * ToRadians);
	float sinAngle = sinf(cameraRotationY * ToRadians);
	float localSpeedX = (-cameraSpeedX * cosAngle) + (cameraSpeedZ * sinAngle);
	float localSpeedZ = (cameraSpeedX * sinAngle) + (cameraSpeedZ * cosAngle);

	cout << horizontal << endl;
	cout << localSpeedX << " " << localSpeedZ << endl;

	glTranslatef(-localSpeedX, cameraPosY, localSpeedZ);
	cameraOffsetX -= localSpeedX;
	cameraOffsetZ += localSpeedZ;

	DrawPolygon();
	DrawPolygon1();
	DrawPolygon2();
	DrawPolygon3();
	DrawPolygon4();
	DrawPolygon5();

	glFlush();

	rotation += 1;
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
		glColor4f(0.3, 0.3, 0.3, 0.3);
		glVertex3f(2, 4, -10);
		glVertex3f(-2, 4, -10);
		glVertex3f(-2, -4, -10);
		glVertex3f(2, -4, -10);

		glEnd();
	}

	glPopMatrix();
}

void OpenGL::DrawPolygon2()
{
	glPushMatrix();

	glBegin(GL_POLYGON);
	{
		glColor4f(0.3, 0.3, 0.3, 0.3);
		glVertex3f(2, -1, -10);
		glVertex3f(2, -1, 10);
		glVertex3f(2, -4, 10);
		glVertex3f(2, -4, -10);

		glEnd();
	}

	glBegin(GL_POLYGON);
	{
		glColor4f(0.3, 0.3, 0.3, 0.3);
		glVertex3f(2, 4, -10);
		glVertex3f(2, 4, 10);
		glVertex3f(2, 1, 10);
		glVertex3f(2, 1, -10);

		glEnd();
	}

	glBegin(GL_POLYGON);
	{
		glColor4f(0.3, 0.3, 0.3, 0.3);
		glVertex3f(2, -1, -10);
		glVertex3f(2, -1, 10);
		glVertex3f(3, -1, 10);
		glVertex3f(3, -1, -10);

		glEnd();
	}

	glBegin(GL_POLYGON);
	{
		glColor4f(0.3, 0.3, 0.3, 0.3);
		glVertex3f(2, 1, -10);
		glVertex3f(2, 1, 10);
		glVertex3f(3, 1, 10);
		glVertex3f(3, 1, -10);

		glEnd();
	}

	glPopMatrix();
}

void OpenGL::DrawPolygon3()
{
	glPushMatrix();

	glBegin(GL_POLYGON);
	{
		glColor4f(0.3, 0.3, 0.3, 0.3);
		glVertex3f(-2, -1, -10);
		glVertex3f(-2, -1, 10);
		glVertex3f(-2, -4, 10);
		glVertex3f(-2, -4, -10);

		glEnd();
	}

	glBegin(GL_POLYGON);
	{
		glColor4f(0.3, 0.3, 0.3, 0.3);
		glVertex3f(-2, 4, -10);
		glVertex3f(-2, 4, 10);
		glVertex3f(-2, 1, 10);
		glVertex3f(-2, 1, -10);

		glEnd();
	}

	glBegin(GL_POLYGON);
	{
		glColor4f(0.3, 0.3, 0.3, 0.3);
		glVertex3f(-2, -1, -10);
		glVertex3f(-2, -1, 10);
		glVertex3f(-3, -1, 10);
		glVertex3f(-3, -1, -10);

		glEnd();
	}

	glBegin(GL_POLYGON);
	{
		glColor4f(0.3, 0.3, 0.3, 0.3);
		glVertex3f(-2, 1, -10);
		glVertex3f(-2, 1, 10);
		glVertex3f(-3, 1, 10);
		glVertex3f(-3, 1, -10);

		glEnd();
	}

	glPopMatrix();
}

void OpenGL::DrawPolygon4()
{
	glPushMatrix();

	glBegin(GL_POLYGON);
	{
		glColor4f(0.3, 0.3, 0.3, 0.3);
		glVertex3f(2, 4, 10);
		glVertex3f(-2, 4, 10);
		glVertex3f(-2, -4, 10);
		glVertex3f(2, -4, 10);

		glEnd();
	}

	glPopMatrix();
}

void OpenGL::DrawPolygon5()
{
	glPushMatrix();

	glBegin(GL_POLYGON);
	{
		glColor4f(0.4, 0.4, 0.4, 0.4);
		glVertex3f(2, -4, -10);
		glVertex3f(2, -4, 10);
		glVertex3f(-2, -4, 10);
		glVertex3f(-2, -4, -10);

		glEnd();
	}

	glBegin(GL_POLYGON);
	{
		glColor4f(0.4, 0.4, 0.4, 0.4);
		glVertex3f(2, 4, -10);
		glVertex3f(2, 4, 10);
		glVertex3f(-2, 4, 10);
		glVertex3f(-2, 4, -10);

		glEnd();
	}

	glPopMatrix();
}