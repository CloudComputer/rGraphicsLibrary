#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "BoundingVolume.h"
#include <vector>

struct Vertex{
	glm::vec3 position;
	glm::vec3 normal;

	Vertex(){}
	Vertex(const Vertex *v){
		position = v->position;
		normal = v->normal;
	}
	Vertex(glm::vec3 position,glm::vec3 normal = glm::vec3(0,1,0)){
		this->position = position;
		this->normal = normal;
	}

};

struct Triangle{
	Vertex *v0,*v1,*v2;
	glm::vec3 normal;

	Triangle(Vertex *v0,Vertex *v1,Vertex *v2);
	Triangle(){}
	virtual ~Triangle(){}

	void calculateNormal();

	float distance(const glm::vec3 position,bool signedDistance = false)const;
	float distance(const Vertex *vertex,bool signedDistance = false)const;
};

class Mesh
{
	std::vector<Vertex *> _vertices;
	std::vector<Triangle> _triangles;
	BoundingAABB _boundingAABB;
public:
	Mesh(void);
	~Mesh(void);

	Vertex* addVertex(const Vertex v);
	unsigned int addTriangle(const Vertex v0,const Vertex v1,const Vertex v2);

	void calculateNormals();

	BoundingAABB getBoundingAABB()const{return _boundingAABB;}

#ifdef GL_VERSION_1_1
	void draw(){
		glBegin(GL_TRIANGLES);
		for (int i = 0;i<_triangles.size();i++){
			Vertex *v0 = _triangles[i].v0;
			Vertex *v1 = _triangles[i].v1;
			Vertex *v2 = _triangles[i].v2;

			glNormal3fv(glm::value_ptr(v0->normal));
			glVertex3fv(glm::value_ptr(v0->position));

			glNormal3fv(glm::value_ptr(v1->normal));
			glVertex3fv(glm::value_ptr(v1->position));

			glNormal3fv(glm::value_ptr(v2->normal));
			glVertex3fv(glm::value_ptr(v2->position));
		}
		glEnd();


	}
#endif

	static Mesh* LoadWavefront(const char* filename);
	float distance( glm::vec3 worldPos , bool signedDistnace = false)const;
};

