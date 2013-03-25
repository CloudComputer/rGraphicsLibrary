#include "RBF.h"


float RBFSystem::eval(glm::vec3 worldPos)const{
	glm::vec3 c = glm::vec3(worldPos - _min.x) / (_max.x - _min.x);

	float v = _trend.eval(c.x,c.y,c.z);
	for(auto k = _kernels.begin();k!=_kernels.end();++k){
		v += (*k)->eval(c.x,c.y,c.z);
	}
	return v;
}