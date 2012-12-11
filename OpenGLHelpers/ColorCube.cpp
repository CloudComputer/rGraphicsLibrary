#include "ColorCube.h"

#include "OpenGLInfo.h"

#include <iostream>
#include <string>
#include <sstream>
#include "AttribPusher.h"
#include <glm/gtc/type_ptr.hpp>

#define BUFFER_OFFSET(i) ((char*)NULL + (i))

const GLfloat ColorCube::_colors[8*4] = {0,0,0,1, //0 black
										 0,0,1,1, //1 blue
										 0,1,0,1, //2 green
										 0,1,1,1, //3 cyan
										 1,0,0,1, //4 red
										 1,0,1,1, //5 magenta
										 1,1,0,1, //6 yellow
										 1,1,1,1};

const GLubyte ColorCube::_indices[4*6] = {0,4,5,1,
	0,2,6,4,
	0,1,3,2,
	7,6,2,3,
	5,4,6,7,
	5,7,3,1};

void ColorCube::create(){
	if(isCreated)
		return;
	glGenBuffers(1,&vboColors);
	glGenBuffers(1,&vboVertices);
	glBindBuffer(GL_ARRAY_BUFFER,vboVertices);
	glBufferData(GL_ARRAY_BUFFER,sizeof(GLfloat)*8*3,_vertices,GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER,vboColors);
	glBufferData(GL_ARRAY_BUFFER,sizeof(GLfloat)*8*4,_colors,GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER,0);

	isCreated = true;
}

ColorCube::ColorCube(BoundingAABB boundingAABB):_boundingAABB(boundingAABB){
	_vertices = new GLfloat[8*3];
	glm::vec3 minP = boundingAABB.getPosition(glm::vec3(0,0,0));
	glm::vec3 maxP = boundingAABB.getPosition(glm::vec3(1,1,1));
	int a[24] = {0,0,0,0,0,1,0,1,0,0,1,1,1,0,0,1,0,1,1,1,0,1,1,1};
	for(int i= 0;i<8;i++){
		_vertices[i*3+0] = a[i*3+0] == 1 ? maxP.x : minP.x;
		_vertices[i*3+1] = a[i*3+1] == 1 ? maxP.y : minP.y;
		_vertices[i*3+2] = a[i*3+2] == 1 ? maxP.z : minP.z;
	}

	isCreated = false;
}

void ColorCube::draw(){
	if(!isCreated)
		create();

	AttribPusher lightAttrib(GL_LIGHTING_BIT);chkGLErr();
	glDisable(GL_LIGHTING);chkGLErr();

	glBindBuffer(GL_ARRAY_BUFFER,vboVertices);
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(3, GL_FLOAT,0,0);

	glBindBuffer(GL_ARRAY_BUFFER,vboColors);
	glEnableClientState(GL_VERTEX_ARRAY);
	glColorPointer( 4, GL_FLOAT, 0,0);

	glDrawElements(GL_QUADS,6*4,GL_UNSIGNED_BYTE,_indices);chkGLErr();


	glDisableClientState(GL_COLOR_ARRAY);chkGLErr();
	glDisableClientState(GL_VERTEX_ARRAY);chkGLErr();

	glBindBuffer(GL_ARRAY_BUFFER,0);chkGLErr();
}
