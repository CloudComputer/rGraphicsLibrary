#include "Face.h"


Face::Face(glm::vec3 normal):_N(normal){

}

Face::~Face(){

}


glm::vec3 &Face::getNormal(){
	return _N;
}

glm::vec3 Face::getNormal()const{
	return _N;
}

void Face::setNormal(glm::vec3 normal){
	_N = normal;
}
