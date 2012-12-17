#include "OpenGLInfo.h"

#include <stdint.h>
#ifdef _WIN32
	#include <windows.h>
#endif
#include <glew/gl/glew.h>
#include <gl/GL.h>


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

void OpenGLInfo::printOGLInformation(std::ostream &stream){
	glm::ivec3 maxUnif = getMaxUniformsPerStage();

	stream << "OpenGL Version :" << getOpenGLVersion() << std::endl;
	stream << "Glew   Version :" << getGlewVersion()   << std::endl;
	stream << "GLSL   Version :" << getGLSLVersion()   << std::endl;
	stream << "OpenGL Vendor  :" << getOpenGLVendor()   << std::endl;
	stream << "OpenGL Renderer:" << getOpenGLRenderer()   << std::endl;
	stream << "Max Attribs    :" << getMaxAttribs() << std::endl;
	stream << "Max Uniforms   :" << getMaxUniforms()  << "( " << maxUnif.x << " " << maxUnif.y << " " << maxUnif.z << ")" << std::endl;
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
			std::cerr <<i++ << " GL_INVALID_ENUM @ " << file <<":"<<line << ": An unacceptable value is specified for an enumerated argument. The offending command is ignored and has no other side effect than to set the error flag." << std::endl;
			break;
#endif
#ifdef GL_INVALID_VALUE
		case GL_INVALID_VALUE:
			std::cerr <<i++  << "GL_INVALID_VALUE @ " << file <<":"<<line  << ": A numeric argument is out of range. The offending command is ignored and has no other side effect than to set the error flag." << std::endl;
			break;
#endif
#ifdef GL_INVALID_OPERATION
		case GL_INVALID_OPERATION:
			std::cerr <<i++  << "GL_INVALID_OPERATION @ " << file <<":"<<line  << ": The specified operation is not allowed in the current state. The offending command is ignored and has no other side effect than to set the error flag." << std::endl;
			break;
#endif
#ifdef GL_STACK_OVERFLOW
		case GL_STACK_OVERFLOW:
			std::cerr <<i++  << "GL_STACK_OVERFLOW @ " << file <<":"<<line  << ": This command would cause a stack overflow. The offending command is ignored and has no other side effect than to set the error flag." << std::endl;
			break;
#endif
#ifdef GL_STACK_UNDERFLOW
		case GL_STACK_UNDERFLOW:
			std::cerr <<i++  << "GL_STACK_UNDERFLOW @ " << file <<":"<<line  << ": This command would cause a stack underflow. The offending command is ignored and has no other side effect than to set the error flag." << std::endl;
			break;
#endif
#ifdef GL_OUT_OF_MEMORY
		case GL_OUT_OF_MEMORY:
			std::cerr <<i++  << "GL_OUT_OF_MEMORY @ " << file <<":"<<line  << ": There is not enough memory left to execute the command. The state of the GL is undefined, except for the state of the error flags, after this error is recorded." << std::endl;
			break;
#endif
#ifdef GL_TABLE_TOO_LARGE
		case GL_TABLE_TOO_LARGE:
			std::cerr <<i++  << "GL_TABLE_TOO_LARGE @ " << file <<":"<<line  << ": The specified table exceeds the implementation's maximum supported table size. The offending command is ignored and has no other side effect than to set the error flag." << std::endl;
			break;
#endif
#ifdef GL_INVALID_FRAMEBUFFER_OPERATION
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			std::cerr <<i++  << "GL_INVALID_FRAMEBUFFER_OPERATION @ " << file <<":"<<line  << ": The framebuffer object is not complete. The offending command is ignored and has no other side effect than to set the error flag." << std::endl;
			break;
#endif

		default:
			std::cerr <<i++  << "GL_ERROR @ " << file <<":"<<line  << ": Unkown error:" << err << std::endl;
		}
		err = glGetError();
	}
}
