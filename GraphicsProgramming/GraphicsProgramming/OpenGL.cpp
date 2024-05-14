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
bool isEnterPressed = false;

float angle = 1;

float rotation = 0;
float rotatePuzzle1 = 0;
float rotatePuzzle2 = 0;
float rotatePuzzle3 = 0;
float rotatePuzzle4 = 0;
float rotatePuzzle5 = 0;
float rotatePuzzle6 = 0;
float rotatePuzzle7 = 0;
float rotatePuzzle8 = 0;
float rotatePuzzle9 = 0;

float rotationRoof1 = 0;
float rotationRoof2 = 0;
float rotationRoof3 = 0;
float rotationRoof4 = 0;

float fx = 0.2;
float bx = -fx;

float ty = fx;
float by = -fx;

bool moveRight = true;
bool moveUp = true;

bool skyboxSpawned = false;
bool onPuzzle = false;
bool onObservatory = false;
bool flipBack = false;

bool freezePlayer = false;
float rotatePlayerForCutscene = 0;

int puzzleCounter = 0;
int puzzleCorrect = 0;

bool puzzleComplete = false;

float alpha = 1;
float rotationDoor = 0;
float skyboxCullingEdgeForward = 99 / sqrt(3);
float skyboxCullingEdge = 99 / sqrt(3);

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

vector<vector<bool>> isFlipped =
{
	{false, false, false},
	{false, false, false},
	{false, false, false}
};

vector<vector<bool>> isSelected =
{
	{false, false, false},
	{false, false, false},
	{false, false, false}
};

int currentSelectionVertical;
int currentSelectionHorizontal;

vector<vector<float>> geometryCoordsHorizontal =
{
	{1.5, 2.5, 10.5, -10.5},
	{-1.5, -2.5, 10.5, -10.5},
	{9.5, 10, 40, 10},
	{-9.5, -10, 40, 10},
	{2.5, 3, 50, 40},
	{-2.5, -3, 50, 40},
	{9.5, 10, 70, 50},
	{-9.5, -10, 70, 50}
};

