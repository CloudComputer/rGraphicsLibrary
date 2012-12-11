#include "EulerianFluid.h"

#include <boost/numeric/ublas/matrix_sparse.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/lu.hpp>


template<typename T> void swap(T *a,T *b){
	T *tmp = b;
	b = a;
	a = tmp;
}

void add0p5(float &x,glm::vec3 pos){
	x += 0.5;
}

EulerianFluid::EulerianFluid(LevelSet *levelSet):
_bounds(levelSet->getBoundingAABB())
{
	_vf = new VectorField(levelSet->getDimensions(),_bounds);
	_tmpVf = new VectorField(levelSet->getDimensions(),_bounds);
	_tmpVf2 = new VectorField(levelSet->getDimensions(),_bounds);
	_preasure = new ScalarField(levelSet->getDimensions(),_bounds);
	_density = new LevelSet(levelSet->getDimensions(),_bounds);
	memcpy(_density->getData(),levelSet->getData(),_density->getMemSize());
	_density->foreach(add0p5);
	_tmpDensity = new LevelSet(levelSet->getDimensions(),_bounds);
	
	mass = 1;
	viscosity = 1;
}

EulerianFluid::EulerianFluid(BoundingAABB bounds,glm::ivec3 dimmensions):_bounds(bounds)
{
	_vf = new VectorField(dimmensions);
	_tmpVf = new VectorField(dimmensions);
	_tmpVf2 = new VectorField(dimmensions);
	_preasure = new ScalarField(dimmensions);
	_density = new LevelSet(dimmensions);
	_tmpDensity = new LevelSet(dimmensions);
	mass = 1;
	viscosity = 1;
}


EulerianFluid::~EulerianFluid(void)
{
}


void EulerianFluid::addSource(BoundingVolume *b){
	_sources.push_back(b);
}

void EulerianFluid::init(){
	assert(_sources.size()!=0 && "must have sources of water before initialization");
	_vf->setZero();
	_preasure->setZero();
	_density->setZero();
	//glm::ivec3 dim = _vf->getDimensions();
	//std::vector<BoundingVolume *>::iterator fluids;
	//FOR(dim){
	//	glm::vec3 pos = _bounds.getPosition(glm::vec3(x/(float)dim.x,y/(float)dim.y,z/(float)dim.z));
	//	bool inside = false;
	//	for(fluids = _sources.begin();fluids!=_sources.end() && !inside;++fluids){
	//		inside = (*fluids)->inside(pos);
	//	}
	//	if(!inside || (x==0||y==0||z==0||x==dim.x-1||y==dim.y-1||z==dim.z-1)){
	//		_density->set(glm::ivec3(x,y,z),0.0);
	//	}else{
	//		_density->set(glm::ivec3(x,y,z),1.0);
	//	}
	//}
	currentT = 0;
}



ScalarField* EulerianFluid::getDensityField(){
	return _density;
}

void EulerianFluid::applyForce( VectorField * inVF, glm::vec3 force, double dt )
{
	assert(false && "Not yet implemented");
	//glm::ivec3 dim = inVF->getDimensions();
	//FOR(dim){
	//	glm::vec3 v = inVF->get(glm::ivec3(x,y,z));
	//	v += (force / mass) * (float) dt;
	//	inVF->set(glm::ivec3(x,y,z),v);
	//}
}


void EulerianFluid::update(double dt){
	assert(dt>0 && "Dt must be greater than zero");
	while(dt>=MAX_DT){
		update(MAX_DT);
		dt -= MAX_DT;
	}
//	selfAdvect(_vf,_tmpVf,dt);
//	swap(_vf,_tmpVf);
//	for(auto force = _externalForces.begin();force != _externalForces.end();++force){
//		applyForce(_vf,*force,dt);
//		swap(_vf,_tmpVf);
//	}
//	//_vf->viscousStress(_tmpVf,_viscousity,dt);
//	//splitDivergenceAndCurl(_vf,_tmpVf2);
//	enforceBoundryConditions(); //check where to do this
//
//	advect(_density,_tmpDensity,_vf,dt);
//	swap(_density,_tmpDensity);
}

