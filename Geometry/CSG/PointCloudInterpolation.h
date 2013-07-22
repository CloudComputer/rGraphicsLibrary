#ifndef _POINTCLOUDINTERPOLATION_H_
#define _POINTCLOUDINTERPOLATION_H_

#include "CSG.h"

#include <Geometry\KDTree\KDTree.h>

class PointCloudInterpolation;

struct PointCloudInterpolationHints{
	PointCloudInterpolationHints():
		K(20),
		m(15),
		TOverlap(1.5),
		Tsa(2.0e-5),
		Treg(1.0e-5),
		supportSize(0.05){}
	int K;
	int m;
	float TOverlap;
	float Tsa;
	float Treg;
	float supportSize; //TODO remove this and make it auto
};

struct Point{
	Point(const glm::vec3 &P):P(P),N(0,0,0),density(0),overlap(0){
	
	}
	glm::vec3 P;
	glm::vec3 N;
	float overlap;
	float density;
};


struct Center{
	Center(const Point& p,PointCloudInterpolation *cloud,const PointCloudInterpolationHints &hints);
	glm::vec3 P;
	float supportSize,A,B,C,D,E,F,lambda;
	glm::vec3 u,v,w;
	float g(const glm::vec3 &worldPos);
};

class PointCloudInterpolation : public CSG{
	friend class Center;
	K3DTree<Point> _points;
	K3DTree<Center> _centers;
	float L;
	float maxSupportSize;
	BoundingAABB _aabb;
public:
	PointCloudInterpolation(std::vector<glm::vec3> pointCloud, PointCloudInterpolationHints hints = PointCloudInterpolationHints());
	virtual ~PointCloudInterpolation(){}

	K3DTree<Center> *getCenters(){return &_centers;}
	float getMaxSupportSize(){return maxSupportSize;}

	const BoundingAABB &getAABB(){return _aabb;}

	virtual float eval(const glm::vec3 &worldPos);

	virtual std::string toString()const{return "PointCloudInterpolation";}

};

#endif