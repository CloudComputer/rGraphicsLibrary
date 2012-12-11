#ifndef __SCALARFIELD_H__
#define __SCALARFIELD_H__

#include <glm/glm.hpp>
#include <Volume/VectorField.h>
#include <Geometry/BoundingVolume.h>


//#define FOR(dim) for(int z = 0;z<dim.z;z++)for(int y = 0;y<dim.y;y++)for(int x = 0;x<dim.x;x++)
//#define INDEX(vf,x,y,z) x+vf->getDimensions().x*(y+vf->getDimensions().y*z)

//class ScalarField
//{
//protected:
//	glm::ivec3 _dimmensions;
//	double *_data;
//
//
//	void allocateData();
//	void freeData();
//public:
//	ScalarField(glm::ivec3 dimmensions);
//	ScalarField(unsigned int dimmensionsX,unsigned int dimmensionsY,unsigned int dimmensionsZ);
//	ScalarField(const ScalarField& vectorField );
//
//	~ScalarField(void);
//
//	void setZero();
//	void copy(const ScalarField& vectorField);
//
//	void selfAdvect(const ScalarField& vectorField);
//
//	void getData(double *outData)const;
//
//	static ScalarField* CreateField(glm::ivec3 dimmensions,double(*funcPtr)(double,double,double),glm::vec3 minPoint = glm::vec3(1,1,1),glm::vec3 maxPoint = glm::vec3(0,0,0));
//	static ScalarField* CreateField(unsigned int dimmensionX,unsigned int dimmensionY,unsigned int dimmensionZ,double(*funcPtr)(double,double,double),double minX = 1,double minY = 1,double minZ = 1,double maxX = 0,double maxY = 0,double maxZ = 0);
//
//
//	glm::ivec3 getDimensions()const;
//	double getScalar( unsigned int x,unsigned int y,unsigned int z );
//	unsigned int getMemSize();
//	void setScalar( int x, int y, int z, double value );
//	glm::vec3 getGradient( int x, int y, int z );
//	double getInterpolatedScalar( glm::vec3 pos );
//};

class ScalarField : public Field3D<float>{
public:
	ScalarField(glm::ivec3 dimensions = glm::ivec3(64,64,64),BoundingAABB boundingAABB = BoundingAABB(glm::vec3(0,0,0),glm::vec3(1,1,1)));

	glm::vec3 getGradient(glm::vec3 worldPos)const;
	glm::vec3 getGradient(glm::ivec3 internalPos)const;

	VectorField *getGradientField()const;

};

#ifndef __SCALARFIELD_HPP__
#include "Field3D.hpp"
#endif

#endif