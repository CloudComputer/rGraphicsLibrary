#include "EventHandler.h"

#include <algorithm>

WindowEventHandler* WindowEventHandler::handler=0;
KeyboardEventHandler* KeyboardEventHandler::handler=0;
MouseEventHandler* MouseEventHandler::handler=0;



/*
*    Listeners Constructors
*/

WindowEventListener::WindowEventListener(bool autoRegister){
	if(autoRegister)
		WindowEventHandler::Handler()->registerListener(this);
}

KeyboardEventListener::KeyboardEventListener(bool autoRegister){
	if(autoRegister)
		KeyboardEventHandler::Handler()->registerListener(this);
}

MouseEventListener::MouseEventListener(bool autoRegister){
	if(autoRegister)
		MouseEventHandler::Handler()->registerListener(this);
}


/*
*  Listeners Destructores
*/
WindowEventListener::~WindowEventListener(){
	WindowEventHandler::Handler()->removeListener(this);
}

KeyboardEventListener::~KeyboardEventListener(){
	KeyboardEventHandler::Handler()->removeListener(this);
}

MouseEventListener::~MouseEventListener(){
	MouseEventHandler::Handler()->removeListener(this);
}




/*
*     Event register and removes
*/

void WindowEventHandler::registerListener(WindowEventListener *l){
	_listeners.push_back(l);
}
void WindowEventHandler::removeListener(WindowEventListener *l){
	auto found = std::find(_listeners.begin(),_listeners.end(),l);
	if(found!=_listeners.end())
		_listeners.erase(found);
}

void KeyboardEventHandler::registerListener(KeyboardEventListener *l){
	_listeners.push_back(l);
}
void KeyboardEventHandler::removeListener(KeyboardEventListener *l){
	auto found = std::find(_listeners.begin(),_listeners.end(),l);
	if(found!=_listeners.end())
		_listeners.erase(found);
}

void MouseEventHandler::registerListener(MouseEventListener *l){
	_listeners.push_back(l);
}
void MouseEventHandler::removeListener(MouseEventListener *l){
	auto found = std::find(_listeners.begin(),_listeners.end(),l);
	if(found!=_listeners.end())
		_listeners.erase(found);
}



/*
*Handlers static singelton getters
*/
WindowEventHandler* WindowEventHandler::Handler(){
	if(handler == 0)
		handler = new WindowEventHandler();
	return handler;
}
KeyboardEventHandler* KeyboardEventHandler::Handler(){
	if(handler == 0)
		handler = new KeyboardEventHandler();
	return handler;
}
MouseEventHandler* MouseEventHandler::Handler(){
	if(handler == 0)
		handler = new MouseEventHandler();
	return handler;
}


/*
*Event callbacks
*/

#define __FOR__ for(auto l = _listeners.begin();l!=_listeners.end();++l)

void WindowEventHandler::windowClosed(){
	__FOR__ (*l)->windowClosed();
}
void WindowEventHandler::windowRefreshed(){
	__FOR__ (*l)->windowRefreshed();
}
void WindowEventHandler::windowResized(int w,int h){
	__FOR__ (*l)->windowResized(w,h);
}

	
void KeyboardEventHandler::buttonPressed(int button,int state){
	__FOR__ (*l)->buttonPressed(button,state);
}
void KeyboardEventHandler::characterPressed(char character,int state){
	__FOR__ (*l)->characterPressed(character,state);
}
	

void MouseEventHandler::mouseButtonClicked(int button,int state,int x,int y){
	__FOR__ (*l)->mouseButtonClicked(button,state,x,y);
}
void MouseEventHandler::mouseMotion(int x,int y,int dx,int dy){
	__FOR__ (*l)->mouseMotion(x,y,dx,dy);
}
void MouseEventHandler::mouseWheel(int i,int di){
	__FOR__ (*l)->mouseWheel(i,di);
}