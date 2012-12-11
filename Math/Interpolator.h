#pragma once

#include <glm/glm.hpp>

template<typename T>
class LinearInterpolator
{
	T _min;
	T _max;
public:
	LinearInterpolator(T minValue,T maxValue){
		_min = minValue;
		_max = maxValue;
	}
	~LinearInterpolator(void){}




	T interpolate(float t){
		return interpolate(_min,_max,t);
	}

	T interpolate(double t){
		return interpolate(_min,_max,t);
	}

	///Static function

	static T interpolate(T a,T b,float t){
		return a + t*(b-a);
	}

	static T interpolate(T a,T b,double t){
		return a + t*(b-a);
	}
};

template<typename T>
class BiLinearInterpolator{
	LinearInterpolator<T> i0, i1;
public:
	BiLinearInterpolator(T a,T b,T c,T d):i0(a,b),i1(c,d){}

	T interpolate(glm::vec2 t){
		return LinearInterpolator<T>::interpolate(i0.interpolate(t.x),i1.interpolate(t.x),t.y);
	}
	
	static T interpolate(T a,T b,T c,T d,glm::vec2 t){
		BiLinearInterpolator i(a,b,c,d);
			return i.interpolate(t);
	}

};

template<typename T>
class TriLinearInterpolator{
	BiLinearInterpolator<T> i0,i1;
public:
	TriLinearInterpolator(T a,T b,T c,T d,T e,T f,T g,T h):i0(a,b,c,d),i1(e,f,g,h){


	}

	T interpolate(glm::vec3 t){
		glm::vec2 t2(t.x,t.y);
		return LinearInterpolator<T>::interpolate(i0.interpolate(t2),i1.interpolate(t2),t.z);
	}

	T interpolate(T a,T b,T c,T d,T e,T f,T g,T h,glm::vec3 t){
		TriLinearInterpolator i(a,b,c,d,e,f,g,h);
			return i.interpolate(t);
	}
};