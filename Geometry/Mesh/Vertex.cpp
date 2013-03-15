#include "Vertex.h"


Vertex::Vertex(const Vertex &v):
	_P(v._P),
	_N(v._N)
{
	
}

Vertex::Vertex(glm::vec4 position,glm::vec3 normal):
	_P(position),
	_N(normal)
{
}

Vertex::Vertex(glm::vec3 position,glm::vec3 normal):_P(glm::vec4(position,1)),_N(normal){
	
}
	
glm::vec4 &Vertex::getPosition(){
	return _P;
}

glm::vec3 &Vertex::getNormal(){
	return _N;
}

glm::vec4 Vertex::getPosition()const{
	return _P;
}

glm::vec3 Vertex::getNormal()const{
	return _N;
}


void Vertex::setPosition(glm::vec4 position){
	_P = position;
}

void Vertex::setNormal(glm::vec3 normal){
	_N = normal;
}

std::string Vertex::toString()const{
	std::stringstream ss;
	ss << "Vertex, pos:[ "<< _P.x << " " << _P.y << " "<< _P.z << " "<< _P.w << "], normal: [" << _N.x << " " << _N.y << " "<< _N.z << "]";
	return ss.str();
}