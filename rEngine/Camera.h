#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "rObject.h"

#include <tinyxml2\tinyxml2.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "rWindow.h"

#include <iostream>

class Camera : public rObject , public ResizeListener{
protected:
	glm::mat4 _projection,_camera;
	bool perspective;
	float fov,nearPlane,farPlane,right,left,top,bottom;
public:
	Camera(rWindow *owner);
	virtual ~Camera();

	virtual void update(float dt){};

	glm::mat4 getProjectionMatrix(){
		return _projection;
	}

	glm::mat4 getCameraMatrix(){
		return _camera;
	}

	virtual void onResize(glm::ivec2 newSize);

	static Camera *CreateCamera(rWindow *win,tinyxml2::XMLElement *ele);
};

class FPSCamera : public Camera{
	friend class Camera;
	bool alowFlying;
	glm::vec3 _pos,_dir;
public:
	FPSCamera(glm::vec3 pos,glm::vec3 dir,rWindow *owner):Camera(owner){
		_pos = pos;
		_dir = dir;
		_camera = glm::lookAt(pos,pos+dir,glm::vec3(0,1,0));
	}
	virtual void update(float dt){}
};

class TrackballCamera : public Camera , public MouseMotionListener , public MouseButtonListener{
	friend class Camera;
	glm::vec3 _direction,_focus;
	float _zoom;
	bool _state;

	
public:
	TrackballCamera(glm::vec3 direction,glm::vec3 focus,float zoom,rWindow *owner):
		Camera(owner),
		MouseMotionListener(owner),
		MouseButtonListener(owner),
		_direction(glm::normalize(direction)),
		_focus(focus),
		_zoom(zoom),
		_state(false)
	{
		auto eye = _focus-(_direction*_zoom);
		glm::vec3 up(0,1,0);
		if(std::abs(glm::dot(_direction,up))>0.9)
			up = glm::vec3(-1,0,0);
		_camera = glm::lookAt(eye,_focus,up);
	}
	virtual void update(float dt){}


	virtual void mouseClick(int button,int state){
		if(button == 0)
			_state = state;
	}

	virtual void mouseMotion(glm::ivec2 delta);
};


#endif