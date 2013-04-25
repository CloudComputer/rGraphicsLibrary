#include <fstream>;

#include <OpenGLHelpers/Shader.h>
#include <OpenGLHelpers/FBO.h>
#include <OpenGLHelpers/OpenGLInfo.h>
#include <Util/StopClock.h>

#include <gl/glfw.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <iostream>

#include <Physics\SpringDamperSystem.h>

StopClock s(true);
float dt;

SpringDamperSystem<glm::vec3> *sds;

void draw(){
	chkGLErr();
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(glm::value_ptr(glm::lookAt(glm::vec3(0,0,3),glm::vec3(0,0,0),glm::vec3(0,1,0))));

	std::vector<SpringParticle<glm::vec3>*> particles = sds->getParticles();
	std::vector<SpringParticle<glm::vec3>*>::iterator particle;	
	glColor4f(1,1,1,1);
	glPointSize(2);
	glBegin(GL_POINTS);
	for(particle = particles.begin();particle != particles.end();++particle){
		glVertex3fv(glm::value_ptr((*particle)->P));
	}

	glEnd();

	glfwSwapBuffers();
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

void init(){
	chkGLErr();
	glClearColor(0,0,0,0);
	
	sds = SpringDamperSystem<glm::vec3>::CreateSquared(1.2,1.2,30,30,20.2,1,1);
	sds->getParticles()[30*30-1]->setPinned();
	sds->getParticles()[30*29]->setPinned();
	sds->addExternalForce(glm::vec3(0,-0.009,0));

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
	glfwSetWindowTitle ("rGraphicsLibrary - Shader Example");

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

	while(true){
		if (glfwGetKey(GLFW_KEY_ESC) == GLFW_PRESS)
			break;
		dt = s.getFractionElapsedSeconds();
		s.restart();
		sds->evolve(dt);
		//sds->evolve(0.001);
		draw();
		
	}
	return 0;
}

