#ifndef _RANDOM_H_
#define _RANDOM_H_

#include <vector>
#include <glm\glm.hpp>

#include <Geometry\BoundingGeometry\BoundingVolume.h>

class Random{
	Random(int seed = 0);
	static Random *inst;

	template <typename F> F __getMinMax(F min,F max);

public:
	static Random* getRandomGenerator(int seed = 0);
	virtual ~Random();
	
	void seed();
	void seed(int seed);

	float getFloat(float min = 0.0f,float max = 1.0f){return __getMinMax(min,max);}
	double getDouble(double min = 0.f,double max = 1.0){return __getMinMax(min,max);}
	int getInt(int min = 0,int max = 100);
	
	glm::vec3 getInBox(const BoundingAABB &_aabb);
	std::vector<glm::vec3> getInBox(const BoundingAABB &_aabb,int amount);
	
	glm::vec4 randomPointCloseToCubeSurface(float dist);
	std::vector<glm::vec4> randomPointsCloseToCubeSurface(int amount,float dist = 0.1);
	
	glm::vec3 randomPointOnSphereSurface();
	glm::vec4 randomPointCloseToSphereSurface(float dist);
	std::vector<glm::vec4> randomPointsCloseToSphereSurface(int amount,float dist = 0.1);

};

#endif