void EulerianFluid::splitDivergenceAndCurl(VectorField *_divergenceFree, VectorField * _curlFree){
	assert(false && "Not yet implemented");
	//unsigned int size = _vf->getDimensions().x*_vf->getDimensions().y*_vf->getDimensions().z;
	//
	//boost::numeric::ublas::mapped_matrix<double> A(size,size,size*7);
	//boost::numeric::ublas::permutation_matrix<std::size_t> P(size);
	//boost::numeric::ublas::vector<double> x(size);
	//boost::numeric::ublas::vector<double> b(size);

	////Build A



	////Build b
	//boost::numeric::ublas::lu_factorize(A,P);
	//x = b;
	//lu_substitute(A, P, x);

}

void EulerianFluid::selfAdvect( VectorField * inVF, VectorField * outVF, double dt ,unsigned int steps ) {
	assert(false && "Not yet implemented");
	/*glm::ivec3 dim = inVF->getDimensions();
	float _dt = dt / steps;
	FOR(dim){
		glm::vec3 pos = _bounds.getPosition(glm::vec3(x/(float)dim.x,y/(float)dim.y,z/(float)dim.z));
		for(int i = 0;i<steps;i++){
			glm::vec3 dir = _vf->getFromWorldPos(pos);
			pos -= dir * _dt;
		}
		outVF->set(glm::ivec3(x,y,z),pos);
	}*/
}

void EulerianFluid::enforceBoundryConditions()
{
	assert(false && "Not yet implemented");
	/*glm::ivec3 dim = _vf->getDimensions();
	FOR(dim)if(classify(glm::ivec3(x,y,z)) == FluidCell){
		glm::vec3 v = _vf->get(glm::ivec3(x,y,z));
		if(v.x>0 && (classify( glm::ivec3(x,y,z)+glm::ivec3(1,0,0)) == SolidCell)){
			v.x = 0;
		}
		if(v.x<0 && (classify( glm::ivec3(x,y,z)-glm::ivec3(1,0,0)) == SolidCell)){
			v.x = 0;
		}	

		if(v.y>0 && (classify( glm::ivec3(x,y,z)+glm::ivec3(0,1,0)) == SolidCell)){
			v.y = 0;
		}
		if(v.y<0 && (classify( glm::ivec3(x,y,z)-glm::ivec3(0,1,0)) == SolidCell)){
			v.y = 0;
		}	

		if(v.z>0 && (classify( glm::ivec3(x,y,z)+glm::ivec3(0,0,1)) == SolidCell)){
			v.z = 0;
		}
		if(v.z<0 && (classify( glm::ivec3(x,y,z)-glm::ivec3(0,0,1)) == SolidCell)){
			v.z = 0;
		}	
		_vf->set(glm::ivec3(x,y,z),v);
	}*/
}

Classification EulerianFluid::classify( glm::ivec3 position )
{
	assert(false && "Not yet implemented");
	/*glm::ivec3 dim = _vf->getDimensions();
	if(position.x==0||position.y==0||position.z==0||position.x==dim.x-1||position.y==dim.y-1||position.z==dim.z-1)
		return SolidCell;
	if(_density->get(position)>=0.5)
		return FluidCell;*/
	return AirCell;
}

void EulerianFluid::advect( ScalarField * _inDensity, ScalarField * _outDensity, VectorField * _vf, double dt , unsigned int steps)
{
	assert(false && "Not yet implemented");
	//glm::ivec3 dim = _vf->getDimensions();

	//float _dt = dt / steps;
	//FOR(dim){
	//	glm::vec3 N = _inDensity->getGradient(glm::ivec3(x,y,z));
	//	N = glm::normalize(N);
	//	glm::vec3 pos = _bounds.getPosition(glm::vec3(x/(float)dim.x,y/(float)dim.y,z/(float)dim.z));
	//	for(int i = 0;i<steps;i++){
	//		glm::vec3 dir = _vf->getF(_bounds.getDiscretePosition(pos,dim));
	//		pos -= dir * _dt;
	//		glm::clamp(pos,0,1);
	//	}
	//	float t = _inDensity->getInterpolatedScalar(pos);

	//	_outDensity->setScalar(x,y,z,t);
	//}
}
