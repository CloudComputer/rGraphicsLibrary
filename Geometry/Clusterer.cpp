#include "Clusterer.h"

#include "kdtree.h"
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

void PointCluster::addPoint(const glm::vec3 &point){
	_points.push_back(point);
}
void PointCluster::addPoints(const std::vector<glm::vec3> &points){
	_points.insert(_points.end(),points.begin(),points.end());
}
void PointCluster::addPoints(const PointCluster &cluster){
	addPoints(cluster._points);
}

std::vector<PointCluster> Clusterer::ClusterPoints(const std::vector<glm::vec3> &_points,float d){
	std::vector<glm::vec3>::const_iterator point;
	std::vector<glm::vec3> left;
	for(point = _points.begin(); point != _points.end() ; ++point){
		left.push_back(*point);
	}
	std::vector<PointCluster> clusters;
	while(left.size()){
		PointCluster c; //Create new cluster
		c.addPoint(left[0]); //add first point to cluster
		left.erase(left.begin()); //remove it from the list
		int a = 0;
		while(a<c._points.size()){
			for(int i = left.size()-1;i>0;i--){
				if(glm::distance(c._points[a],left[i])<=d){
					c.addPoint(left[i]);
					left.erase(left.begin()+i);
				}
			}
			a++;
		}
		clusters.push_back(c);
		std::cout << "cluster created: " << c._points.size() << " " << left.size() << std::endl;
	}



	return clusters;
}