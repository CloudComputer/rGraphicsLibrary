#ifndef _INDEXEDMESH_H_
#define _INDEXEDMESH_H_

#include "Mesh.h"

#include "Triangle.h"

#include "../KDTree/KDtree.h"

#include <iostream>

class IndexedMesh : public Mesh
{
	K3DTree<Vertex*> _vertices;
	//K3DTree<Triangle*> _triangles;
	std::vector<Triangle*> _triangles;
	BoundingAABB _boundingAABB;
public:
	IndexedMesh(void);
	~IndexedMesh(void);

	virtual Vertex *addVertex(glm::vec3 &pos, glm::vec3 &normal = glm::vec3(0,0,0));
	virtual Face* addFace(std::vector<glm::vec3> &positions);
	Face *addTriangle(const glm::vec3 &p0,const glm::vec3 &p1,const glm::vec3 &p2);

	std::vector<Triangle*> getFaces(){return _triangles;}

	unsigned int getNumVertices(){return _vertices.size();}

	void clear(bool onlyFaces = false);

	virtual void calculateNormals();

	void removeDoubleTriangles(bool removeBoth = false);

	BoundingAABB getBoundingAABB()const{return _boundingAABB;}

#ifdef GL_VERSION_1_1
	void draw(){
		glBegin(GL_TRIANGLES);
		for (auto tri = _triangles.begin(); tri != _triangles.end(); ++tri){
			 //(*tri)->gl();
			 glNormal3fv(glm::value_ptr((*tri)->v0()->getNormal()));
			 glVertex3fv(glm::value_ptr((*tri)->v0()->getPosition()));

			 glNormal3fv(glm::value_ptr((*tri)->v1()->getNormal()));
			 glVertex3fv(glm::value_ptr((*tri)->v1()->getPosition()));

			 glNormal3fv(glm::value_ptr((*tri)->v2()->getNormal()));
			 glVertex3fv(glm::value_ptr((*tri)->v2()->getPosition()));
		}
		glEnd();
	}
#endif
	virtual float distance(glm::vec3 &worldPos , bool signedDistnace = false);

	virtual std::string toString()const{
		return "IndexedMesh";
	}
};




#endif
