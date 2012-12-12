#ifndef __FIELD3D_HPP__
#define __FIELD3D_HPP__

#ifndef __FIELD3D_H__
#include "Field3D.h"
#endif

#include <Math/Interpolator.h>
#include <stdio.h>
#include <string.h>

template <typename T>
Field3D<T>::Field3D(glm::ivec3 dimensions,BoundingAABB boundingAABB):
_dimensions(dimensions),
_boundingAABB(boundingAABB)
{
	_delta =_boundingAABB.getPosition(glm::vec3(1,1,1)) - _boundingAABB.getPosition(glm::vec3(0,0,0));
	_delta /= _dimensions;
	_data = 0;
	_allocate();
}


template<typename T>
Field3D<T>::~Field3D(void)
{
	_deallocate();
}

template<typename T>
void Field3D<T>::setZero(){
	memset(_data,0,getMemSize());
}
template<typename T>
T Field3D<T>::getMin()const{
	T v = _data[0];
	FOR(_dimensions){
		int i = _index(glm::ivec3(x,y,z));
		if(_data[i]<v)v=_data[i];
	}
	return v;
}

template<typename T>
T Field3D<T>::getMax()const{
	T v = _data[0];
	FOR(_dimensions){
		int i = _index(glm::ivec3(x,y,z));
		if(_data[i]>v)v=_data[i];
	}
	return v;
}


template<typename T>
void  Field3D<T>::normalize(){
	T minV,maxV;
	minV = maxV = _data[0];
	FOR(_dimensions){
		int i = _index(glm::ivec3(x,y,z));
		if(_data[i]>maxV)maxV=_data[i];
		if(_data[i]<minV)minV=_data[i];
	}
	FOR(_dimensions){
		int i = _index(glm::ivec3(x,y,z));
		_data[i] = (_data[i] - minV) / (maxV - minV);
	}
}

template<typename T>
void Field3D<T>::foreach(T (*funcPtr)(T,glm::vec3)){
	for(unsigned int z = 0;z<_dimensions.z;z++)
		for(unsigned int y = 0;y<_dimensions.y;y++)
			for(unsigned int x = 0;x<_dimensions.x;x++){
				int i = _index(glm::ivec3(x,y,z));
				_data[i] = funcPtr(_data[i],_getWorldPos(glm::ivec3(x,y,z)));
			}

}

template<typename T>
void Field3D<T>::foreach(void (*funcPtr)(T&,glm::vec3)){
	for(unsigned int z = 0;z<_dimensions.z;z++)
		for(unsigned int y = 0;y<_dimensions.y;y++)
			for(unsigned int x = 0;x<_dimensions.x;x++){
				int i = _index(glm::ivec3(x,y,z));
				funcPtr(_data[i],_getWorldPos(glm::ivec3(x,y,z)));
			}

}

template<typename T>
void Field3D<T>::set(glm::ivec3 pos,T value){
	_data[_index(pos)] = value;
}

template<typename T>
T Field3D<T>::get(glm::ivec3 pos)const{
	return _data[_index(pos)];
}

template<typename T>
T Field3D<T>::getFromWorldPos(glm::vec3 worldPos)const{
	glm::vec3 internalPosition = (glm::vec3)_boundingAABB.getDiscretePosition(worldPos,_dimensions);
	glm::ivec3 dPos =  (glm::ivec3)internalPosition;
	glm::vec3 t = internalPosition - (glm::vec3)dPos;

	TriLinearInterpolator<T> interpolator(  _data[_index(dPos+glm::ivec3(0,0,0))],
		_data[_index(dPos+glm::ivec3(1,0,0))],
		_data[_index(dPos+glm::ivec3(0,1,0))],
		_data[_index(dPos+glm::ivec3(1,1,0))],
		_data[_index(dPos+glm::ivec3(0,0,1))],
		_data[_index(dPos+glm::ivec3(1,0,1))],
		_data[_index(dPos+glm::ivec3(0,1,1))],
		_data[_index(dPos+glm::ivec3(1,1,1))]);

	return interpolator.interpolate(t);
}

template<typename T>
glm::ivec3 Field3D<T>::getDimensions()const{
	return _dimensions;
}

template<typename T>
BoundingAABB Field3D<T>::getBoundingAABB()const{
	return _boundingAABB;

}


template<typename T>
T* Field3D<T>::getData()const{
	return _data;
}

template<typename T>
unsigned int Field3D<T>::getMemSize()const{
	return sizeof(T)*_dimensions.x*_dimensions.y*_dimensions.z;
}

template<typename T>
unsigned int Field3D<T>::_index(glm::ivec3 pos,bool clamp)const
{
	if(pos.x>=_dimensions.x){
		if(clamp) pos.x=_dimensions.x-1;
		else while(pos.x<_dimensions.x) pos.x -= _dimensions.x;
	}
	if(pos.y>=_dimensions.y){
		if(clamp) pos.y=_dimensions.y-1;
		else while(pos.y<_dimensions.y) pos.y -= _dimensions.y;
	}
	if(pos.z>=_dimensions.z){
		if(clamp) pos.z=_dimensions.z-1;
		else while(pos.z<_dimensions.z) pos.z -= _dimensions.z;
	}

	if(pos.x < 0){
		if(clamp) pos.x = 0;
		else while(pos.x>0) pos.x += _dimensions.x;
	}

	if(pos.y < 0){
		if(clamp) pos.y = 0;
		else while(pos.y>0) pos.y += _dimensions.y;
	}

	if(pos.z < 0){
		if(clamp) pos.z = 0;
		else while(pos.z>0) pos.z += _dimensions.z;
	}
	return pos.x + _dimensions.x * (pos.y + pos.z*_dimensions.y);
}

template<typename T>
glm::vec3 Field3D<T>::_getWorldPos(glm::ivec3 pos)const{
	glm::vec3 t = (glm::vec3)pos;
	t /= _dimensions;
	return _boundingAABB.getPosition(t);
}

template<typename T>
void Field3D<T>::_allocate()
{
	if(_data != 0){
		_deallocate();
	}
	_data = new T[_dimensions.x*_dimensions.y*_dimensions.z];
}

template<typename T>
void Field3D<T>::_deallocate()
{
	delete _data;
	_data = 0;
}

#endif
