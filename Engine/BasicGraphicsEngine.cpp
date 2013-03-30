#include "BasicGraphicsEngine.h"

#ifdef _WIN32
	#include <windows.h>
#endif
#include <glew/GL/glew.h>
#include <GL/GL.h>
#include <gl/glfw.h>

#include "OpenGLHelpers\MatrixPusher.h"

#include "TrackballCameraHandler.h"

BasicGraphicsEngine::BasicGraphicsEngine():_cameraHandler(new TrackballCameraHandler()){


}


BasicGraphicsEngine::~BasicGraphicsEngine(){


}


void BasicGraphicsEngine::addDrawableObject(DrawableObject* obj){
	_drawableObjects.push_back(obj);
}


void BasicGraphicsEngine::init(){
	glClearColor(0,0,0,0);
	glEnable(GL_DEPTH_TEST);
}

void BasicGraphicsEngine::draw(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	if(_cameraHandler!=0){
		_cameraHandler->setCamera();
	}
	for(auto obj = _drawableObjects.begin();obj != _drawableObjects.end();++obj){
		MatrixPusher m();
		(*obj)->draw();
	}

}


CameraHandler* BasicGraphicsEngine::getCameraHandler(){
	return _cameraHandler;
}

void BasicGraphicsEngine::setCameraHandler(CameraHandler *cameraHandler){
	_cameraHandler = cameraHandler;
}
