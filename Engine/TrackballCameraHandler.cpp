#include "TrackballCameraHandler.h"

#include <OpenGLHelpers\OpenGLInfo.h>

#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

TrackballCameraHandler::TrackballCameraHandler():
_fov(45.0f),
_near(0.001f),
_far(100.0f),
_mouseState(0),
_mouseButton(0),
_distance(3.0f),
_rotSpeed(1.0f)
{
}


TrackballCameraHandler::~TrackballCameraHandler(){

}


void TrackballCameraHandler::windowResized(int w,int h){
	glMatrixMode(GL_PROJECTION);
	_projectionMatrix = glm::perspective(_fov,float(w)/h,_near,_far);
	glLoadMatrixf(glm::value_ptr(_projectionMatrix));
	glViewport(0,0,w,h);
	glMatrixMode(GL_MODELVIEW);
}

void TrackballCameraHandler::buttonPressed(int button,int state){

}

void TrackballCameraHandler::setCamera()const{
	glLoadMatrixf(glm::value_ptr(glm::translate(glm::mat4(),glm::vec3(0,0,-_distance))));
	glMultMatrixf(glm::value_ptr(_viewMatrix));
}



void TrackballCameraHandler::mouseButtonClicked(int button,int state,int x,int y){
	if(_mouseButton == button){
		_mouseState = state;
	}
}

void TrackballCameraHandler::mouseMotion(int x,int y,int dx,int dy){
	if(!_mouseState)
		return;
	glm::vec4 up = glm::inverse(_viewMatrix) * glm::vec4(0,1,0,0);
	_viewMatrix = glm::rotate(_viewMatrix,-dx*_rotSpeed,glm::vec3(up));
	
	glm::vec4 side = glm::inverse(_viewMatrix) * glm::vec4(1,0,0,0);
	_viewMatrix = glm::rotate(_viewMatrix,-dy*_rotSpeed,glm::vec3(side));
}

//#include <iostream>
void TrackballCameraHandler::mouseWheel(int i,int di){
	_distance *= (1+di*0.1);
	//std::cout << _distance << " " << di << std::endl;
}
