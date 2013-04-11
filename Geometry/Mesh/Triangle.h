#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

#include "Vertex.h"
#include "Face.h"

class Triangle : public Face{
	Vertex *_v0,*_v1,*_v2;
public:

	Triangle(Vertex *v0,Vertex *v1,Vertex *v2);
	Triangle();
	virtual ~Triangle();

	void calculateNormal();

	float distance(const glm::vec3 position,bool signedDistance = false)const;
	float distance(const Vertex *vertex,bool signedDistance = false)const;
	
	Vertex *v0(){return _v0;}
	Vertex *v1(){return _v1;}
	Vertex *v2(){return _v2;}

	float area()const;

	virtual std::string toString()const;

//#ifdef GL_VERSION_1_1
//	virtual void gl(){
//		_v0->gl();
//		_v1->gl();
//		_v2->gl();
//	}
//#endif
};


#endif
