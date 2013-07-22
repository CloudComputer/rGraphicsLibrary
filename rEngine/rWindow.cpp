#include "rWindow.h"
#include "rEngine.h"
#include "Viewport.h"

#include <Util\Macros.h>

#include <iostream>
#include <algorithm>

#include <boost/algorithm/string.hpp>   


static void GLFW_error(int error,const char* desc){
	std::cerr << "GLFW Error: " << error << " (" << desc << ")" << std::endl;
}

void GLFW_resize(GLFWwindow *win,int w,int h)
{
	rWindow *window = rWindow::_windows[win];
	window->onResize(glm::ivec2(w,h));
}


std::map<GLFWwindow*,rWindow*> rWindow::_windows;

rWindowHints::rWindowHints():
size(600,400),
title("Unnamed window - rEngine"),
monitor(NULL),
share(NULL)
{
	
}


rWindow::rWindow() :  _parent(0){
}


rWindow::~rWindow(){
	destroy();
}

void rWindow::destroy(){
	for(int i = _childWindows.size()-1 ; i>= 0 ; i--){
		//(_childWindows[i])->destroy();
		glfwSetWindowShouldClose(_childWindows[i]->_window,true);
		_childWindows[i]->_parent = 0;
	}

	if(_parent){
		auto it = std::find(_parent->_childWindows.begin(),_parent->_childWindows.end(),this);
		if(it != _parent->_childWindows.end())
			_parent->_childWindows.erase(it);
		_parent = 0;
	}
	if(_window != 0)
		glfwDestroyWindow(_window);
	_window = 0;
}

void rWindow::updateAll(){
	GLFWwindow *win = 0;
	while(true){
		IT_FOR(_windows,w){
			if(w->second->shouldClose()){
				win = w->first;
				break;
			}
		}
		if(win){
			auto w = _windows[win];
			_windows.erase(win);
			delete w;
			win = 0;
		}else
			break;
	}
		

	IT_FOR(_windows,w){
		w->second->redraw();
	}
	
}


		//for(int i = _windows.size()-1 ; i>= 0; i--){
		//	if(!_windows[i])
		//		_windows.erase(_windows.begin() + i ) ;
		//	if(_windows[i]->shouldClose()){
		//		auto w = _windows[i];
		//		_windows.erase(_windows.begin() + i ) ;
		//		delete w;
		//	}
		//}

void rWindow::onResize(glm::ivec2 size){
	this->_size = size;
	IT_FOR(_resizeListeners,l){
		(*l)->onResize(size);
	}
}

void rWindow::addResizeListener(ResizeListener *listener){
	_resizeListeners.push_back(listener);
}
void rWindow::removeResizeListener(ResizeListener *listener){
	auto it = std::find(_resizeListeners.begin() , _resizeListeners.end() , listener);
	if(it != _resizeListeners.end() )
		_resizeListeners.erase(it);
}

void rWindow::setClearColor(const glm::vec4 &color){
	_clearColor = color;
	glClearColor(color.r,color.g,color.b,color.a);
}
glm::vec4 rWindow::getClearColor()const{return _clearColor;}

void rWindow::redraw(){

	glfwMakeContextCurrent(_window);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	IT_FOR(_viewports,v){
		(*v)->draw();
	}

	glfwSwapBuffers(_window);
    glfwPollEvents();
}

rWindow* rWindow::OpenWindow(rWindowHints hints){

	rWindow* win = new rWindow();
	
	if (!glfwInit())
        exit(EXIT_FAILURE);
	
	

	glfwSetErrorCallback(GLFW_error);
	win->_window = glfwCreateWindow(hints.size.x, //size x
									hints.size.y, //size y
									hints.title.c_str(), //window title
									hints.monitor, // monitor to create window at
									(hints.share == NULL) ? NULL :  hints.share->_window);

	if(hints.share != NULL){
		hints.share->_childWindows.push_back(win);
		win->_parent = hints.share;
	}

	if (!win->_window){
	
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

	_windows[win->_window] = win;
	win->_size = hints.size;

	
	
	glfwMakeContextCurrent(win->_window);
	if(glewInit() != GLEW_OK)
	{
		std::cerr << "Failed to init glew" << std::endl;
		return false;
	}
	
	win->setClearColor(glm::vec4(0.3,0.3,0.3,0));

	
	OpenGLInfo::printOGLInformation(std::cout);
	std::cout << "GLFW Version:\t" << glfwGetVersionString() << std::endl;

	glfwSetWindowSizeCallback(win->_window,GLFWwindowsizefun(GLFW_resize));

	/*
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);*/

	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	return win;
}

void rWindow::ReadXML(rEngine *engine,tinyxml2::XMLElement *window){
	rWindowHints hints;
		
	auto size = window->FirstChildElement("size");
	auto title = window->FirstChildElement("title");
	auto share = window->FirstChildElement("share");
	auto viewport = window->FirstChildElement("viewport");

	if(size){
		std::string val = size->GetText();
		int w=0,h=0;

		auto it = val.find(' ');
		w = atoi(val.substr(0,it).c_str());
		h = atoi(val.substr(it).c_str());
		if(w>1 && h>1)
			hints.size =glm::ivec2(w,h);
	}
			
	if(title){
		std::string val = title->GetText();
		if(val.length()!=0)
			hints.title = val;
	}
			
	if(share){
		std::string val = share->GetText();
		auto win = IDObject::GetObjectD<rWindow>(val);
		if(win)
			hints.share = win;
	}
			

	rWindow *win = rWindow::OpenWindow(hints);
	std::string id = window->Attribute("id");
	if(id.length()!=0)
		win->setID(id);

	while(viewport){
		std::string type = viewport->Attribute("type");
		boost::algorithm::to_lower(type);
		auto texture = viewport->FirstChildElement("texture");
		std::string textureID,textureSRC;
		textureID = texture->Attribute("name");
		auto c  = texture->Attribute("src");
		textureSRC =  (!c) ? "" : c;
		if(textureSRC.length()!=0)
			Texture::loadTexture(textureID,textureSRC);
			
		Viewport *vp = 0;
		if(type == "fullscreen"){
			vp = new FullscreenViewport(win);
		}
		else if(type == "relativeviewport"){
			glm::vec2 topLeft;
			glm::vec2 size;
			std::string tl = viewport->FirstChildElement("topleft")->GetText();  //TODO this will crash if topleft does not exist
			std::string s = viewport->FirstChildElement("size")->GetText();
			
			auto it = tl.find(' ');
			topLeft.x = atof(tl.substr(0,it).c_str());
			topLeft.y = atof(tl.substr(it).c_str());
			
			it = s.find(' ');
			size.x = atof(s.substr(0,it).c_str());
			size.y = atof(s.substr(it).c_str());

			vp = new RelativeViewport(win,topLeft,size);
		}
		else{
			std::cerr << "No or unsupported viewport type: " << type << std::endl;
		}

		if(vp == 0)
			continue;
		vp->setTexture(textureID);
		win->addViewport(vp);
		viewport = viewport->NextSiblingElement("viewport");
	}
}
