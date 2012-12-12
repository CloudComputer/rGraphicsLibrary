#include "ScalarField.h"

#include "VectorField.h"
#include <Math\Interpolator.h>

ScalarField::ScalarField(glm::ivec3 dimensions,BoundingAABB boundingAABB):Field3D<float>(dimensions,boundingAABB){

}

glm::vec3 ScalarField::getGradient(glm::vec3 worldPos)const{
	glm::vec3 g;
	g.x = DiffXpm(worldPos);
	g.y = DiffYpm(worldPos);
	g.z = DiffZpm(worldPos);
	return g;
}

glm::vec3 ScalarField::getUpWindGradient(glm::vec3 worldPos,glm::vec3 V)const{
	glm::vec3 g;
	if(V.x > 0)
		g.x = DiffXp(worldPos);
	else
		g.x = DiffXm(worldPos);
	if(V.y > 0)
		g.y = DiffYp(worldPos);
	else
		g.y = DiffYm(worldPos);
	if(V.z > 0)
		g.z = DiffZp(worldPos);
	else
		g.z = DiffZm(worldPos);
	return g;
}


VectorField* ScalarField::getGradientField()const{
	VectorField *vf = new VectorField(_dimensions,_boundingAABB);
	FOR(_dimensions){
		glm::vec3 worldPos = _boundingAABB.getPosition(glm::vec3(x,y,z)/(glm::vec3)_dimensions);
		glm::vec3 g = getGradient(worldPos);
		vf->set(glm::ivec3(x,y,z),g);
	}
	return vf;
}



float ScalarField::DiffXp(glm::vec3 worldPos)const{
	glm::ivec3 ip = (glm::ivec3)_boundingAABB.getDiscretePosition(worldPos,_dimensions);
	return (_data[_index(ip+glm::ivec3(1,0,0))] - _data[_index(ip)])/_delta.x;
}

float ScalarField::DiffXm(glm::vec3 worldPos)const{
	glm::ivec3 ip = (glm::ivec3)_boundingAABB.getDiscretePosition(worldPos,_dimensions);
	return (_data[_index(ip)] - _data[_index(ip-glm::ivec3(1,0,0))])/_delta.x;
}
float ScalarField::DiffXpm(glm::vec3 worldPos)const{
	glm::ivec3 ip = (glm::ivec3)_boundingAABB.getDiscretePosition(worldPos,_dimensions);
	return (_data[_index(ip+glm::ivec3(1,0,0))] - _data[_index(ip-glm::ivec3(1,0,0))])/(2*_delta.x);
}

float ScalarField::DiffYp(glm::vec3 worldPos)const{
	glm::ivec3 ip = (glm::ivec3)_boundingAABB.getDiscretePosition(worldPos,_dimensions);
	return (_data[_index(ip+glm::ivec3(0,1,0))] - _data[_index(ip)])/_delta.y;
}
float ScalarField::DiffYm(glm::vec3 worldPos)const{
	glm::ivec3 ip = (glm::ivec3)_boundingAABB.getDiscretePosition(worldPos,_dimensions);
	return (_data[_index(ip)] - _data[_index(ip-glm::ivec3(0,1,0))])/_delta.y;
}
float ScalarField::DiffYpm(glm::vec3 worldPos)const{
	glm::ivec3 ip = (glm::ivec3)_boundingAABB.getDiscretePosition(worldPos,_dimensions);
	return (_data[_index(ip+glm::ivec3(0,1,0))] - _data[_index(ip-glm::ivec3(0,1,0))])/(2*_delta.y);
}

float ScalarField::DiffZp(glm::vec3 worldPos)const{
	glm::ivec3 ip = (glm::ivec3)_boundingAABB.getDiscretePosition(worldPos,_dimensions);
	return (_data[_index(ip+glm::ivec3(0,0,1))] - _data[_index(ip)])/_delta.z;
}
float ScalarField::DiffZm(glm::vec3 worldPos)const{
	glm::ivec3 ip = (glm::ivec3)_boundingAABB.getDiscretePosition(worldPos,_dimensions);
	return (_data[_index(ip)] - _data[_index(ip-glm::ivec3(0,0,1))])/_delta.z;
}
float ScalarField::DiffZpm(glm::vec3 worldPos)const{
	glm::ivec3 ip = (glm::ivec3)_boundingAABB.getDiscretePosition(worldPos,_dimensions);
	return (_data[_index(ip+glm::ivec3(0,0,1))] - _data[_index(ip-glm::ivec3(0,0,1))])/(2*_delta.z);
}

float ScalarField::DiffXXpm(glm::vec3 worldPos)const{
	assert(false && "Not yet implemented");
	return 0;
}

float ScalarField::DiffXYpm(glm::vec3 worldPos)const{
	assert(false && "Not yet implemented");
	return 0;
}
float ScalarField::DiffXZpm(glm::vec3 worldPos)const{
	assert(false && "Not yet implemented");
	return 0;
}
float ScalarField::DiffYYpm(glm::vec3 worldPos)const{
	assert(false && "Not yet implemented");
	return 0;
}
float ScalarField::DiffYZpm(glm::vec3 worldPos)const{
	assert(false && "Not yet implemented");
	return 0;
}
float ScalarField::DiffZZpm(glm::vec3 worldPos)const{
	assert(false && "Not yet implemented");
	return 0;
}