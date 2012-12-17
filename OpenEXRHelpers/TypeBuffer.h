#ifndef _TYPEBUFFER_H_
#define _TYPEBUFFER_H_

#include "OpenEXRIncludes.h"

half weight(const Imf::Rgba &rgba);
half avg(const Imf::Rgba &rgba);

enum MappingType{
	LatLong
};

template<typename T>
struct TypeBuffer{
	T *buffer;
	Imath::V2i dim;

	T sample(int u,int v,MappingType mappingType);

	unsigned int size(){return dim.x*dim.y;}

	template<typename T2> static TypeBuffer<T> Convert(const TypeBuffer<T2> &buffer,T (converter)(const T2 &value));
};

typedef TypeBuffer<Imf::Rgba> RGBABuffer;
typedef TypeBuffer<half> GreyBuffer;

#ifndef _TYPEBUFFER_HPP_
#include "TypeBuffer.hpp"
#endif


#endif
