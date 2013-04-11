#include "SuperGraphicsEngine.h"

#ifdef _WIN32
	#include <windows.h>
#endif
#include <glew/GL/glew.h>
#include <GL/GL.h>
#include <gl/glfw.h>


#include <OpenGLHelpers\MatrixPusher.h>


#include <Engine\TrackballCameraHandler.h>

SuperGraphicsEngine::SuperGraphicsEngine():
_cameraHandler(new TrackballCameraHandler())
{


}


SuperGraphicsEngine::~SuperGraphicsEngine(){


}


void SuperGraphicsEngine::addDrawableObject(DrawableObject* obj){
	_drawableObjects.push_back(obj);
}


void SuperGraphicsEngine::init(){
	glClearColor(0,0,0,0);
	glEnable(GL_DEPTH_TEST);
}

void SuperGraphicsEngine::draw(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	if(_cameraHandler!=0){
		_cameraHandler->setCamera();
	}
	for(auto obj = _drawableObjects.begin();obj != _drawableObjects.end();++obj){
		MatrixPusher m();
		(*obj)->draw();
	}

}


CameraHandler* SuperGraphicsEngine::getCameraHandler(){
	return _cameraHandler;
}

void SuperGraphicsEngine::setCameraHandler(CameraHandler *cameraHandler){
	_cameraHandler = cameraHandler;
}
