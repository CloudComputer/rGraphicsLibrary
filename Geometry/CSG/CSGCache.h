#ifndef _CSGCACHE_H_
#define _CSGCACHE_H_


#include "../KDTree/KDTree.h"
#include "CSG.h"

class CSGCache : public CSG{
	CSG* _csg;
	KDTree<float,3,float> _tree;
public:
	CSGCache(CSG *csg);
	virtual ~CSGCache();
	
	virtual float eval(glm::vec3 worldPos);
	
	virtual std::string toString()const{return "Cached CSG";}

};

#endif