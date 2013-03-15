#ifndef _VERTEX_H_
#define _VERTEX_H_

#include <Base/Object.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Vertex : public Object{
	glm::vec4 _P;
	glm::vec3 _N;
public:
	Vertex(const Vertex &v);
	Vertex(glm::vec4 position = glm::vec4(0,0,0,1),glm::vec3 normal = glm::vec3(0,1,0));
	Vertex(glm::vec3 position,glm::vec3 normal = glm::vec3(0,1,0));

	glm::vec4 getPosition()const;
	glm::vec3 getNormal()const;
	glm::vec4 &getPosition();
	glm::vec3 &getNormal();

	void setPosition(glm::vec4 position);
	void setNormal(glm::vec3 normal);
	
	virtual std::string toString()const;


#ifdef GL_VERSION_1_1
	virtual void gl(){
		//glNormal3fv(glm::value_ptr(_N));
		//glVertex4fv(glm::value_ptr(_P));
	}
#endif
};



#endif
