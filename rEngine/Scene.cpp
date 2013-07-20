#include "Scene.h"

#include <OpenGLHelpers\OpenGLInfo.h>

#include <Util\Macros.h>
#include <iostream>

#include <OpenGLHelpers\Light.h>
#include <OpenGLHelpers\Shader.h>

#include "rObject.h"
#include "FBO.h"
#include "Camera.h"
#include "Texture.h"

Scene::Scene(){
	_cam = 0;
	_fbo = 0;
	_LightsShader = ShaderProgram::CreateShaderProgramFromSources(GLSL_DIR"/rScene.vert",GLSL_DIR"/rScene.frag");
	_CombieShader = ShaderProgram::CreateShaderProgramFromSources(GLSL_DIR"/rSceneCombine.vert",GLSL_DIR"/rSceneCombine.frag");
}


Scene::~Scene(){
	for(int i = 0;i<_lights.size();i++){
		delete _lights[i];
	}
	for(int i = 0;i<_objects.size();i++){
		delete _objects[i];
	}
	if(_cam)
		delete _cam;
	if(_fbo)
		delete _fbo;
}


void Scene::setActive(bool active){_active = active;}
bool Scene::isActive(){return _active;}

void Scene::update(float dt){
	IT_FOR(_objects,o) (*o)->update(dt);
}
void Scene::render(){
	chkGLErr();
	PassScene();
	chkGLErr();
	PassLight();
	chkGLErr();
	PassCombine();
	chkGLErr();
}

void Scene::PassScene(){
	glEnable(GL_DEPTH_TEST);
	_fbo->clear();
	_fbo->bind();

	chkGLErr();
	IT_FOR(_objects,o){
		(*o)->draw(this);
	}
	_fbo->unbind();
}

void Scene::PassLight(){
	glDisable(GL_DEPTH_TEST);
	_LightsShader->bind();
	_lightFBO->clear();
	_lightFBO->bind();
	IT_FOR(_lights,light){
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glOrtho(0,1,0,1,-1,1);
	

		glMatrixMode(GL_MODELVIEW);
		
		
		_LightsShader->setTexture("inSpecular",GL_TEXTURE_2D,1,Texture::getTextureID("scene_Specular"));chkGLErr();
		//_LightsShader->setTexture("inAmbient",GL_TEXTURE_2D,2,Texture::getTextureID("scene_Ambient"));chkGLErr();
		_LightsShader->setTexture("inNormal",GL_TEXTURE_2D,3,Texture::getTextureID("scene_Normal"));chkGLErr();
		_LightsShader->setTexture("inDepth",GL_TEXTURE_2D,4,Texture::getTextureID("scene_depthTex"));chkGLErr();
		_LightsShader->setTexture("inDiffuse",GL_TEXTURE_2D,0,Texture::getTextureID("scene_Diffuse"));chkGLErr();
		
		_LightsShader->setUniform("camMatrix",_cam->getCameraMatrix());chkGLErr();
		_LightsShader->setUniform("projMatrix",_cam->getProjectionMatrix());chkGLErr();
		
		_LightsShader->setUniform("lightPos",(*light)->getPosition());chkGLErr();
		_LightsShader->setUniform("lightColor",glm::vec3((*light)->getDiffuseColor()));chkGLErr();

		glLoadIdentity();

		glBegin(GL_QUADS);
		glColor4f(1,1,0,1);
		glTexCoord2f(0,0);
		glVertex2f(0,0);

		glTexCoord2f(1,0);
		glVertex2f(1,0);

		glTexCoord2f(1,1);
		glVertex2f(1,1);

		glTexCoord2f(0,1);
		glVertex2f(0,1);

		glEnd();


	
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
	}
	_lightFBO->unbind();
	

	_LightsShader->unbind();
	
	chkGLErr();
}

void Scene::PassCombine(){
	glDisable(GL_DEPTH_TEST);
	
	_CombieShader->bind();
	_combineFBO->clear();
	_combineFBO->bind();
	
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glOrtho(0,1,0,1,-1,1);
	

	glMatrixMode(GL_MODELVIEW);
		
		
	/*_CombieShader->setTexture("inAmbient",GL_TEXTURE_2D,2,Texture::getTextureID("scene_Ambient"));chkGLErr();
	_CombieShader->setTexture("inNormal",GL_TEXTURE_2D,3,Texture::getTextureID("scene_Normal"));chkGLErr();
	_CombieShader->setTexture("inDepth",GL_TEXTURE_2D,4,Texture::getTextureID("scene_depthTex"));chkGLErr();
	*/
	_CombieShader->setTexture("inSpecular",GL_TEXTURE_2D,1,Texture::getTextureID("scene_light_specular"));chkGLErr();
	_CombieShader->setTexture("inDiffuse",GL_TEXTURE_2D,0,Texture::getTextureID("scene_light_diffuse"));chkGLErr();
		
	//_CombieShader->setUniform("camMatrix",_cam->getCameraMatrix());chkGLErr();
	//_CombieShader->setUniform("projMatrix",_cam->getProjectionMatrix());chkGLErr();

	glLoadIdentity();

	glBegin(GL_QUADS);
	glColor4f(1,1,0,1);
	glTexCoord2f(0,0);
	glVertex2f(0,0);

	glTexCoord2f(1,0);
	glVertex2f(1,0);

	glTexCoord2f(1,1);
	glVertex2f(1,1);

	glTexCoord2f(0,1);
	glVertex2f(0,1);

	glEnd();


	
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	
	_combineFBO->unbind();
	_CombieShader->unbind();



}

void Scene::createLight(tinyxml2::XMLElement *lightEle){
	auto c = lightEle->Attribute("type");
	std::string type = "point";
	if(c) type = c;
	if(type == "point"){
		glm::vec3 pos;
		glm::ivec3 color;
		auto posEle = lightEle->FirstChildElement("position");
		auto colorEle = lightEle->FirstChildElement("color");
		auto posText = posEle->GetText();
		auto colorText = colorEle->GetText();
		if(!posText || ! colorText){
			std::cerr << "Color or position is not set for light" << std::endl;
		}else{
			std::istringstream posIss(posText);
			std::istringstream colorIss(colorText);
			if(!(posIss >> pos.x >> pos.y >> pos.z)){
				std::cerr << "Failed to reading position information" << std::endl;
			}else if(!(colorIss >> color.x >> color.y >> color.z)){
				std::cerr << "Failed to reading color information" << std::endl;
			}else{
				Light* l = new Light(_lights.size()); 
				l->setDiffuseColor(glm::vec4(glm::vec3(color)*glm::vec3(1.0/255,1.0/255,1.0/255),1.0));
				l->setPosition(pos);
				_lights.push_back(l);
			}
		}
	}else{
		std::cerr << "No code for creating light of type " << type << std::endl;
	}
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
	
	s->_lightFBO = new FBO(win);
	s->_lightFBO->init(id+"_light_depthTex",win->getSize());
	s->_lightFBO->createRenderTarget(id+"_light_diffuse");
	s->_lightFBO->createRenderTarget(id+"_light_specular");
	
	
	s->_combineFBO = new FBO(win);
	s->_combineFBO->init(id+"_combine_depthTex",win->getSize());
	s->_combineFBO->createRenderTarget(id+"_combine");
	

	auto camera = ele->FirstChildElement("camera");
	chkGLErr();
	if(camera){
		s->_cam = Camera::CreateCamera(win,camera);
	}else{
		s->_cam = new Camera(win);
	}
	chkGLErr();

	FOR_XML_ELE("light",ele,light){s->createLight(light);}

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