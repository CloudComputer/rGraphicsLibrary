#include "OpenEXRIncludes.h"

template<>
Imf::Rgba TypeBuffer<Imf::Rgba>::sample(int u,int v,MappingType mappingType){
	assert(u<dim.x);
	assert(v<dim.y);
	Imf::Rgba val = buffer[u+dim.x*v];
	if(mappingType == LatLong){
		float _v = v;
		_v /= dim.y;
		float s = cos(M_PI*_v - 0.5*M_PI);
		val.r *= s;
		val.g *= s;
		val.b *= s;
	}else{
		std::cerr << "Sampling with unkown mapping type, no scaling of values are performed" << __FUNCTION__ << " " << __FILE__ << " " << __LINE__ << std::endl;
	}


	return val;
}

template<>
half TypeBuffer<half>::sample(int u,int v,MappingType mappingType){
	assert(u<dim.x);
	assert(v<dim.y);
	half val = buffer[u+dim.x*v];
	if(mappingType == LatLong){
		float _v = v;
		_v /= dim.y;
		float s = cos(M_PI*_v - 0.5*M_PI);
		val *= s;
	}else{
		std::cerr << "Sampling with unkown mapping type, no scaling of values are performed" << __FUNCTION__ << " " << __FILE__ << " " << __LINE__ << std::endl;
	}
	return val;
}


half weight(const Imf::Rgba &rgba){
	half w[3] =	{0.2125 ,0.7454 , 0.0721};
	return (w[0] * rgba.r + w[1] * rgba.g + w[2] * rgba.b);
}

half avg(const Imf::Rgba &rgba){
	return rgba.a * (rgba.r + rgba.g + rgba.b)/3.0;
}

