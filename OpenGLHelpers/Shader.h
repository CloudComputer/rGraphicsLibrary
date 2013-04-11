#ifndef _SHADER_H_
#define _SHADER_H_

#include <stdint.h>
#ifdef _WIN32
	#include <windows.h>
#endif
#include <glew/gl/glew.h>
#include <gl/GL.h>
#include <glm/glm.hpp>


#include <string>
#include <map>

class Shader{
protected:
	GLuint _type,_shader;
	std::string _source;
	Shader();

public:
	virtual ~Shader();
	void setSourceFromFile(std::string filename);
	void setSouce(std::string source);

	GLuint getShader();

	virtual void init() = 0;
	void compile();
};


class VertexShader : public Shader{
public:
	VertexShader(){}
	virtual ~VertexShader(){}
	virtual void init();

	static VertexShader* CreateNewFromFile(std::string filename);
};


class FragmentShader : public Shader{
public:
	FragmentShader(){}
	virtual ~FragmentShader(){}
	virtual void init();

	static FragmentShader* CreateNewFromFile(std::string filename);
};


class ShaderProgram{
	friend class Shader;
	std::map<std::string,GLint> _locations;

	GLuint _programID;
	VertexShader *_vertexShader;
	FragmentShader *_fragmentShader;

	static bool __isInit;
	static std::map<std::string,std::string> __stubbs;
	static void staticInit();
	static void readFunctionSubbs();
public:
	ShaderProgram();
	~ShaderProgram();

	GLint getLocation(std::string loc);

	static void reReadFunctionSubbs();
	void init();
	void setShader(VertexShader *v);
	void setShader(FragmentShader *f);
	void link();

	void bind();
	void unbind();

	void setUniform(std::string name,const GLfloat v)const;
	void setUniform(std::string name,const GLfloat v0,const GLfloat v1)const;
	void setUniform(std::string name,const GLfloat v0,const GLfloat v1,const GLfloat v2)const;
	void setUniform(std::string name,const GLfloat v0,const GLfloat v1,const GLfloat v2,const GLfloat v3)const;
	void setUniform(std::string name,const GLint v)const;
	void setUniform(std::string name,const GLint v0,const GLint v1)const;
	void setUniform(std::string name,const GLint v0,const GLint v1,const GLint v2)const;
	void setUniform(std::string name,const GLint v0,const GLint v1,const GLint v2,const GLint v3)const;
	void setUniform(std::string name,const GLuint v)const;
	void setUniform(std::string name,const GLuint v0,const GLuint v1)const;
	void setUniform(std::string name,const GLuint v0,const GLuint v1,const GLuint v2)const;
	void setUniform(std::string name,const GLuint v0,const GLuint v1,const GLuint v2,const GLuint v3)const;

	void setUniform(std::string name,const glm::vec2 v)const;
	void setUniform(std::string name,const glm::vec3 v)const;
	void setUniform(std::string name,const glm::vec4 v)const;
	void setUniform(std::string name,const glm::ivec2 v)const;
	void setUniform(std::string name,const glm::ivec3 v)const;
	void setUniform(std::string name,const glm::ivec4 v)const;
	void setUniform(std::string name,const glm::uvec2 v)const;
	void setUniform(std::string name,const glm::uvec3 v)const;
	void setUniform(std::string name,const glm::uvec4 v)const;

	void setUniform(std::string name,const glm::mat2 m)const;
	void setUniform(std::string name,const glm::mat2x3 m)const;
	void setUniform(std::string name,const glm::mat2x4 m)const;
	void setUniform(std::string name,const glm::mat3 m)const;
	void setUniform(std::string name,const glm::mat3x2 m)const;
	void setUniform(std::string name,const glm::mat3x4 m)const;
	void setUniform(std::string name,const glm::mat4 m)const;
	void setUniform(std::string name,const glm::mat4x2 m)const;
	void setUniform(std::string name,const glm::mat4x3 m)const;

	void setUniform(std::string name,const glm::dmat2 m)const;
	void setUniform(std::string name,const glm::dmat2x3 m)const;
	void setUniform(std::string name,const glm::dmat2x4 m)const;
	void setUniform(std::string name,const glm::dmat3 m)const;
	void setUniform(std::string name,const glm::dmat3x2 m)const;
	void setUniform(std::string name,const glm::dmat3x4 m)const;
	void setUniform(std::string name,const glm::dmat4 m)const;
	void setUniform(std::string name,const glm::dmat4x2 m)const;
	void setUniform(std::string name,const glm::dmat4x3 m)const;

	void setVertexAttrib(std::string name,const GLfloat v)const;
	void setVertexAttrib(std::string name,const GLfloat v0,const GLfloat v1)const;
	void setVertexAttrib(std::string name,const GLfloat v0,const GLfloat v1,const GLfloat v2)const;
	void setVertexAttrib(std::string name,const GLfloat v0,const GLfloat v1,const GLfloat v2,const GLfloat v3)const;
	void setVertexAttrib(std::string name,const GLint v)const;
	void setVertexAttrib(std::string name,const GLint v0,const GLint v1)const;
	void setVertexAttrib(std::string name,const GLint v0,const GLint v1,const GLint v2)const;
	void setVertexAttrib(std::string name,const GLint v0,const GLint v1,const GLint v2,const GLint v3)const;
	void setVertexAttrib(std::string name,const GLuint v)const;
	void setVertexAttrib(std::string name,const GLuint v0,const GLuint v1)const;
	void setVertexAttrib(std::string name,const GLuint v0,const GLuint v1,const GLuint v2)const;
	void setVertexAttrib(std::string name,const GLuint v0,const GLuint v1,const GLuint v2,const GLuint v3)const;

	void setVertexAttrib(std::string name,const glm::vec2 v)const;
	void setVertexAttrib(std::string name,const glm::vec3 v)const;
	void setVertexAttrib(std::string name,const glm::vec4 v)const;
	void setVertexAttrib(std::string name,const glm::ivec2 v)const;
	void setVertexAttrib(std::string name,const glm::ivec3 v)const;
	void setVertexAttrib(std::string name,const glm::ivec4 v)const;
	void setVertexAttrib(std::string name,const glm::uvec2 v)const;
	void setVertexAttrib(std::string name,const glm::uvec3 v)const;
	void setVertexAttrib(std::string name,const glm::uvec4 v)const;

	void loadLocations();

	static ShaderProgram * CreateShaderProgramFromSources(std::string vertexShader,std::string fragmentShader);
	void setTexture( std::string location, GLenum textureType, int activeTexture, GLuint texture );
};



#endif
