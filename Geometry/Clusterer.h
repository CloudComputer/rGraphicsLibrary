#ifndef _CLUSTERER_H_
#define _CLUSTERER_H_

#include "KDTree.h"

#include <vector>
#include <glm/glm.hpp>


class PointCluster{
	friend class Clusterer;

	KDTree<glm::vec3,3,float>* _points;
public:
	PointCluster();
	~PointCluster();
	void addPoint(float point[3],glm::vec3 normal = glm::vec3(0,0,0)); 
	//void addPoints(const PointCluster &cluster);
	
	KDTree<glm::vec3,3,float>* getPoints(){return _points;}
};

class Clusterer{
public:
	static std::vector<PointCluster> ClusterPoints(KDTree<glm::vec3,3,float> *_points,float joinDistance,unsigned int minSize = 1);
};


#endif
