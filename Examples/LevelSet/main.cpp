
#include <fstream>;

#include <FluidSimulator/EulerianFluid.h>
#include <Math/Interpolator.h>
#include <Volume/VectorField.h>
#include <Volume/ScalarField.h>
#include <Geometry/Mesh.h>

#include <OpenGLHelpers/Shader.h>
#include <OpenGLHelpers/FBO.h>
#include <OpenGLHelpers/OpenGLInfo.h>
#include <OpenGLHelpers/ColorCube.h>
#include <OpenGLHelpers/FBO.h>
#include <Util/StopClock.h>

#include <gl/glfw.h>

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

ColorCube colorCube(BoundingAABB(glm::vec3(-1,-1,-1),glm::vec3(1,1,1)));
ShaderProgram *pgm;
LevelSet *levelSet;
GLuint textureLevelSet;

bool mouse0State = false;
float rx = 0,ry = 0,scale = 1;
float iso = 0;

glm::vec2 winSize;
glm::vec3 textureDimesnions(10,10,10);

void updateTexture(){DBG();
	glm::ivec3 dim = levelSet->getDimensions();

	float minV,maxV;
	minV = levelSet->getMin();
	maxV = levelSet->getMax();
	iso = 0-minV;
	iso /= maxV-minV;
	levelSet->normalize();

	std::cout << minV << " " << maxV << " " << iso  << std::endl;

	GLubyte *data = new GLubyte[dim.x*dim.y*dim.z*4];
	int i = 0;
	FOR(dim){
		data[i++] = levelSet->get(glm::ivec3(x,y,z))*255;
		data[i++] = levelSet->get(glm::ivec3(x,y,z))*255;
		data[i++] = levelSet->get(glm::ivec3(x,y,z))*255;
		data[i++] = levelSet->get(glm::ivec3(x,y,z))*255;
	}
	glBindTexture(GL_TEXTURE_3D,textureLevelSet);
	glTexImage3D(GL_TEXTURE_3D,0,GL_RGBA,dim.x,dim.y,dim.z,0,GL_RGBA,GL_UNSIGNED_BYTE,data);
	//glTexImage3D(GL_TEXTURE_3D,0,GL_R32F,dim.x,dim.y,dim.z,0,GL_RED,GL_FLOAT,levelSet->getData());
	glBindTexture(GL_TEXTURE_3D,0);

	delete data;
}

void resize(int width,int height){DBG();
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


void draw(){DBG();
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
	colorCube.draw();
	fboBackFace->unbind();

	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);


	Raycast();
	
	glfwSwapBuffers();
	chkGLErr();
}
void keyDown(unsigned char key,int x,int y){DBG();
	//glutPostRedisplay();
}
void keyUp(unsigned char key,int x,int y){DBG();
	//glutPostRedisplay();
}
void mouseButton(int button,int state,int x,int y){DBG();
	if(button == 0)
		mouse0State = !state;
	//glutPostRedisplay();
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


	//glutPostRedisplay();
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

	pgm = ShaderProgram::CreateShaderProgramFromSources("glsl/levelset.vert","glsl/levelset.frag");


	levelSet = new LevelSet(Mesh::LoadWavefront("C:/Users/rickard/Dropbox/LIU/år 1/2011 VY P2 - Animation and Modeling/Objs/cow.obj"),(glm::ivec3)textureDimesnions);

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
{DBG();
	if (glfwInit() != GL_TRUE){
		std::cout << "Could not init glfw"<< std::endl;
	}
	
	if (glfwOpenWindow(600, 400, 8, 8, 8, 8, 32, 0, GLFW_WINDOW) != GL_TRUE){
		std::cout << "Could not create window"<< std::endl;
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

	resize(600,400);
	while(true){
		if (glfwGetKey(GLFW_KEY_ESC) == GLFW_PRESS)
			break;



		draw();
	}

	//glutKeyboardFunc(keyDown);DBG();
	//glutKeyboardUpFunc(keyUp);DBG();
	//glutMouseFunc(mouseButton);DBG();
	//glutMotionFunc(mouseMotion);DBG();
	////glutPassiveMotionFunc(mouseMotion);DBG();
	//glutReshapeFunc(resize);DBG();


	//glutIdleFunc(idle);
	/*glutDisplayFunc(draw);DBG();

	glutMainLoop();DBG();*/
	return 0;
}



void Raycast(){DBG();
	chkGLErr();
	pgm->bind();chkGLErr();

	pgm->setTexture("texVol",GL_TEXTURE_3D,0,textureLevelSet);chkGLErr();chkGLErr();
	pgm->setTexture("texBack",GL_TEXTURE_2D,0,fboBackFace->getTexture());chkGLErr();
	pgm->setUniform("winSize",winSize);chkGLErr();
	pgm->setUniform("texSize",textureDimesnions);chkGLErr();
//	pgm->setUniform("iso",iso);chkGLErr();


	colorCube.draw();chkGLErr();


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,0);
	glBindTexture(GL_TEXTURE_3D,0);

	pgm->unbind();
	chkGLErr();
	DBG();
}