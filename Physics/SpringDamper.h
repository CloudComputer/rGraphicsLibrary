#ifndef _SPRINGDAMPER_H_
#define _SPRINGDAMPER_H_

#include "SpringParticle.h"


template<typename T> 
class SpringDamper{
	SpringParticle<T> *p0,*p1;
	float k; // spring konstant
	float d; // damping konstant
	float l; //resting length

public:
	SpringDamper(SpringParticle<T>* p0,SpringParticle<T>* p1,float springConstant = 1,float dampingConstant=1,float restLength=-1):
	p0(p0),p1(p1),k(springConstant),d(dampingConstant),l(restLength){
		if(l == -1){
			l = glm::length(p0->P - p1->P);
		}
	}

	void evaluate(){
		float L,f,fs,fd,v0,v1;
		T dir = p1->P - p0->P;
		L = glm::length(dir);
		dir /= L;
		v0 = glm::dot(dir,p0->V);
		v1 = glm::dot(dir,p1->V);

		//fs = -k * (l - L);
		fs = -k * (1 - L/l);
		fd = -d * (v0-v1);
		f = fs+fd;
		p0->F += dir * f;
		p1->F -= dir * f;
	}
};


#endif
