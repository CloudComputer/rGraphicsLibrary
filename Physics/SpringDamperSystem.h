#ifndef _SPRINGDAMPERSYSTEM_H_
#define _SPRINGDAMPERSYSTEM_H_

#include <vector>

#include "SpringDamper.h"

template<typename T>
class SpringDamperSystem{
	std::vector<SpringDamper<T>*> _springs;
	std::vector<SpringParticle<T>*> _particles;
	std::vector<T> _externalForces;

public:
	std::vector<SpringParticle<T>*> getParticles(){return _particles;}

	void addExternalForce(const T &f){_externalForces.push_back(f);}

	void evolve(float dt){
		std::vector<SpringParticle<T>*>::iterator particle;	
		std::vector<SpringDamper<T>*>::iterator spring;
		std::vector<T>::iterator force;

		for(spring = _springs.begin();spring != _springs.end();++spring){
			(*spring)->evaluate();
		}

		for(particle = _particles.begin();particle != _particles.end();++particle){
			for(force = _externalForces.begin();force != _externalForces.end();++force)
				(*particle)->addForce(*force);
			(*particle)->evolve(dt);
		}

	}


	static SpringDamperSystem* CreateSquared(float w,float h,int sw = 20,int sh = 20,float k = 1,float m = 1,float d = 1){
		
		SpringDamperSystem *sds = new SpringDamperSystem();

		for(float y = -h/2 ; y<h/2 ; y+=h/sh)for(float x = -w/2 ; x<w/2 ; x+=w/sw){
			T pos(0);
			pos.x = x;
			pos.y = y;
			sds->_particles.push_back(new SpringParticle<T>(pos,m));
		}

		for(int y = 0;y<sh;y++)for(int x = 0;x<sw;x++){
			int id = x + (y*sh);
			if(x!=sw-1){
				sds->_springs.push_back(new SpringDamper<T>(sds->_particles[id],sds->_particles[id+1]));
			}
			if(y!=sh-1){
				sds->_springs.push_back(new SpringDamper<T>(sds->_particles[id],sds->_particles[id+sw]));
			}
			if(x!=sw-1 && y!=sh-1){
				sds->_springs.push_back(new SpringDamper<T>(sds->_particles[id],sds->_particles[id+sw+1]));
				sds->_springs.push_back(new SpringDamper<T>(sds->_particles[id+1],sds->_particles[id+sw]));
			}
		}

		return sds;
	}
};

#endif
