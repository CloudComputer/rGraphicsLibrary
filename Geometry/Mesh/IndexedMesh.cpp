#include "IndexedMesh.h"

#include <iostream>
#include <fstream>

IndexedMesh::IndexedMesh(void):
_boundingAABB(glm::vec3(0,0,0),glm::vec3(0,0,0))
{
}


IndexedMesh::~IndexedMesh(void)
{
	clear(false);
}

void IndexedMesh::removeDoubleTriangles(bool removeBoth){
	for(int i = 0;i<_triangles.size()-1;i++){
		auto t = &_triangles[i];
		glm::vec3 N = _triangles[i].getNormal();
		bool removed = false;
		for(int j = _triangles.size()-1;j>i;j--){
			auto t2 = &_triangles[j];
			if(std::abs(glm::dot(N,t2->getNormal()))>=0.9999){
				if(!(t->v0() == t2->v0()||t->v0() == t2->v1()||t->v0() == t2->v2()))
					continue;
				if(!(t->v1() == t2->v0()||t->v1() == t2->v1()||t->v1() == t2->v2()))
					continue;
				if(!(t->v2() == t2->v0()||t->v2() == t2->v1()||t->v2() == t2->v2()))
					continue;
				_triangles.erase(_triangles.begin()+j);
				removed = true;
			}
		}
		if(removed && removeBoth){
			_triangles.erase(_triangles.begin()+i);
			i--;
		}
	}
}

Face* IndexedMesh::addFace(std::vector<glm::vec3> &pos){
	assert(pos.size() == 3 && "Only triangles are supported"); 
	Vertex *v0,*v1,*v2;
	v0 = addVertex(pos[0]);
	v1 = addVertex(pos[1]);
	v2 = addVertex(pos[2]);
	if(v0 == v1 || v0 == v2 || v2 == v1)
		return 0;
	_triangles.push_back(Triangle(v0,v1,v2));
	return &_triangles[_triangles.size()-1];
}

Vertex* IndexedMesh::addVertex(glm::vec3 &pos, glm::vec3 &normal){
	auto node = _vertices.findNearest(pos);
	if(node){
		float d = glm::distance(pos,glm::vec3(node->get().getPosition()));
		if(d<0.001){
			return &node->get();
		}
	}

	 node = _vertices.insert(pos,Vertex(pos,normal));
	_boundingAABB.extend(pos);
	return &node->get();
}

void IndexedMesh::calculateNormals(){
	for (auto vert = _vertices.begin_data();vert != _vertices.end_data();++vert){
		vert->setNormal(glm::vec3(0,0,0));
	}

	for(auto tri = _triangles.begin();tri != _triangles.end();++tri){
		tri->v0()->setNormal(tri->v0()->getNormal() + tri->getNormal());
		tri->v1()->setNormal(tri->v1()->getNormal() + tri->getNormal());
		tri->v2()->setNormal(tri->v2()->getNormal() + tri->getNormal());
	}

	for (auto vert = _vertices.begin_data();vert != _vertices.end_data();++vert){
		vert->setNormal(glm::normalize(vert->getNormal()));
	}
}

float IndexedMesh::distance( glm::vec3 &worldPos, bool signedDistnace )
{
	float dist = 9999999;
	for(auto tri = _triangles.begin();tri != _triangles.end();++tri){
		float d = tri->distance(worldPos,signedDistnace);
		if(abs(dist) > abs(d)) dist = d;
	}

	return dist;
}

void IndexedMesh::clear(bool onlyFaces){
	this->_triangles.clear();
	if(!onlyFaces){
		_vertices.clear();
	}
}

Face *IndexedMesh::addTriangle(const glm::vec3 &p0,const glm::vec3 &p1,const glm::vec3 &p2){
	std::vector<glm::vec3> p;
	p.push_back(p0);
	p.push_back(p1);
	p.push_back(p2);
	return addFace(p);
}