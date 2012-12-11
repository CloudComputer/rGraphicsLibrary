#include "LevelSet.h"

LevelSet::LevelSet(Mesh *m,glm::ivec3 dimensions):
	ScalarField(dimensions,m->getBoundingAABB())
{
	FOR(_dimensions){
		glm::vec3 worldPos = _getWorldPos(glm::ivec3(x,y,z));
		float d = m->distance(worldPos,true);
		set(glm::ivec3(x,y,z),d);
	}
}

LevelSet::LevelSet(glm::ivec3 dimensions,BoundingAABB boundingAABB):
	ScalarField(dimensions,boundingAABB)
{
}