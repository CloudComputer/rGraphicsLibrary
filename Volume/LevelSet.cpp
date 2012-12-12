#include "LevelSet.h"

#include <Util/StopClock.h>
#include <iostream>

#include <queue>

LevelSet::LevelSet(Mesh *m,glm::ivec3 dimensions):
	ScalarField(dimensions,m->getBoundingAABB())
{
	StopClock sw(true);
	FOR(_dimensions){
		if(y==0&&z!=0&&x==0){
			std::cout << sw.getFractionElapsedSeconds() << "Seconds, " << (100.0*z)/_dimensions.z  << "%" << std::endl;
			sw.restart();
		}
		glm::vec3 worldPos = _getWorldPos(glm::ivec3(x,y,z));
		float d = m->distance(worldPos,true);
		set(glm::ivec3(x,y,z),d);
	}
	
}

LevelSet::LevelSet(glm::ivec3 dimensions,BoundingAABB boundingAABB):
	ScalarField(dimensions,boundingAABB)
{
}

void LevelSet::erode(float c){
	
}

void LevelSet::dilate(float c){

}

void LevelSet::advect(VectorField *f,float dt){
	std::cout << "Start advecting" << std::endl;
	FOR(_dimensions){
		glm::vec3 worldPos = _getWorldPos(glm::ivec3(x,y,z));	
		glm::vec3 V = f->getFromWorldPos(worldPos);
		glm::vec3 G = getUpWindGradient(worldPos,V);
		float r = -glm::dot(V,G);
		glm::vec3 a = _delta / glm::abs(V);
		dt = 0.1f * glm::min(glm::min(a.x,a.y),a.z);
		set(glm::ivec3(x,y,z),get(glm::ivec3(x,y,z))+r*dt);
	}
	std::cout << "Advection done" << std::endl;
}

struct queueIVec3{
	glm::ivec3 pos;
	float value;

	
};

bool operator<(const queueIVec3 p0,const queueIVec3 p1){
	return abs(p0.value) > abs(p1.value);
}

const glm::ivec3 __x(1,0,0), __y(0,1,0), __z(0,0,1);

void LevelSet::recalculate(){
	std::priority_queue<queueIVec3> queue;
	Field3D<bool> done(_dimensions,_boundingAABB);
	done.setZero();
	int left = _dimensions.x * _dimensions.y * _dimensions.z;
	FOR(_dimensions){
		if(done.get(glm::ivec3(x,y,z))){
			continue;
		}
		queueIVec3 p;
		p.pos = glm::ivec3(x,y,z);
		p.value = get(p.pos);

		if(abs(p.value)>=_delta.x*5){
			set(p.pos,_delta.x*5);
			left--;
			continue;
		}

		int sig = glm::sign(p.value);
		if(sig != glm::sign(get(p.pos + __x))){
			done.set(p.pos,true);
			done.set(p.pos + __x,true);
			left -= 2;
			continue;
		}
		if(sig != glm::sign(get(p.pos - __x))){
			done.set(p.pos,true);
			done.set(p.pos - __x,true);
			left -= 2;
			continue;
		}
		if(sig != glm::sign(get(p.pos + __y))){
			done.set(p.pos,true);
			done.set(p.pos + __y,true);
			left -= 2;
			continue;
		}
		if(sig != glm::sign(get(p.pos - __y))){
			done.set(p.pos,true);
			done.set(p.pos - __y,true);
			left -= 2;
			continue;
		}

		if(sig != glm::sign(get(p.pos + __z))){
			done.set(p.pos,true);
			done.set(p.pos + __z,true);
			left -= 2;
			continue;
		}
		if(sig != glm::sign(get(p.pos - __z))){
			done.set(p.pos,true);
			done.set(p.pos - __z,true);
			left -= 2;
			continue;
		}
		queue.push(p);
	}
	std::cout << "asdf"<< std::endl;
}
