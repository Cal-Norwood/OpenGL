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

	bool LoadRAW(const char* path, int width, int height);

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

//Texture2D* f;
GLuint f;
GLuint l;
GLuint r;
GLuint b;
GLuint u;
GLuint d;

string CubeMap[]
{
	"yellowcloud_bk.jpg",
	"yellowcloud_dn.jpg",
	"yellowcloud_ft.jpg",
	"yellowcloud_lf.jpg",
	"yellowcloud_rt.jpg",
	"yellowcloud_up.jpg"
};

vector<vector<float>> geometryCoordsHorizontal =
{
	{1.5, 2.5, 10.5, -10.5},
	{-1.5, -2.5, 10.5, -10.5},
	{9.5, 10, 40, 10},
	{-9.5, -10, 40, 10}
};

vector<vector<float>> geometryCoordsDepth =
{
	{-9.5, -10.5, 2.5, -2.5},
	{10, 10.5, 10, 2.5, 1},
	{ 10, 10.5, -2.5, -10, 1},
	{39.5,40, 10, -10, 0}
};

void HorizontalCollidersCheck(float &localSpeedX)
{
	for (vector<float> colliderPoint : geometryCoordsHorizontal)
	{
		if (colliderPoint[0] > 0)
		{
			// 0 - inner, 1 - outer, 2 - lengthForward, 3 - lengthBackward
			if (cameraOffsetX > colliderPoint[0] && cameraOffsetX < colliderPoint[1] && cameraOffsetZ < colliderPoint[2] && cameraOffsetZ > colliderPoint[3])
			{
				if (localSpeedX < 0)
				{
					localSpeedX = 0;
				}
			}
		}
		else
		{
			if (cameraOffsetX < colliderPoint[0] && cameraOffsetX > colliderPoint[1] && cameraOffsetZ < colliderPoint[2] && cameraOffsetZ > colliderPoint[3])
			{
				if (localSpeedX > 0)
				{
					localSpeedX = 0;
				}
			}
		}
	}
}

void DepthCollidersCheck(float &localSpeedZ)
{
	for (vector<float> colliderPoint : geometryCoordsDepth)
	{
		if (colliderPoint[0] > 0)
		{
			// 0 - inner, 1 - outer, 2 - lengthLeft, 3 - lengthRight
			if (cameraOffsetZ > colliderPoint[0] && cameraOffsetZ < colliderPoint[1] && cameraOffsetX < colliderPoint[2] && cameraOffsetX > colliderPoint[3]) // needs fix
			{
				if (colliderPoint[4] == 0)
				{
					if (localSpeedZ > 0)
					{
						localSpeedZ = 0;
					}
				}
				else
				{
					if (localSpeedZ < 0)
					{
						localSpeedZ = 0;
					}
				}
			}
		}
		else
		{
			if (cameraOffsetZ < colliderPoint[0] && cameraOffsetZ > colliderPoint[1] && cameraOffsetX < colliderPoint[2] && cameraOffsetX > colliderPoint[3])
			{
				if (localSpeedZ < 0)
				{
					localSpeedZ = 0;
				}
			}
		}
	}
}




