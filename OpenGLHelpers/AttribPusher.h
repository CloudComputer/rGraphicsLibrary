#ifndef _ATTRIBPUSHER_H_
#define _ATTRIBPUSHER_H_

#include <stdint.h>
#ifdef _WIN32
#include <windows.h>
#endif
#include <glew/gl/glew.h>
#include <gl/GL.h>

class AttribPusher{
private:
	GLbitfield _attrib;
public:
	AttribPusher(GLbitfield attrib);
	~AttribPusher();

};



#endif

