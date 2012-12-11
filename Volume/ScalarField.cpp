#include "ScalarField.h"

#include "VectorField.h"
#include <Math\Interpolator.h>
//
//ScalarField::ScalarField(glm::ivec3 dimmensions){
//	_dimmensions = dimmensions;
//	allocateData();
//	setZero();
//}
//
//ScalarField::ScalarField(unsigned int dimmensionsX,unsigned int dimmensionsY,unsigned int dimmensionsZ){
//	_dimmensions.x = dimmensionsX;
//	_dimmensions.y = dimmensionsY;
//	_dimmensions.z = dimmensionsZ;
//	allocateData();
//	setZero();
//}
//
//ScalarField::ScalarField(const ScalarField& ScalarField ){
//	_dimmensions.x = ScalarField._dimmensions.x;
//	_dimmensions.y = ScalarField._dimmensions.y;
//	_dimmensions.z = ScalarField._dimmensions.z;
//	allocateData();
//	copy(ScalarField);
//}
//
//
//ScalarField::~ScalarField(void)
//{
//	freeData();
//}
//
//
//void ScalarField::allocateData(){
//	_data = new double[_dimmensions.x * _dimmensions.y * _dimmensions.z];
//}
//
//void ScalarField::freeData(){
//	delete _data;
//}
//
//void ScalarField::setZero(){
//	for(int i = 0;i<_dimmensions.x * _dimmensions.y * _dimmensions.z;i++)
//		_data[i] = 0;
//}
//
//void ScalarField::copy(const ScalarField& ScalarField){
//	if(&ScalarField == this)
//		return;
//	if(_dimmensions != ScalarField._dimmensions){
//		freeData();
//		_dimmensions.x = ScalarField._dimmensions.x;
//		_dimmensions.y = ScalarField._dimmensions.y;
//		_dimmensions.z = ScalarField._dimmensions.z;
//		allocateData();
//	}
//	memcpy(_data,ScalarField._data,sizeof(glm::vec3)*_dimmensions.x * _dimmensions.y * _dimmensions.z);
//
//
//}
//
//void ScalarField::getData( double *outData ) const
//{
//	memcpy(outData,_data,sizeof(double)*_dimmensions.x * _dimmensions.y * _dimmensions.z);
//}
//
//ScalarField* ScalarField::CreateField( glm::ivec3 dimmensions,double(*funcPtr)(double,double,double),glm::vec3 minPoint,glm::vec3 maxPoint )
//{
//	ScalarField *f = new ScalarField(dimmensions);
//	double dx,dy,dz;
//	dx = 1.0 / ((dimmensions.x-1)/(maxPoint.x - minPoint.x));
//	dy = 1.0 / ((dimmensions.y-1)/(maxPoint.y - minPoint.y));
//	dz = 1.0 / ((dimmensions.z-1)/(maxPoint.z - minPoint.z));
//	FOR(dimmensions){
//		double a,b,c;
//		a = minPoint.x + dx*x;
//		b = minPoint.y + dy*y;
//		c = minPoint.z + dz*z;
//		f->_data[INDEX(f,x,y,z)] = funcPtr(a,b,c);
//	}
//
//	return f;
//}
//
//ScalarField* ScalarField::CreateField(unsigned int dimmensionX,unsigned int dimmensionY,unsigned int dimmensionZ,double(*funcPtr)(double,double,double),double minX,double minY,double minZ,double maxX,double maxY,double maxZ){
//	return CreateField(glm::ivec3(dimmensionX,dimmensionY,dimmensionZ),funcPtr,glm::vec3(minX,minY,minZ),glm::vec3(maxX,maxY,maxZ));
//}
//
//glm::ivec3 ScalarField::getDimensions()const
//{
//	return _dimmensions;
//}
//
//double ScalarField::getScalar( unsigned int x,unsigned int y,unsigned int z )
//{
//	return _data[INDEX(this,x,y,z)];
//}
//
//unsigned int ScalarField::getMemSize()
//{
//	return sizeof(double) * _dimmensions.x * _dimmensions.y * _dimmensions.z;
//}
//
//void ScalarField::setScalar( int x, int y, int z, double value )
//{
//	_data[INDEX(this,x,y,z)] = value;
//}
//
//glm::vec3 ScalarField::getGradient( int x, int y, int z )
//{
//	int i = 0;
//	glm::vec3 g;
//
//	if(x==_dimmensions.x-1){
//		g.x = getScalar(x,y,z)-getScalar(x-1,y,z);
//	}else if(x==0){
//		g.x = getScalar(x+1,y,z)-getScalar(x,y,z);
//	}else{
//		g.x = 0.5*(getScalar(x+1,y,z)-getScalar(x-1,y,z));
//
//	}
//
//	if(y==_dimmensions.y-1){
//		g.y = getScalar(x,y,z)-getScalar(x,y-1,z);
//	}else if(y==0){
//		g.y = getScalar(x,y+1,z)-getScalar(x,y,z);
//	}else{
//		g.y = 0.5*(getScalar(x,y+1,z)-getScalar(x,y-1,z));
//	}
//
//	if(z==_dimmensions.z-1){
//		g.z = getScalar(x,y,z)-getScalar(x,y,z-1);
//	}else if(y==0){
//		g.z = getScalar(x,y,z+1)-getScalar(x,y,z);
//	}else{
//		g.z = 0.5*(getScalar(x,y,z+1)-getScalar(x,y,z-1));
//	}
//
//	return g;
//}
//
//double ScalarField::getInterpolatedScalar( glm::vec3 pos )
//{
//	glm::vec3 t;
//	glm::ivec3 dPos;
//	dPos.x = (int)pos.x;
//	dPos.y = (int)pos.y;
//	dPos.z = (int)pos.z;
//	t.x = pos.x - dPos.x;
//	t.y = pos.y - dPos.y;
//	t.z = pos.z - dPos.z;
//	TriLinearInterpolator<double> interpolator ( _data[INDEX(this,(dPos.x),(dPos.y),(dPos.z))],
//		_data[INDEX(this,(dPos.x+1),(dPos.y),(dPos.z))],
//		_data[INDEX(this,(dPos.x),(dPos.y+1),(dPos.z))],
//		_data[INDEX(this,(dPos.x+1),(dPos.y+1),(dPos.z))],
//		_data[INDEX(this,(dPos.x),(dPos.y),(dPos.z+1))],
//		_data[INDEX(this,(dPos.x+1),(dPos.y),(dPos.z+1))],
//		_data[INDEX(this,(dPos.x),(dPos.y+1),(dPos.z+1))],
//		_data[INDEX(this,(dPos.x+1),(dPos.y+1),(dPos.z+1))]);
//
//	return interpolator.interpolate(t);
//}


ScalarField::ScalarField(glm::ivec3 dimensions,BoundingAABB boundingAABB):Field3D<float>(dimensions,boundingAABB){

}

glm::vec3 ScalarField::getGradient(glm::vec3 worldPos)const{
	return glm::vec3();
}


glm::vec3 ScalarField::getGradient(glm::ivec3 internalPos)const{
	return glm::vec3();
}

VectorField *ScalarField::getGradientField()const{
	return 0;

}