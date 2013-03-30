#include "MatrixPusher.h"

#ifdef _WIN32
	#include <windows.h>
#endif
#include <glew/GL/glew.h>
#include <GL/GL.h>
#include <gl/glfw.h>

MatrixPusher::MatrixPusher(){
	glPushMatrix();
}


MatrixPusher::~MatrixPusher(){
	glPopMatrix();
}


