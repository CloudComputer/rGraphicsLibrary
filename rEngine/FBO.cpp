#include "FBO.h"

#include "Texture.h"

#include <Util\Macros.h>
#include <Util\Logger.h>

FBO::FBO(rWindow *owner):ResizeListener(owner), _size(-1,-1),_isInit(false){

}


FBO::~FBO(){
	if(_isInit){
		glDeleteFramebuffersEXT(1,&_fbo);
	}
}


void FBO::init(std::string depthTexName){
	chkGLErr();
	if(_isInit) return;
	_isInit = true;
	

	_depthTex = Texture::createTextureID(depthTexName);
	glBindTexture(GL_TEXTURE_2D, _depthTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, _size.x, _size.y, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
	
	glBindTexture(GL_TEXTURE_2D, 0);
	chkGLErr();


	glGenFramebuffersEXT(1, &_fbo);
	bind();
	
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT,  GL_TEXTURE_2D, _depthTex, 0);
	fboerror();

	
	unbind();
	
	chkGLErr();

	LOG_INFO("FBO initzilized with size " << _size << " and depth buffer " << depthTexName);

}

glm::ivec2 FBO::getSize()const{return _size;}

void FBO::createRenderTarget(std::string textureName){
	chkGLErr();
	bind();
	auto texID = Texture::createTextureID(textureName);
	glBindTexture(GL_TEXTURE_2D, texID);
	chkGLErr();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	chkGLErr();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	chkGLErr();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,  _size.x, _size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	chkGLErr();
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0+_renderTargets.size(), GL_TEXTURE_2D, texID, 0);
	chkGLErr();
	
	chkGLErr();
	_renderTargets.push_back(GL_COLOR_ATTACHMENT0+_renderTargets.size());
	_textures.push_back(texID);
	chkGLErr();

	unbind();
	LOG_INFO("Render target " << textureName << " created with size" << _size);
}

void FBO::onResize(glm::ivec2 newSize){
	_size = newSize;
	if(_isInit){
		glBindTexture(GL_TEXTURE_2D, _depthTex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, _size.x, _size.y, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
		IT_FOR(_textures,tex){
			glBindTexture(GL_TEXTURE_2D, *tex);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,  _size.x, _size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		}
		glBindTexture(GL_TEXTURE_2D, 0);
		bind();
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT,  GL_TEXTURE_2D, _depthTex, 0);
		
		int i = 0;
		IT_FOR(_textures,tex){
			glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0+i++, GL_TEXTURE_2D, *tex, 0);
		}
		unbind();
		glBindTexture(GL_TEXTURE_2D, 0);
	chkGLErr();
	}
	LOG_INFO("FBO Resized to " << _size);
	
}

void FBO::bind(){
	glBindTexture(GL_TEXTURE_2D, _depthTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	glBindTexture(GL_TEXTURE_2D, 0);

	chkGLErr();
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, _fbo);
	chkGLErr();
	//GLenum *asdf = &_renderTargets[0];
	//std::cout << _renderTargets.size() << std::endl;
	if(_renderTargets.size()!=0)
		glDrawBuffers(_renderTargets.size(),&_renderTargets[0]);
	chkGLErr();
	glViewport(0,0,_size.x, _size.y);
}

void FBO::clear(){
	chkGLErr();
	bind();
	chkGLErr();
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	chkGLErr();
	unbind();
	chkGLErr();
}

void FBO::unbind(){
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	
	glBindTexture(GL_TEXTURE_2D, _depthTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void FBO::fboerror(){
	chkGLErr();
	GLenum res = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	switch(res)
	{
	case GL_FRAMEBUFFER_COMPLETE_EXT:
		//printf("FRAMEBUFFER_COMPLETE\n");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
		printf("FRAMEBUFFER_INCOMPLETE_ATTACHMENT\n");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
		printf("FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT\n");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
		printf("FRAMEBUFFER_INCOMPLETE_DIMENSIONS\n");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
		printf("FRAMEBUFFER_INCOMPLETE_FORMATS\n");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
		printf("FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER\n");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
		printf("FRAMEBUFFER_INCOMPLETE_READ_BUFFER\n");
		break;
	case GL_FRAMEBUFFER_UNSUPPORTED:
		printf("FRAMEBUFFER_UNSUPPORTED\n");
		break;
	case GL_FRAMEBUFFER_UNDEFINED:
		printf("GL_FRAMEBUFFER_UNDEFINED \n");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE :
		printf("GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE  \n");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS  :
		printf("GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS   \n");
		break;
	default:
		printf("paws\n");
		break;
	}
}

