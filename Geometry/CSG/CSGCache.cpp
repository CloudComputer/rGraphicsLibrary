#include "CSGCache.h"


#include <glm/gtc/type_ptr.hpp>

CSGCache::CSGCache(CSG *csg):_csg(csg){


}


CSGCache::~CSGCache(){


}


float CSGCache::eval(glm::vec3 worldPos){
	float pos[3];
	pos[0] = worldPos.x;
	pos[1] = worldPos.y;
	pos[2] = worldPos.z;
	KDNode<float,3,float> *node = _tree.find((const float *)glm::value_ptr(worldPos));
	if(node){
		return node->get();
	}
	float v = _csg->eval(worldPos);
	_tree.insert(pos,v);
	return v;
}
