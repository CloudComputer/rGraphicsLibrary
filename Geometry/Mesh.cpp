#include "Mesh.h"

#include <iostream>
#include <fstream>

Triangle::Triangle(Vertex *v0,Vertex *v1,Vertex *v2){
	this->v0 = v0;
	this->v1 = v1;
	this->v2 = v2;
	calculateNormal();
}

void Triangle::calculateNormal(){
	glm::vec3 e0,e1;
	e0 = v1->position - v0->position;
	e1 = v2->position - v0->position;
	
	normal = glm::normalize(glm::cross(e0,e1));
}

float Triangle::distance(const glm::vec3 position,bool signedDistance)const {
	float a,b,c,d,e,f,s,t,det;
	int region;
	glm::vec3 closestPoint,e0,e1,D;
	e0 = v1->position - v0->position;
	e1 = v2->position - v0->position;

	D = v0->position - position;

	a = glm::dot(e0,e0);
	b = glm::dot(e0,e1);
	c = glm::dot(e1,e1);
	d = glm::dot(e0,D);
	e = glm::dot(e1,D);
	f = glm::dot(D,D);

	det = a*c - b*b;
	s = b*e - c*d;
	t = b*d - a*e;

	if(s+t <= det){
		if(s<0) region = t<0 ? 4 : 3;
		else if(t < 0) region = 5;
		else region = 0;
		
	}else{
		if(s<0) region = 2;
		else if (t<0) region = 6;
		else region = 1;
	}
	int sign = 1;
	if(signedDistance){
		sign = glm::dot(normal,D) >= 0 ? 1:-1;
	}

	


	if(region == 0){
		glm::vec3 tp = v0->position + s*e0 + t*e1;
		return glm::distance(tp,position) * sign;
	}

	else if(region == 1){
		float numer = c+e-b-d;
		if(numer<=0)
			s = 0;
		else{
			float denom = a-2*b+c;
			s = (numer >= denom ? 1 : numer/denom);
		}
		t = 1-s;
		glm::vec3 tp = v0->position + s*e0 + t*e1;
		return glm::distance(tp,position) * sign;

	}

	else if(region == 2){
		return glm::distance(v2->position,position) * sign;
	}

	else if(region == 3){
		t = (e >= 0 ? 0 : (-e >= c ? 1 : -e/c));
		glm::vec3 tp = v0->position + t*e1;
		return glm::distance(tp,position) * sign;

	}


	else if(region == 4){
		return glm::distance(v0->position,position) * sign;
	}

	else if(region == 5){
		s = (d >= 0 ? 0 : (-d >= a ? 1 : -d/a));
		glm::vec3 tp = v0->position + s*e0;
		return glm::distance(tp,position) * sign;
	}

	else if(region == 6){
		return glm::distance(v1->position,position) * sign;
	}

	float dist = a*s*s   + 2*b*s*t  + c*t*t   +  2*d*s    + 2*e*t   +   f;
	return dist;
}

float Triangle::distance(const Vertex *vertex,bool signedDistance)const {
	return distance(vertex->position,signedDistance);
}

Mesh::Mesh(void):
_boundingAABB(glm::vec3(0,0,0),glm::vec3(0,0,0))
{
}


Mesh::~Mesh(void)
{
}

Vertex* Mesh::addVertex(const Vertex v){
	for (int i = 0;i<_vertices.size();i++)
	{
		if(_vertices[i] == &v || _vertices[i]->position == v.position){
			return _vertices[i];
		}
	}

	Vertex *vv = new Vertex();
	vv->position = v.position;
	vv->normal   = v.normal;
	_vertices.push_back(vv);

	_boundingAABB.extend(v.position);

	return vv;
}

unsigned int Mesh::addTriangle(const Vertex v0,const Vertex v1,const Vertex v2){
	_triangles.push_back(Triangle(addVertex(v0),addVertex(v1),addVertex(v2)));
	return _triangles.size()-1;
}

void Mesh::calculateNormals(){
	std::vector<Vertex *>::iterator vert;
	std::vector<Triangle>::iterator tri;

	for (vert = _vertices.begin();vert != _vertices.end();++vert){
		(*vert)->normal = glm::vec3(0,0,0);
	}

	for(tri = _triangles.begin();tri != _triangles.end();++tri){
		tri->v0->normal += tri->normal;
		tri->v1->normal += tri->normal;
		tri->v2->normal += tri->normal;
	}

	for (vert = _vertices.begin();vert != _vertices.end();++vert){
		(*vert)->normal = glm::normalize((*vert)->normal);
	}
}

Mesh* Mesh::LoadWavefront(const char* filename){
	Mesh *mesh = new Mesh();

	std::ifstream inFile;
	inFile.open(filename);
	if(!inFile.is_open()){
		std::cerr << "Could not open file"  << filename<< std::endl;
		return mesh;
	}

	std::vector<glm::vec3> positions;
	positions.push_back(glm::vec3()); // dummy, wavefront start their indices at 1
	char line[256];
	while(!inFile.eof()){
		char peeked = inFile.peek();
		if(peeked == '#' || peeked == '\0'){
			inFile.getline(line,256);
			continue;
		}
		if(peeked == 'v'){
			inFile.getline(line,256,' ');
			inFile.getline(line,256,' ');
			glm::vec3 pos;
			pos.x = atof(line);
			inFile.getline(line,256,' ');
			pos.y = atof(line);
			inFile.getline(line,256);
			pos.z = atof(line);
			positions.push_back(pos);
		}else if(peeked == 'f'){
			unsigned int i0,i1,i2;
			inFile.getline(line,256,' ');
			inFile.getline(line,256,' ');
			i0 = atoi(line);
			inFile.getline(line,256,' ');
			i1 = atoi(line);
			inFile.getline(line,256);
			i2 = atoi(line);
			Vertex v0 = Vertex(positions[i0]);
			Vertex v1 = Vertex(positions[i1]);
			Vertex v2 = Vertex(positions[i2]);
			mesh->addTriangle(v0,v1,v2);
		}else {
			inFile.getline(line,256);
			std::cout << line << std::endl;
		}
	}

	return mesh;
}

float Mesh::distance( glm::vec3 worldPos, bool signedDistnace )const
{
	std::vector<Triangle>::const_iterator tri;
	float dist = 9999999;
	for(tri = _triangles.begin();tri != _triangles.end();++tri){
		float d = tri->distance(worldPos,signedDistnace);
		if(abs(dist) > abs(d)) dist = d;
	}

	return dist;
}
