#ifndef _TRACKBALLCAMERAHANDLER_H_
#define _TRACKBALLCAMERAHANDLER_H_

#include "EventHandler.h"
#include "CameraHandler.h"

#include "glm\glm.hpp"

class TrackballCameraHandler : public CameraHandler, public WindowEventListener , public MouseEventListener, public KeyboardEventListener{
	float _fov;
	glm::mat4 _projectionMatrix;
	glm::mat4 _viewMatrix;
	float _near;
	float _far;
	bool _mouseState;
	int _mouseButton;
	float _distance;
	float _rotSpeed;
public:
	TrackballCameraHandler();
	virtual ~TrackballCameraHandler();

	virtual void windowResized(int w,int h);
	virtual void buttonPressed(int button,int state);

	
	virtual void mouseButtonClicked(int button,int state,int x,int y);
	virtual void mouseMotion(int x,int y,int dx,int dy);
	virtual void mouseWheel(int i,int di);

	virtual void setCamera()const;

	virtual std::string toString()const{return "TrackballCameraHandler";}

};

#endif