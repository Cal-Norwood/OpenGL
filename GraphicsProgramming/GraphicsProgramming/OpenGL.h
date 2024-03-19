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

	   void DrawPolygon();
	   void DrawPolygon1();
	   void DrawPolygon2();
	   void DrawPolygon3();
	   void DrawPolygon4();
	   void DrawPolygon5();
};
