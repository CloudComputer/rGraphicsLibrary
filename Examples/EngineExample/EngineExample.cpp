#include <Engine\DrawableObject.h>

#include <Engine\GLFWEngine.h>

#include <iostream>

#include <OpenGLHelpers\ColorCube.h>

class TestObject : public DrawableObject{
	ColorCube cube;
public:
	TestObject():cube(BoundingAABB(glm::vec3(-1,-1,-1),glm::vec3(1,1,1))){}
	virtual void draw(){cube.draw();}
	virtual std::string toString()const{return "TestObject";}
};



void init(Engine *t){
	static_cast<BasicGraphicsEngine*>(GLFWEngine::getEngine()->getGraphicEngine())->addDrawableObject(new TestObject());
}

ENGINE_MAIN(GLFWEngine::CreateEngine(),init)