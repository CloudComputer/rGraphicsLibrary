#include "IndexedMesh.h"

#include <iostream>
#include <fstream>

IndexedMesh::IndexedMesh(void):
_boundingAABB(glm::vec3(0,0,0),glm::vec3(0,0,0))
{
}


IndexedMesh::~IndexedMesh(void)
{
}

Face* IndexedMesh::addFace(std::vector<glm::vec3> pos){
	assert(pos.size() == 3 && "Only triangles are supported"); 
	Vertex *v0,*v1,*v2;
	v0 = addVertex(pos[0]);
	v1 = addVertex(pos[1]);
	v2 = addVertex(pos[2]);
	if(v0 == v1 || v0 == v2 || v2 == v1)
		return 0;
	Triangle *t = new Triangle(v0,v1,v2);
	_triangles.push_back(t);
	return t;
}

Vertex* IndexedMesh::addVertex(glm::vec3 pos, glm::vec3 normal){
	auto node = _vertices.findNearest(glm::value_ptr(pos));
	if(node){
		float d = glm::distance(pos,glm::vec3(node->get()->getPosition()));
		if(d<0.0001){
			return node->get();
		}
	}

	 Vertex *vv = new Vertex(pos,normal);
	 vv->setNormal(normal);
	 _vertices.insert(glm::value_ptr(pos),vv);
	_boundingAABB.extend(pos);
	return vv;
}

void IndexedMesh::calculateNormals(){
	for (auto vert = _vertices.begin_data();vert != _vertices.end_data();++vert){
		(*vert)->setNormal(glm::vec3(0,0,0));
	}

	for(auto tri = _triangles.begin();tri != _triangles.end();++tri){
		(*tri)->v0()->setNormal((*tri)->v0()->getNormal() + (*tri)->getNormal());
		(*tri)->v1()->setNormal((*tri)->v1()->getNormal() + (*tri)->getNormal());
		(*tri)->v2()->setNormal((*tri)->v2()->getNormal() + (*tri)->getNormal());
	}

	for (auto vert = _vertices.begin_data();vert != _vertices.end_data();++vert){
		(*vert)->setNormal(glm::normalize((*vert)->getNormal()));
	}
}

float IndexedMesh::distance( glm::vec3 worldPos, bool signedDistnace )const
{
	float dist = 9999999;
	for(auto tri = _triangles.begin();tri != _triangles.end();++tri){
		float d = (*tri)->distance(worldPos,signedDistnace);
		if(abs(dist) > abs(d)) dist = d;
	}

	return dist;
}
