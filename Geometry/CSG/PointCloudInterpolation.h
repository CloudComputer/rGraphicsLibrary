#ifndef _POINTCLOUDINTERPOLATION_H_
#define _POINTCLOUDINTERPOLATION_H_

#include "CSG.h"

#include <Geometry\KDTree\KDTree.h>

class SubPointCloud;
struct PointCloudPoint{
	float shift;
	glm::vec3 N,P;
	PointCloudPoint(const glm::vec3 &P,const glm::vec3 &N);
	float G(const glm::vec3 &p);
	void init(SubPointCloud *owner);
	float phi(float r,float supportSize);
private:
	glm::vec3 u,v,w;
	float A,B,C;
};

class SubPointCloud {
	friend class PointCloudPoint;
	friend class PointCloudInterpolation;
	K3DTree<PointCloudPoint> _points;
	SubPointCloud *_childs[8];
	BoundingAABB _aabb;
	float supportSize;
public:
	SubPointCloud(K3DTree<glm::vec3> &points);
	SubPointCloud();
	~SubPointCloud();

	bool hasChilds();

	float eval(const glm::vec3 &worldPos);

	void init();
	void subdivide();
};


class PointCloudInterpolation : public CSG{
	SubPointCloud _points;
public:
	PointCloudInterpolation(K3DTree<glm::vec3> &points);
	virtual ~PointCloudInterpolation();

	const BoundingAABB &getAABB(){return _points._aabb;}

	virtual float eval(const glm::vec3 &worldPos);

	virtual std::string toString()const{return "PointCloudInterpolation";}

};

#endif