vector<vector<float>> geometryCoordsDepth =
{
	{-9.5, -10.5, 2.5, -2.5},
	{10, 10.5, 10, 2.5, 1},
	{ 10, 10.5, -2.5, -10, 1},
	{39.5,40, 10, -10, 0},
	{39.5,40, 10, -10, 0},
	{50,50.5, 10, 3, 1},
	{50,50.5, -3, -10, 1},
	{69.5 ,70, 10, -10, 0}
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

void PuzzleTirggerCollider()
{
	if (cameraOffsetZ < 27 && cameraOffsetZ > 23 && cameraOffsetX < 2 && cameraOffsetX > -2)
	{
		onPuzzle = true;
	}
	else 
	{
		onPuzzle = false;
	}
}

void ObservatoryTriggerCollider()
{
	if (cameraOffsetZ < 62.5 && cameraOffsetZ > 57.5 && cameraOffsetX < 2 && cameraOffsetX > -2)
	{
		onObservatory = true;
	}
	else
	{
		onObservatory = false;
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

void FlipPuzzleSquare(int x, int y)
{
	isFlipped[y][x] = true;
}

void PuzzleMechanic()
{
	bool firstTime = true;
	while (true)
	{
		while (onPuzzle == true)
		{
			if (firstTime == true)
			{
				currentSelectionVertical = 1;
				currentSelectionHorizontal = 1;
				isSelected[1][1] = true;
				firstTime = false;
			}

			if (isEnterPressed)
			{
				if (isFlipped[currentSelectionVertical][currentSelectionHorizontal] == false)
				{
					puzzleCounter++;
					isEnterPressed = false;
					FlipPuzzleSquare(currentSelectionHorizontal, currentSelectionVertical);

					if (puzzleCounter == 2)
					{
						this_thread::sleep_for(chrono::milliseconds(1000));
						flipBack = true;
						puzzleCounter = 0;
						this_thread::sleep_for(chrono::milliseconds(1000));
						flipBack = false;
					}
				}
				else
				{
					isEnterPressed = false;
				}
			}
			
			if (puzzleComplete == false)
			{
				for (int a = 0; a < 3; a++)
				{
					for (int b = 0; b < 3; b++)
					{
						if (isFlipped[a][b])
						{
							puzzleCorrect++;
						}
					}
				}

				if (puzzleCorrect == 8)
				{
					puzzleComplete = true;
					skyboxCullingEdgeForward += 50;
					geometryCoordsDepth[3] = { 39.5,40, 10, 3, 0 };
					geometryCoordsDepth[4] = { 39.5,40, -3, -10, 0 };
				}

				puzzleCorrect = 0;
			}
			else
			{
				if (alpha > 0)
				{
					alpha -= 0.01f;
					this_thread::sleep_for(chrono::milliseconds(25));
				}
			}
		}

		if (puzzleComplete)
		{
			if (alpha > 0)
			{
				alpha -= 0.01f;
				this_thread::sleep_for(chrono::milliseconds(25));
			}
		}

		if (firstTime == false) 
		{
			for (int a = 0; a < 3; a++)
			{
				for (int b = 0; b < 3; b++)
				{
					isSelected[a][b] = false;
				}
			}

			firstTime = true;
		}
	}
}

void ObservatoryHandler()
{
	while (true)
	{
		while (onObservatory)
		{
			if (isEnterPressed)
			{
				freezePlayer = true;
				isEnterPressed = false;
			}
			else if (rotatePlayerForCutscene < -80)
			{
				if (rotationRoof1 < 90)
				{
					rotationRoof1 += 0.05;
					rotationRoof2 += 0.05;
					rotationRoof3 += 0.05;
					rotationRoof4 -= 0.05;
					this_thread::sleep_for(chrono::milliseconds(50));
				}
			}
		}
	}
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
	if (!freezePlayer)
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
		case 13:
			if (onPuzzle && puzzleCounter != 2 || onObservatory)
			{
				isEnterPressed = true;
			}
			break;
		}
	}
	else 
	{
		isWPressed = false;
		isAPressed = false;
		isDPressed = false;
		isSPressed = false;
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

void SpecialInput(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		if (onPuzzle)
		{
			if (currentSelectionVertical - 1 >= 0)
			{
				isSelected[currentSelectionVertical][currentSelectionHorizontal] = false;
				currentSelectionVertical--;
				isSelected[currentSelectionVertical][currentSelectionHorizontal] = true;
			}
		}
	break;

	case GLUT_KEY_LEFT:
		if (onPuzzle)
		{
			if (currentSelectionHorizontal - 1 >= 0 && isSelected[2][2] == false)
			{
				isSelected[currentSelectionVertical][currentSelectionHorizontal] = false;
				currentSelectionHorizontal--;
				isSelected[currentSelectionVertical][currentSelectionHorizontal] = true;
			}
		}
	break;

	case GLUT_KEY_RIGHT:
		if (onPuzzle)
		{
			if (currentSelectionHorizontal + 1 <= 2 && isSelected[2][0] == false)
			{
				isSelected[currentSelectionVertical][currentSelectionHorizontal] = false;
				currentSelectionHorizontal++;
				isSelected[currentSelectionVertical][currentSelectionHorizontal] = true;
			}
		}
	break;

	case GLUT_KEY_DOWN:
		if (onPuzzle)
		{
			if (currentSelectionVertical + 1 <= 2 && isSelected[1][1] == false)
			{
				isSelected[currentSelectionVertical][currentSelectionHorizontal] = false;
				currentSelectionVertical++;
				isSelected[currentSelectionVertical][currentSelectionHorizontal] = true;
			}
		}
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

	//cout << horizontal << endl;
	glTranslatef(-cameraOffsetX, 0, -cameraOffsetZ);
	if(!freezePlayer)
	{
		glRotatef(horizontal, 0.0f, 1.0f, 0.0f);
		cameraRotationY += horizontal;

		if (fabs(cameraRotationY) > 360)
		{
			cameraRotationY = 0;
		}
	}
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
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
	glutSpecialFunc(SpecialInput);
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

	thread puzzle(PuzzleMechanic);
	thread observatory(ObservatoryHandler);

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

	HorizontalCollidersCheck(localSpeedX);
	DepthCollidersCheck(localSpeedZ);
	PuzzleTirggerCollider();
	ObservatoryTriggerCollider();

	//cout << localSpeedX << endl;
	cout << cameraRotationY << endl;

	if (freezePlayer)
	{
		if (rotatePlayerForCutscene > -90)
		{
			glTranslatef(-cameraOffsetX, 0, -cameraOffsetZ);
			glRotatef(-1, 1 * cosAngle, 0, 1 * sinAngle);
			glTranslatef(cameraOffsetX, 0, cameraOffsetZ);
			rotatePlayerForCutscene -= 1;
		}
		else if (fabs(cameraRotationY) >= 1.5f)
		{
			glTranslatef(-cameraOffsetX, 0, -cameraOffsetZ);
			cout << "working" << endl;
			if (cameraRotationY < 0)
			{
				glRotatef(1.9, 0, 1, 0);
				cameraRotationY += 1.9;
			}
			else 
			{
				glRotatef(-1.9, 0, 1, 0);
				cameraRotationY -= 1.9;
			}
			glTranslatef(cameraOffsetX, 0, cameraOffsetZ);
		}
		else 
		{

		}
	}

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
	DrawObservatory();
	DrawObservatoryRoof();
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
	glBindTexture(GL_TEXTURE_2D, f);

	glBegin(GL_QUADS);
	{
		glColor4f(1, 1, 1, 1);
		glTexCoord2f(1, 1);
		glVertex3f(-skyboxCullingEdge, -skyboxCullingEdge, -skyboxCullingEdgeForward);
		glTexCoord2f(1, 0);
		glVertex3f(-skyboxCullingEdge, skyboxCullingEdge, -skyboxCullingEdgeForward);
		glTexCoord2f(0, 0);
		glVertex3f(skyboxCullingEdge, skyboxCullingEdge, -skyboxCullingEdgeForward);
		glTexCoord2f(0, 1);
		glVertex3f(skyboxCullingEdge, -skyboxCullingEdge, -skyboxCullingEdgeForward);

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
		glVertex3f(-skyboxCullingEdge, -skyboxCullingEdge, -skyboxCullingEdgeForward);
		glTexCoord2f(0, 1);
		glVertex3f(-skyboxCullingEdge, skyboxCullingEdge, -skyboxCullingEdgeForward);

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
		glVertex3f(skyboxCullingEdge, skyboxCullingEdge, -skyboxCullingEdgeForward);
		glTexCoord2f(1, 0);
		glVertex3f(skyboxCullingEdge, -skyboxCullingEdge, -skyboxCullingEdgeForward);
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
		glVertex3f(-skyboxCullingEdge, -skyboxCullingEdge, -skyboxCullingEdgeForward);
		glTexCoord2f(1, 0);
		glVertex3f(skyboxCullingEdge, -skyboxCullingEdge, -skyboxCullingEdgeForward);
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
		glVertex3f(-skyboxCullingEdge, skyboxCullingEdge, -skyboxCullingEdgeForward);
		glTexCoord2f(1, 0);
		glVertex3f(skyboxCullingEdge, skyboxCullingEdge, -skyboxCullingEdgeForward);
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
		glColor4f(0, 1, 0, 1);
		glVertex3f(0.05, -0.05, -2);
		glVertex3f(-0.05, -0.05, -2);
		glVertex3f(-0.05, 0.05, -2);
		glVertex3f(0.05, 0.05, -2);

		glEnd();
	}

	glBegin(GL_LINE_LOOP);
	{
		glColor4f(0, 0, 1, 1);
		glVertex3f(0.05, 0.05, -1);
		glVertex3f(0.05, 0.05, -2);
		glVertex3f(0.05, -0.05, -2);
		glVertex3f(0.05, -0.05, -1);

		glEnd();
	}

	glBegin(GL_LINE_LOOP);
	{
		glColor4f(1, 0, 1, 1);
		glVertex3f(-0.05, 0.05, -1);
		glVertex3f(-0.05, 0.05, -2);
		glVertex3f(-0.05, -0.05, -2);
		glVertex3f(-0.05, -0.05, -1);

		glEnd();
	}

	glBegin(GL_LINE_LOOP);
	{
		glColor4f(1, 1, 1, 1);
		glVertex3f(0.05, -0.05, -1);
		glVertex3f(-0.05, -0.05, -1);
		glVertex3f(-0.05, -0.05, -2);
		glVertex3f(0.05, -0.05, -2);

		glEnd();
	}

	glBegin(GL_LINE_LOOP);
	{
		glColor4f(1, 1, 0, 1);
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
		glColor4f(0.3, 0.3, 0.3, 1);
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
		glColor4f(0.4, 0.4, 0.4, 1);
		glVertex3f(2, -4, -10);
		glVertex3f(2, -4, 10);
		glVertex3f(-2, -4, 10);
		glVertex3f(-2, -4, -10);

		glEnd();
	}

	glBegin(GL_POLYGON);
	{
		glColor4f(0.4, 0.4, 0.4, 1);
		glVertex3f(2, 4, -10);
		glVertex3f(2, 4, 10);
		glVertex3f(-2, 4, 10);
		glVertex3f(-2, 4, -10);

		glEnd();
	}

	glBegin(GL_POLYGON);
	{
		glColor4f(0.4, 0.4, 0.4, 1);
		glVertex3f(2, 4, -10);
		glVertex3f(2, 4, 10);
		glVertex3f(2, 40, 10);
		glVertex3f(2, 40, -10);

		glEnd();
	}

	glBegin(GL_POLYGON);
	{
		glColor4f(0.4, 0.4, 0.4, 1);
		glVertex3f(-2, 4, -10);
		glVertex3f(-2, 4, 10);
		glVertex3f(-2, 40, 10);
		glVertex3f(-2, 40, -10);

		glEnd();
	}

	glBegin(GL_POLYGON);
	{
		glColor4f(0.4, 0.4, 0.4, 1);
		glVertex3f(2, 40, -10);
		glVertex3f(2, 40, 10);
		glVertex3f(-2, 40, 10);
		glVertex3f(-2, 40, -10);

		glEnd();
	}

	glBegin(GL_POLYGON);
	{
		glColor4f(0.4, 0.4, 0.4, 1);
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
		glColor4f(0.4, 0.4, 0.4, 1);
		glVertex3f(10, -4, -10);
		glVertex3f(-10, -4, -10);
		glVertex3f(-10, -4, -40);
		glVertex3f(10, -4, -40);

		glEnd();
	}

	glBegin(GL_POLYGON);
	{
		glColor4f(0.4, 0.4, 0.4, 1);
		glVertex3f(10,20, -10);
		glVertex3f(-10, 20, -10);
		glVertex3f(-10, 20, -40);
		glVertex3f(10, 20, -40);

		glEnd();
	}

	glBegin(GL_POLYGON);
	{
		glColor4f(0.3, 0.3, 0.3, 1);
		glVertex3f(10, -4, -10);
		glVertex3f(10, 20, -10);
		glVertex3f(10, 20, -40);
		glVertex3f(10, -4, -40);

		glEnd();
	}

	glBegin(GL_POLYGON);
	{
		glColor4f(0.3, 0.3, 0.3, 1);
		glVertex3f(-10, -4, -10);
		glVertex3f(-10, 20, -10);
		glVertex3f(-10, 20, -40);
		glVertex3f(-10, -4, -40);

		glEnd();
	}

	glBegin(GL_POLYGON);
	{
		glColor4f(0.3, 0.3, 0.3, 1);
		glVertex3f(-10, -4, -10);
		glVertex3f(-10, 20, -10);
		glVertex3f(-2, 20, -10);
		glVertex3f(-2, -4, -10);

		glEnd();
	}

	glBegin(GL_POLYGON);
	{
		glColor4f(0.3, 0.3, 0.3, 1);
		glVertex3f(10, -4, -10);
		glVertex3f(10, 20, -10);
		glVertex3f(2, 20, -10);
		glVertex3f(2, -4, -10);

		glEnd();
	}

	glBegin(GL_POLYGON);
	{
		glColor4f(0.3, 0.3, 0.3, 1);
		glVertex3f(10, -4, -40);
		glVertex3f(10, 20, -40);
		glVertex3f(3, 20, -40);
		glVertex3f(3, -4, -40);

		glEnd();
	}

	glBegin(GL_POLYGON);
	{
		glColor4f(0.3, 0.3, 0.3, 1);
		glVertex3f(-10, -4, -40);
		glVertex3f(-10, 20, -40);
		glVertex3f(-3, 20, -40);
		glVertex3f(-3, -4, -40);

		glEnd();
	}

	glBegin(GL_POLYGON);
	{
		glColor4f(0.3, 0.3, 0.3, 1);
		glVertex3f(3, 6, -40);
		glVertex3f(3, 20, -40);
		glVertex3f(-3, 20, -40);
		glVertex3f(-3, 6, -40);

		glEnd();
	}

	glBegin(GL_POLYGON);
	{
		glColor4f(0, 0, 0, 1);
		glVertex3f(2, -3.9, -27);
		glVertex3f(2, -3.9, -23);
		glVertex3f(-2, -3.9, -23);
		glVertex3f(-2, -3.9, -27);

		glEnd();
	}

	glPopMatrix();
}

void OpenGL::DrawPuzzle1()
{
	glPushMatrix();

	if (isFlipped[0][0])
	{
		glTranslatef(-6, 15, -39.9);
		glRotatef(rotatePuzzle1, 0, 1, 0);
		glTranslatef(6, -15, 39.9);

		if (flipBack == true)
		{
			if (isFlipped[0][0] && isFlipped[2][0])
			{
				
			}
			else
			{
				if (rotatePuzzle1 > 0)
				{
					rotatePuzzle1 -= 5;
				}
				else
				{
					isFlipped[0][0] = false;
				}
			}
		}
		else 
		{
			if (rotatePuzzle1 < 180)
			{
				rotatePuzzle1 += 5;
			}
		}
	}

	glBegin(GL_POLYGON);
	{
		if (isSelected[0][0])
		{
			glColor4f(1, 0.8, 0, 1);
		}
		else 
		{
			glColor4f(1, 1, 1, 1);
		}
		glVertex3f(-9, 18, -39.9);
		glVertex3f(-3, 18, -39.9);
		glVertex3f(-3, 12, -39.9);
		glVertex3f(-9, 12, -39.9);

		glEnd();
	}

	glBegin(GL_POLYGON);
	{
		if (puzzleComplete == true)
		{
			glColor4f(0, 1, 0, 1);
		}
		else
		{
			glColor4f(0, 1, 1, 1);
		}
		
		glVertex3f(-9, 18, -39.902f);
		glVertex3f(-3, 18, -39.902f);
		glVertex3f(-3, 12, -39.902f);
		glVertex3f(-9, 12, -39.902f);

		glEnd();
	}

	glPopMatrix();
}

void OpenGL::DrawPuzzle2()
{
	glPushMatrix();

	if (isFlipped[0][1])
	{
		glTranslatef(0, 15, -39.9);
		glRotatef(rotatePuzzle2, 0, 1, 0);
		glTranslatef(0, -15, 39.9);

		if (flipBack == true)
		{
			if (isFlipped[0][1] && isFlipped[2][2])
			{
				
			}
			else
			{
				if (rotatePuzzle2 > 0)
				{
					rotatePuzzle2 -= 5;
				}
				else
				{
					isFlipped[0][1] = false;
				}
			}
		}
		else
		{
			if (rotatePuzzle2 < 180)
			{
				rotatePuzzle2 += 5;
			}
		}
	}

	glBegin(GL_POLYGON);
	{
		if (isSelected[0][1])
		{
			glColor4f(1, 0.8, 0, 1);
		}
		else
		{
			glColor4f(1, 1, 1, 1);
		}
		glVertex3f(-3, 18, -39.9);
		glVertex3f(3, 18, -39.9);
		glVertex3f(3, 12, -39.9);
		glVertex3f(-3, 12, -39.9);

		glEnd();
	}

	glBegin(GL_POLYGON);
	{
		if (puzzleComplete == true)
		{
			glColor4f(0, 1, 0, 1);
		}
		else
		{
			glColor4f(1, 0, 1, 1);
		}
		
		glVertex3f(-3, 18, -39.902f);
		glVertex3f(3, 18, -39.902f);
		glVertex3f(3, 12, -39.902f);
		glVertex3f(-3, 12, -39.902f);

		glEnd();
	}

	glPopMatrix();
}

void OpenGL::DrawPuzzle3()
{
	glPushMatrix();

	if (isFlipped[0][2])
	{
		glTranslatef(6, 15, -39.9);
		glRotatef(rotatePuzzle3, 0, 1, 0);
		glTranslatef(-6, -15, 39.9);

		if (flipBack == true)
		{
			if (isFlipped[0][2] && isFlipped[1][1])
			{
				
			}
			else
			{
				if (rotatePuzzle3 > 0)
				{
					rotatePuzzle3 -= 5;
				}
				else
				{
					isFlipped[0][2] = false;
				}
			}
		}
		else
		{
			if (rotatePuzzle3 < 180)
			{
				rotatePuzzle3 += 5;
			}
		}
	}

	glBegin(GL_POLYGON);
	{
		if (isSelected[0][2])
		{
			glColor4f(1, 0.8, 0, 1);
		}
		else
		{
			glColor4f(1, 1, 1, 1);
		}
		glVertex3f(3, 18, -39.9);
		glVertex3f(9, 18, -39.9);
		glVertex3f(9, 12, -39.9);
		glVertex3f(3, 12, -39.9);

		glEnd();
	}

	glBegin(GL_POLYGON);
	{
		if (puzzleComplete == true)
		{
			glColor4f(0, 1, 0, 1);
		}
		else
		{
			glColor4f(1, 1, 0, 1);
		}
		
		glVertex3f(3, 18, -39.902f);
		glVertex3f(9, 18, -39.902f);
		glVertex3f(9, 12, -39.902f);
		glVertex3f(3, 12, -39.902f);

		glEnd();
	}

	glPopMatrix();
}

void OpenGL::DrawPuzzle4()
{
	glPushMatrix();

	if (isFlipped[1][0])
	{
		glTranslatef(-6, 9, -39.9);
		glRotatef(rotatePuzzle4, 0, 1, 0);
		glTranslatef(6, -9, 39.9);

		if (flipBack == true)
		{
			if (isFlipped[1][0] && isFlipped[1][2])
			{
				
			}
			else
			{
				if (rotatePuzzle4 > 0)
				{
					rotatePuzzle4 -= 5;
				}
				else
				{
					isFlipped[1][0] = false;
				}
			}
		}
		else
		{
			if (rotatePuzzle4 < 180)
			{
				rotatePuzzle4 += 5;
			}
		}
	}

	glBegin(GL_POLYGON);
	{
		if (isSelected[1][0])
		{
			glColor4f(1, 0.8, 0, 1);
		}
		else
		{
			glColor4f(1, 1, 1, 1);
		}
		glVertex3f(-9, 12, -39.9);
		glVertex3f(-3, 12, -39.9);
		glVertex3f(-3, 6, -39.9);
		glVertex3f(-9, 6, -39.9);

		glEnd();
	}

	glBegin(GL_POLYGON);
	{
		if (puzzleComplete == true)
		{
			glColor4f(0, 1, 0, 1);
		}
		else
		{
			glColor4f(1, 0, 0, 1);
		}
		
		glVertex3f(-9, 12, -39.902f);
		glVertex3f(-3, 12, -39.902f);
		glVertex3f(-3, 6, -39.902f);
		glVertex3f(-9, 6, -39.902f);

		glEnd();
	}

	glPopMatrix();
}

void OpenGL::DrawPuzzle5()
{
	glPushMatrix();

	if (isFlipped[1][1])
	{
		glTranslatef(0, 9, -39.9);
		glRotatef(rotatePuzzle5, 0, 1, 0);
		glTranslatef(0, -9, 39.9);

		if (flipBack == true)
		{
			if (isFlipped[0][2] && isFlipped[1][1])
			{
				
			}
			else
			{
				if (rotatePuzzle5 > 0)
				{
					rotatePuzzle5 -= 5;
				}
				else
				{
					isFlipped[1][1] = false;
				}
			}
		}
		else
		{
			if (rotatePuzzle5 < 180)
			{
				rotatePuzzle5 += 5;
			}
		}
	}

	glBegin(GL_POLYGON);
	{
		if (isSelected[1][1])
		{
			glColor4f(1, 0.8, 0, 1);
		}
		else
		{
			glColor4f(1, 1, 1, 1);
		}
		glVertex3f(-3, 12, -39.9);
		glVertex3f(3, 12, -39.9);
		glVertex3f(3, 6, -39.9);
		glVertex3f(-3, 6, -39.9);

		glEnd();
	}

	glBegin(GL_POLYGON);
	{
		if (puzzleComplete == true)
		{
			glColor4f(0, 1, 0, 1);
		}
		else
		{
			glColor4f(1, 1, 0, 1);
		}
		
		glVertex3f(-3, 12, -39.902f);
		glVertex3f(3, 12, -39.902f);
		glVertex3f(3, 6, -39.902f);
		glVertex3f(-3, 6, -39.902f);

		glEnd();
	}

	glPopMatrix();
}

void OpenGL::DrawPuzzle6()
{
	glPushMatrix();

	if (isFlipped[1][2])
	{
		glTranslatef(6, 9, -39.9);
		glRotatef(rotatePuzzle6, 0, 1, 0);
		glTranslatef(-6, -9, 39.9);

		if (flipBack == true)
		{
			if (isFlipped[1][0] && isFlipped[1][2])
			{
				
			}
			else
			{
				if (rotatePuzzle6 > 0)
				{
					rotatePuzzle6 -= 5;
				}
				else
				{
					isFlipped[1][2] = false;
				}
			}
		}
		else
		{
			if (rotatePuzzle6 < 180)
			{
				rotatePuzzle6 += 5;
			}
		}
	}

	glBegin(GL_POLYGON);
	{
		if (isSelected[1][2])
		{
			glColor4f(1, 0.8, 0, 1);
		}
		else
		{
			glColor4f(1, 1, 1, 1);
		}
		glVertex3f(3, 12, -39.9);
		glVertex3f(9, 12, -39.9);
		glVertex3f(9, 6, -39.9);
		glVertex3f(3, 6, -39.9);

		glEnd();
	}

	glBegin(GL_POLYGON);
	{
		if (puzzleComplete == true)
		{
			glColor4f(0, 1, 0, 1);
		}
		else
		{
			glColor4f(1, 0, 0, 1);
		}
		
		glVertex3f(3, 12, -39.902f);
		glVertex3f(9, 12, -39.902f);
		glVertex3f(9, 6, -39.902f);
		glVertex3f(3, 6, -39.902f);

		glEnd();
	}

	glPopMatrix();
}

void OpenGL::DrawPuzzle7()
{
	glPushMatrix();

	if (isFlipped[2][0])
	{
		glTranslatef(-6, 3, -39.9);
		glRotatef(rotatePuzzle7, 0, 1, 0);
		glTranslatef(6, -3, 39.9);

		if (flipBack == true)
		{
			if (isFlipped[0][0] && isFlipped[2][0])
			{
				
			}
			else
			{
				if (rotatePuzzle7 > 0)
				{
					rotatePuzzle7 -= 5;
				}
				else
				{
					isFlipped[2][0] = false;
				}
			}
		}
		else
		{
			if (rotatePuzzle7 < 180)
			{
				rotatePuzzle7 += 5;
			}
		}
	}

	glBegin(GL_POLYGON);
	{
		if (isSelected[2][0])
		{
			glColor4f(1, 0.8, 0, 1);
		}
		else
		{
			glColor4f(1, 1, 1, 1);
		}
		glVertex3f(-9, 6, -39.9);
		glVertex3f(-3, 6, -39.9);
		glVertex3f(-3, 0, -39.9);
		glVertex3f(-9, 0, -39.9);

		glEnd();
	}

	glBegin(GL_POLYGON);
	{
		if (puzzleComplete == true)
		{
			glColor4f(0, 1, 0, 1);
		}
		else
		{
			glColor4f(0, 1, 1, 1);
		}
		
		glVertex3f(-9, 6, -39.902f);
		glVertex3f(-3, 6, -39.902f);
		glVertex3f(-3, 0, -39.902f);
		glVertex3f(-9, 0, -39.902f);

		glEnd();
	}

	glPopMatrix();
}

void OpenGL::DrawPuzzle8()
{
	glPushMatrix();

	glBegin(GL_POLYGON);
	{
		glColor4f(0, 0, 0, alpha);
		glVertex3f(3, 6, -40);
		glVertex3f(-3, 6, -40);
		glVertex3f(-3, -4, -40);
		glVertex3f(3, -4, -40);

		glEnd();
	}

	glPopMatrix();
}

void OpenGL::DrawPuzzle9()
{
	glPushMatrix();

	if (isFlipped[2][2])
	{
		glTranslatef(6, 3, -39.9);
		glRotatef(rotatePuzzle9, 0, 1, 0);
		glTranslatef(-6, -3, 39.9);

		if (flipBack == true)
		{
			if (isFlipped[0][1] && isFlipped[2][2])
			{
				
			}
			else
			{
				if (rotatePuzzle9 > 0)
				{
					rotatePuzzle9 -= 5;
				}
				else
				{
					isFlipped[2][2] = false;
				}
			}
		}
		else
		{
			if (rotatePuzzle9 < 180)
			{
				rotatePuzzle9 += 5;
			}
		}
	}

	glBegin(GL_POLYGON);
	{
		if (isSelected[2][2])
		{
			glColor4f(1, 0.8, 0, 1);
		}
		else
		{
			glColor4f(1, 1, 1, 1);
		}
		glVertex3f(3, 6, -39.9);
		glVertex3f(9, 6, -39.9);
		glVertex3f(9, 0, -39.9);
		glVertex3f(3, 0, -39.9);

		glEnd();
	}

	glBegin(GL_POLYGON);
	{
		if (puzzleComplete == true)
		{
			glColor4f(0, 1, 0, 1);
		}
		else
		{
			glColor4f(1, 0, 1, 1);
		}
		glVertex3f(3, 6, -39.902f);
		glVertex3f(9, 6, -39.902f);
		glVertex3f(9, 0, -39.902f);
		glVertex3f(3, 0, -39.902f);

		glEnd();
	}

	glPopMatrix();
}

void OpenGL::DrawObservatory()
{
	glPushMatrix();

	glBegin(GL_POLYGON);
	{
		glColor4f(0.3, 0.3, 0.3, 1);
		glVertex3f(-3, -4, -40);
		glVertex3f(-3, -4, -50);
		glVertex3f(-3, 6, -50);
		glVertex3f(-3, 6, -40);

		glEnd();
	}

	glBegin(GL_POLYGON);
	{
		glColor4f(0.3, 0.3, 0.3, 1);
		glVertex3f(3, -4, -40);
		glVertex3f(3, -4, -50);
		glVertex3f(3, 6, -50);
		glVertex3f(3, 6, -40);

		glEnd();
	}

	glBegin(GL_POLYGON);
	{
		glColor4f(0.4, 0.4, 0.4, 1);
		glVertex3f(-3, -4, -40);
		glVertex3f(-3, -4, -50);
		glVertex3f(3, -4, -50);
		glVertex3f(3, -4, -40);

		glEnd();
	}

	glBegin(GL_POLYGON);
	{
		glColor4f(0.4, 0.4, 0.4, 1);
		glVertex3f(-3, 6, -40);
		glVertex3f(-3, 6, -50);
		glVertex3f(3, 6, -50);
		glVertex3f(3, 6, -40);

		glEnd();
	}

	glBegin(GL_POLYGON);
	{
		glColor4f(0.4, 0.4, 0.4, 1);
		glVertex3f(10, -4, -70);
		glVertex3f(10, -4, -50);
		glVertex3f(-10, -4, -50);
		glVertex3f(-10, -4, -70);

		glEnd();
	}

	glBegin(GL_POLYGON);
	{
		glColor4f(0.3, 0.3, 0.3, 1);
		glVertex3f(10, -4, -70);
		glVertex3f(10, -4, -50);
		glVertex3f(10, 20, -50);
		glVertex3f(10, 20, -70);

		glEnd();
	}

	glBegin(GL_POLYGON);
	{
		glColor4f(0.3, 0.3, 0.3, 1);
		glVertex3f(-10, -4, -70);
		glVertex3f(-10, -4, -50);
		glVertex3f(-10, 20, -50);
		glVertex3f(-10, 20, -70);

		glEnd();
	}

	glBegin(GL_POLYGON);
	{
		glColor4f(0.3, 0.3, 0.3, 1);
		glVertex3f(-10, -4, -50);
		glVertex3f(-3, -4, -50);
		glVertex3f(-3, 20, -50);
		glVertex3f(-10, 20, -50);

		glEnd();
	}

	glBegin(GL_POLYGON);
	{
		glColor4f(0.3, 0.3, 0.3, 1);
		glVertex3f(10, -4, -50);
		glVertex3f(3, -4, -50);
		glVertex3f(3, 20, -50);
		glVertex3f(10, 20, -50);

		glEnd();
	}

	glBegin(GL_POLYGON);
	{
		glColor4f(0.3, 0.3, 0.3, 1);
		glVertex3f(3, 6, -50);
		glVertex3f(-3, 6, -50);
		glVertex3f(-3, 20, -50);
		glVertex3f(3, 20, -50);

		glEnd();
	}

	glBegin(GL_POLYGON);
	{
		glColor4f(0.3, 0.3, 0.3, 1);
		glVertex3f(10, -4, -70);
		glVertex3f(-10, -4, -70);
		glVertex3f(-10, 20, -70);
		glVertex3f(10, 20, -70);

		glEnd();
	}

	glBegin(GL_POLYGON);
	{
		glColor4f(0, 0, 0, 1);
		glVertex3f(2, -3.9, -57.5);
		glVertex3f(2, -3.9, -62.5);
		glVertex3f(-2, -3.9, -62.5);
		glVertex3f(-2, -3.9, -57.5);

		glEnd();
	}

	glPopMatrix();
}

void OpenGL::DrawObservatoryRoof()
{
	glPushMatrix();

	glTranslatef(0, -23.33, 53.33);
	glRotatef(rotationRoof1, 1, 0, 0);
	glTranslatef(0, 23.33,-53.33);
	glBegin(GL_POLYGON);
	{
		glColor4f(0.25, 0.25, 0.25, 1);
		glVertex3f(10, 20, -50);
		glVertex3f(-10, 20, -50);
		glVertex3f(0, 30, -60);

		glEnd();
	}

	glPopMatrix();

	glPushMatrix();

	glTranslatef(0, -23.33, 53.3);
	glRotatef(rotationRoof2, 0, 0, -1);
	glTranslatef(0, 23.33, -53.3);
	glBegin(GL_POLYGON);
	{
		glColor4f(0.2, 0.2, 0.2, 1);
		glVertex3f(10, 20, -50);
		glVertex3f(10, 20, -70);
		glVertex3f(0, 30, -60);

		glEnd();
	}

	glPopMatrix();

	glPushMatrix();

	glTranslatef(0, -23.33, 53.3);
	glRotatef(rotationRoof3, 0, 0, 1);
	glTranslatef(0, 23.33, -53.3);
	glBegin(GL_POLYGON);
	{
		glColor4f(0.2, 0.2, 0.2, 1);
		glVertex3f(-10, 20, -50);
		glVertex3f(-10, 20, -70);
		glVertex3f(0, 30, -60);

		glEnd();
	}

	glPopMatrix();

	glPushMatrix();

	glTranslatef(0, -23.33, 53.3);
	glRotatef(rotationRoof4, 1, 0, 0);
	glTranslatef(0, 23.33, -53.3);
	glBegin(GL_POLYGON);
	{
		glColor4f(0.25, 0.25, 0.25, 1);
		glVertex3f(10, 20, -70);
		glVertex3f(-10, 20, -70);
		glVertex3f(0, 30, -60);

		glEnd();
	}

	glPopMatrix();
}
