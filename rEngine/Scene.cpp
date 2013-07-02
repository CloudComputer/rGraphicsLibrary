#include "Scene.h"

#include <OpenGLHelpers\OpenGLInfo.h>

#include <Util\Macros.h>
#include <iostream>



#include "rObject.h"
#include "FBO.h"
#include "Camera.h"

Scene::Scene(){

}


Scene::~Scene(){


}


void Scene::setActive(bool active){_active = active;}
bool Scene::isActive(){return _active;}

void Scene::update(float dt){
	IT_FOR(_objects,o) (*o)->update(dt);
}
void Scene::render(){
	chkGLErr();
	_fbo->clear();
	_fbo->bind();

	chkGLErr();
	IT_FOR(_objects,o){
		(*o)->draw(this);
	}
	chkGLErr();
	_fbo->unbind();
	chkGLErr();
}

Scene* Scene::CreateScene(tinyxml2::XMLElement *ele){
	chkGLErr();
	Scene *s = new Scene();
	auto c = ele->Attribute("id");
	std::string id = (!c) ? "" : c;
	auto c2 = ele->Attribute("window");
	std::string windowID = (!c2) ? "" : c2;
	if(id.length() != 0)
		s->setID(id);
	if(windowID.length() == 0){
		std::cerr << "No window specified for scene" << std::endl;
		exit(-1);
	}
	auto win = IDObject::GetObjectD<rWindow>(windowID);
	if(!win){
		std::cerr << "No window with ID " << windowID << std::endl;
		exit(-1);
	}

	s->_fbo = new FBO(win);
	s->_fbo->init(id+"_depthTex",win->getSize());
	s->_fbo->createRenderTarget(id+"_Diffuse");
	s->_fbo->createRenderTarget(id+"_Specular");
	s->_fbo->createRenderTarget(id+"_Ambient");
	s->_fbo->createRenderTarget(id+"_Normal");
	//s->_fbo->createRenderTarget(id+"_Depth");

	auto camera = ele->FirstChildElement("camera");
	chkGLErr();
	if(camera){
		s->_cam = Camera::CreateCamera(win,camera);
	}else{
		s->_cam = new Camera(win);
	}
	chkGLErr();

	auto child = ele->FirstChildElement();
	while(child){
		rObject* obj = rObject::CreateObject(child);
		if(obj)
			s->_objects.push_back(obj);
		child = child->NextSiblingElement();
		chkGLErr();
	}
	
	chkGLErr();
	return s;
}