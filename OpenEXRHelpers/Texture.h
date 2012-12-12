#ifndef __OPENEXR_TEXTURE__H_
#define __OPENEXR_TEXTURE__H_

class OpenEXRTexture{
public:
	static GLuint getAsTexture(RGBABuffer buffer);
};

#endif