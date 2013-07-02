#include "Camera.h"


#include <boost/algorithm/string.hpp>   
#include <glm\gtc\matrix_transform.hpp>

#include <sstream>

Camera::Camera(rWindow *owner):ResizeListener(owner){

}


Camera::~Camera(){
}

void Camera::onResize(glm::ivec2 newSize){
	if(perspective)
		_projection = glm::perspective(fov,float(newSize.x)/newSize.y,nearPlane,farPlane);
	else
		_projection = glm::ortho(left,right,bottom,top,nearPlane,farPlane);
}

Camera* Camera::CreateCamera(rWindow *win,tinyxml2::XMLElement *ele){
	std::string type = ele->Attribute("type");
	boost::algorithm::to_lower(type);
	
	Camera* cam = 0;

	if(type == "fps"){
		glm::vec3 p=glm::vec3(0,0,0),d=glm::vec3(0,0,-1);
		auto pos = ele->FirstChildElement("position");
		auto dir = ele->FirstChildElement("direction");

		if(pos){
			auto s = pos->GetText();
			if(s){
				std::istringstream iss(s);
				iss >> p.x >> p.y >> p.z;
			}
		}
		if(dir){
			auto s = dir->GetText();
			if(s){
				std::istringstream iss(s);
				iss >> d.x >> d.y >> d.z;
			}
		}
		auto fps = new FPSCamera(p,d,win);
		cam = fps;
		
		auto fly = ele->FirstChildElement("allowFlying");
		fps->alowFlying = (!fly) ? 0 : 1;
		

	}


	if(cam == 0)
		return cam;
	
	auto proj = ele->FirstChildElement("projection");
	if(!proj){
		cam->fov = 45;
		cam->nearPlane = 0.1;
		cam->farPlane = 100;
		cam->perspective = true;
	}else{
		std::string projtype = proj->Attribute("type");
		boost::algorithm::to_lower(projtype);
		cam->perspective = projtype == "perspective";
		if(cam->perspective){
			cam->nearPlane = proj->FloatAttribute("near");
			cam->farPlane = proj->FloatAttribute("far");
			cam->fov = proj->FloatAttribute("fov");
		}else{
			cam->nearPlane = proj->FloatAttribute("near");
			cam->farPlane = proj->FloatAttribute("far");
			cam->right = proj->FloatAttribute("right");
			cam->left = proj->FloatAttribute("left");
			cam->top = proj->FloatAttribute("top");
			cam->bottom = proj->FloatAttribute("bottom");
		}
	}
	cam->onResize(win->getSize());

	
	
	
	
	return cam;
}