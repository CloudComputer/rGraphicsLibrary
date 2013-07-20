#include "rEngine.h"

#include <iostream>

#include "rWindow.h"
#include "Viewport.h"

#include <Util\Macros.h>
#include "IDObject.h"


std::map<std::string,IDObject *> IDObject::_objects;

rEngine::rEngine(){


}


rEngine::~rEngine(){
	

}


bool rEngine::init(int argc,char **argv){
	return true;
}


void rEngine::loop(){
	_sw.restart();
	while(rWindow::hasOpenWindows()){
		float dt = _sw.getFractionElapsedSeconds();
		_sw.restart();
		
		IT_FOR(_scenes,s){if((*s)->isActive()) (*s)->update(dt);}
		IT_FOR(_scenes,s){if((*s)->isActive()) (*s)->render();}

		rWindow::updateAll();

		//IT_FOR(_windows,w) (*w)->redraw();
		
		glfwPollEvents();

	}
	glfwTerminate();
}
