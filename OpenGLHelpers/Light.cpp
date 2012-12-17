#include "Light.h"

#ifdef _WIN32
	#include <windows.h>
#endif
#include <glew/GL/glew.h>
#include <GL/GL.h>

#include <glm/gtc/type_ptr.hpp>

#include "OpenGLInfo.h"

Light::Light(int id):
_id(id),
_ambientColor(0,0,0,1),
_diffuseColor(1,1,1,1),
_specularColor(1,1,1,1),
_attenuation(1,0,0),
_position(0,0,1,0),
_spotDirection(0,0,-1),
_spotCutOff(180.0),
_spotExponent(0)
{}
Light::~Light(){}

void Light::set(){
	
	chkGLErr();
	int light = GL_LIGHT0+_id;
	glEnable(light);chkGLErr();

	glLightfv(light, GL_AMBIENT, glm::value_ptr(_ambientColor));chkGLErr();
	glLightfv(light, GL_DIFFUSE, glm::value_ptr(_diffuseColor));chkGLErr();
	glLightfv(light, GL_SPECULAR, glm::value_ptr(_specularColor));chkGLErr();
	
	
	chkGLErr();

	glLightf(light, GL_CONSTANT_ATTENUATION,   _attenuation.x);
	glLightf(light, GL_LINEAR_ATTENUATION,     _attenuation.y);
	glLightf(light, GL_QUADRATIC_ATTENUATION,  _attenuation.z);

	glLightfv(light, GL_POSITION, glm::value_ptr(_position));
	
	chkGLErr();
	
	glLightf(light, GL_SPOT_CUTOFF, _spotCutOff);
	glLightf(light, GL_SPOT_EXPONENT, _spotExponent);
	glLightfv(light, GL_SPOT_DIRECTION,  glm::value_ptr(_spotDirection));
	
	chkGLErr();
}

void Light::setAmbientColor(glm::vec4 color){
	_ambientColor = color;
}
void Light::setDiffuseColor(glm::vec4 color){
	_diffuseColor = color;
}
void Light::setSpecularColor(glm::vec4 color){
	_specularColor = color;
}
	
void Light::setConstantAttenuation(float constantAttenuation){
	_attenuation.x = constantAttenuation;
}
void Light::setLinearAttenuation(float linearAttenuation){
	_attenuation.y = linearAttenuation;
}
void Light::setQuadraticAttenuation(float quadraticAttenuation){
	_attenuation.z = quadraticAttenuation;
}

void Light::setPosition(glm::vec3 pos){
	_position = glm::vec4(pos,1);
}
void Light::setDirection(glm::vec3 dir){
	_position = glm::vec4(dir,0);
}
	
void Light::setSpotDir(glm::vec3 dir){
	_spotDirection = dir;
}
void Light::setSpotCutOff(float cutOff){
	_spotCutOff = cutOff;
}
void Light::setSpotExponent(float exponent){
	_spotExponent = exponent;
}

glm::vec4 Light::getAmbientColor()const{
	return _ambientColor;
}
glm::vec4 Light::getDiffuseColor()const{
	return _diffuseColor;
}
glm::vec4 Light::getSpecularColor()const{
	return _specularColor;
}
	
float Light::getConstantAttenuation()const{
	return _attenuation.x;
}
float Light::getLinearAttenuation()const{
	return _attenuation.y;
}
float Light::getQuadraticAttenuation()const{
	return _attenuation.z;
}

glm::vec3 Light::getPosition()const{
	return glm::vec3(_position.x,_position.y,_position.z);
}
glm::vec3 Light::getDirection()const{
	return glm::vec3(_position.x,_position.y,_position.z);
}
	
glm::vec3 Light::getSpotDir()const{
	return _spotDirection;
}
float Light::getSpotCutOff()const{
	return _spotCutOff;
}
float Light::getSpotExponent()const{
	return _spotExponent;
}