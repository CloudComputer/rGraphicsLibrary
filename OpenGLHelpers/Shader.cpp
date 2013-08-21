
#include "Shader.h"
#include "OpenGLInfo.h"

#include <Util\Directory.h>
#include <Util\Macros.h>

#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <sstream>

bool ShaderProgram::__isInit = false;
std::map<std::string,std::string> ShaderProgram::__stubbs;

void ShaderProgram::staticInit(){
	readFunctionSubbs();

	__isInit = true;
}

void ShaderProgram::reReadFunctionSubbs(){
	__stubbs.clear();
	readFunctionSubbs();
}

GLint ShaderProgram::getLocation(std::string loc){
	return _locations[loc];
}

void ShaderProgram::readFunctionSubbs(){
	std::vector<std::string> files = Directory::getAllFilesInFolder(GLSL_DIR,"\.rgl$");

	std::vector<std::string>::iterator file;
	for(file = files.begin();file != files.end();++file){
		std::ifstream f(file->c_str());
		
		std::stringstream s;
		s << f.rdbuf();
		//std::cout << s << std::endl;
		std::string source = s.str(),name;
		if(source.substr(0,4).compare("#RGL")!=0){
			std::cerr << "Error loading " << file->c_str() << "" << std::endl;
			std::cerr << '\t' << "Must start with #RGL [TEMPLATE NAME]" << std::endl;
			continue;
		}
		int i = source.find("\n");
		name = source.substr(0,i);
		source = source.substr(i);
		__stubbs[name] = source;
	}

}

Shader::Shader(){
	_shader = -1;
}

Shader::~Shader(){
	if(_shader != -1)
		glDeleteShader(_shader);
}



GLuint Shader::getShader(){
	return _shader;
}

void Shader::setSourceFromFile(std::string filename,std::vector<std::string> defines){
	std::ifstream f(filename.c_str());
	assert(f.is_open());

	std::stringstream s;
	s << f.rdbuf();
	setSouce(s.str(),defines);
}

void Shader::setSouce(std::string source,std::vector<std::string> defines){
	std::stringstream ss;
	IT_FOR(defines,def){
		ss << "#define " << *def << std::endl;
	}
	ss << source;
	_source = ss.str();
	static std::map<std::string,std::string>::iterator itr;
	int changes = 0;
	do{
		changes = 0;
		for(itr = ShaderProgram::__stubbs.begin();itr!=ShaderProgram::__stubbs.end();++itr){
			int pos = _source.find(itr->first);
			if(pos != -1){
				_source.replace(pos, itr->first.length(), itr->second);
				changes++;
			}
		}
	}while(changes);

}

void VertexShader::init(){
	_shader = glCreateShader(GL_VERTEX_SHADER);
}


void FragmentShader::init(){
	_shader = glCreateShader(GL_FRAGMENT_SHADER);
}

void Shader::compile(){
	int l = _source.size();
	GLchar *c = new GLchar[l+1];
	strcpy(c,_source.c_str());
	glShaderSource(_shader, 1,(const GLchar **)&c, &l);
	glCompileShader(_shader);

	int mL;
	glGetShaderiv(_shader, GL_INFO_LOG_LENGTH, &mL);
	if(mL != 1){
		char * log = new char[mL];
		glGetShaderInfoLog(_shader, mL, &mL, log);
		//TODO write out the line with error (maybe plusminus 1 row)
		std::cout << log << std::endl;
		delete [] log;
	}
}

VertexShader* VertexShader::CreateNewFromFile(std::string filename,std::vector<std::string> defines){
	VertexShader *v = new VertexShader();
	v->init();
	v->setSourceFromFile(filename,defines);
	v->compile();
	return v;
}

FragmentShader* FragmentShader::CreateNewFromFile(std::string filename,std::vector<std::string> defines){
	FragmentShader *f = new FragmentShader();
	f->init();
	f->setSourceFromFile(filename,defines);
	f->compile();
	return f;
}

ShaderProgram::ShaderProgram(){
	_programID =  0;
}

ShaderProgram::~ShaderProgram(){
	if(_programID != 0){
		glDeleteProgram(_programID);
	}
	delete _vertexShader;
	delete _fragmentShader;
}

void ShaderProgram::init(){
	if(!ShaderProgram::__isInit){
		ShaderProgram::staticInit();
	}
	_programID = glCreateProgram();
}

void ShaderProgram::setShader(VertexShader *v){
	_vertexShader = v;
}

void ShaderProgram::setShader(FragmentShader *f){
	_fragmentShader = f;
}

