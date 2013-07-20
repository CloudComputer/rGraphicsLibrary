#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "rObject.h"

#include <tinyxml2\tinyxml2.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "rWindow.h"

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

#endif