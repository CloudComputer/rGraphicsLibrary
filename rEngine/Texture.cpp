#include "Texture.h"

#include <IMHelpers\IMHelpers.h>

std::map<std::string,GLuint> Texture::_textures;

TextureHints::TextureHints():
wrapping_r(repeat),
wrapping_s(repeat),
wrapping_t(repeat),
minFilter(mipLinearLinear),
magFilter(linear)
{}

Texture::Texture(){


}


Texture::~Texture(){


}


GLuint Texture::createTextureID(std::string textureName){
	if(_textures.find(textureName) != _textures.end()){
		std::cerr << "Texture with name " << textureName << " already exists (id: " << _textures[textureName] << ")" << std::endl;

		//exit(-1);
		return _textures[textureName];
	}
	_textures[textureName] = 0;
	glGenTextures(1,&_textures[textureName]);

	std::cout << "Created Texture " << textureName << " (id: " << _textures[textureName] << ")" << std::endl;

	return _textures[textureName];
}

GLuint Texture::getTextureID(std::string textureName){
	return _textures[textureName];
}

GLuint Texture::loadTexture(tinyxml2::XMLElement * ele){
	auto name = ele->Attribute("name");
	auto src = ele->Attribute("src");
	if(name && src)
		return loadTexture(name,src);
	return 0;
}

GLuint Texture::loadTexture(std::string name,std::string filename,TextureHints hints){
	int id = createTextureID(name);
	if(id == (unsigned int)-1){
		exit(-1);
		return -1;
	}

	IM_Pointer img = IM_LoadImage(filename.c_str());

	if(!img.get()){
		std::cerr << "Image could not be loaded or file not found" << std::endl;
		return -1;
	}

	if(img->color_space != IM_RGB && img->color_space != IM_GRAY){
		std::cerr << "Unsupported colorspace in texture image: " << filename << std::endl;
		exit(-1);
		return -1;
	}
	if(img->data_type != IM_BYTE ){
		std::cerr << "Unsupported data type in texture image: " << filename << std::endl;
		exit(-1);
		return -1;
	}

	unsigned char *data = (unsigned char *)malloc(img->height * img->width * 4);
	for(int i = 0;i<img->height * img->width;i++){
		float r,g,b,a = 255;
		if(img->color_space == IM_RGB){
			r = ((unsigned char *)img->data[0])[i];
			g = ((unsigned char *)img->data[1])[i];
			b = ((unsigned char *)img->data[2])[i];
		}else if(img->color_space == IM_GRAY){
			r = g = b = ((unsigned char *)img->data[0])[i];
		}

		if(img->has_alpha){
			a = ((unsigned char *)img->data[img->depth])[i];
		}

		data[i*4+0] = r;
		data[i*4+1] = g;
		data[i*4+2] = b;
		data[i*4+3] = a;
	}
	
	glBindTexture(GL_TEXTURE_2D,id);
	
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,hints.minFilter);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,hints.magFilter);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,hints.wrapping_s);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,hints.wrapping_t);
	
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,img->width,img->height,0,GL_RGBA,GL_UNSIGNED_BYTE,data);


	glGenerateMipmap( GL_TEXTURE_2D );

	
	glBindTexture(GL_TEXTURE_2D,0);
	free(data);
	return id;
}