void ShaderProgram::link(){
	chkGLErr();
	glAttachShader(_programID,_vertexShader->getShader()); //TODO this should only be done once
	glAttachShader(_programID,_fragmentShader->getShader());
	relink();
}
void ShaderProgram::relink(){
	glLinkProgram(_programID);
	chkGLErr();
	GLint err;
	glGetProgramiv(_programID, GL_LINK_STATUS, &err);
	if(!err){
		int mL;
        glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &mL);
        char * log = new char[mL];
        glGetProgramInfoLog(_programID, mL, &mL, log);
        std::cout << "Could not Link shaders: " << std::endl;
        std::cout << log << std::endl;
        delete [] log;
		chkGLErr();
	}
	chkGLErr();
	loadLocations();
}


void ShaderProgram::loadLocations(){
	chkGLErr();
	GLint activeUniforms,activeAttributes;
	bind();
	glGetProgramiv(_programID,GL_ACTIVE_UNIFORMS,&activeUniforms);
	glGetProgramiv(_programID,GL_ACTIVE_ATTRIBUTES,&activeAttributes);
	chkGLErr();
	for(int i = 0;i<activeUniforms;i++){
		GLsizei l;
		GLint s;
		GLenum t;
		GLchar n[256];
		glGetActiveUniform(_programID,i,256,&l,&s,&t,n);
		_locations[n] = glGetUniformLocation(_programID,n);
		//std::cout << n << " " << _locations[n] << std::endl;
		chkGLErr();
	}
	chkGLErr();
	for(int i = 0;i<activeAttributes;i++){
		GLsizei l;
		GLint s;
		GLenum t;
		GLchar n[256];
		glGetActiveAttrib(_programID,i,256,&l,&s,&t,n);
		_locations[n] = glGetAttribLocation(_programID,n);
		//std::cout << n << " " << _locations[n] << std::endl;
		chkGLErr();
	}
	chkGLErr();
	unbind();
	chkGLErr();
}

ShaderProgram *ShaderProgram::CreateShaderProgramFromSources(std::string vertexShader,std::string fragmentShader,std::vector<std::string> defines){
	ShaderProgram *p = new ShaderProgram();chkGLErr();
	p->init();chkGLErr();
	p->setShader(VertexShader::CreateNewFromFile(vertexShader,defines));chkGLErr();
	p->setShader(FragmentShader::CreateNewFromFile(fragmentShader,defines));chkGLErr();
	p->link();chkGLErr();
	p->loadLocations();chkGLErr();
	return p;
}


void ShaderProgram::bind(){
	if(_programID)
		glUseProgram(_programID);
}
void ShaderProgram::unbind(){
	glUseProgram(0);
}

void ShaderProgram::setUniform(std::string name,const GLfloat v)const{
	glUniform1f(_locations.find(name)->second,v);
}
void ShaderProgram::setUniform(std::string name,const GLfloat v0,const GLfloat v1)const{
	glUniform2f(_locations.find(name)->second,v0,v1);
}
void ShaderProgram::setUniform(std::string name,const GLfloat v0,const GLfloat v1,const GLfloat v2)const{
	glUniform3f(_locations.find(name)->second,v0,v1,v2);
}
void ShaderProgram::setUniform(std::string name,const GLfloat v0,const GLfloat v1,const GLfloat v2,const GLfloat v3)const{
	glUniform4f(_locations.find(name)->second,v0,v1,v2,v3);
}
void ShaderProgram::setUniform(std::string name,const GLint v)const{
	chkGLErr();
	glUniform1i(_locations.find(name)->second,v);
	chkGLErr();
}
void ShaderProgram::setUniform(std::string name,const GLint v0,const GLint v1)const{
	glUniform2i(_locations.find(name)->second,v0,v1);
}
void ShaderProgram::setUniform(std::string name,const GLint v0,const GLint v1,const GLint v2)const{
	glUniform3i(_locations.find(name)->second,v0,v1,v2);
}
void ShaderProgram::setUniform(std::string name,const GLint v0,const GLint v1,const GLint v2,const GLint v3)const{
	glUniform4i(_locations.find(name)->second,v0,v1,v2,v3);
}

void ShaderProgram::setUniform(std::string name,const GLuint v)const{
	glUniform1ui(_locations.find(name)->second,v);
}
void ShaderProgram::setUniform(std::string name,const GLuint v0,const GLuint v1)const{
	glUniform2ui(_locations.find(name)->second,v0,v1);
}
void ShaderProgram::setUniform(std::string name,const GLuint v0,const GLuint v1,const GLuint v2)const{
	glUniform3ui(_locations.find(name)->second,v0,v1,v2);
}
void ShaderProgram::setUniform(std::string name,const GLuint v0,const GLuint v1,const GLuint v2,const GLuint v3)const{
	glUniform4ui(_locations.find(name)->second,v0,v1,v2,v3);
}

