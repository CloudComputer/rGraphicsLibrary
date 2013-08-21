#include "rObject.h"
#include <boost/algorithm/string.hpp> 

#include <OpenGLHelpers\OpenGLInfo.h>
#include <OpenGLHelpers\Shader.h>

#include "MeshRenderer.h"

#include <glm\gtc\matrix_transform.hpp>

#include <Util\Macros.h>

#include "Scene.h"
#include "Camera.h"
#include "CSGReader.h"

rObject::rObject(){
	_shader = 0;
}


rObject::~rObject(){
	if(_shader != 0){
		delete _shader;
	}
}

void rObject::draw(Scene *s){
	chkGLErr();	
	if(!_shader)
		return;
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
	}else if(type == "csg"){
		CSGReader::ReadXML(obj,ele);
	}

	if(!obj)
		return obj;

	chkGLErr();

	auto c2 = ele->Attribute("id");
	std::string id = (!c2) ? "" : c2;
	if(id.length() != 0)
		obj->setID(id);

	auto transform = ele->FirstChildElement("transform");
	if(transform){
		FOR_XML_ALL_ELE(transform,e){
			auto name = e->Name();
			if(strcmp(name,"translate") == 0){
				std::istringstream iss(e->GetText());
				glm::vec3 offSet = glm::vec3(0,0,0);
				iss >> offSet.x>>offSet.y>>offSet.z;
				obj->_model = glm::translate(obj->_model,offSet);
			}
			if(strcmp(name,"scale") == 0){
				std::istringstream iss(e->GetText());
				glm::vec3 scale = glm::vec3(0,0,0);
				iss >> scale.x>>scale.y>>scale.z;
				obj->_model = glm::scale(obj->_model,scale);
			}
			if(strcmp(name,"rotate") == 0){
				std::istringstream iss(e->GetText());
				glm::vec3 axis = glm::vec3(0,0,0);
				float angle;
				iss >> angle >> axis.x>>axis.y>>axis.z;
				obj->_model = glm::rotate(obj->_model,angle,axis);
			}
		}
	}


	return obj;
}


