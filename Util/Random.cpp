#include "Random.h"

#include <cstdlib>
#include <ctime> 

Random *Random::inst = 0;

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
	double r = __getMinMax(0.0,1.0);
	return (r-min)/(max-min);
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
	r /= RAND_MAX-1; //include max
	return (r-min)/(max-min);
}

