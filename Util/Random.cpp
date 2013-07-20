#include "Random.h"

#include <cmath>

#include <cstdlib>
#include <ctime> 

Random *Random::inst = 0;
Random* Random::getRandomGenerator(int seed){
	if(inst  == 0) inst = new Random(seed);
	return inst;
}
Random::Random(int seed){
	this->seed(seed);
}


Random::~Random(){
	
}

void Random::seed(){
	srand(time(NULL));
}

void Random::seed(int seed){
	srand(seed);
}

int Random::getInt(int min,int max){
	return std::floor( __getMinMax(0.0,max+1.0));
}


glm::vec3 Random::getInBox(const BoundingAABB &_aabb){
	return _aabb.getPosition(glm::vec3(getFloat(0,1),getFloat(0,1),getFloat(0,1)));
}


std::vector<glm::vec3> Random::getInBox(const BoundingAABB &_aabb,int amount){
	std::vector<glm::vec3> r;
	for(int i = 0;i<amount;i++){
		r.push_back(getInBox(_aabb));
	}
	return r;
}

template <typename F> F Random::__getMinMax(F min,F max){
	F r = rand();
	r /= RAND_MAX; //include max
	return r*(max-min)+min;
}
	
#include <iostream>
glm::vec4 Random::randomPointCloseToCubeSurface(float dist){
	int side = getInt(0,5);
	float x = getFloat();
	float y = getFloat();
	glm::vec4 p;
	p.w = getFloat(-dist,dist);
		
	switch(side){
	case 0:
		p.x = 1 + p.w;
		p.y = x*2-1;
		p.z = y*2-1;
		break;
	case 1:
		p.x = -1 - p.w;
		p.y = x*2-1;
		p.z = y*2-1;
		break;
	case 2:
		p.y = 1 + p.w;
		p.x = x*2-1;
		p.z = y*2-1;
		break;
	case 3:
		p.y = -1 - p.w;
		p.x = x*2-1;
		p.z = y*2-1;
		break;
	case 4:
		p.z = 1 + p.w;
		p.x = x*2-1;
		p.y = y*2-1;
		break;
	case 5:
		p.z = -1 - p.w;
		p.x = x*2-1;
		p.y = y*2-1;
		break;
	}
	return p;
}

std::vector<glm::vec4> Random::randomPointsCloseToCubeSurface(int amount,float dist){
	std::vector<glm::vec4> points;
	for(int i = 0;i<amount;i++){
		points.push_back(randomPointCloseToCubeSurface(dist));
	}
	return points;
}


glm::vec3 Random::inTriangle(const glm::vec3 &v0,const glm::vec3 &v1,const glm::vec3 &v2){
	float u,v,a,b,c;
	u = std::sqrtf(getFloat());
	v = getFloat();
	
	a = 1 - u;
	b = u*(1-v);
	c = u*v;

	return a*v0 + b*v1 + c*v2;
}

glm::vec3 Random::randomPointOnSphereSurface(){
	float theta = getFloat(0,2*M_PI);
	float phi   = std::acosf(getFloat(-1,1));
	
	float ct = std::cosf(theta);
	float st = std::sinf(theta);
	float sp = std::sinf(phi);
	float cp = std::cosf(phi);
	return glm::vec3(ct*sp,st*sp,cp);
}

glm::vec4 Random::randomPointCloseToSphereSurface(float dist){
	glm::vec4 p = glm::vec4(randomPointOnSphereSurface(),0);
	float d = getFloat(-dist,dist);
	p *= 1+d;
	p.w = d;
	return p;
}

std::vector<glm::vec4> Random::randomPointsCloseToSphereSurface(int amount,float dist){
	std::vector<glm::vec4> points;
	for(int i = 0;i<amount;i++){
		points.push_back(randomPointCloseToSphereSurface(dist));
	}
	return points;
}