#ifndef _OPENGLINFO_H_
#define _OPENGLINFO_H_

#include <stdint.h>
#ifdef _WIN32
	#include <windows.h>
#endif
#include <glew/gl/glew.h>
#include <gl/GL.h>


#include <string>
#include <glm/glm.hpp>

#define chkGLErr() OpenGLInfo::checkGLErrors(__FILE__,__LINE__)

class OpenGLInfo{
	OpenGLInfo();
public:
	static std::string getOpenGLVendor();
	static std::string getOpenGLRenderer();
	static std::string getOpenGLVersion();
	static std::string getOpenGLMaxLights();
	static std::string getGLSLVersion();
	static std::string getGlewVersion();
	
	static int getMaxUniforms();
	static glm::ivec3 getMaxUniformsPerStage();
	static int getMaxAttribs();

	static void printOGLInformation(std::ostream &stream);

	static void checkGLErrors(std::string file = "", int line = -1);
};


#endif