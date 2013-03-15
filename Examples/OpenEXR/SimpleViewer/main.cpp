#include <fstream>;

#include <OpenGLHelpers/FBO.h>
#include <OpenGLHelpers/Shader.h>
#include <OpenGLHelpers/OpenGLInfo.h>
#include <Util/StopClock.h>

#include <Geometry/Mesh/Mesh.h>

#include <gl/glfw.h>

#include <OpenEXRHelpers/OpenEXRIncludes.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <iostream>

StopClock s(true);
ShaderProgram *pgm;

std::string filename;

float expCenter = 0.5;
float expWidth = 1;
float gamma = 1.0;

RGBABuffer buffer;
GLuint tex;

bool flip = !true;

bool mouse0State = false;

std::vector<glm::vec2> lights;

void setTitle(){
	std::stringstream s;
	s << "rGraphicsLibrary - OpenEXR SimpleViewer Example - ";
	s << "Gamma: " << gamma;
	s << "expCenter: " << expCenter;
	s << "expWidth: " << expWidth;
	s << "minExp: " << expCenter - expWidth/2;
	s << "maxExp: " << expCenter + expWidth/2;

	glfwSetWindowTitle (s.str().c_str());

}

void draw(){
	chkGLErr();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);

	pgm->bind();
	pgm->setUniform("gamma",gamma);
	pgm->setUniform("minExp",expCenter - expWidth/2);
	pgm->setUniform("maxExp",expCenter + expWidth/2);
	glBindTexture(GL_TEXTURE_RECTANGLE_NV,tex);

	glColor3f(1,1,1);
	glBegin(GL_QUADS);
	//glColor3f(1,1,1);
	glTexCoord2f(0,flip ? buffer.dim.y : 0);
	glVertex2f(0,0);

	//glColor3f(1,0,0);
	glTexCoord2f(buffer.dim.x,flip ? buffer.dim.y : 0);
	glVertex2f(1,0);

	//glColor3f(0,0,1);
	glTexCoord2f(buffer.dim.x,!flip ? buffer.dim.y : 0);
	glVertex2f(1,1);

	//glColor3f(0,1,0);
	glTexCoord2f(0,!flip ? buffer.dim.y : 0);
	glVertex2f(0,1);

	glEnd();
	pgm->unbind();

	
	glBindTexture(GL_TEXTURE_RECTANGLE_NV,0);
	glPointSize(3.0f);
	glColor3f(1,0,0);
	glBegin(GL_POINTS);
	for(int i = 0;i<lights.size();i++){
		glVertex2f(lights[i].x,lights[i].y);
	}

	glEnd();


	chkGLErr();
	glfwSwapBuffers();
}


int prevScroll = 0;
void wheel(int i){
	gamma *= 1+((i-prevScroll)*0.01);
	prevScroll = i;

	setTitle();
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
		expWidth += dx*0.001;
		expCenter += dy*0.001;
	}
	setTitle();
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
	buffer = OpenEXRReader::readRGBA(filename);
	lights = LightExtractor::ExtractLightLocations(buffer,3);
	tex = OpenEXRTexture::getAsTexture(buffer);
	chkGLErr();
	
	pgm = ShaderProgram::CreateShaderProgramFromSources(GLSL_DIR "/simpleOEXRViewer.vert",GLSL_DIR "/simpleOEXRViewer.frag");
	chkGLErr();
}

void key(int button, int state){
	if(button == 'F' && state){
		flip = !flip;
	}

	setTitle();
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
	
	setTitle();

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
	std::cout << "OpenGL Max Lights: "<< OpenGLInfo::getOpenGLMaxLights()   << std::endl;
	chkGLErr();

	if(argc > 1){
		filename = argv[1];
	}else{
		filename = EXR_IMAGES_DIR "/grace-new.exr";
	}

	init();

	glfwSetWindowSizeCallback(GLFWwindowsizefun(resize));
	glfwSetKeyCallback(GLFWkeyfun(key));

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

