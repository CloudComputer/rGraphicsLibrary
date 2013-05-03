#include "VectorField.h"
#include <Math\Interpolator.h>

VectorField::VectorField(glm::ivec3 dimensions,BoundingAABB boundingAABB):
Field3D<glm::vec3>(dimensions,boundingAABB)
{
}


glm::vec3 VectorField::curl(const glm::vec3 &worldPos)const{
	glm::vec3 c;
	glm::ivec3 ip = (glm::ivec3)_boundingAABB.getDiscretePosition(worldPos,_dimensions);
	
	auto Fx = (_data[_index(ip+glm::ivec3(1,0,0))] - _data[_index(ip-glm::ivec3(1,0,0))])/(2.0f);
	auto Fy = (_data[_index(ip+glm::ivec3(0,1,0))] - _data[_index(ip-glm::ivec3(0,1,0))])/(2.0f);
	auto Fz = (_data[_index(ip+glm::ivec3(0,0,1))] - _data[_index(ip-glm::ivec3(0,0,1))])/(2.0f);
	
	/*c.x = Fz.y / (_delta.y) - Fy.z / (_delta.z); 
	c.y = Fx.z / (_delta.z) - Fz.x / (_delta.x); 
	c.z = Fz.x / (_delta.x) - Fx.y / (_delta.y); 
*/
	c.x = Fy.z - Fz.y; 
	c.y = Fz.x - Fx.z; 
	c.z = Fx.y - Fy.x; 

	return c;
}

VectorField *VectorField::getCurlField()const{
	VectorField *vf = new VectorField(_dimensions,_boundingAABB);

	FOR(_dimensions){
		glm::vec3 worldPos = _boundingAABB.getPosition(glm::vec3(x,y,z)/(glm::vec3)_dimensions);
		glm::vec3 g = curl(worldPos);
		vf->set(glm::ivec3(x,y,z),g);
	}
	return vf;
}