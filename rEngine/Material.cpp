#include "Material.h"



Material::Material():
	diffuse(1,1,1,1),
	specular(0,0,0,0),
	ambient(1,1,1)
{
}


Material::~Material(){


}


	
void Material::setDiffuse(const glm::vec3 &col){
	setDiffuse(glm::vec4(col,1.0f));
}
void Material::setSpecular(const glm::vec3 &col){
	specular.x = col.x;
	specular.y = col.y;
	specular.z = col.z;
}
void Material::setAmbient(const glm::vec3 &col){
	ambient = col;
}
void Material::setDiffuse(const glm::vec4 &col){
	diffuse = col;
}
void Material::setSpecularity(const float &f){
	specular.w = f;
}
	
glm::vec4 Material::getDiffuse()const{return diffuse;}
glm::vec3 Material::getSpecular()const{return glm::vec3(specular);}
glm::vec3 Material::getAmbient()const{return ambient;}
float Material::getSpecularity()const{return specular.w;}

