#ifndef _SUPERENGINE_H_
#define _SUPERENGINE_H_

#include <Engine\Engine.h>
#include "SuperGraphicsEngine.h"

#ifdef _WIN32
	#include <windows.h>
#endif
#include <glew/GL/glew.h>
#include <GL/GL.h>
#include <gl/glfw.h>

#include <glm\glm.hpp>

class SuperEngine : public Engine{
	SuperEngine(std::string title,glm::ivec2 winSize);
	virtual ~SuperEngine();
	static SuperEngine *engine;

	std::string _title;
	glm::ivec2 _winSize;

	GraphicsEngine *graphicEngine;
	
	MouseEventHandler *mouseEventHandler;
	KeyboardEventHandler *keyboardEventHandler;
	WindowEventHandler *windowEventHandler;

	virtual void resize(int w,int h);
	virtual void mouseButton(int button,int state,int x,int y);
	virtual void mouseMotion(int x,int y,int dx,int dy);
	virtual void mouseWheel(int i,int di);
	virtual void keyboard(int button,int state);
	virtual void charKeyboard(char character,int state);
	virtual void winClose();
	virtual void winRefresh();

	friend void GLFWresize(int w,int h);
	friend void GLFWMouseButton(int button,int state,int x,int y);
	friend void GLFWMouseMotion(int x,int y);
	friend void GLFWMouseWheel(int i);
	friend void GLFWKeyboard(int button,int state);
	friend void GLFWCharKeyboard(char character,int state);
	friend void GLFWWinClose();
	friend void GLFWWinRefresh();

	bool runnning;

public:
	
	static SuperEngine* getEngine();
	static SuperEngine* CreateEngine(std::string title = "Unamed window",glm::ivec2 winSize = glm::ivec2(400,400));
	
	GraphicsEngine *getGraphicEngine();

	virtual bool init();
	virtual void loop();

	virtual std::string toString()const{
		return "GLFWEngine";
	}
};



#ifdef ENGINE_MAIN
	#undef ENGINE_MAIN
#endif
#define ENGINE_MAIN(ENGINE,init) int main(int argv,char **argc){\
	Engine *t = ENGINE;	\
	if(!t->init()){std::cerr << "Could not init engine" << std::endl;return RGL_EXIT_CODE_ENGINE_INIT_FAILURE;}\
	init(argv,argc,t);\
	t->loop();\
	return RGL_EXIT_CODE_OK;\
}\

#endif