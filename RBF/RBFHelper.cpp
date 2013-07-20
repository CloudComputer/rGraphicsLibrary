#include "RBFHelper.h"

#include <Util\Macros.h>
#include <Geometry\KDTree\KDTree.h>
#include <Math\Plane.h>



RBFHelper::RBFHelper(){
	rbf = 0;
}


RBFHelper::~RBFHelper(){
	delete rbf;

}


void RBFHelper::setPoints(std::vector<glm::vec3> points){
	K3DTree<char> tree; //must be some data, void not yet supported
	IT_FOR(points,p){
		tree.insert(*p,'0');
	}

	IT_FOR(tree,n){
		auto nodes = tree.findNNearest(n->getPosition(),15);
		std::vector<glm::vec3> closePoints;
			

		IT_FOR(nodes,node){
			closePoints.push_back(glm::vec3((*node)->getPosition()[0],(*node)->getPosition()[1],(*node)->getPosition()[2]));
		}
		Plane plane(closePoints);

		glm::vec3 pos;
		pos.x = n->getPosition()[0];
		pos.y = n->getPosition()[1];
		pos.z = n->getPosition()[2];
		
		centers.push_back(glm::vec4(pos,0));
		centers.push_back(glm::vec4(pos+plane.getNormal() * hints.offSurfPointDistnace,hints.offSurfPointDistnace));
		centers.push_back(glm::vec4(pos+plane.getNormal() * (-hints.offSurfPointDistnace),-hints.offSurfPointDistnace));
	}

	std::cout << centers.size() << " " << points.size() << std::endl;
}

bool __rbfCenterReductionNode::sortOnId = false;
bool operator< (const __rbfCenterReductionNode& a, const __rbfCenterReductionNode& b){
	if(__rbfCenterReductionNode::sortOnId)
		return b.id < a.id;
	else
		return b.e < a.e;
		
}

