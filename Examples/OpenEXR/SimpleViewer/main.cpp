#include <fstream>;

#include <OpenGLHelpers/FBO.h>
#include <OpenGLHelpers/Shader.h>
#include <OpenGLHelpers/OpenGLInfo.h>
#include <Util/StopClock.h>

#include <Geometry/Mesh.h>

#include <gl/glfw.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <iostream>

StopClock s(true);
ShaderProgram *pgm;

#include <OpenEXRHelpers/OpenEXRIncludes.h>



RGBABuffer buffer;
GLuint tex;


bool mouse0State = false;
float rx = 0,ry = 0,scale = 1;

void draw(){
	chkGLErr();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(glm::value_ptr(glm::lookAt(glm::vec3(0,0,2),glm::vec3(0,0,0),glm::vec3(0,1,0))));
	glRotatef(ry,1,0,0);
	glRotatef(rx,0,1,0);
	glScalef(scale,scale,scale);

	pgm->bind();

	glBindTexture(GL_TEXTURE_RECTANGLE_NV,tex);

	glColor3f(1,1,1);
	glBegin(GL_QUADS);
	//glColor3f(1,1,1);
	glTexCoord2f(0,0);
	glVertex2f(0,0);

	//glColor3f(1,0,0);
	glTexCoord2f(buffer.dim.x,0);
	glVertex2f(1,0);

	//glColor3f(0,0,1);
	glTexCoord2f(buffer.dim.x,buffer.dim.y);
	glVertex2f(1,1);

	//glColor3f(0,1,0);
	glTexCoord2f(0,buffer.dim.y);
	glVertex2f(0,1);

	glEnd();
	pgm->unbind();

	chkGLErr();
	glfwSwapBuffers();
}


int prevScroll = 0;
void wheel(int i){
	scale *= 1+((i-prevScroll)*0.01);
	prevScroll = i;
}

void mouseButton(int button,int state,int x,int y){
if(button == 0)
	mouse0State = state;
}

int mouseX = -1,mouseY;
void mouseMotion(int x,int y){
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


void resize(int width,int height){
	chkGLErr();
	glMatrixMode(GL_PROJECTION);
	chkGLErr();
	
	float ar = width;
	ar /= height;
	glLoadMatrixf(glm::value_ptr(glm::ortho(0.f,1.f,0.f,1.f,-10.f,10.f)));
	chkGLErr();

	glMatrixMode(GL_MODELVIEW);
	glViewport(0,0,width,height);
	chkGLErr();
}

void init(){
	chkGLErr();
	glClearColor(0,0,0,0);
	glEnable(GL_TEXTURE_RECTANGLE_NV);
	glEnable(GL_TEXTURE_2D);
	chkGLErr();
	buffer = OpenEXRReader::readRGBA(EXR_IMAGES_DIR "/uffizi-large.exr");
	tex = OpenEXRTexture::getAsTexture(buffer);
	chkGLErr();
	
	pgm = ShaderProgram::CreateShaderProgramFromSources(GLSL_DIR "/simpleOEXRViewer.vert",GLSL_DIR "/simpleOEXRViewer.frag");
	chkGLErr();
}

int main( int argc, char* argv[] )
{
	if (glfwInit() != GL_TRUE){
		std::cout << "Could not init glfw"<< std::endl;
		return 2;
	}

	if (glfwOpenWindow(600, 400, 8, 8, 8, 8, 32, 0, GLFW_WINDOW) != GL_TRUE){
		std::cout << "Could not create window"<< std::endl;
		return 3;
	}
	glfwSetWindowTitle ("rGraphicsLibrary - OpenEXR SimpleViewer Example");

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

	glfwSetMouseButtonCallback(GLFWmousebuttonfun(mouseButton));
	glfwSetMousePosCallback(GLFWmouseposfun(mouseMotion));
	glfwSetMouseWheelCallback(GLFWmousewheelfun(wheel));

	while(true){
		if (glfwGetKey(GLFW_KEY_ESC) == GLFW_PRESS)
			break;

		draw();
	}
	return 0;
}

