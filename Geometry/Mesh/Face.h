#ifndef _FACE_H_
#define _FACE_H_

#include "glm/glm.hpp"
#include "Base/Object.h"

class Face : public Object{
protected:
	glm::vec3 _N;
public:
	Face(glm::vec3 normal);
	virtual ~Face();
	
	glm::vec3 &getNormal();
	glm::vec3 getNormal()const;
	void setNormal(glm::vec3 normal);

	virtual float area()const = 0;
};



#endif
