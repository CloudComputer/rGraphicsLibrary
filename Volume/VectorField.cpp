#include "VectorField.h"
#include <Math\Interpolator.h>

//
//
//VectorField::VectorField(glm::ivec3 dimmensions){
//	_dimmensions = dimmensions;
//	allocateData();
//	setZero();
//}
//
//VectorField::VectorField(unsigned int dimmensionsX,unsigned int dimmensionsY,unsigned int dimmensionsZ){
//	_dimmensions.x = dimmensionsX;
//	_dimmensions.y = dimmensionsY;
//	_dimmensions.z = dimmensionsZ;
//	allocateData();
//	setZero();
//}
//
//VectorField::VectorField(const VectorField& vectorField ){
//	_dimmensions.x = vectorField._dimmensions.x;
//	_dimmensions.y = vectorField._dimmensions.y;
//	_dimmensions.z = vectorField._dimmensions.z;
//	allocateData();
//	copy(vectorField);
//}
//
//
//VectorField::~VectorField(void)
//{
//	freeData();
//}
//
//
//void VectorField::allocateData(){
//	_data = new glm::vec3[_dimmensions.x * _dimmensions.y * _dimmensions.z];
//}
//
//void VectorField::freeData(){
//	delete _data;
//}
//
//void VectorField::setZero(){
//	for(int i = 0;i<_dimmensions.x * _dimmensions.y * _dimmensions.z;i++)
//		_data[i] = glm::vec3(0,0,0);
//}
//
//void VectorField::copy(const VectorField& vectorField){
//	if(&vectorField == this)
//		return;
//	if(_dimmensions != vectorField._dimmensions){
//		freeData();
//		_dimmensions.x = vectorField._dimmensions.x;
//		_dimmensions.y = vectorField._dimmensions.y;
//		_dimmensions.z = vectorField._dimmensions.z;
//		allocateData();
//	}
//	memcpy(_data,vectorField._data,sizeof(glm::vec3)*_dimmensions.x * _dimmensions.y * _dimmensions.z);
//	
//
//}
//
//void VectorField::getData( glm::vec3 *outData ) const
//{
//	memcpy(outData,_data,sizeof(glm::vec3)*_dimmensions.x * _dimmensions.y * _dimmensions.z);
//}
//
//
//void VectorField::getData( float *outData ) const
//{
//	memcpy(outData,_data,sizeof(glm::vec3)*_dimmensions.x * _dimmensions.y * _dimmensions.z);
//}
//
//VectorField* VectorField::CreateField( glm::ivec3 dimmensions,glm::vec3(*funcPtr)(double,double,double),glm::vec3 minPoint,glm::vec3 maxPoint )
//{
//	VectorField *f = new VectorField(dimmensions);
//	float dx,dy,dz;
//	dx = 1.0 / (maxPoint.x - minPoint.x);
//	dy = 1.0 / (maxPoint.y - minPoint.y);
//	dz = 1.0 / (maxPoint.z - minPoint.z);
//	FOR(dimmensions){
//		float a,b,c;
//		a = minPoint.x + dx*x;
//		b = minPoint.y + dy*y;
//		c = minPoint.z + dz*z;
//		f->_data[INDEX(f,x,y,z)] = funcPtr(a,b,c);
//	}
//
//	return f;
//}
//
//VectorField* VectorField::CreateField(unsigned int dimmensionX,unsigned int dimmensionY,unsigned int dimmensionZ,glm::vec3(*funcPtr)(double,double,double),double minX,double minY,double minZ,double maxX,double maxY,double maxZ){
//	return CreateField(glm::ivec3(dimmensionX,dimmensionY,dimmensionZ),funcPtr,glm::vec3(minX,minY,minZ),glm::vec3(maxX,maxY,maxZ));
//}
//
//glm::ivec3 VectorField::getDimensions()const
//{
//	return _dimmensions;
//}
//
//glm::vec3 VectorField::getVector( unsigned int x,unsigned int y,unsigned int z )
//{
//	return _data[INDEX(this,x,y,z)];
//}
//
//void VectorField::setVector( int x, int y, int z, glm::vec3 pos )
//{
//	_data[INDEX(this,x,y,z)] = pos;
//}
//
//glm::vec3 VectorField::getInterpolatedVector( glm::vec3 pos )
//{
//	glm::vec3 t;
//	glm::ivec3 dPos;
//	dPos.x = (int)pos.x;
//	dPos.y = (int)pos.y;
//	dPos.z = (int)pos.z;
//	t.x = pos.x - dPos.x;
//	t.y = pos.y - dPos.y;
//	t.z = pos.z - dPos.z;
//	TriLinearInterpolator<glm::vec3> interpolator ( _data[INDEX(this,(dPos.x),(dPos.y),(dPos.z))],
//													_data[INDEX(this,(dPos.x+1),(dPos.y),(dPos.z))],
//													_data[INDEX(this,(dPos.x),(dPos.y+1),(dPos.z))],
//													_data[INDEX(this,(dPos.x+1),(dPos.y+1),(dPos.z))],
//													_data[INDEX(this,(dPos.x),(dPos.y),(dPos.z+1))],
//													_data[INDEX(this,(dPos.x+1),(dPos.y),(dPos.z+1))],
//													_data[INDEX(this,(dPos.x),(dPos.y+1),(dPos.z+1))],
//													_data[INDEX(this,(dPos.x+1),(dPos.y+1),(dPos.z+1))]);
//
//	return interpolator.interpolate(t);
//}
//

VectorField::VectorField(glm::ivec3 dimensions,BoundingAABB boundingAABB):
Field3D<glm::vec3>(dimensions,boundingAABB)
{
}