void ShaderProgram::setUniform(std::string name,const glm::vec2 v)const{
	glUniform2fv(_locations.find(name)->second,1,glm::value_ptr(v));
}
void ShaderProgram::setUniform(std::string name,const glm::vec3 v)const{
	glUniform3fv(_locations.find(name)->second,1,glm::value_ptr(v));
}
void ShaderProgram::setUniform(std::string name,const glm::vec4 v)const{
	glUniform4fv(_locations.find(name)->second,1,glm::value_ptr(v));
}
void ShaderProgram::setUniform(std::string name,const glm::ivec2 v)const{
	glUniform2iv(_locations.find(name)->second,1,glm::value_ptr(v));
}
void ShaderProgram::setUniform(std::string name,const glm::ivec3 v)const{
	glUniform3iv(_locations.find(name)->second,1,glm::value_ptr(v));
}
void ShaderProgram::setUniform(std::string name,const glm::ivec4 v)const{
	glUniform4iv(_locations.find(name)->second,1,glm::value_ptr(v));
}
void ShaderProgram::setUniform(std::string name,const glm::uvec2 v)const{
	glUniform2uiv(_locations.find(name)->second,1,glm::value_ptr(v));
}
void ShaderProgram::setUniform(std::string name,const glm::uvec3 v)const{
	glUniform3uiv(_locations.find(name)->second,1,glm::value_ptr(v));
}
void ShaderProgram::setUniform(std::string name,const glm::uvec4 v)const{
	glUniform4uiv(_locations.find(name)->second,1,glm::value_ptr(v));
}

void ShaderProgram::setUniform(std::string name,const glm::mat2 m)const{
	glUniformMatrix2fv(_locations.find(name)->second,1,0,glm::value_ptr(m));
}
void ShaderProgram::setUniform(std::string name,const glm::mat2x3 m)const{
	glUniformMatrix2x3fv(_locations.find(name)->second,1,0,glm::value_ptr(m));
}
void ShaderProgram::setUniform(std::string name,const glm::mat2x4 m)const{
	glUniformMatrix2x4fv(_locations.find(name)->second,1,0,glm::value_ptr(m));
}
void ShaderProgram::setUniform(std::string name,const glm::mat3 m)const{
	glUniformMatrix3fv(_locations.find(name)->second,1,0,glm::value_ptr(m));
}
void ShaderProgram::setUniform(std::string name,const glm::mat3x2 m)const{
	glUniformMatrix3x2fv(_locations.find(name)->second,1,0,glm::value_ptr(m));
}
void ShaderProgram::setUniform(std::string name,const glm::mat3x4 m)const{
	glUniformMatrix3x4fv(_locations.find(name)->second,1,0,glm::value_ptr(m));
}
void ShaderProgram::setUniform(std::string name,const glm::mat4 m)const{
	glUniformMatrix4fv(_locations.find(name)->second,1,0,glm::value_ptr(m));
}
void ShaderProgram::setUniform(std::string name,const glm::mat4x2 m)const{
	glUniformMatrix4x2fv(_locations.find(name)->second,1,0,glm::value_ptr(m));
}
void ShaderProgram::setUniform(std::string name,const glm::mat4x3 m)const{
	glUniformMatrix4x3fv(_locations.find(name)->second,1,0,glm::value_ptr(m));
}

void ShaderProgram::setUniform(std::string name,const glm::dmat2 m)const{
	glUniformMatrix2dv(_locations.find(name)->second,1,0,glm::value_ptr(m));
}
void ShaderProgram::setUniform(std::string name,const glm::dmat2x3 m)const{
	glUniformMatrix2x3dv(_locations.find(name)->second,1,0,glm::value_ptr(m));
}
void ShaderProgram::setUniform(std::string name,const glm::dmat2x4 m)const{
	glUniformMatrix2x4dv(_locations.find(name)->second,1,0,glm::value_ptr(m));
}
void ShaderProgram::setUniform(std::string name,const glm::dmat3 m)const{
	glUniformMatrix3dv(_locations.find(name)->second,1,0,glm::value_ptr(m));
}
void ShaderProgram::setUniform(std::string name,const glm::dmat3x2 m)const{
	glUniformMatrix3x2dv(_locations.find(name)->second,1,0,glm::value_ptr(m));
}
void ShaderProgram::setUniform(std::string name,const glm::dmat3x4 m)const{
	glUniformMatrix3x4dv(_locations.find(name)->second,1,0,glm::value_ptr(m));
}
void ShaderProgram::setUniform(std::string name,const glm::dmat4 m)const{
	glUniformMatrix4dv(_locations.find(name)->second,1,0,glm::value_ptr(m));
}
void ShaderProgram::setUniform(std::string name,const glm::dmat4x2 m)const{
	glUniformMatrix4x2dv(_locations.find(name)->second,1,0,glm::value_ptr(m));
}
void ShaderProgram::setUniform(std::string name,const glm::dmat4x3 m)const{
	glUniformMatrix4x3dv(_locations.find(name)->second,1,0,glm::value_ptr(m));
}


