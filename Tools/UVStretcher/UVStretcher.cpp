#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

#include "../../ext/glm/glm.hpp"


struct Vertex{
	Vertex():
		pos(0,0,0),
		normal(0,0,0),
		uv(0,0),
		F(0,0),
		V(0,0){}

	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec2 uv;

	glm::vec2 F,V;

	bool equals(const Vertex &v)const{
		if(&v == this)
			return true;
		return v.pos == pos && v.uv == uv;
	}

	bool equals(glm::vec3 POS,glm::vec2 UV)const{
		return POS == pos && UV == uv;
	}

};

class Face{
	friend class Mesh;
	std::vector<Vertex*> _vertices;
	float size; //worldspace
	float uv_size; //uv space
	float relativeSize;
	float relativeUVSize;
public:

	void calcSize();
	void calcSizeUV();
	void calcSizes(){calcSize();calcSizeUV();}
	
	float getSize(){return size;}
	float getSizeUV(){return uv_size;}

};


struct Mesh{
	std::vector<Face*> _faces;	
	std::vector<Vertex*> _vertices;

	float meshArea;
	float uvArea;

public:
	static Mesh* ReadMesh(const char* filename);
	void saveMesh(const char* filename);
	
	
	Vertex *createVertex(glm::vec3 pos,glm::vec2 uv,glm::vec3 normal = glm::vec3(0,0,0));

	float getArea(){return meshArea;}
	float getAreaUV(){return uvArea;}

	void calcMeshArea();
	void calcUVArea();
	void calcAreas(){calcMeshArea();calcUVArea();}

	void calcRelativeMeshAreas();
	void calcRelativeUVAreas();
	void calcRelativeAreas(){calcRelativeUVAreas();calcRelativeMeshAreas();}
	
	float getSquaredUVError();


	float work(float dt);
};

Vertex *Mesh::createVertex(glm::vec3 pos,glm::vec2 uv,glm::vec3 normal){
	for(int i = 0;i<_vertices.size();i++){
		if(_vertices[i]->equals(pos,uv))
			return _vertices[i];
	}
	
	Vertex *v = new Vertex();
	v->pos = pos;
	v->uv = uv;
	v->normal = normal;
	_vertices.push_back(v);
	return v;
}

float Mesh::getSquaredUVError(){
	float e = 0;
	for(int i = 0;i<_faces.size();i++){
		float d = _faces[i]->relativeSize / _faces[i]->relativeUVSize;
		e += d*d;
	}
	return e / _faces.size();
}

void Mesh::calcRelativeMeshAreas(){
	calcMeshArea();
	for(int i = 0;i<_faces.size();i++){
		_faces[i]->relativeSize = _faces[i]->size / meshArea;
	}
}

void Mesh::calcRelativeUVAreas(){
	calcUVArea();
	for(int i = 0;i<_faces.size();i++){
		_faces[i]->relativeUVSize = _faces[i]->uv_size / uvArea;
	}
}


void Mesh::calcUVArea(){
	uvArea = 0;
	for(int i = 0;i<_faces.size();i++){
		_faces[i]->calcSizeUV();
		uvArea += _faces[i]->getSizeUV();
	}
}

void Mesh::calcMeshArea(){
	meshArea = 0;
	for(int i = 0;i<_faces.size();i++){
		_faces[i]->calcSize();
		meshArea += _faces[i]->getSize();
	}
}

float triangleSize(const glm::vec3 &p0,const glm::vec3 &p1,const glm::vec3 &p2){
	glm::vec3 e0,e1;
	e0 = p1 - p0;
	e1 = p2 - p0;
	glm::vec3 N = glm::cross(e0,e1);
	return glm::length(N)/2.0f;
}


float triangleSize(const glm::vec2 &p0,const glm::vec2 &p1,const glm::vec2 &p2){
	//http://www.mathopenref.com/coordtrianglearea.html
	float a = 0;
	a += p0.x * (p1.y - p2.y);
	a += p1.x * (p2.y - p0.y);
	a += p2.x * (p0.y - p1.y);
	a /= 2;
	return a < 0 ? -a : a;
}

