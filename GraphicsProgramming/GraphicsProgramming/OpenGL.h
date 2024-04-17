#pragma once
#include "GLUTCallbacks.h"

class OpenGL
{
public:

	   //contructor definition
	   OpenGL(int argc, char* argv[]);

	   //destructor
	   ~OpenGL(void);

	   void Display();

	   void DrawPolygon0();
	   void DrawPolygon();
	   void DrawPolygon1();
	   void DrawPolygon2();
	   void DrawPolygon3();
	   void DrawPolygon4();
	   void DrawPolygon5();
};

class Texture2D
{
private:
	GLuint _ID;
	int _width, _height;

public:

	Texture2D();
	~Texture2D();

	bool Load(const char* path, int width, int height);

	GLuint GetID() const
	{
		return _ID;
	}
	int GetWidth() const { return _width; }
	int GetHeight() const { return _height; }
};