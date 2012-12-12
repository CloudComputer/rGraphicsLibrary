
#include <fstream>;




#include <OpenGLHelpers/Shader.h>
#include <OpenGLHelpers/FBO.h>
#include <OpenGLHelpers/OpenGLInfo.h>
#include <OpenGLHelpers/ColorCube.h>
#include <OpenGLHelpers/FBO.h>
#include <Util/StopClock.h>

#include <gl/glfw.h>

#include <FluidSimulator/EulerianFluid.h>
#include <Math/Interpolator.h>
#include <Volume/VectorField.h>
#include <Volume/ScalarField.h>
#include <Geometry/Mesh.h>


#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <iostream>


//#define DBG() std::cout << __FUNCTION__ << " " << __FILE__ << " " << __LINE__ << std::endl
#ifndef DBG
#define DBG() 
#endif


void Raycast();

FBO *fboBackFace;

ColorCube *colorCube;
ShaderProgram *pgm;
LevelSet *levelSet;
GLuint textureLevelSet;


bool mouse0State = false;
float rx = 0,ry = 0,scale = 1;
float iso = 0.0;

glm::vec2 winSize;
glm::vec3 textureDimesnions(20,20,20);

VectorField *vf = 0;

void updateTexture()
{
	DBG();
	glm::ivec3 dim = levelSet->getDimensions();
	glBindTexture(GL_TEXTURE_3D,textureLevelSet);
	glTexImage3D(GL_TEXTURE_3D,0,GL_R32F,dim.x,dim.y,dim.z,0,GL_RED,GL_FLOAT,levelSet->getData());
	glBindTexture(GL_TEXTURE_3D,0);

}

void resize(int width,int height)
{
	DBG();
	chkGLErr();
	glMatrixMode(GL_PROJECTION);DBG();
	chkGLErr();

	float ar = width;
	ar /= height;
	glLoadMatrixf(glm::value_ptr(glm::perspective(45.0f, ar,0.1f,100.0f)));DBG();
	chkGLErr();


	glMatrixMode(GL_MODELVIEW);DBG();
	glViewport(0,0,width,height);DBG();
	chkGLErr();DBG();

	fboBackFace->setSize(width,height);DBG();chkGLErr();
	fboBackFace->init();DBG();chkGLErr();
	DBG();
	winSize.x = width;
	winSize.y = height;

	std::cout << "Resizing done" << std::endl;
}

void draw()
{
	DBG();
    fboBackFace->clear();
	chkGLErr();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(glm::value_ptr(glm::lookAt(glm::vec3(0,0,3),glm::vec3(0,0,0),glm::vec3(0,1,0))));
	
	glRotatef(ry,1,0,0);
	glRotatef(rx,0,1,0);
	glScalef(scale,scale,scale);
	

	fboBackFace->bind();
	glCullFace(GL_FRONT);
	glEnable(GL_DEPTH_TEST);
	colorCube->draw();
	fboBackFace->unbind();

	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);

	Raycast();

	vf->draw();

	glfwSwapBuffers();
	chkGLErr();
}

int prevScroll = 0;
void wheel(int i){
	scale *= 1+((i-prevScroll)*0.01);
	prevScroll = i;
}

void key(int button, int state){
	if(button == 'E' && state){
		levelSet->erode();
		updateTexture();
	}
	if(button == 'D' && state){
		levelSet->dilate();
		updateTexture();
	}
	if(button == 'A' && state){
		levelSet->advect(vf);
		updateTexture();
	}

}

void mouseButton(int button,int state,int x,int y){DBG();
	if(button == 0)
		mouse0State = state;
}

int mouseX = -1,mouseY;
void mouseMotion(int x,int y){DBG();
	int dx,dy;
	if(mouseX == -1){
		mouseX = x;
		mouseY = y;
	}
	dx = x - mouseX;
	dy = y - mouseY;
	mouseX = x;
	mouseY = y;
	if(mouse0State){
		rx += dx * 1;
		ry += dy * 1;
	}
}

void createLevelSet(float &x,glm::vec3 worldPos){
	x = 9999999;
	x = glm::min(x,glm::distance(worldPos,glm::vec3(0.5,0.5,0.5))-0.4f);
	x = glm::min(x,glm::distance(worldPos,glm::vec3(-0.5,0.5,0.5))-0.4f);
	x = glm::min(x,glm::distance(worldPos,glm::vec3(0.5,-0.5,0.5))-0.4f);
	x = glm::min(x,glm::distance(worldPos,glm::vec3(-0.5,-0.5,0.5))-0.4f);
	x = glm::min(x,glm::distance(worldPos,glm::vec3(0.5,0.5,-0.5))-0.4f);
	x = glm::min(x,glm::distance(worldPos,glm::vec3(-0.5,0.5,-0.5))-0.4f);
	x = glm::min(x,glm::distance(worldPos,glm::vec3(0.5,-0.5,-0.5))-0.4f);
	x = glm::min(x,glm::distance(worldPos,glm::vec3(-0.5,-0.5,-0.5))-0.4f);
}
void createLevelSet2(float &x,glm::vec3 worldPos){
	x = glm::distance(worldPos,glm::vec3(0,0,0))-0.6f;
}
void createLevelSet3(float &x,glm::vec3 worldPos){
	x = worldPos.y;
}

