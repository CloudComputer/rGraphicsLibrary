#include "SuperEngine.h"

#include <iostream>

#include <OpenGLHelpers\OpenGLInfo.h>

SuperEngine *SuperEngine::engine = 0;

SuperEngine* SuperEngine::getEngine(){
	return engine;
}

SuperEngine* SuperEngine::CreateEngine(std::string title,glm::ivec2 winSize){
	if(engine != 0)
		exit(RGL_EXIT_CODE_ENGINE_ALREADY_CREATED);
	return engine = new SuperEngine(title,winSize);
}

SuperEngine::SuperEngine(std::string title,glm::ivec2 winSize):_title(title),_winSize(winSize)
{
	graphicEngine = new SuperGraphicsEngine();
	mouseEventHandler = MouseEventHandler::Handler();
	keyboardEventHandler = KeyboardEventHandler::Handler();
	windowEventHandler = WindowEventHandler::Handler();
}


SuperEngine::~SuperEngine(){

}

void GLFWresize(int w,int h){
	SuperEngine::getEngine()->resize(w,h);
}

void GLFWMouseButton(int button,int state,int x,int y){
	SuperEngine::getEngine()->mouseButton(button,state,x,y);
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

	SuperEngine::getEngine()->mouseMotion(x,y,dx,dy);
}

void GLFWMouseWheel(int i){
	static int oldi = 0xFFFFF;
	if(oldi==0xFFFFF){ //First time only
		oldi = i;
	}
	int di = oldi - i;
	oldi = i;
	SuperEngine::getEngine()->mouseWheel(i,di);
}

void GLFWKeyboard(int button,int state){
	SuperEngine::getEngine()->keyboard(button,state);
}

void GLFWCharKeyboard(char c,int state){
	SuperEngine::getEngine()->charKeyboard(c,state);
}

void GLFWWinClose(){
	SuperEngine::getEngine()->winClose();
}

void GLFWWinRefresh(){
	SuperEngine::getEngine()->winRefresh();
}


bool SuperEngine::init(){
	if (glfwInit() != GL_TRUE){
		std::cout << "Could not init glfw"<< std::endl;
		return false;
	}

	bool fullscreen = !true;
	if(fullscreen){
		_winSize = glm::ivec2(1920,1080);
	}

	if (glfwOpenWindow(_winSize.x, _winSize.y, 8, 8, 8, 8, 32, 0,fullscreen ? GLFW_FULLSCREEN  : GLFW_WINDOW) != GL_TRUE){
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
	int maj,min,rev;
	glfwGetVersion(&maj,&min,&rev);
	std::cout << "GLFW Version: " << maj << "." << min << "." << rev  << std::endl;
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

void SuperEngine::loop(){
	runnning = true;
	while(runnning){
		if (glfwGetKey(GLFW_KEY_ESC) == GLFW_PRESS)
			break;
		graphicEngine->draw();
		glfwSwapBuffers();
	}
}





void SuperEngine::mouseButton(int b,int s,int x,int y){
	mouseEventHandler->mouseButtonClicked(b,s,x,y);
	
}

void SuperEngine::mouseMotion(int x,int y,int dx,int dy){
	mouseEventHandler->mouseMotion(x,y,dx,dy);
}

void SuperEngine::mouseWheel(int i,int di){
	mouseEventHandler->mouseWheel(i,di);
}

void SuperEngine::keyboard(int b,int s){
	keyboardEventHandler->buttonPressed(b,s);
}

void SuperEngine::charKeyboard(char c,int s){
	keyboardEventHandler->characterPressed(c,s);
}

void SuperEngine::resize(int w,int h){
	windowEventHandler->windowResized(w,h);	
}

void SuperEngine::winClose(){
	runnning = false;
	windowEventHandler->windowClosed();	

}

void SuperEngine::winRefresh(){
	windowEventHandler->windowRefreshed();
}

GraphicsEngine* SuperEngine::getGraphicEngine(){return graphicEngine;}