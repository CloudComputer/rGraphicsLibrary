#include "OpenGLInfo.h"

#include <stdint.h>
#ifdef _WIN32
	#include <windows.h>
#endif
#include <glew/gl/glew.h>
#include <gl/GL.h>

#include <Util\Logger.h>

#include <iostream>
#include <string>
#include <sstream>

std::string OpenGLInfo::getOpenGLVendor(){
	std::stringstream s;
	s << glGetString(GL_VENDOR);
	return s.str();
}

std::string OpenGLInfo::getOpenGLRenderer(){
	std::stringstream s;
	s << glGetString(GL_RENDERER);
	return s.str();
}

std::string OpenGLInfo::getOpenGLVersion(){
	std::stringstream s;
	s << glGetString(GL_VERSION);
	return s.str();
}
  
std::string OpenGLInfo::getOpenGLMaxLights(){
	int param;
	glGetIntegerv(GL_MAX_LIGHTS, &param);
	std::stringstream s;
	s << param;
	return s.str();
}

std::string OpenGLInfo::getGLSLVersion(){
	std::stringstream s;
	s << glGetString(GL_SHADING_LANGUAGE_VERSION);
	return s.str();
}

std::string OpenGLInfo::getGlewVersion(){
	std::stringstream s;
	s << glewGetString(GLEW_VERSION);
	return s.str();

}

int OpenGLInfo::getMaxUniforms(){
	int i;
	glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE,&i);
	return i;
}

glm::ivec3 OpenGLInfo::getMaxUniformsPerStage(){
	glm::ivec3 i;
	glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS,&i[0]);
	glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS,&i[1]);
	glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS,&i[2]);
	return i;
}


int OpenGLInfo::getMaxAttribs(){
	int i;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS,&i);
	return i;
}

void OpenGLInfo::printOGLInformation(){
	glm::ivec3 maxUnif = getMaxUniformsPerStage();

	LOG_INFO("OpenGL Version :" << getOpenGLVersion());
	LOG_INFO("Glew   Version :" << getGlewVersion());
	LOG_INFO( "GLSL   Version :" << getGLSLVersion());
	LOG_INFO( "OpenGL Vendor  :" << getOpenGLVendor());
	LOG_INFO( "OpenGL Renderer:" << getOpenGLRenderer());
	LOG_INFO( "Max Attribs    :" << getMaxAttribs());
	LOG_INFO( "Max Uniforms   :" << getMaxUniforms()  << "( " << maxUnif.x << " " << maxUnif.y << " " << maxUnif.z << ")");
}

void OpenGLInfo::checkGLErrors(std::string file, int line){
	int i = 0;
	GLenum err = glGetError();
	while(err != GL_NO_ERROR){
		static int ___i = 0;
		if(___i++ >= 3)
			return;
		switch (err){
		case GL_NO_ERROR:
			break;
#ifdef GL_INVALID_ENUM
		case GL_INVALID_ENUM:
			LOG_ERROR(i++ << " GL_INVALID_ENUM @ " << file <<":"<<line << ": An unacceptable value is specified for an enumerated argument. The offending command is ignored and has no other side effect than to set the error flag.");
			break;
#endif
#ifdef GL_INVALID_VALUE
		case GL_INVALID_VALUE:
			LOG_ERROR(i++  << "GL_INVALID_VALUE @ " << file <<":"<<line  << ": A numeric argument is out of range. The offending command is ignored and has no other side effect than to set the error flag.");
			break;
#endif
#ifdef GL_INVALID_OPERATION
		case GL_INVALID_OPERATION:
			LOG_ERROR(i++  << "GL_INVALID_OPERATION @ " << file <<":"<<line  << ": The specified operation is not allowed in the current state. The offending command is ignored and has no other side effect than to set the error flag.");
			break;
#endif
#ifdef GL_STACK_OVERFLOW
		case GL_STACK_OVERFLOW:
			LOG_ERROR(i++  << "GL_STACK_OVERFLOW @ " << file <<":"<<line  << ": This command would cause a stack overflow. The offending command is ignored and has no other side effect than to set the error flag.");
			break;
#endif
#ifdef GL_STACK_UNDERFLOW
		case GL_STACK_UNDERFLOW:
			LOG_ERROR(i++  << "GL_STACK_UNDERFLOW @ " << file <<":"<<line  << ": This command would cause a stack underflow. The offending command is ignored and has no other side effect than to set the error flag.");
			break;
#endif
#ifdef GL_OUT_OF_MEMORY
		case GL_OUT_OF_MEMORY:
			LOG_ERROR(i++  << "GL_OUT_OF_MEMORY @ " << file <<":"<<line  << ": There is not enough memory left to execute the command. The state of the GL is undefined, except for the state of the error flags, after this error is recorded.");
			break;
#endif
#ifdef GL_TABLE_TOO_LARGE
		case GL_TABLE_TOO_LARGE:
			LOG_ERROR(i++  << "GL_TABLE_TOO_LARGE @ " << file <<":"<<line  << ": The specified table exceeds the implementation's maximum supported table size. The offending command is ignored and has no other side effect than to set the error flag.");
			break;
#endif
#ifdef GL_INVALID_FRAMEBUFFER_OPERATION
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			LOG_ERROR(i++  << "GL_INVALID_FRAMEBUFFER_OPERATION @ " << file <<":"<<line  << ": The framebuffer object is not complete. The offending command is ignored and has no other side effect than to set the error flag.");
			break;
#endif

		default:
			LOG_ERROR(i++  << "GL_ERROR @ " << file <<":"<<line  << ": Unkown error:" << err);		}
		err = glGetError();
	}
}
