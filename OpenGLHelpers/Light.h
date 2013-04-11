#ifndef _LIGHT_H_
#define _LIGHT_H_

#include <glm\glm.hpp>



class Light {
	int _id;
	glm::vec4 _ambientColor;
	glm::vec4 _diffuseColor;
	glm::vec4 _specularColor;

	float _radius;
	glm::vec4 _position;

	glm::vec3 _spotDirection;
	float _spotCutOff;
	float _spotExponent;

public:
	Light(int id);
	~Light();

	void set();

	
	void setAmbientColor(glm::vec4 color);
	void setDiffuseColor(glm::vec4 color);
	void setSpecularColor(glm::vec4 color);
	
	void setRadius(float radius);
	/*void setConstantAttenuation(float constantAttenuation);
	void setLinearAttenuation(float linearAttenuation);
	void setQuadraticAttenuation(float quadraticAttenuation);*/

	void setPosition(glm::vec3 pos);
	void setDirection(glm::vec3 dir);
	
	void setSpotDir(glm::vec3 dir);
	void setSpotCutOff(float cutOff);
	void setSpotExponent(float exponent);

	
	glm::vec4 getAmbientColor()const;
	glm::vec4 getDiffuseColor()const;
	glm::vec4 getSpecularColor()const;
	
	float getRadius()const;
	/*float getConstantAttenuation()const;
	float getLinearAttenuation()const;
	float getQuadraticAttenuation()const;
*/
	glm::vec3 getPosition()const;
	glm::vec3 getDirection()const;
	
	glm::vec3 getSpotDir()const;
	float getSpotCutOff()const;
	float getSpotExponent()const;
};

#endif
