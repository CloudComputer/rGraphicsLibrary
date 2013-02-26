#ifndef _SPRINGPARTICLE_H_
#define _SPRINGPARTICLE_H_

template<typename T> 
struct SpringParticle{
	T F;
	T V;
	T P;
	float m;
	bool pinned;
	SpringParticle(T pos = T(),float mass = 1,bool pinned = false):P(pos),F(0),V(0),m(mass),pinned(pinned){}

	void setPinned(bool pinned = true){
		this->pinned = pinned;
	}

	void addForce(const T &f){
		F += f;
	}

	void evolve(float dt){
		if(pinned)
			return;
		V += F/m*dt;
		F = T(0);
		P += V*dt;
	}
};


#endif
