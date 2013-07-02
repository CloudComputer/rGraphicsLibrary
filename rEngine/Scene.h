#ifndef _SCENE_H_
#define _SCENE_H_

#include "IDObject.h"

#include <vector>
#include <tinyxml2\tinyxml2.h>

class rObject;
class FBO;
class Camera;

class Scene : public IDObject{
	bool _active;
	Camera *_cam;

	FBO *_fbo;

	std::vector<rObject*> _objects;

public:
	Scene();
	virtual ~Scene();

	Camera *getCamera(){return _cam;}

	void setActive(bool active = true);
	bool isActive();

	void update(float dt);
	void render();

	static Scene* CreateScene(tinyxml2::XMLElement *ele);
};

#endif