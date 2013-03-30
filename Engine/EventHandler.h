#ifndef _EVENTHANDLER_H_
#define _EVENTHANDLER_H_

#include <Base\Object.h>

#include <vector>

class WindowEventListener : public Object{
protected:
	WindowEventListener(bool autoRegister = true);
public:
	virtual ~WindowEventListener();
	virtual void windowClosed(){}
	virtual void windowRefreshed(){}
	virtual void windowResized(int w,int h){}

};
class KeyboardEventListener : public Object{
protected:
	KeyboardEventListener(bool autoRegister = true);
public:
	virtual ~KeyboardEventListener();
	virtual void buttonPressed(int button,int state){}
	virtual void characterPressed(char character,int state){}

};
class MouseEventListener : public Object{
protected:
	MouseEventListener(bool autoRegister = true);
public:
	virtual ~MouseEventListener();
	virtual void mouseButtonClicked(int button,int state,int x,int y){}
	virtual void mouseMotion(int x,int y,int dx,int dy){}
	virtual void mouseWheel(int i,int di){}
};



class WindowEventHandler : public Object{
	static WindowEventHandler *handler;
	WindowEventHandler(){}
	std::vector<WindowEventListener *> _listeners;
public:
	~WindowEventHandler(){}
	static WindowEventHandler* Handler();
	
	void registerListener(WindowEventListener *l);
	void removeListener(WindowEventListener *l);
	void windowClosed();
	void windowRefreshed();
	void windowResized(int w,int h);

	virtual std::string toString()const{return "WindowEventHandler";}
};

class KeyboardEventHandler : public Object{
	static KeyboardEventHandler *handler;
	KeyboardEventHandler(){}
	std::vector<KeyboardEventListener *> _listeners;
public:
	~KeyboardEventHandler(){}
	static KeyboardEventHandler* Handler();

	void registerListener(KeyboardEventListener *l);
	void removeListener(KeyboardEventListener *l);
	void buttonPressed(int button,int state);
	void characterPressed(char character,int state);
	
	virtual std::string toString()const{return "KeyboardEventHandler";}

};

class MouseEventHandler : public Object{
	static MouseEventHandler *handler;
	MouseEventHandler(){}
	std::vector<MouseEventListener *> _listeners;
public:
	~MouseEventHandler(){}
	static MouseEventHandler* Handler();

	void registerListener(MouseEventListener *l);
	void removeListener(MouseEventListener *l);
	void mouseButtonClicked(int button,int state,int x,int y);
	void mouseMotion(int x,int y,int dx,int dy);
	void mouseWheel(int i,int di);

	virtual std::string toString()const{return "MouseEventHandler";}

};


#endif