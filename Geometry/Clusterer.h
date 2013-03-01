#ifndef _CLUSTERER_H_
#define _CLUSTERER_H_

#include <vector>
#include <glm/glm.hpp>


class PointCluster{
	friend class Clusterer;
	std::vector<glm::vec3> _points;
public:
	void addPoint(const glm::vec3 &point); 
	void addPoints(const std::vector<glm::vec3> &points);
	void addPoints(const PointCluster &cluster);
	
	glm::vec3 getPoint(unsigned int id){return _points[id];}
	std::vector<glm::vec3> getPoints(unsigned int id){return _points;}
};

class Clusterer{
public:
	static std::vector<PointCluster> ClusterPoints(const std::vector<glm::vec3> &_points,float joinDistance);
};


#endif
