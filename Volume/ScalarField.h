#ifndef __SCALARFIELD_H__
#define __SCALARFIELD_H__

//#include "C:\Program Files (x86)\Visual Leak Detector\include\vld.h"

#include <Geometry/KDTree/KDTree.h>
#include <Geometry/Mesh/Vertex.h>

#include <glm/glm.hpp>
#include <Volume/VectorField.h>
#include <Geometry/BoundingGeometry/BoundingVolume.h>
#include <vector>

class ScalarField : public Field3D<float>{
public:
	ScalarField(glm::ivec3 dimensions = glm::ivec3(64,64,64),BoundingAABB boundingAABB = BoundingAABB(glm::vec3(0,0,0),glm::vec3(1,1,1)));

	glm::vec3 getGetGradientCurl(const glm::ivec3 &pos)const;
	glm::vec3 getGradient(const glm::vec3 &worldPos)const;
	glm::vec3 getGradient(const glm::ivec3 &pos)const;
	glm::vec3 getUpWindGradient(const glm::vec3 &worldPos,const glm::vec3 &V)const;

	VectorField *getGradientField()const;

	ScalarField *blur()const;

	

	float DiffXp(const glm::vec3 &worldPos)const;
	float DiffXm(const glm::vec3 &worldPos)const;
	float DiffXpm(const glm::vec3 &worldPos)const;
	float DiffYp(const glm::vec3 &worldPos)const;
	float DiffYm(const glm::vec3 &worldPos)const;
	float DiffYpm(const glm::vec3 &worldPos)const;
	float DiffZp(const glm::vec3 &worldPos)const;
	float DiffZm(const glm::vec3 &worldPos)const;
	float DiffZpm(const glm::vec3 &worldPos)const;

	float DiffXXpm(const glm::vec3 &worldPos)const;
	float DiffXYpm(const glm::vec3 &worldPos)const;
	float DiffXZpm(const glm::vec3 &worldPos)const;
	float DiffYYpm(const glm::vec3 &worldPos)const;
	float DiffYZpm(const glm::vec3 &worldPos)const;
	float DiffZZpm(const glm::vec3 &worldPos)const;

	float DiffXp(glm::ivec3 pos)const;
	float DiffXm(glm::ivec3 pos)const;
	float DiffXpm(glm::ivec3 pos)const;
	float DiffYp(glm::ivec3 pos)const;
	float DiffYm(glm::ivec3 pos)const;
	float DiffYpm(glm::ivec3 pos)const;
	float DiffZp(glm::ivec3 pos)const;
	float DiffZm(glm::ivec3 pos)const;
	float DiffZpm(glm::ivec3 pos)const;

	float DiffXXpm(glm::ivec3 pos)const;
	float DiffXYpm(glm::ivec3 pos)const;
	float DiffXZpm(glm::ivec3 pos)const;
	float DiffYYpm(glm::ivec3 pos)const;
	float DiffYZpm(glm::ivec3 pos)const;
	float DiffZZpm(glm::ivec3 pos)const;

	KDTree<Vertex,3,float> *getSurfacePoints(float threshold)const;

	void saveAsRaw(const char *filename);

	static ScalarField* ReadFromRawfile(const char *filename,unsigned int width,unsigned int height,unsigned int depth,unsigned int bps = 1);

};

#ifndef __SCALARFIELD_HPP__
#include "ScalarField.hpp"
#endif

#endif