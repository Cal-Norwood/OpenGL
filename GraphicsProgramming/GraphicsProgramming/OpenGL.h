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
};
