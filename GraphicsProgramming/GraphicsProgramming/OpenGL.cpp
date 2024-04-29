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
#include <fstream>
#define _USE_MATH_DEFINES
#include <math.h>
#define ToRadians (M_PI/180.0f)
using namespace std;
using namespace chrono;

class Texture2D
{
private:
	GLuint _ID;
	int _width, _height;

public:

	bool Load(const char* path, int width, int height);

	GLuint GetID() const
	{
		return _ID;
	}
	int GetWidth() const { return _width; }
	int GetHeight() const { return _height; }
};

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
float sensitivity = 0.0025f;

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

bool skyboxSpawned = false;

Texture2D* f;
Texture2D* l;
Texture2D* r;
Texture2D* b;
Texture2D* u;
Texture2D* d;

string CubeMap[]
{
	"yellowcloud_bk.jpg",
	"yellowcloud_dn.jpg",
	"yellowcloud_ft.jpg",
	"yellowcloud_lf.jpg",
	"yellowcloud_rt.jpg",
	"yellowcloud_up.jpg"
};

bool Texture2D::Load(const char* path, int width, int height)
{
	char* tempTextureData; int fileSize; ifstream inFile;
	_width = width; _height = height;
	inFile.open(path, ios::binary);
	if (!inFile.is_open()) {
		cerr << "Can't open texture file " << path << endl;
		return false;
	}
	inFile.seekg(0, ios::end);
	fileSize = (int)inFile.tellg();
	tempTextureData = new char[fileSize];
	inFile.seekg(0, ios::beg);
	inFile.read(tempTextureData, fileSize);
	inFile.close();
	glGenTextures(1, &_ID);
	glBindTexture(GL_TEXTURE_2D, _ID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, tempTextureData);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	return true;
}

Texture2D* LoadTextureAndBind(const char* tex)
{
	Texture2D* texture = new Texture2D();
	texture->Load(tex, 1024, 1024);

	//glBindTexture(GL_TEXTURE_2D, texture->GetID());
	//glBindTexture(GL_TEXTURE_2D, 1);

	return texture;
}

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

	gluPerspective(120, 1 / 1, 0.1, 100);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_COLOR_MATERIAL);
	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);

	f = LoadTextureAndBind("yellowcloud_ft.bmp");
	l = LoadTextureAndBind("yellowcloud_lf.bmp");
	r = LoadTextureAndBind("yellowcloud_rt.bmp");
	b = LoadTextureAndBind("yellowcloud_bk.bmp");
	u = LoadTextureAndBind("yellowcloud_up.bmp");
	d = LoadTextureAndBind("yellowcloud_dn.bmp");

	glutDisplayFunc(GLUTCallbacks::Display);
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

	DrawPolygon0();
	DrawPolygon();
	DrawPolygon1();
	DrawPolygon2();
	DrawPolygon3();
	DrawPolygon4();
	DrawPolygon5();

	glFlush();

	rotation += 1;
}

