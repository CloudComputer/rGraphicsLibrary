#ifndef _PLANE_H_
#define _PLANE_H_

#include <vector>
#include <glm\glm.hpp>


class Plane{
	float a,b,c,d;
public:
	Plane();
	Plane(const std::vector<glm::vec3> &points);
	Plane( const Plane& other );
	virtual ~Plane();
	
	glm::vec3 getPoint(float a,float b)const;
	float getDistanceToPlane(const glm::vec3 &point)const;

	glm::vec3 getNormal()const;
};

#endif