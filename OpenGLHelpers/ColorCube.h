#ifndef _COLORCUBE_H_
#define _COLORCUBE_H_

#include <stdint.h>
#ifdef _WIN32
#include <windows.h>
#endif
#include <glew/gl/glew.h>
#include <gl/GL.h>

#include <Geometry/BoundingGeometry/BoundingVolume.h>

class ColorCube{
	BoundingAABB _boundingAABB;
	GLfloat *_vertices;
	static const GLfloat _colors[8*4];
	static const GLubyte _indices[4*6];
	bool isCreated;
	GLuint vboVertices,vboColors;
	void create();
public:
	ColorCube(BoundingAABB boundingAABB);
	void draw();
};


#endif
