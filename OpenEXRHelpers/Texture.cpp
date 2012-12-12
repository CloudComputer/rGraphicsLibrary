
#include "OpenEXRIncludes.h"

GLuint OpenEXRTexture::getAsTexture(RGBABuffer buffer){
	GLenum target = GL_TEXTURE_RECTANGLE_NV;

	GLuint imageTexture;
	glGenTextures(1, &imageTexture);
	glBindTexture(target, imageTexture);
	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexImage2D(target, 0, GL_FLOAT_RGBA16_NV, buffer.dim.x, buffer.dim.y, 0,
		GL_RGBA, GL_HALF_FLOAT_NV, buffer.buffer);

	return imageTexture;
}