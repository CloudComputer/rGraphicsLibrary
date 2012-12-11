#pragma once
#define MAX_DT 10

#include <glm/glm.hpp>

#include <vector>

#include <Volume/LevelSet.h>

enum Classification{
	FluidCell,
	AirCell,
	SolidCell
};

class EulerianFluid
{
	float currentT,mass,viscosity;
	VectorField *_vf,*_tmpVf,*_tmpVf2;
	ScalarField *_preasure;
	LevelSet *_density,*_tmpDensity;
	std::vector<BoundingVolume *> _sources;
	std::vector<glm::vec3> _externalForces;
	BoundingAABB _bounds;
public:
	EulerianFluid(LevelSet *levelSet);
	EulerianFluid(BoundingAABB bounds,glm::ivec3 dimmensions);
	~EulerianFluid(void);

	void addSource(BoundingVolume *b);

	void init();

	ScalarField *getDensityField();

	void splitDivergenceAndCurl(VectorField *_divergenceFree, VectorField * _curlFree);

	void addForce(glm::vec3 force){_externalForces.push_back(force);}
	void update(double dt);
	void selfAdvect( VectorField * _vf, VectorField * _tmpVf, double dt ,unsigned int steps = 6 ) ;
	void applyForce( VectorField * inVF, glm::vec3 force, double dt );
	void enforceBoundryConditions();
	Classification classify( glm::ivec3 position );
	void advect( ScalarField * _density, ScalarField * _tmpDensity, VectorField * _vf, double dt , unsigned int steps = 6 );
};


typedef EulerianFluid Fluid;