#include "rObject.h"
#include <boost/algorithm/string.hpp> 

#include <OpenGLHelpers\OpenGLInfo.h>
#include <OpenGLHelpers\Shader.h>

#include "MeshRenderer.h"

#include <glm\gtc\matrix_transform.hpp>

#include "Scene.h"
#include "Camera.h"

rObject::rObject(){
}


rObject::~rObject(){
	if(_shader != 0){
		delete _shader;
	}
}

void rObject::draw(Scene *s){
	chkGLErr();	
	_shader->bind();chkGLErr();
	
	_shader->setUniform("projection", s->getCamera()->getProjectionMatrix());chkGLErr();
	_shader->setUniform("camera",s->getCamera()->getCameraMatrix());
	_shader->setUniform("model",_model);chkGLErr();
	onDraw();chkGLErr();
	_shader->unbind();chkGLErr();
}

rObject* rObject::CreateObject(tinyxml2::XMLElement *ele){
	rObject* obj = 0;
	
	auto c =  ele->Name();
	std::string type = (!c) ? "" : c;
	//auto asdf =  ele->Value("type");
	//auto zxvc =  ele->Attribute("type");
	//auto fgh =  ele->Attribute("type");
	boost::algorithm::to_lower(type);
	if(type == "mesh"){
		MeshRenderer::CreateObject(obj,ele);
	}else if(type == "fractal"){
		MeshRenderer::CreateFractal(obj,ele);
	}

	if(!obj)
		return obj;

	chkGLErr();

	auto c2 = ele->Attribute("id");
	std::string id = (!c2) ? "" : c2;
	if(id.length() != 0)
		obj->setID(id);



	return obj;
}


