#include "Triangle.h"



Triangle::Triangle(Vertex *v0,Vertex *v1,Vertex *v2):
Face(glm::vec3(0,1,0)),_v0(v0),_v1(v1),_v2(v2)
{
calculateNormal();
}

Triangle::Triangle():
Face(glm::vec3(0,1,0)),_v0(0),_v1(0),_v2(0)
{

}

Triangle::~Triangle(){
	//Do nothing, it does not own the vertices
}

void Triangle::calculateNormal(){
	glm::vec3 e0,e1;
	e0 = glm::vec3(_v1->getPosition() - _v0->getPosition());
	e1 = glm::vec3(_v2->getPosition() - _v0->getPosition());
	_N = glm::normalize(glm::cross(e0,e1));
}

float Triangle::distance(const glm::vec3 position,bool signedDistance)const{
	float a,b,c,d,e,f,s,t,det;
	int region;
	glm::vec3 closestPoint,e0,e1,D;
	e0 = glm::vec3(_v1->getPosition() - _v0->getPosition());
	e1 = glm::vec3(_v2->getPosition() - _v0->getPosition());

	D = glm::vec3(_v0->getPosition()) - position;

	a = glm::dot(e0,e0);
	b = glm::dot(e0,e1);
	c = glm::dot(e1,e1);
	d = glm::dot(e0,D);
	e = glm::dot(e1,D);
	f = glm::dot(D,D);

	det = a*c - b*b;
	s = b*e - c*d;
	t = b*d - a*e;

	if(s+t <= det){
		if(s<0) region = t<0 ? 4 : 3;
		else if(t < 0) region = 5;
		else region = 0;
	}else{
		if(s<0) region = 2;
		else if (t<0) region = 6;
		else region = 1;
	}
	int sign = 1;
	if(signedDistance){
		sign = glm::dot(_N,D) >= 0 ? 1:-1;
	}

	if(region == 0){
		glm::vec3 tp = glm::vec3(_v0->getPosition()) + s*e0 + t*e1;
		return glm::distance(tp,position) * sign;
	}

	else if(region == 1){
		float numer = c+e-b-d;
		if(numer<=0)
			s = 0;
		else{
			float denom = a-2*b+c;
			s = (numer >= denom ? 1 : numer/denom);
		}
		t = 1-s;
		glm::vec3 tp = glm::vec3(_v0->getPosition()) + s*e0 + t*e1;
		return glm::distance(tp,position) * sign;

	}

	else if(region == 2){
		return glm::distance(glm::vec3(_v2->getPosition()),position) * sign;
	}

	else if(region == 3){
		t = (e >= 0 ? 0 : (-e >= c ? 1 : -e/c));
		glm::vec3 tp = glm::vec3(_v0->getPosition()) + t*e1;
		return glm::distance(tp,position) * sign;

	}


	else if(region == 4){
		return glm::distance(glm::vec3(_v0->getPosition()),position) * sign;
	}

	else if(region == 5){
		s = (d >= 0 ? 0 : (-d >= a ? 1 : -d/a));
		glm::vec3 tp = glm::vec3(_v0->getPosition()) + s*e0;
		return glm::distance(tp,position) * sign;
	}

	else if(region == 6){
		return glm::distance(glm::vec3(_v1->getPosition()),position) * sign;
	}

	float dist = a*s*s   + 2*b*s*t  + c*t*t   +  2*d*s    + 2*e*t   +   f;
	return dist;
}


float Triangle::distance(const Vertex *vertex,bool signedDistance)const{
	return distance(glm::vec3(vertex->getPosition()),signedDistance);
}


float Triangle::area()const{
	glm::vec3 e0,e1;
	e0 = glm::vec3(_v1->getPosition() - _v0->getPosition());
	e1 = glm::vec3(_v2->getPosition() - _v0->getPosition());
	return glm::length(glm::cross(e0,e1)) * 0.5f;
}

std::string Triangle::toString()const{
	std::stringstream ss;
	ss << "Triangle: " << std::endl;
	ss << "\t N: [" << _N.x << " "  << _N.y << " "  << _N.z << "]"<< std::endl;
	ss << "\tv0: " << _v0 << std::endl;
	ss << "\tv1: " << _v1 << std::endl;
	ss << "\tv2: " << _v2 << std::endl;
	return ss.str();
}