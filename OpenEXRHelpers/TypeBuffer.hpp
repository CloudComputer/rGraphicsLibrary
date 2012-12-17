#ifndef _TYPEBUFFER_HPP_
#define _TYPEBUFFER_HPP_

#ifndef _TYPEBUFFER_P_
#include "TypeBuffer.h"
#endif

template<typename T> template<typename T2> 
TypeBuffer<T> TypeBuffer<T>::Convert(const TypeBuffer<T2> &from,T (converter)(const T2 &value)){
	TypeBuffer<T> to;
	to.dim = from.dim;
	unsigned int size = to.dim.x*to.dim.y;
	to.buffer = new T[size];

	for(int i = 0;i<size;i++){
		to.buffer[i] = converter(from.buffer[i]);
	}

	return to;
}


//
//template<typename T>
//T TypeBuffer<T>::sample(int u,int v,MappingType mappingType){
//	assert(u<dim.x);
//	assert(v<dim.y);
//	T val = buffer[u+dim.x*v];
//	if(mappingType == LatLong){
//		float _v = v;
//		_v /= dim.y;
//		val *= cos(M_PI*_v - 0.5*M_PI);
//	}else{
//		std::cerr << "Sampling with unkown mapping type, no scaling of values are performed" << __FUNCTION__ << " " << __FILE__ << " " << __LINE__ << std::endl;
//	}
//
//
//	return val;
//}

#endif
