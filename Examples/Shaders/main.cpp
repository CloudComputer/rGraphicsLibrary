#include <fstream>;

#include <OpenGLHelpers/Shader.h>
#include <OpenGLHelpers/FBO.h>
#include <OpenGLHelpers/OpenGLInfo.h>
#include <Util/StopClock.h>

#include <gl/glut.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <iostream>

ShaderProgram *p;

StopClock s;


void idle(){
	chkGLErr();

	glutPostRedisplay();
	chkGLErr();
}
void draw(){
	chkGLErr();
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(glm::value_ptr(glm::lookAt(glm::vec3(0,0,3),glm::vec3(0,0,0),glm::vec3(0,1,0))));

	p->bind();

	p->setUniform("curTime",(float)s.getFractionElapsedSeconds());
	glBegin(GL_QUADS);
	p->setVertexAttrib("tex",glm::vec2(1,0));
	glVertex3f(1,-1,0);

	p->setVertexAttrib("tex",glm::vec2(1,1));
	glVertex3f(1,1,0);

	p->setVertexAttrib("tex",glm::vec2(0,1));
	glVertex3f(-1,1,0);

	p->setVertexAttrib("tex",glm::vec2(0,0));
	glVertex3f(-1,-1,0);
	glEnd();

	p->unbind();

	glutSwapBuffers();
	chkGLErr();
}

void resize(int width,int height){
	chkGLErr();
	glMatrixMode(GL_PROJECTION);
	chkGLErr();
	
	float ar = width;
	ar /= height;
	glLoadMatrixf(glm::value_ptr(glm::perspective(45.0f, ar,0.1f,100.0f)));
	chkGLErr();


	glMatrixMode(GL_MODELVIEW);
	glViewport(0,0,width,height);
	chkGLErr();
}
void keyDown(unsigned char key,int x,int y){}
void keyUp(unsigned char key,int x,int y){}
void mouseButton(int button,int state,int x,int y){}
void mouseMotion(int x,int y){}

void init(){
	chkGLErr();
	p = ShaderProgram::CreateShaderProgramFromSources("glsl/shaders.vert","glsl/shaders.frag");

	glClearColor(0,0,0,0);
	chkGLErr();
}

int main( int argc, char* argv[] )
{
	glutInit (&argc, argv);
	glutInitWindowSize (600, 400);
	glutInitDisplayMode ( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow ("rGraphicsLibrary - Shaders Example");

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

	glutReshapeFunc(resize);
	glutKeyboardFunc(keyDown);
	glutKeyboardUpFunc(keyUp);
	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMotion);
	glutPassiveMotionFunc(mouseMotion);

	glutIdleFunc(idle);
	glutDisplayFunc(draw);

	chkGLErr();
	s.start();
	glutMainLoop();
	return 0;
}

