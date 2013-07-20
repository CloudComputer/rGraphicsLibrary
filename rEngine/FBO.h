#ifndef _FBO_H_
#define _FBO_H_

#include <glm\glm.hpp>

#include "rWindow.h"

class FBO : public ResizeListener{
	glm::ivec2 _size;
	GLuint _fbo,_depthTex;
	std::vector<GLenum> _renderTargets;
	std::vector<GLuint> _textures;

	bool _isInit;
public:
	FBO(rWindow *owner);
	virtual ~FBO();

	void init(std::string depthTexName,const glm::ivec2 &size){onResize(size);init(depthTexName);}
	void init(std::string depthTexName);

	void createRenderTarget(std::string textureName);

	virtual void onResize(glm::ivec2 newSize);
	glm::ivec2 getSize()const;

	void bind();
	void clear();

	void unbind();

	void fboerror();

};

#endif