int LoadTextureTGA(const char* textureFileName)
{
	GLuint ID = 0;
	char* tempHeaderData = new char[18]; //18 Bytes is TGA Header Size
	char* tempTextureData;
	int fileSize;
	char type, pixelDepth, mode;

	ifstream inFile;

	inFile.open(textureFileName, ios::binary);
	if (!inFile.good())
	{
		cerr << "Can't open texture file " << textureFileName << endl;
		return -1;
	}

	//18 Bytes is the size of a TGA Header
	inFile.seekg(0, ios::beg); //Seek back to beginning of file
	inFile.read(tempHeaderData, 18); //Read in all the data in one go

	inFile.seekg(0, ios::end); //Seek to end of file
	fileSize = (int)inFile.tellg() - 18; //Get current position in file - The End, this gives us total file size
	tempTextureData = new char[fileSize]; //Create an new aray to store data
	inFile.seekg(18, ios::beg); //Seek back to beginning of file + 18
	inFile.read(tempTextureData, fileSize); //Read in all the data in one go
	inFile.close(); //Close the file

	type = tempHeaderData[2]; //Get TGA Type out of Header - Must be RGB for this to work
	int _width = ((unsigned char)tempHeaderData[13] << 8u) + (unsigned char)tempHeaderData[12]; // Find the width (Combines two bytes into a short)
	int _height = ((unsigned char)tempHeaderData[15] << 8u) + (unsigned char)tempHeaderData[14]; //Find the height
	pixelDepth = tempHeaderData[16]; // Find the pixel depth (24/32bpp)

	bool flipped = false;
	if ((int)((tempHeaderData[11] << 8) + tempHeaderData[10]) == 0)
		flipped = true;

	//We only support RGB type
	if (type == 2)
	{
		cout << textureFileName << " loaded." << endl;

		glGenTextures(1, &ID); //Get next Texture ID
		glBindTexture(GL_TEXTURE_2D, ID); //Bind the texture to the ID

		mode = pixelDepth / 8;

		//Note that TGA files are stored as BGR(A) - So we need to specify the format as GL_BGR(A)_EXT
		if (mode == 4)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, tempTextureData);
		else
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, tempTextureData);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	}

	delete[] tempHeaderData; //We don't need the header memory anymore
	delete[] tempTextureData; //Clear up the data - We don't need this any more

	return ID;
}



bool Texture2D::LoadRAW(const char* path, int width, int height)
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
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_BGR_EXT, GL_UNSIGNED_BYTE,tempTextureData);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	return true;
}

Texture2D* LoadTextureAndBind(const char* tex)
{
	Texture2D* texture = new Texture2D();
	texture->LoadRAW(tex, 1024, 1024);

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

	f = LoadTextureTGA("yellowcloud_ft.tga");
	l = LoadTextureTGA("yellowcloud_lf.tga");
	r = LoadTextureTGA("yellowcloud_rt.tga");
	b = LoadTextureTGA("yellowcloud_bk.tga");
	u = LoadTextureTGA("yellowcloud_up.tga");
	d = LoadTextureTGA("yellowcloud_dn.tga");

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

	cout << cameraOffsetZ << endl;

	HorizontalCollidersCheck(localSpeedX);
	DepthCollidersCheck(localSpeedZ);

	glTranslatef(-localSpeedX, cameraPosY, localSpeedZ);

	cameraOffsetX -= localSpeedX;
	cameraOffsetZ += localSpeedZ;

	DrawPolygon0();
	DrawPolygon();
	//DrawPolygon1();
	DrawPolygon2();
	DrawPolygon3();
	DrawPolygon4();
	DrawPolygon5();
	DrawPuzzleRoom();
	DrawPuzzle1();
	DrawPuzzle2();
	DrawPuzzle3();
	DrawPuzzle4();
	DrawPuzzle5();
	DrawPuzzle6();
	DrawPuzzle7();
	DrawPuzzle8();
	DrawPuzzle9();

	glFlush();

	rotation += 1;
}

