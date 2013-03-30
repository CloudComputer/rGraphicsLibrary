#include <Engine\DrawableObject.h>

#include <Engine\GLFWEngine.h>

#include <iostream>

#include <Util\Random.h>
#include <Math\Plane.h>

#include <glm\gtc\type_ptr.hpp>

class TestObject : public DrawableObject{
	std::vector<glm::vec3> points;
	Plane p;
	glm::vec3 p0,p1,p2,p3;

public:
	TestObject(){
		for(int i = 0;i<10;i++){
			glm::vec3 p;
			p.x = Random::getRandomGenerator()->getFloat(0,1);
			p.z = Random::getRandomGenerator()->getFloat(0,1);
			p.y = (p.x + p.z) + Random::getRandomGenerator()->getFloat(-0.01,0.01);
			points.push_back(p);
		}
		this->p = Plane(points);
		p0 = this->p.getPoint(-100,-100);
		p1 = this->p.getPoint(100,-100);
		p2 = this->p.getPoint(100,100);
		p3 = this->p.getPoint(-100,100);
		
		std::cout << this->p.getDistanceToPlane(p0) << std::endl;
		std::cout << this->p.getDistanceToPlane(p1) << std::endl;
		std::cout << this->p.getDistanceToPlane(p2) << std::endl;
		std::cout << this->p.getDistanceToPlane(p3) << std::endl;

	}
	virtual void draw(){
		glPointSize(3);
		glBegin(GL_POINTS);
		glColor4f(1,0,0,1);
		for(auto p = points.begin();p!=points.end();++p){
			glVertex3fv(glm::value_ptr(*p));
		}
		glEnd();
		
		glm::vec3 n = this->p.getNormal();

		glBegin(GL_LINES);
		glColor4f(0,1,0,1);
		glVertex3fv(glm::value_ptr(p.getPoint(0,0)));
		glColor4f(1,1,0,1);
		glVertex3fv(glm::value_ptr(n+p.getPoint(0,0)));
		glEnd();


		glColor4f(0,0,1,0.5);
		glBegin(GL_QUADS);
		glVertex3fv(glm::value_ptr(p0));
		glVertex3fv(glm::value_ptr(p1));
		glVertex3fv(glm::value_ptr(p2));
		glVertex3fv(glm::value_ptr(p3));

		glEnd();
		

	}
	virtual std::string toString()const{return "TestObject";}
};



void init(Engine *t){
	static_cast<BasicGraphicsEngine*>(GLFWEngine::getEngine()->getGraphicEngine())->addDrawableObject(new TestObject());

	
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

ENGINE_MAIN(GLFWEngine::CreateEngine(),init)