#ifndef __LEVELSET_H__
#define __LEVELSET_H__


#include <glm/glm.hpp>
#include <Volume/ScalarField.h>
#include <Geometry/Mesh/Mesh.h>

class LevelSet : public ScalarField{
public:
	LevelSet(Mesh *m,glm::ivec3 dimensions = glm::ivec3(64,64,64));
	LevelSet(glm::ivec3 dimensions = glm::ivec3(64,64,64),BoundingAABB boundingAABB = BoundingAABB(glm::vec3(0,0,0),glm::vec3(1,1,1)));

	

	void erode(float c = 0.1);
	void dilate(float c = -0.1);
	void advect(VectorField *f,float dt = 0.001);

	void recalculate();

	void dist(float &x,glm::vec3 pos);
};

#ifndef __LEVELSET_HPP__
#include "LevelSet.hpp"
#endif

#endif