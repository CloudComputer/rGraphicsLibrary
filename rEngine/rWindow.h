#ifndef _RWINDOW_H_
#define _RWINDOW_H_



#include <glm\glm.hpp>
#include <glew\GL\glew.h>
#include <GLFW\glfw3.h>


#include <vector>
#include <map>
#include <string>

#include "IDObject.h"

#include <OpenGLHelpers\OpenGLInfo.h>

#include <tinyxml2\tinyxml2.h>

class rEngine;
class rWindow;
class Viewport;

struct rWindowHints{
	glm::ivec2 size;
	std::string title;
	
	GLFWmonitor *monitor;
	rWindow *share;

	rWindowHints();


};

class ResizeListener;

class rWindow : public IDObject{
	GLFWwindow *_window;

	static std::map<GLFWwindow*,rWindow*> _windows;

	std::vector<rWindow*> _childWindows;
	rWindow* _parent;

	std::vector<Viewport*> _viewports;
	glm::vec4 _clearColor;
	
	glm::ivec2 _size;

	std::vector<ResizeListener*> _resizeListeners;

	rWindow();
public:
	virtual ~rWindow();
	
	void destroy();
	void onResize(glm::ivec2 size);

	void addViewport(Viewport *viewport){_viewports.push_back(viewport);}

	static bool hasOpenWindows(){
		return _windows.size();
	}

	bool shouldClose(){
		return glfwWindowShouldClose(_window);
	}

	void addResizeListener(ResizeListener *listener);
	void removeResizeListener(ResizeListener *listener);

	glm::ivec2 getSize()const{return _size;}

	void setClearColor(const glm::vec4 &color);
	glm::vec4 getClearColor()const;

	void redraw();

	static rWindow* OpenWindow(rWindowHints hints = rWindowHints());
	static void ReadXML(rEngine *engine,tinyxml2::XMLElement *window);
	
	static void updateAll();

	friend void GLFW_resize(GLFWwindow *win,int w,int h);
	friend class ResizeListener;
};


struct ResizeListener{
	rWindow *owner;
	ResizeListener(rWindow *owner):owner(owner){
		owner->addResizeListener(this);
	}
	virtual ~ResizeListener(){
		owner->removeResizeListener(this);
	}
	virtual void onResize(glm::ivec2 newSize) = 0;
};



#endif