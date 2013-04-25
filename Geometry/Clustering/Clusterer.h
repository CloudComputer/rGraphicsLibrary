#ifndef _CLUSTERER_H_
#define _CLUSTERER_H_

#include "../KDTree/KDTree.h"

#include "Geometry/Mesh/Vertex.h"

#include <vector>
#include <glm/glm.hpp>

class PointCluster{
	friend class Clusterer;

	KDTree<Vertex,3,float>* _points;
public:
	PointCluster();
	~PointCluster();
	void addPoint(float point[3],const Vertex &v); 
	//void addPoints(const PointCluster &cluster);
	
	int size()const{
		return _points->size();
	}

	KDTree<Vertex,3,float>* getPoints(){return _points;}
};

class Clusterer{
public:
	static std::vector<PointCluster> ClusterPoints(KDTree<Vertex,3,float> *_points,float joinDistance,unsigned int minSize = 1,bool sortBySize = true);
};


#endif
