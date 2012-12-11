#ifndef __VECTORFIELD_H__
#define __VECTORFIELD_H__

#include <glm/glm.hpp>
#include <Volume/Field3D.h>
#include <Geometry/BoundingVolume.h>



class VectorField : public Field3D<glm::vec3>{
public:
	VectorField(glm::ivec3 dimensions = glm::ivec3(64,64,64),BoundingAABB boundingAABB = BoundingAABB(glm::vec3(0,0,0),glm::vec3(1,1,1)));
};

#ifndef __VECTORFIELD_HPP__
#include "VectorField.hpp"
#endif

#endif