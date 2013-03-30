#ifndef _BASICGRAPHICSENGINE_H_
#define _BASICGRAPHICSENGINE_H_

#include "GraphicsEngine.h"
#include "EventHandler.h"
#include "DrawableObject.h"
#include "CameraHandler.h"

class BasicGraphicsEngine : public GraphicsEngine{
	std::vector<DrawableObject*> _drawableObjects;

	CameraHandler *_cameraHandler;
public:
	BasicGraphicsEngine();
	virtual ~BasicGraphicsEngine();

	virtual void draw();
	virtual void init();

	void addDrawableObject(DrawableObject* object);
	
	CameraHandler *getCameraHandler();
	void setCameraHandler(CameraHandler *cameraHandler);


	virtual std::string toString()const{return "BasicGraphicsEngine";}

};

#endif