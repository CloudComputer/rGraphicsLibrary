#ifndef _ROBJECT_H_
#define _ROBJECT_H_

#include "IDObject.h"

#include <OpenGLHelpers\OpenGLInfo.h>

#include "glm\glm.hpp"
#include <tinyxml2\tinyxml2.h>

class Scene;
class ShaderProgram;

class rObject : public IDObject{
protected:
	glm::mat4 _model;
	ShaderProgram *_shader;
protected:
	virtual void onDraw(){}
	rObject();
public:
	virtual ~rObject();

	virtual void update(float dt){}
	void draw(Scene *s);

	virtual void postDraw(Scene *s){}


	static rObject* CreateObject(tinyxml2::XMLElement *ele);
	//virtual void CreateObject(rObject *obj,tinyxml2::XMLElement *ele) = 0;

};

#endif