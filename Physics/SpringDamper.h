#ifndef _SPRINGDAMPER_H_
#define _SPRINGDAMPER_H_

template<typename T> 
struct SpringParticle{
	T F;
	T V;
	T P;
	float m;
	SpringParticle():F(0),V(0){}

	void update(float dt){
		V += F/m*dt;
		F = T(0);
		P += V*dt;
	}
};

template<typename T> 
class SpringDamper{
	SpringParticle<T> *p0,*p1;
	float k; // spring konstant
	float d; // damping konstant
	float l; //resting length

	void evolve(){
		float L,f,fs,fd,v0,v1;
		T dir = p1->P - p0->P;
		L = glm::length(dir);
		dir /= L;
		v0 = glm::dor(dir,p0->V);
		v1 = glm::dor(dir,p1->V);

		fs = -k * (l - L);
		fd = -d * (v0-v1);
		f = fs+fd;
		p0->F += dir * f;
		p1->F -= dir * f;
	}
};


#endif
