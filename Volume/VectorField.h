#ifndef __VECTORFIELD_H__
#define __VECTORFIELD_H__

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Volume/Field3D.h>
#include <Geometry/BoundingGeometry/BoundingVolume.h>

#ifdef GL_VERSION_1_1
#include <OpenGLHelpers/AttribPusher.h>
#endif


class VectorField : public Field3D<glm::vec3>{
	friend class UltrasoundVariationalClassification;
public:
	VectorField(glm::ivec3 dimensions = glm::ivec3(64,64,64),BoundingAABB boundingAABB = BoundingAABB(glm::vec3(0,0,0),glm::vec3(1,1,1)));

	VectorField *getCurlField()const;

#ifdef GL_VERSION_1_1
	void draw(float s = 0.1){
		//AttribPusher __p(GL_CURRENT_BIT);
		glColor3f(1,1,1);
		glBegin(GL_LINES);
		FOR(_dimensions){
			glm::vec3 worldPos = _boundingAABB.getPosition(glm::vec3(x,y,z)/(glm::vec3)_dimensions);
			glVertex3fv(glm::value_ptr(worldPos));
			worldPos+= get(glm::ivec3(x,y,z))*s;
			glVertex3fv(glm::value_ptr(worldPos));
		}
		glEnd();
	}
#endif

	glm::vec3 curl(glm::vec3 worldPos)const;
};

#ifndef __VECTORFIELD_HPP__
#include "VectorField.hpp"
#endif

#endif