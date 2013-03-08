#ifndef _CLUSTERER_H_
#define _CLUSTERER_H_

#include "KDTree.h"

#include <vector>
#include <glm/glm.hpp>

#include "SurfacePoint.h"

class PointCluster{
	friend class Clusterer;

	KDTree<NormalPoint,3,float>* _points;
public:
	PointCluster();
	~PointCluster();
	void addPoint(float point[3],NormalPoint np); 
	//void addPoints(const PointCluster &cluster);
	
	KDTree<NormalPoint,3,float>* getPoints(){return _points;}
};

class Clusterer{
public:
	static std::vector<PointCluster> ClusterPoints(KDTree<NormalPoint,3,float> *_points,float joinDistance,unsigned int minSize = 1);
};


#endif
