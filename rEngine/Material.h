#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include <glm\glm.hpp>

#include "IDObject.h"


class Material : public IDObject{
	glm::vec4 diffuse;
	glm::vec4 specular;
	glm::vec3 ambient;
public:
	Material();
	virtual ~Material();
	
	void setDiffuse(const glm::vec3 &col);
	void setSpecular(const glm::vec3 &col);
	void setAmbient(const glm::vec3 &col);
	void setDiffuse(const glm::vec4 &col);
	void setSpecularity(const float &f);
	
	glm::vec4 getDiffuse()const;
	glm::vec3 getSpecular()const;
	glm::vec3 getAmbient()const;
	float getSpecularity()const;
};

#endif