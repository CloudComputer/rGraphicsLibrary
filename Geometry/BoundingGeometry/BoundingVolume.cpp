#include "BoundingVolume.h"

#include <Math/Interpolator.h>

#include <cmath>


BoundingVolume::BoundingVolume(void)
{	
}


BoundingVolume::~BoundingVolume(void)
{
}


glm::vec3 BoundingAABB::getPosition( glm::vec3 t )const
{
	return _minPos + (_maxPos-_minPos) * t;
}

glm::vec3 BoundingAABB::getInternalPosition(const glm::vec3 &worldPos)const
{
	glm::vec3 pos;
	pos.x = (worldPos.x - _minPos.x) / (_maxPos.x - _minPos.x);
	pos.y = (worldPos.y - _minPos.y) / (_maxPos.y - _minPos.y);
	pos.z = (worldPos.z - _minPos.z) / (_maxPos.z - _minPos.z);
	return pos;
}

double round(double a){
	return std::floor(a+0.5);
}

glm::vec3 BoundingAABB::getDiscretePosition( glm::vec3 worldPos, glm::ivec3 dim )const
{
	//TODO check if the -1 one is correct
	glm::vec3 oPos;
	oPos.x = round((dim.x-1) * (worldPos.x - _minPos.x) / (_maxPos.x - _minPos.x));
	oPos.y = round((dim.y-1) * (worldPos.y - _minPos.y) / (_maxPos.y - _minPos.y));
	oPos.z = round((dim.z-1) * (worldPos.z - _minPos.z) / (_maxPos.z - _minPos.z));
	return oPos;
}
