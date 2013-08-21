#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#ifdef _WIN32
	#include <windows.h>
#endif

#include <glew\GL\glew.h>
#include <GL\GL.h>

#include <map>

#include <iostream>
#include <string>

#include <tinyxml2\tinyxml2.h>


struct TextureHints{
	TextureHints();

	enum {
		repeat = GL_REPEAT,
		clamp = GL_CLAMP
	}wrapping_r,wrapping_s,wrapping_t;

	enum{
		nearest = GL_NEAREST,
		linear = GL_LINEAR,
		mipNearestNearest = GL_NEAREST_MIPMAP_NEAREST,
		mipLinearNearest = GL_LINEAR_MIPMAP_NEAREST,
		mipNearestLinear = GL_NEAREST_MIPMAP_LINEAR,
		mipLinearLinear = GL_LINEAR_MIPMAP_LINEAR
	}minFilter,magFilter;
};

class Texture{
	Texture();
public:
	virtual ~Texture();

	static std::map<std::string,GLuint> _textures;

	static GLuint createTextureID(std::string textureName);

	static GLuint getTextureID(std::string textureName);

	static GLuint loadTexture(tinyxml2::XMLElement * ele);

	static GLuint loadTexture(std::string name,std::string filename,TextureHints hints = TextureHints());
	
	static void screenshot(const char* filename,std::string textureName);
	static void screenshot(const char* filename,GLuint texID);


};

#endif