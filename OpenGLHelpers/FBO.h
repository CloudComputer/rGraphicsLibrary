#pragma once

#include <stdint.h>
#ifdef _WIN32
#include <windows.h>
#endif
#include <glew/GL/glew.h>
#include <GL/GL.h>
#include <glm/glm.hpp>

class FBOa{
	uint16_t _w,_h;
	bool isInit;
	GLuint _fbo,_texID,_depthTex;
public:
	FBOa();
	virtual ~FBOa();
	void init();
	void setSize(uint16_t w,uint16_t h);
	glm::ivec2 getSize(){return glm::ivec2(_w,_h);}
	GLuint getTexture();

	void bind();
	void clear();
	void unbind();

	void fboerror();

	void render(); // render on a fullscreen quad
};
