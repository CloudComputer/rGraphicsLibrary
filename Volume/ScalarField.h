#ifndef __SCALARFIELD_H__
#define __SCALARFIELD_H__

#include <glm/glm.hpp>
#include <Volume/VectorField.h>
#include <Geometry/BoundingVolume.h>
#include <vector>

class ScalarField : public Field3D<float>{
public:
	ScalarField(glm::ivec3 dimensions = glm::ivec3(64,64,64),BoundingAABB boundingAABB = BoundingAABB(glm::vec3(0,0,0),glm::vec3(1,1,1)));

	glm::vec3 getGradient(glm::vec3 worldPos)const;
	glm::vec3 getUpWindGradient(glm::vec3 worldPos,glm::vec3 V)const;

	VectorField *getGradientField()const;

	float DiffXp(glm::vec3 worldPos)const;
	float DiffXm(glm::vec3 worldPos)const;
	float DiffXpm(glm::vec3 worldPos)const;
	float DiffYp(glm::vec3 worldPos)const;
	float DiffYm(glm::vec3 worldPos)const;
	float DiffYpm(glm::vec3 worldPos)const;
	float DiffZp(glm::vec3 worldPos)const;
	float DiffZm(glm::vec3 worldPos)const;
	float DiffZpm(glm::vec3 worldPos)const;

	float DiffXXpm(glm::vec3 worldPos)const;
	float DiffXYpm(glm::vec3 worldPos)const;
	float DiffXZpm(glm::vec3 worldPos)const;
	float DiffYYpm(glm::vec3 worldPos)const;
	float DiffYZpm(glm::vec3 worldPos)const;
	float DiffZZpm(glm::vec3 worldPos)const;

	std::vector<glm::vec3> getSurfacePoints()const;

	static ScalarField* ReadFromRawfile(const char *filename,unsigned int width,unsigned int height,unsigned int depth,unsigned int bps = 1);

};

#ifndef __SCALARFIELD_HPP__
#include "ScalarField.hpp"
#endif

#endif