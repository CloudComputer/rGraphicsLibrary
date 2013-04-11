#ifndef _SUPERGRAPHICSENGINE_H_
#define _SUPERGRAPHICSENGINE_H_

#include <Engine\GraphicsEngine.h>
#include <Engine\EventHandler.h>
#include <Engine\DrawableObject.h>
#include <Engine\CameraHandler.h>

class SuperGraphicsEngine : public GraphicsEngine{
	std::vector<DrawableObject*> _drawableObjects;

	CameraHandler *_cameraHandler;
public:
	SuperGraphicsEngine();
	virtual ~SuperGraphicsEngine();

	virtual void draw();
	virtual void init();

	void addDrawableObject(DrawableObject* object);
	
	CameraHandler *getCameraHandler();
	void setCameraHandler(CameraHandler *cameraHandler);


	virtual std::string toString()const{return "BasicGraphicsEngine";}

};

#endif