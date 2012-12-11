
#include "FBO.h"

#include "OpenGLInfo.h"
#include "AttribPusher.h"
#include "Shader.h"

#include <iostream>

const std::string vertShader = 
"#version 120\n"
"\n"
"varying vec2 texCoord;\n"
"\n"
"void main()\n"
"{\n"
"	texCoord = gl_MultiTexCoord0.xy;\n"
"	gl_Position =  gl_ModelViewProjectionMatrix * gl_Vertex;\n"
"}\n";
const std::string fragShader = 
	"#version 120\n"
	"varying vec2 texCoord;\n"
	"uniform sampler2D texture;\n"
	"void main(){\n"
	"gl_FragColor = texture2D(texture,texCoord);\n"
	"}\n";

ShaderProgram *shader = 0;

//#define DBG() std::cout << __FUNCTION__ << " " << __FILE__ << " " << __LINE__ << std::endl
#ifndef DBG
#define DBG() 
#endif

FBO::FBO(){
	DBG();
	isInit = false;
	_w = -1;
	_h = -1;
}

FBO::~FBO(){
	DBG();
	if(isInit){
		glDeleteFramebuffersEXT(1,&_fbo);
	}
}
void FBO::init(){
	chkGLErr();
	if(shader == 0){
		chkGLErr();
		shader = new ShaderProgram();
		shader->init();

		VertexShader *v = new VertexShader();
		v->init();
		v->setSouce(vertShader);
		v->compile();
		shader->setShader(v);

		FragmentShader *f = new FragmentShader();
		f->init();
		f->setSouce(fragShader);
		f->compile();
		shader->setShader(f);

		shader->link();
		chkGLErr();
	}
	DBG();
	if(isInit)
		return;
	isInit = true;
	std::cout << "Initizializing framebuffer object" << std::endl;
	

	glGenTextures(1, &_depthTex);
	glBindTexture(GL_TEXTURE_2D, _depthTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, _w, _h, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
	chkGLErr();
	glGenTextures(1, &_texID);
	glBindTexture(GL_TEXTURE_2D, _texID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,  _w, _h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);


	glBindTexture(GL_TEXTURE_2D, 0);chkGLErr();
	
	chkGLErr();
	glGenFramebuffersEXT(1, &_fbo);DBG();chkGLErr();
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, _fbo);DBG();chkGLErr();
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT,  GL_TEXTURE_2D, _depthTex, 0);DBG();
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _texID, 0);DBG();chkGLErr();

	DBG();chkGLErr();
	fboerror();DBG();chkGLErr();
	std::cout << "Completed Initizializing framebuffer object" << std::endl;
	DBG();
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);DBG();


}

void FBO::setSize(uint16_t w,uint16_t h){
	_w = w;
	_h = h;
	std::cout << _w << " " << _h << std::endl;
	if(isInit){
		DBG();
		glBindTexture(GL_TEXTURE_2D, _texID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,  _w, _h, 0, GL_RGBA, GL_UNSIGNED_INT, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		DBG();
	}
}

GLuint FBO::getTexture(){return _texID;}

void FBO::bind(){
	DBG();
	fboerror();
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, _fbo);fboerror();
	glDrawBuffer(GL_COLOR_ATTACHMENT0);fboerror();
	fboerror();
	DBG();
}

void FBO::clear(){
	DBG();
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, _fbo);
	glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	fboerror();
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	glDrawBuffer(GL_BACK);
	DBG();
}


void FBO::unbind(){
	DBG();
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	glDrawBuffer(GL_BACK);
	fboerror();
	DBG();
}

void FBO::render()
{
	DBG();chkGLErr();
	AttribPusher(GL_ALL_ATTRIB_BITS);
	glActiveTexture(GL_TEXTURE0);
	//glUseProgram(0);
	shader->bind();
	glViewport(0, 0, _w, _h);
	
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_CULL_FACE);
	
	glColor4f(1.f, 1.f, 1.f, 1.f);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, 1, 1, 0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	
	glBindTexture(GL_TEXTURE_2D, _texID);chkGLErr();
	
	glBegin(GL_QUADS);
	glTexCoord2f(0.f, 1.f); glVertex2f(0.0f, 0.f);
	glTexCoord2f(0.f, 0.f); glVertex2f(0.0f, 1.0f);
	glTexCoord2f(1.f, 0.f); glVertex2f(1.0f, 1.0f);
	glTexCoord2f(1.f, 1.f); glVertex2f(1.0f, 0.f);
	glEnd();chkGLErr();
	
	glMatrixMode(GL_PROJECTION);chkGLErr();
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);chkGLErr();
	glPopMatrix();chkGLErr();
	
	fboerror();
	chkGLErr();
	DBG();
	shader->unbind();
}

void FBO::fboerror()
{
	DBG();
	chkGLErr();
	DBG();
	GLenum res = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	DBG();
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
	DBG();
}