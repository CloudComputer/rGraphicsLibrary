#include "Clusterer.h"

#include <glm/gtc/type_ptr.hpp>

#include <iostream>


PointCluster::PointCluster(){
	_points = new KDTree<glm::vec3,3,float>();
}

PointCluster::~PointCluster(){
	
}

void PointCluster::addPoint(float point[3],glm::vec3 normal){
	_points->insert(point,normal);
}
//
//template<unsigned int dimmensions>
//void PointCluster<dimmensions>::addPoints(const std::vector<glm::vec3> &points){
//	_points.insert(_points.end(),points.begin(),points.end());
//}

//template<unsigned int dimmensions>
//void PointCluster<dimmensions>::addPoints(const PointCluster &cluster){
//	addPoints(cluster._points);
//}


std::vector<PointCluster> 
Clusterer::ClusterPoints(KDTree<glm::vec3,3,float> *_points,float d,unsigned int minSize){
	KDTree<glm::vec3,3,float>::NodeIterator point;
	std::vector<KDTree<glm::vec3,3,float>::Node*>::iterator it;

	KDTree<glm::vec3,3,float> tree;
	KDTree<glm::vec3,3,float>::Node* node;
	std::vector<KDTree<glm::vec3,3,float>::Node*> left;


	for(point = _points->begin(); point != _points->end() ; ++point){
		tree.insert(point->getPosition(),point->get());
	}


	float __p[] = {0.0f,0.0f,0.0f};
	std::vector<PointCluster> clusters;
	while(!tree.empty()){
		PointCluster cluster;
		left.push_back(tree.findNearest(__p));
		
		while(!left.empty()){
			node = left[0];
			left.erase(left.begin());
			std::vector<KDTree<glm::vec3,3,float>::Node*> nodes = tree.findCloseTo(node->getPosition(),d);
			for(it = nodes.begin();it != nodes.end();++it){
				float pos[3];
				memcpy(pos,(*it)->getPosition(),sizeof(float)*3);
				left.push_back(cluster._points->insert(pos,(*it)->get()));
				tree.erase(*it);
			}

		}
		if(cluster._points->size() >= minSize)
			clusters.push_back(cluster);
		assert(tree.isOk());
	}


	return clusters;
}