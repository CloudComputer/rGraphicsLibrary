#ifndef _OPENGLINFO_H_
#define _OPENGLINFO_H_

#include <string>

#define chkGLErr() OpenGLInfo::checkGLErrors(__FILE__,__LINE__)

class OpenGLInfo{
	OpenGLInfo();
public:
	static std::string getOpenGLVendor();
	static std::string getOpenGLRenderer();
	static std::string getOpenGLVersion();
	static std::string getGLSLVersion();
	static std::string getGlewVersion();

	static void checkGLErrors(std::string file = "", int line = -1);
};


#endif