void ShaderProgram::setVertexAttrib(std::string name,const GLfloat v)const{
	glVertexAttrib1f(_locations.find(name)->second,v);
}
void ShaderProgram::setVertexAttrib(std::string name,const GLfloat v0,const GLfloat v1)const{
	glVertexAttrib2f(_locations.find(name)->second,v0,v1);
}
void ShaderProgram::setVertexAttrib(std::string name,const GLfloat v0,const GLfloat v1,const GLfloat v2)const{
	glVertexAttrib3f(_locations.find(name)->second,v0,v1,v2);
}
void ShaderProgram::setVertexAttrib(std::string name,const GLfloat v0,const GLfloat v1,const GLfloat v2,const GLfloat v3)const{
	glVertexAttrib4f(_locations.find(name)->second,v0,v1,v2,v3);
}
void ShaderProgram::setVertexAttrib(std::string name,const GLint v)const{
	glVertexAttribI1i(_locations.find(name)->second,v);
}
void ShaderProgram::setVertexAttrib(std::string name,const GLint v0,const GLint v1)const{
	glVertexAttribI2i(_locations.find(name)->second,v0,v1);
}
void ShaderProgram::setVertexAttrib(std::string name,const GLint v0,const GLint v1,const GLint v2)const{
	glVertexAttribI3i(_locations.find(name)->second,v0,v1,v2);
}
void ShaderProgram::setVertexAttrib(std::string name,const GLint v0,const GLint v1,const GLint v2,const GLint v3)const{
	glVertexAttribI4i(_locations.find(name)->second,v0,v1,v2,v3);
}

void ShaderProgram::setVertexAttrib(std::string name,const GLuint v)const{
	glVertexAttribI1ui(_locations.find(name)->second,v);
}
void ShaderProgram::setVertexAttrib(std::string name,const GLuint v0,const GLuint v1)const{
	glVertexAttribI2ui(_locations.find(name)->second,v0,v1);
}
void ShaderProgram::setVertexAttrib(std::string name,const GLuint v0,const GLuint v1,const GLuint v2)const{
	glVertexAttribI3ui(_locations.find(name)->second,v0,v1,v2);
}
void ShaderProgram::setVertexAttrib(std::string name,const GLuint v0,const GLuint v1,const GLuint v2,const GLuint v3)const{
	glVertexAttribI4ui(_locations.find(name)->second,v0,v1,v2,v3);
}

void ShaderProgram::setVertexAttrib(std::string name,const glm::vec2 v)const{
	glVertexAttrib2fv(_locations.find(name)->second,glm::value_ptr(v));
}
void ShaderProgram::setVertexAttrib(std::string name,const glm::vec3 v)const{
	glVertexAttrib3fv(_locations.find(name)->second,glm::value_ptr(v));
}
void ShaderProgram::setVertexAttrib(std::string name,const glm::vec4 v)const{
	glVertexAttrib4fv(_locations.find(name)->second,glm::value_ptr(v));
}
void ShaderProgram::setVertexAttrib(std::string name,const glm::ivec2 v)const{
	glVertexAttribI2iv(_locations.find(name)->second,glm::value_ptr(v));
}
void ShaderProgram::setVertexAttrib(std::string name,const glm::ivec3 v)const{
	glVertexAttribI3iv(_locations.find(name)->second,glm::value_ptr(v));
}
void ShaderProgram::setVertexAttrib(std::string name,const glm::ivec4 v)const{
	glVertexAttrib4iv(_locations.find(name)->second,glm::value_ptr(v));
}
void ShaderProgram::setVertexAttrib(std::string name,const glm::uvec2 v)const{
	glVertexAttribI2uiv(_locations.find(name)->second,glm::value_ptr(v));
}
void ShaderProgram::setVertexAttrib(std::string name,const glm::uvec3 v)const{
	glVertexAttribI3uiv(_locations.find(name)->second,glm::value_ptr(v));
}
void ShaderProgram::setVertexAttrib(std::string name,const glm::uvec4 v)const{
	glVertexAttrib4uiv(_locations.find(name)->second,glm::value_ptr(v));
}

void ShaderProgram::setTexture( std::string location, GLenum textureType, int activeTexture, GLuint texture )
{
	chkGLErr();
	setUniform(location,activeTexture);chkGLErr();
	glActiveTexture(GL_TEXTURE0 + activeTexture);chkGLErr();
	glBindTexture(textureType,texture);chkGLErr();
}
