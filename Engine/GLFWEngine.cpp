#include "GLFWEngine.h"



#include <iostream>

#include <OpenGLHelpers\OpenGLInfo.h>

GLFWEngine *GLFWEngine::engine = 0;

GLFWEngine* GLFWEngine::getEngine(){
	return engine;
}

GLFWEngine* GLFWEngine::CreateEngine(std::string title,glm::ivec2 winSize){
	if(engine != 0)
		exit(RGL_EXIT_CODE_ENGINE_ALREADY_CREATED);
	return engine = new GLFWEngine(title,winSize);
}

GLFWEngine::GLFWEngine(std::string title,glm::ivec2 winSize):_title(title),_winSize(winSize)
{
	graphicEngine = new BasicGraphicsEngine();
	mouseEventHandler = MouseEventHandler::Handler();
	keyboardEventHandler = KeyboardEventHandler::Handler();
	windowEventHandler = WindowEventHandler::Handler();
}


GLFWEngine::~GLFWEngine(){

}

void GLFWresize(int w,int h){
	GLFWEngine::getEngine()->resize(w,h);
}

void GLFWMouseButton(int button,int state,int x,int y){
	GLFWEngine::getEngine()->mouseButton(button,state,x,y);
}

void GLFWMouseMotion(int x,int y){
	static int oldX = 0xFFFFF,oldY;
	if(oldX==0xFFFFF){ //First time only
		oldX = x;
		oldY = y;
	}
	int dx = oldX - x;
	int dy = oldY - y;
	oldX = x;
	oldY = y;

	GLFWEngine::getEngine()->mouseMotion(x,y,dx,dy);
}

void GLFWMouseWheel(int i){
	static int oldi = 0xFFFFF;
	if(oldi==0xFFFFF){ //First time only
		oldi = i;
	}
	int di = oldi - i;
	oldi = i;
	GLFWEngine::getEngine()->mouseWheel(i,di);
}

void GLFWKeyboard(int button,int state){
	GLFWEngine::getEngine()->keyboard(button,state);
}

void GLFWCharKeyboard(char c,int state){
	GLFWEngine::getEngine()->charKeyboard(c,state);
}

void GLFWWinClose(){
	GLFWEngine::getEngine()->winClose();
}

void GLFWWinRefresh(){
	GLFWEngine::getEngine()->winRefresh();
}


bool GLFWEngine::init(){
	if (glfwInit() != GL_TRUE){
		std::cout << "Could not init glfw"<< std::endl;
		return false;
	}

	if (glfwOpenWindow(_winSize.x, _winSize.y, 8, 8, 8, 8, 32, 0, GLFW_WINDOW) != GL_TRUE){
		std::cout << "Could not create window"<< std::endl;
		return false;
	}
	glfwSetWindowTitle (_title.c_str());

	if(glewInit() != GLEW_OK)
	{
		std::cerr << "Failed to init glew" << std::endl;
		return false;
	}

	chkGLErr();
	std::cout << "OpenGL Version: " << OpenGLInfo::getOpenGLVersion() << std::endl;
	std::cout << "Glew   Version: " << OpenGLInfo::getGlewVersion()   << std::endl;
	std::cout << "GLSL   Version: " << OpenGLInfo::getGLSLVersion()   << std::endl;
	std::cout << "OpenGL Vendor: "  << OpenGLInfo::getOpenGLVendor()   << std::endl;
	std::cout << "OpenGL Renderer: "<< OpenGLInfo::getOpenGLRenderer()   << std::endl;
	chkGLErr();

	glfwSetWindowSizeCallback(GLFWwindowsizefun(GLFWresize));
	glfwSetMouseButtonCallback(GLFWmousebuttonfun(GLFWMouseButton));
	glfwSetMousePosCallback(GLFWmouseposfun(GLFWMouseMotion));
	glfwSetMouseWheelCallback(GLFWmousewheelfun(GLFWMouseWheel));
	glfwSetKeyCallback(GLFWkeyfun(GLFWKeyboard));
	glfwSetCharCallback(GLFWcharfun(GLFWCharKeyboard));
	glfwSetWindowCloseCallback(GLFWwindowclosefun(GLFWWinClose));
	glfwSetWindowRefreshCallback(GLFWwindowrefreshfun(GLFWWinRefresh));

	if(graphicEngine != 0)
		graphicEngine->init();

	return true;
}

void GLFWEngine::loop(){
	runnning = true;
	while(runnning){
		if (glfwGetKey(GLFW_KEY_ESC) == GLFW_PRESS)
			break;
		graphicEngine->draw();
		glfwSwapBuffers();
	}
}





void GLFWEngine::mouseButton(int b,int s,int x,int y){
	mouseEventHandler->mouseButtonClicked(b,s,x,y);
	
}

void GLFWEngine::mouseMotion(int x,int y,int dx,int dy){
	mouseEventHandler->mouseMotion(x,y,dx,dy);
}

void GLFWEngine::mouseWheel(int i,int di){
	mouseEventHandler->mouseWheel(i,di);
}

void GLFWEngine::keyboard(int b,int s){
	keyboardEventHandler->buttonPressed(b,s);
}

void GLFWEngine::charKeyboard(char c,int s){
	keyboardEventHandler->characterPressed(c,s);
}

void GLFWEngine::resize(int w,int h){
	windowEventHandler->windowResized(w,h);	
}

void GLFWEngine::winClose(){
	runnning = false;
	windowEventHandler->windowClosed();	

}

void GLFWEngine::winRefresh(){
	windowEventHandler->windowRefreshed();
}

GraphicsEngine* GLFWEngine::getGraphicEngine(){return graphicEngine;}