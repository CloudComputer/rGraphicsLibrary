#ifndef _IMPLICITFUNCTION_H_
#define _IMPLICITFUNCTION_H_

#include <glm/glm.hpp>
#include <Base/Object.h>

class ImplicitFunction : public Object{
public:
	ImplicitFunction(){};
	virtual ~ImplicitFunction(){};

	virtual float eval(float x,float y,float z){
		return eval(glm::vec3(x,y,z));
	}

	virtual float eval(glm::vec3 worldPos) = 0;
};


#endif