void OpenGL::DrawPolygon0()
{
	float skyboxCullingEdge = 99 / sqrt(3);

	glBindTexture(GL_TEXTURE_2D, f);

	glBegin(GL_QUADS);
	{
		glColor4f(1, 1, 1, 1);
		glTexCoord2f(1, 1);
		glVertex3f(-skyboxCullingEdge, -skyboxCullingEdge, -skyboxCullingEdge);
		glTexCoord2f(1, 0);
		glVertex3f(-skyboxCullingEdge, skyboxCullingEdge, -skyboxCullingEdge);
		glTexCoord2f(0, 0);
		glVertex3f(skyboxCullingEdge, skyboxCullingEdge, -skyboxCullingEdge);
		glTexCoord2f(0, 1);
		glVertex3f(skyboxCullingEdge, -skyboxCullingEdge, -skyboxCullingEdge);

		glEnd();
	}

	glBindTexture(GL_TEXTURE_2D, l);

	glBegin(GL_QUADS);
	{
		glColor4f(1, 1, 1, 1);
		glTexCoord2f(1, 1);
		glVertex3f(-skyboxCullingEdge, skyboxCullingEdge, skyboxCullingEdge);
		glTexCoord2f(1, 0);
		glVertex3f(-skyboxCullingEdge, -skyboxCullingEdge, skyboxCullingEdge);
		glTexCoord2f(0, 0);
		glVertex3f(-skyboxCullingEdge, -skyboxCullingEdge, -skyboxCullingEdge);
		glTexCoord2f(0, 1);
		glVertex3f(-skyboxCullingEdge, skyboxCullingEdge, -skyboxCullingEdge);

		glEnd();
	}

	glBindTexture(GL_TEXTURE_2D, b);

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

	glBindTexture(GL_TEXTURE_2D, r);

	glBegin(GL_QUADS);
	{
		glColor4f(1, 1, 1, 1);
		glTexCoord2f(1, 1);
		glVertex3f(skyboxCullingEdge, skyboxCullingEdge, -skyboxCullingEdge);
		glTexCoord2f(1, 0);
		glVertex3f(skyboxCullingEdge, -skyboxCullingEdge, -skyboxCullingEdge);
		glTexCoord2f(0, 0);
		glVertex3f(skyboxCullingEdge, -skyboxCullingEdge, skyboxCullingEdge);
		glTexCoord2f(0, 1);
		glVertex3f(skyboxCullingEdge, skyboxCullingEdge, skyboxCullingEdge);

		glEnd();
	}

	glBindTexture(GL_TEXTURE_2D, d);

	glBegin(GL_QUADS);
	{
		glColor4f(1, 1, 1, 1);
		glTexCoord2f(1, 1);
		glVertex3f(-skyboxCullingEdge, -skyboxCullingEdge, -skyboxCullingEdge);
		glTexCoord2f(1, 0);
		glVertex3f(skyboxCullingEdge, -skyboxCullingEdge, -skyboxCullingEdge);
		glTexCoord2f(0, 0);
		glVertex3f(skyboxCullingEdge, -skyboxCullingEdge, skyboxCullingEdge);
		glTexCoord2f(0, 1);
		glVertex3f(-skyboxCullingEdge, -skyboxCullingEdge, skyboxCullingEdge);

		glEnd();
	}

	glBindTexture(GL_TEXTURE_2D, u);

	glBegin(GL_QUADS);
	{
		glColor4f(1, 1, 1, 1);
		glTexCoord2f(1, 1);
		glVertex3f(-skyboxCullingEdge, skyboxCullingEdge, -skyboxCullingEdge);
		glTexCoord2f(1, 0);
		glVertex3f(skyboxCullingEdge, skyboxCullingEdge, -skyboxCullingEdge);
		glTexCoord2f(0, 0);
		glVertex3f(skyboxCullingEdge, skyboxCullingEdge, skyboxCullingEdge);
		glTexCoord2f(0, 1);
		glVertex3f(-skyboxCullingEdge, skyboxCullingEdge, skyboxCullingEdge);

		glEnd();
	}


	glPopMatrix();
}