void Face::calcSize(){
	//http://en.wikipedia.org/wiki/Triangle#Using_vectors
	glm::vec3 c(0,0,0);
	for(int i = 0;i<_vertices.size();i++){
		c += _vertices[i]->pos / glm::vec3(_vertices.size());
	}

	size = 0;
	for(int i = 0;i<_vertices.size();i++){
		size += triangleSize(c,
							 _vertices[i]->pos,
							 _vertices[(i+1)%_vertices.size()]->pos);
	}
}
void Face::calcSizeUV(){
	glm::vec2 c(0,0);
	for(int i = 0;i<_vertices.size();i++){
		c += _vertices[i]->uv / glm::vec2(_vertices.size());
	}

	uv_size = 0;
	for(int i = 0;i<_vertices.size();i++){
		uv_size += triangleSize(c,
							 _vertices[i]->uv,
							 _vertices[(i+1)%_vertices.size()]->uv);
	}
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
	std::stringstream ss(s);
	std::string item;
	while(std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}

glm::ivec3 interpret_face(std::string face){
	std::vector<std::string> e = split(face,'/');
	while(e.size() < 3){
		e.push_back("-1");
	}
	glm::ivec3 ret(-1,-1,-1);
	for(int i = 0;i<3;i++){
		if(e[i].size() == 0) continue;
		ret[i] = atoi(e[i].c_str())-1; //Subtracting one to make it into range [0,max-1] instead of wavefront [1,max]
	}
	return ret;
}

Mesh* Mesh::ReadMesh(const char* filename){
	Mesh *m = new Mesh();
	std::vector<glm::vec3> _pos;
	std::vector<glm::vec3> _normals;
	std::vector<glm::vec2> _texCoords;
	
	
	std::ifstream file;
	file.open(filename);
	if(!file.is_open()){
		std::cerr << "File not found or could not be opened: " << filename << std::endl;
		exit(2);
		return 0;
	}
	char buf[1024];
	std::string line;
	while(!file.eof()){
		file >> line;
		if(line[0] == '#'){
			
		}else if(line == "v"){
			glm::vec3 pos;
			file >> pos.x  >> pos.y >> pos.z;
			_pos.push_back(pos);
		}else if(line == "vn"){
			glm::vec3 n;
			file >> n.x  >> n.y >> n.z;
			_normals.push_back(n);
		}else if(line == "vt"){
			glm::vec2 uv;
			file >> uv.x  >> uv.y;
			_texCoords.push_back(uv);
		}
		else if(line == "f"){
			Face *f = new Face();
			while(file.peek() != '\n' && file.peek() != '\r'){
				file >> line;
				glm::ivec3 ids = interpret_face(line);
				glm::vec3 pos,normal;
				glm::vec2 uv;
				
				if(ids.x != -1){
					pos = _pos[ids.x];
				}if(ids.y != -1){
					uv = _texCoords[ids.y];
				}if(ids.z != -1){
					normal = _normals[ids.z];
				}
				f->_vertices.push_back(m->createVertex(pos,uv,normal));
			}
			m->_faces.push_back(f);
		}
		file.getline(buf,1024);
	}
	m->calcRelativeAreas();
	return m;
}


void Mesh::saveMesh(const char* filename){
	
}

inline float T(float f,float r){
	return f == f ? f : r;
}

float Mesh::work(float dt){
	for(int i = 0;i<_vertices.size();i++){
		_vertices[i]->F = glm::vec2(0,0);
	}

	for(int i = 0;i<_faces.size();i++){
		glm::vec2 c(0,0),dir,damp;
		Face *f = _faces[i];

		
		for(int i = 0;i<f->_vertices.size();i++){
			c += f->_vertices[i]->uv / glm::vec2(f->_vertices.size());
		}
		
		float force = glm::clamp(f->relativeSize - f->relativeUVSize,-1.f,1.f)* 0.3f; 
		for(int i = 0;i<f->_vertices.size();i++){
			dir = (_vertices[i]->uv - c);
			damp = dir * T(glm::dot(glm::normalize(dir),f->_vertices[i]->V),0) * 0.5f;

			f->_vertices[i]->F += dir * force + damp;
		}
	}

	for(int i = 0;i<_vertices.size();i++){
		_vertices[i]->V += _vertices[i]->F * dt;
		_vertices[i]->uv += _vertices[i]->V * dt;
	}

	calcRelativeAreas();
	return getSquaredUVError();
}

int main(int argc,char ** argv){
	if(argc != 3){
		std::cerr << "Incorect number of parameters" << std::endl;
		std::cout << "Usage: " << argv[0] << " inFile outfile" << std::endl; 
		exit(1);
		return 1;
	}
	Mesh *m = Mesh::ReadMesh(argv[1]);
	std::cout << "Mesh loaded, size: " << m->getArea() << std::endl << "UV Size: " << m->getAreaUV() << std::endl << "Error: " << m->getSquaredUVError() << std::endl; 
	std::cout << m->getArea() << " " << m-> getAreaUV() << " " << m->getSquaredUVError() << std::endl;
	while(m->work(0.01) > 10){
		std::cout << m->getArea() << " " << m-> getAreaUV() << " " << m->getSquaredUVError() << std::endl;
	}
	std::cout << m->getArea() << " " << m-> getAreaUV() << " " << m->getSquaredUVError() << std::endl;
}