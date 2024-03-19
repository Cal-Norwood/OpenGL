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
#define _USE_MATH_DEFINES
#include <math.h>
using namespace std;
using namespace chrono;

float rotation = 0;
float fx = 0.2;
float bx = -fx;

float ty = fx;
float by = -fx;

float angle;

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
	gluPerspective(45, 1/1, 0.1, 10);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

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

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glutMainLoop();
}

OpenGL::~OpenGL(void)
{

}

void OpenGL::Display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	DrawPolygon();
	DrawPolygon1();

	glFlush();

	rotation += 0.01;
	glutPostRedisplay();
}

void OpenGL::DrawPolygon()
{
	glPushMatrix();
	glRotatef(rotation, 1, 1, 1);

	glPointSize(10);

	glBegin(GL_LINE_LOOP);
	{
		glColor4f(1, 0, 0, 0);
		glVertex3f(0.05, 0.05, 0.05);
		glVertex3f(-0.05, 0.05, 0.05);
		glVertex3f(-0.05, -0.05, 0.05);
		glVertex3f(0.05, -0.05, 0.05);

		glEnd();
	}

	glBegin(GL_LINE_LOOP);
	{
		glColor4f(0, 1, 0, 0);
		glVertex3f(0.05, -0.05, -0.05);
		glVertex3f(-0.05, -0.05, -0.05);
		glVertex3f(-0.05, 0.05, -0.05);
		glVertex3f(0.05, 0.05, -0.05);

		glEnd();
	}

	glBegin(GL_LINE_LOOP);
	{
		glColor4f(0, 0, 1, 0);
		glVertex3f(0.05, 0.05, 0.05);
		glVertex3f(0.05, 0.05, -0.05);
		glVertex3f(0.05, -0.05, -0.05);
		glVertex3f(0.05, -0.05, 0.05);

		glEnd();
	}

	glBegin(GL_LINE_LOOP);
	{
		glColor4f(1, 0, 1, 0);
		glVertex3f(-0.05, 0.05, 0.05);
		glVertex3f(-0.05, 0.05, -0.05);
		glVertex3f(-0.05, -0.05, -0.05);
		glVertex3f(-0.05, -0.05, 0.05);

		glEnd();
	}

	glBegin(GL_LINE_LOOP);
	{
		glColor4f(1, 1, 1, 0);
		glVertex3f(0.05, -0.05, 0.05);
		glVertex3f(-0.05, -0.05, 0.05);
		glVertex3f(-0.05, -0.05, -0.05);
		glVertex3f(0.05, -0.05, -0.05);

		glEnd();
	}

	glBegin(GL_LINE_LOOP);
	{
		glColor4f(1, 1, 0, 0);
		glVertex3f(0.05, 0.05, 0.05);
		glVertex3f(-0.05, 0.05, 0.05);
		glVertex3f(-0.05, 0.05, -0.05);
		glVertex3f(0.05, 0.05, -0.05);

		glEnd();
	}

	glPopMatrix();
}

void OpenGL::DrawPolygon1() 
{
	glPushMatrix();

	glBegin(GL_QUADS);
	{
		glColor4f(1, 0, 0, 0);
		glVertex3f(0.9, 0.75, -1);
		glVertex3f(-0.9, 0.75, -1);
		glVertex3f(-0.9, -0.75, -1);
		glVertex3f(0.9, -0.75, -1);

		glEnd();
	}

	glPopMatrix();
}