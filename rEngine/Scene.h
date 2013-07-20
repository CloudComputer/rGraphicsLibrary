#ifndef _SCENE_H_
#define _SCENE_H_

#include "IDObject.h"

#include <vector>
#include <tinyxml2\tinyxml2.h>

class rObject;
class FBO;
class Camera;
class Light;
class ShaderProgram;

class Scene : public IDObject{
	bool _active;
	Camera *_cam;
	
	FBO *_fbo;
	FBO *_lightFBO;
	FBO *_combineFBO;
	ShaderProgram *_LightsShader;
	ShaderProgram *_CombieShader;

	std::vector<rObject*> _objects;
	std::vector<Light*> _lights;
	void createLight(tinyxml2::XMLElement *light);
	
	void PassScene();
	void PassLight();
	void PassCombine();

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