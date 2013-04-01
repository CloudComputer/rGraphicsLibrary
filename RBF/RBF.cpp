#include "RBF.h"

float RBFSystem::meanSqError(const std::vector<glm::vec4> &points){
	float d = 0,dx;
	for(auto p = points.begin();p!=points.end();p++){
		dx =  p->w - eval(glm::vec3(*p));
		d += dx*dx;
	}
	return d/points.size();
}

float RBFSystem::eval(glm::vec3 worldPos){
	glm::vec3 c = glm::vec3(worldPos - _min.x) / (_max.x - _min.x);

	float v = _trend.eval(c.x,c.y,c.z);
	for(auto k = _kernels.begin();k!=_kernels.end();++k){
		v += (*k)->eval(c.x,c.y,c.z);
	}
	return v;
}