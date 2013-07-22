#include "CSG.h"

#include <Util\Macros.h>


CSGCombine::CSGCombine(CSGOperation operation):
	_operation(operation){}

CSGCombine::CSGCombine(CSG *c0, CSG *c1,CSGOperation operation):
_operation(operation)
{
	_subs.push_back(c0);
	_subs.push_back(c1);
}



float CSGCombine::eval(const glm::vec3 &worldPos){
	if(_subs.empty()) return 0;
	float v = _subs[0]->eval(worldPos);

	IT_FOR(_subs,c){
		float vc = (*c)->eval(worldPos);
		switch(_operation){
		case Union:
			v =  std::min(v,vc);
			break;
		case Difference:
			v =  std::max(v,vc);
			break;
		case Intersection:
			v =  std::max(v,-vc);
			break;
		}
	}
	return v;
}

void CSGCombine::addCSG(CSG *c){
	_subs.push_back(c);
}

std::string CSGCombine::toString()const{
	switch(_operation){
	case Union:
		return "CSGCombine Union";
		break;
	case Difference:
		return "CSGCombine Difference";
		break;
	case Intersection:
		return "CSGCombine Intersection";
		break;
	}
	return "";
}


CSGSphere::CSGSphere(float r, glm::vec3 c):
_r(r),
_c(c)
{
	
}

float CSGSphere::eval(const glm::vec3 &worldPos){
	return glm::distance(_c,worldPos) - _r;
}


std::string CSGSphere::toString()const{
	std::stringstream ss;
	ss << "CSGSphere, radius: " << _r << " center: [" << _c.x << " " << _c.y << " " << _c.z << "]" << std::endl;
	return ss.str();
}