void circle(glm::vec3 &v,glm::vec3 worldPos){
	v.x = -worldPos.y;
	v.y = worldPos.x;
	v.z = 0.0;
	if(glm::length(v) != 0)
		v = glm::normalize(v);
}


void init(){DBG();
	chkGLErr();
	glClearColor(0,0,0,0);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE_3D);
	chkGLErr();
	fboBackFace  = new FBO();
	chkGLErr();

	pgm = ShaderProgram::CreateShaderProgramFromSources( GLSL_DIR "/levelset.vert", GLSL_DIR "/levelset.frag");

	levelSet = new LevelSet((glm::ivec3)textureDimesnions,BoundingAABB(glm::vec3(-1,-1,-1),glm::vec3(1,1,1)));
	levelSet->foreach(createLevelSet3);
	colorCube = new ColorCube(levelSet->getBoundingAABB());

	vf = new VectorField(glm::ivec3(10,10,10),BoundingAABB(glm::vec3(-1,-1,-1),glm::vec3(1,1,1)));
	vf->foreach(circle);

	glGenTextures(1,&textureLevelSet);
	glBindTexture(GL_TEXTURE_3D,textureLevelSet);
	glTexParameteri(GL_TEXTURE_3D,GL_TEXTURE_WRAP_S,GL_CLAMP);
	glTexParameteri(GL_TEXTURE_3D,GL_TEXTURE_WRAP_T,GL_CLAMP);
	glTexParameteri(GL_TEXTURE_3D,GL_TEXTURE_WRAP_R,GL_CLAMP);
	glTexParameteri(GL_TEXTURE_3D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glBindTexture(GL_TEXTURE_3D,0);
	updateTexture();
}


int main( int argc, char* argv[] )
{
	DBG();
	if (glfwInit() != GL_TRUE){
		std::cout << "Could not init glfw"<< std::endl;
		return 2;
	}
	
	if (glfwOpenWindow(600, 400, 8, 8, 8, 8, 32, 0, GLFW_WINDOW) != GL_TRUE){
		std::cout << "Could not create window"<< std::endl;
		return 3;
	}
	glfwSetWindowTitle ("rGraphicsLibrary - LevelSet Example");

	if(glewInit() != GLEW_OK)
	{
		std::cerr << "Failed to init glew" << std::endl;
		exit(1);
	}
	chkGLErr();
	std::cout << "OpenGL Version: " << OpenGLInfo::getOpenGLVersion() << std::endl;
	std::cout << "Glew   Version: " << OpenGLInfo::getGlewVersion()   << std::endl;
	std::cout << "GLSL   Version: " << OpenGLInfo::getGLSLVersion()   << std::endl;
	std::cout << "OpenGL Vendor: "  << OpenGLInfo::getOpenGLVendor()   << std::endl;
	std::cout << "OpenGL Renderer: "<< OpenGLInfo::getOpenGLRenderer()   << std::endl;
	chkGLErr();


	init();

	glfwSetWindowSizeCallback(GLFWwindowsizefun(resize));
	glfwSetKeyCallback(GLFWkeyfun(key));

	glfwSetMouseButtonCallback(GLFWmousebuttonfun(mouseButton));
	glfwSetMousePosCallback(GLFWmouseposfun(mouseMotion));
	glfwSetMouseWheelCallback(GLFWmousewheelfun(wheel));

	
	while(true){
		if (glfwGetKey(GLFW_KEY_ESC) == GLFW_PRESS)
			break;

		levelSet->advect(vf);
		levelSet->recalculate();
		updateTexture();

		draw();
	}

	return 0;
}



void Raycast(){DBG();
	chkGLErr();
	pgm->bind();chkGLErr();

	pgm->setTexture("texVol",GL_TEXTURE_3D,1,textureLevelSet);chkGLErr();chkGLErr();
	pgm->setTexture("texBack",GL_TEXTURE_2D,0,fboBackFace->getTexture());chkGLErr();
	pgm->setUniform("winSize",winSize);chkGLErr();
	pgm->setUniform("texSize",textureDimesnions);chkGLErr();
	pgm->setUniform("iso",iso);chkGLErr();

	colorCube->draw();chkGLErr();

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_3D,0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,0);

	pgm->unbind();
	chkGLErr();
	DBG();
}