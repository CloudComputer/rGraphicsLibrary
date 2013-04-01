#include "CSG.h"


CSGCombine::CSGCombine(CSG *c0, CSG *c1,CSGOperation operation):
_c0(c0),
_c1(c1),
_operation(operation)
{

}

float CSGCombine::eval(glm::vec3 worldPos){
	float v0 = _c0->eval(worldPos);
	float v1 = _c1->eval(worldPos);
	switch(_operation){
	case Union:
		return std::min(v0,v1);
		break;
	case Difference:
		return std::max(v0,v1);
		break;
	case Intersection:
		return std::max(v0,-v1);
		break;
	}
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

float CSGSphere::eval(glm::vec3 worldPos)const{
	return glm::distance(_c,worldPos) - _r;
}


std::string CSGSphere::toString()const{
	std::stringstream ss;
	ss << "CSGSphere, radius: " << _r << " center: [" << _c.x << " " << _c.y << " " << _c.z << "]" << std::endl;
	return ss.str();
}