void OpenGL::DrawPolygon()
{
	glPushMatrix();
	glRotatef(rotation, 0.1, 0, 3);

	glPointSize(10);

	glBegin(GL_LINE_LOOP);
	{
		glColor4f(1, 0, 0, 1);
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
		glColor4f(0.3, 0.3, 0.3, 1);
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
		glColor4f(0.3, 0.3, 0.3, 1);
		glVertex3f(2, -1, -10);
		glVertex3f(2, -1, 10);
		glVertex3f(2, -4, 10);
		glVertex3f(2, -4, -10);

		glEnd();
	}

	glBegin(GL_POLYGON);
	{
		glColor4f(0.3, 0.3, 0.3, 1);
		glVertex3f(2, 4, -10);
		glVertex3f(2, 4, 10);
		glVertex3f(2, 1, 10);
		glVertex3f(2, 1, -10);

		glEnd();
	}

	glBegin(GL_POLYGON);
	{
		glColor4f(0.3, 0.3, 0.3, 1);
		glVertex3f(2, -1, -10);
		glVertex3f(2, -1, 10);
		glVertex3f(3, -1, 10);
		glVertex3f(3, -1, -10);

		glEnd();
	}

	glBegin(GL_POLYGON);
	{
		glColor4f(0.3, 0.3, 0.3, 1);
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
		glColor4f(0.3, 0.3, 0.3, 1);
		glVertex3f(-2, -1, -10);
		glVertex3f(-2, -1, 10);
		glVertex3f(-2, -4, 10);
		glVertex3f(-2, -4, -10);

		glEnd();
	}

	glBegin(GL_POLYGON);
	{
		glColor4f(0.3, 0.3, 0.3, 1);
		glVertex3f(-2, 4, -10);
		glVertex3f(-2, 4, 10);
		glVertex3f(-2, 1, 10);
		glVertex3f(-2, 1, -10);

		glEnd();
	}

	glBegin(GL_POLYGON);
	{
		glColor4f(0.3, 0.3, 0.3, 1);
		glVertex3f(-2, -1, -10);
		glVertex3f(-2, -1, 10);
		glVertex3f(-3, -1, 10);
		glVertex3f(-3, -1, -10);

		glEnd();
	}

	glBegin(GL_POLYGON);
	{
		glColor4f(0.3, 0.3, 0.3, 1);
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

	glBegin(GL_POLYGON);
	{
		glColor4f(0.4, 0.4, 0.4, 0.4);
		glVertex3f(2, 4, -10);
		glVertex3f(2, 4, 10);
		glVertex3f(2, 40, 10);
		glVertex3f(2, 40, -10);

		glEnd();
	}

	glBegin(GL_POLYGON);
	{
		glColor4f(0.4, 0.4, 0.4, 0.4);
		glVertex3f(-2, 4, -10);
		glVertex3f(-2, 4, 10);
		glVertex3f(-2, 40, 10);
		glVertex3f(-2, 40, -10);

		glEnd();
	}

	glBegin(GL_POLYGON);
	{
		glColor4f(0.4, 0.4, 0.4, 0.4);
		glVertex3f(2, 40, -10);
		glVertex3f(2, 40, 10);
		glVertex3f(-2, 40, 10);
		glVertex3f(-2, 40, -10);

		glEnd();
	}

	glBegin(GL_POLYGON);
	{
		glColor4f(0.4, 0.4, 0.4, 0.4);
		glVertex3f(2, 4, 10);
		glVertex3f(2, 40, 10);
		glVertex3f(-2, 40, 10);
		glVertex3f(-2, 4, 10);

		glEnd();
	}

	glPopMatrix();
}

void OpenGL::DrawPuzzleRoom()
{
	glPushMatrix();

	glBegin(GL_POLYGON);
	{
		glColor4f(0.4, 0.4, 0.4, 0.4);
		glVertex3f(10, -4, -10);
		glVertex3f(-10, -4, -10);
		glVertex3f(-10, -4, -40);
		glVertex3f(10, -4, -40);

		glEnd();
	}

	glBegin(GL_POLYGON);
	{
		glColor4f(0.4, 0.4, 0.4, 0.4);
		glVertex3f(10,20, -10);
		glVertex3f(-10, 20, -10);
		glVertex3f(-10, 20, -40);
		glVertex3f(10, 20, -40);

		glEnd();
	}

	glBegin(GL_POLYGON);
	{
		glColor4f(0.3, 0.3, 0.3, 0.3);
		glVertex3f(10, -4, -10);
		glVertex3f(10, 20, -10);
		glVertex3f(10, 20, -40);
		glVertex3f(10, -4, -40);

		glEnd();
	}

	glBegin(GL_POLYGON);
	{
		glColor4f(0.3, 0.3, 0.3, 0.3);
		glVertex3f(-10, -4, -10);
		glVertex3f(-10, 20, -10);
		glVertex3f(-10, 20, -40);
		glVertex3f(-10, -4, -40);

		glEnd();
	}

	glBegin(GL_POLYGON);
	{
		glColor4f(0.3, 0.3, 0.3, 0.3);
		glVertex3f(-10, -4, -10);
		glVertex3f(-10, 20, -10);
		glVertex3f(-2, 20, -10);
		glVertex3f(-2, -4, -10);

		glEnd();
	}

	glBegin(GL_POLYGON);
	{
		glColor4f(0.3, 0.3, 0.3, 0.3);
		glVertex3f(10, -4, -10);
		glVertex3f(10, 20, -10);
		glVertex3f(2, 20, -10);
		glVertex3f(2, -4, -10);

		glEnd();
	}

	glBegin(GL_POLYGON);
	{
		glColor4f(0.3, 0.3, 0.3, 0.3);
		glVertex3f(10, -4, -40);
		glVertex3f(10, 20, -40);
		glVertex3f(-10, 20, -40);
		glVertex3f(-10, -4, -40);

		glEnd();
	}

	glPopMatrix();
}

void OpenGL::DrawPuzzle1()
{
	glPushMatrix();
	glTranslatef(-6, 15, -39);
	glRotatef(rotation * 1.1, 0, 1, 0);
	glTranslatef(6, -15, 39);
	glBegin(GL_POLYGON);
	{
		glColor4f(1, 1, 1, 1);
		glVertex3f(-9, 18, -39);
		glVertex3f(-3, 18, -39);
		glVertex3f(-3, 12, -39);
		glVertex3f(-9, 12, -39);

		glEnd();
	}

	glBegin(GL_POLYGON);
	{
		glColor4f(0, 1, 1, 1);
		glVertex3f(-9, 18, -39.01f);
		glVertex3f(-3, 18, -39.01f);
		glVertex3f(-3, 12, -39.01f);
		glVertex3f(-9, 12, -39.01f);

		glEnd();
	}

	glPopMatrix();
}

void OpenGL::DrawPuzzle2()
{
	glPushMatrix();
	glTranslatef(0, 15, -39);
	glRotatef(rotation * 1.1, 0, 1, 0);
	glTranslatef(0, -15, 39);
	glBegin(GL_POLYGON);
	{
		glColor4f(1, 1, 1, 1);
		glVertex3f(-3, 18, -39);
		glVertex3f(3, 18, -39);
		glVertex3f(3, 12, -39);
		glVertex3f(-3, 12, -39);

		glEnd();
	}

	glBegin(GL_POLYGON);
	{
		glColor4f(1, 0, 1, 1);
		glVertex3f(-3, 18, -39.01f);
		glVertex3f(3, 18, -39.01f);
		glVertex3f(3, 12, -39.01f);
		glVertex3f(-3, 12, -39.01f);

		glEnd();
	}

	glPopMatrix();
}

void OpenGL::DrawPuzzle3()
{
	glPushMatrix();
	glTranslatef(6, 15, -39);
	glRotatef(rotation * 1.1, 0, 1, 0);
	glTranslatef(-6, -15, 39);
	glBegin(GL_POLYGON);
	{
		glColor4f(1, 1, 1, 1);
		glVertex3f(3, 18, -39);
		glVertex3f(9, 18, -39);
		glVertex3f(9, 12, -39);
		glVertex3f(3, 12, -39);

		glEnd();
	}

	glBegin(GL_POLYGON);
	{
		glColor4f(1, 1, 0, 1);
		glVertex3f(3, 18, -39.01f);
		glVertex3f(9, 18, -39.01f);
		glVertex3f(9, 12, -39.01f);
		glVertex3f(3, 12, -39.01f);

		glEnd();
	}

	glPopMatrix();
}

void OpenGL::DrawPuzzle4()
{
	glPushMatrix();
	glTranslatef(-6, 9, -39);
	glRotatef(rotation * 1.1, 0, 1, 0);
	glTranslatef(6, -9, 39);
	glBegin(GL_POLYGON);
	{
		glColor4f(1, 1, 1, 1);
		glVertex3f(-9, 12, -39);
		glVertex3f(-3, 12, -39);
		glVertex3f(-3, 6, -39);
		glVertex3f(-9, 6, -39);

		glEnd();
	}

	glBegin(GL_POLYGON);
	{
		glColor4f(1, 0, 0, 1);
		glVertex3f(-9, 12, -39.01f);
		glVertex3f(-3, 12, -39.01f);
		glVertex3f(-3, 6, -39.01f);
		glVertex3f(-9, 6, -39.01f);

		glEnd();
	}

	glPopMatrix();
}

void OpenGL::DrawPuzzle5()
{
	glPushMatrix();
	glTranslatef(0, 9, -39);
	glRotatef(rotation * 1.1, 0, 1, 0);
	glTranslatef(0, -9, 39);
	glBegin(GL_POLYGON);
	{
		glColor4f(1, 1, 1, 1);
		glVertex3f(-3, 12, -39);
		glVertex3f(3, 12, -39);
		glVertex3f(3, 6, -39);
		glVertex3f(-3, 6, -39);

		glEnd();
	}

	glBegin(GL_POLYGON);
	{
		glColor4f(1, 1, 0, 1);
		glVertex3f(-3, 12, -39.01f);
		glVertex3f(3, 12, -39.01f);
		glVertex3f(3, 6, -39.01f);
		glVertex3f(-3, 6, -39.01f);

		glEnd();
	}

	glPopMatrix();
}

void OpenGL::DrawPuzzle6()
{
	glPushMatrix();
	glTranslatef(6, 9, -39);
	glRotatef(rotation * 1.1, 0, 1, 0);
	glTranslatef(-6, -9, 39);
	glBegin(GL_POLYGON);
	{
		glColor4f(1, 1, 1, 1);
		glVertex3f(3, 12, -39);
		glVertex3f(9, 12, -39);
		glVertex3f(9, 6, -39);
		glVertex3f(3, 6, -39);

		glEnd();
	}

	glBegin(GL_POLYGON);
	{
		glColor4f(1, 0, 0, 1);
		glVertex3f(3, 12, -39.01f);
		glVertex3f(9, 12, -39.01f);
		glVertex3f(9, 6, -39.01f);
		glVertex3f(3, 6, -39.01f);

		glEnd();
	}

	glPopMatrix();
}

void OpenGL::DrawPuzzle7()
{
	glPushMatrix();
	glTranslatef(-6, 3, -39);
	glRotatef(rotation * 1.1, 0, 1, 0);
	glTranslatef(6, -3, 39);
	glBegin(GL_POLYGON);
	{
		glColor4f(1, 1, 1, 1);
		glVertex3f(-9, 6, -39);
		glVertex3f(-3, 6, -39);
		glVertex3f(-3, 0, -39);
		glVertex3f(-9, 0, -39);

		glEnd();
	}

	glBegin(GL_POLYGON);
	{
		glColor4f(0, 1, 1, 1);
		glVertex3f(-9, 6, -39.01f);
		glVertex3f(-3, 6, -39.01f);
		glVertex3f(-3, 0, -39.01f);
		glVertex3f(-9, 0, -39.01f);

		glEnd();
	}

	glPopMatrix();
}

void OpenGL::DrawPuzzle8()
{
	glPushMatrix();

	glBegin(GL_POLYGON);
	{
		glColor4f(0, 0, 0, 1);
		glVertex3f(-3, 6, -39.f);
		glVertex3f(3, 6, -39.f);
		glVertex3f(3, 0, -39.f);
		glVertex3f(-3, 0, -39.f);

		glEnd();
	}

	glPopMatrix();
}

void OpenGL::DrawPuzzle9()
{
	glPushMatrix();
	glTranslatef(6, 3, -39);
	glRotatef(rotation * 1.1, 0, 1, 0);
	glTranslatef(-6, -3, 39);
	glBegin(GL_POLYGON);
	{
		glColor4f(1, 1, 1, 1);
		glVertex3f(3, 6, -39);
		glVertex3f(9, 6, -39);
		glVertex3f(9, 0, -39);
		glVertex3f(3, 0, -39);

		glEnd();
	}

	glBegin(GL_POLYGON);
	{
		glColor4f(1, 0, 1, 1);
		glVertex3f(3, 6, -39.01f);
		glVertex3f(9, 6, -39.01f);
		glVertex3f(9, 0, -39.01f);
		glVertex3f(3, 0, -39.01f);

		glEnd();
	}

	glPopMatrix();
}