void OpenGL::DrawPolygon0()
{
	float skyboxCullingEdge = 99 / sqrt(3);

	glBindTexture(GL_TEXTURE_2D, f->GetID());

	glBegin(GL_QUADS);
	{
		glColor4f(1, 1, 1, 1);
		glTexCoord2f(0, 0);
		glVertex3f(-skyboxCullingEdge, -skyboxCullingEdge, -skyboxCullingEdge);
		glTexCoord2f(0, 1);
		glVertex3f(-skyboxCullingEdge, skyboxCullingEdge, -skyboxCullingEdge);
		glTexCoord2f(1, 1);
		glVertex3f(skyboxCullingEdge, skyboxCullingEdge, -skyboxCullingEdge);
		glTexCoord2f(1, 0);
		glVertex3f(skyboxCullingEdge, -skyboxCullingEdge, -skyboxCullingEdge);

		glEnd();
	}

	/*glBindTexture(GL_TEXTURE_2D, l->GetID());

	glBegin(GL_QUADS);
	{
		glColor4f(1, 1, 1, 1);
		glTexCoord2f(0, 0);
		glVertex3f(-skyboxCullingEdge, skyboxCullingEdge, skyboxCullingEdge);
		glTexCoord2f(0, 1);
		glVertex3f(-skyboxCullingEdge, -skyboxCullingEdge, skyboxCullingEdge);
		glTexCoord2f(1, 1);
		glVertex3f(-skyboxCullingEdge, -skyboxCullingEdge, -skyboxCullingEdge);
		glTexCoord2f(1, 0);
		glVertex3f(-skyboxCullingEdge, skyboxCullingEdge, -skyboxCullingEdge);

		glEnd();
	}

	glBindTexture(GL_TEXTURE_2D, b->GetID());

	glBegin(GL_QUADS);
	{
		glColor4f(1, 1, 1, 1);
		glTexCoord2f(0, 0);
		glVertex3f(-skyboxCullingEdge, skyboxCullingEdge, skyboxCullingEdge);
		glTexCoord2f(0, 1);
		glVertex3f(-skyboxCullingEdge, -skyboxCullingEdge, skyboxCullingEdge);
		glTexCoord2f(1, 1);
		glVertex3f(skyboxCullingEdge, -skyboxCullingEdge, skyboxCullingEdge);
		glTexCoord2f(1, 0);
		glVertex3f(skyboxCullingEdge, skyboxCullingEdge, skyboxCullingEdge);

		glEnd();
	}

	glBindTexture(GL_TEXTURE_2D, r->GetID());

	glBegin(GL_QUADS);
	{
		glColor4f(1, 1, 1, 1);
		glTexCoord2f(0, 0);
		glVertex3f(skyboxCullingEdge, skyboxCullingEdge, -skyboxCullingEdge);
		glTexCoord2f(0, 1);
		glVertex3f(skyboxCullingEdge, -skyboxCullingEdge, -skyboxCullingEdge);
		glTexCoord2f(1, 1);
		glVertex3f(skyboxCullingEdge, -skyboxCullingEdge, skyboxCullingEdge);
		glTexCoord2f(1, 0);
		glVertex3f(skyboxCullingEdge, skyboxCullingEdge, skyboxCullingEdge);

		glEnd();
	}

	glBindTexture(GL_TEXTURE_2D, d->GetID());

	glBegin(GL_QUADS);
	{
		glColor4f(1, 1, 1, 1);
		glTexCoord2f(0, 0);
		glVertex3f(-skyboxCullingEdge, -skyboxCullingEdge, -skyboxCullingEdge);
		glTexCoord2f(0, 1);
		glVertex3f(skyboxCullingEdge, -skyboxCullingEdge, -skyboxCullingEdge);
		glTexCoord2f(1, 1);
		glVertex3f(skyboxCullingEdge, -skyboxCullingEdge, skyboxCullingEdge);
		glTexCoord2f(1, 0);
		glVertex3f(-skyboxCullingEdge, -skyboxCullingEdge, skyboxCullingEdge);

		glEnd();
	}

	glBindTexture(GL_TEXTURE_2D, u->GetID());

	glBegin(GL_QUADS);
	{
		glColor4f(1, 1, 1, 1);
		glTexCoord2f(0, 0);
		glVertex3f(-skyboxCullingEdge, skyboxCullingEdge, -skyboxCullingEdge);
		glTexCoord2f(0, 1);
		glVertex3f(skyboxCullingEdge, skyboxCullingEdge, -skyboxCullingEdge);
		glTexCoord2f(1, 1);
		glVertex3f(skyboxCullingEdge, skyboxCullingEdge, skyboxCullingEdge);
		glTexCoord2f(1, 0);
		glVertex3f(-skyboxCullingEdge, skyboxCullingEdge, skyboxCullingEdge);

		glEnd();
	}*/


	glPopMatrix();
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