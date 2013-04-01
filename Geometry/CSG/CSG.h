#ifndef _CSG_H_
#define _CSG_H_

#include "ImplicitFunction.h"
#include <glm/glm.hpp>


class CSG : public ImplicitFunction{
protected:
public:
	CSG(){}
	virtual ~CSG(){}

	virtual float eval(glm::vec3 worldPos) = 0;
};


enum CSGOperation{
	Union,
	Difference,
	Intersection
};

class CSGCombine : public CSG{
	CSG *_c0,*_c1;
	CSGOperation _operation;
public:
	CSGCombine(CSG *c0, CSG *c1,CSGOperation _operation);
	virtual float eval(glm::vec3 worldPos);

	virtual std::string toString()const;
};


class CSGSphere : public CSG {
	glm::vec3 _c;
	float _r;
public:
	CSGSphere(float radius = 1.0 , glm::vec3 center = glm::vec3(0,0,0));
	virtual float eval(glm::vec3 worldPos)const;
	
	virtual std::string toString()const;
};

#include "CSGCache.h"

#endif
