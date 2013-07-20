#ifndef _INDEXEDMESH_H_
#define _INDEXEDMESH_H_

#include "Mesh.h"

#include "Triangle.h"

#include "../KDTree/KDtree.h"

#include <iostream>

class IndexedMesh : public Mesh
{
	K3DTree<Vertex> _vertices;
	//K3DTree<Triangle*> _triangles;
	std::vector<Triangle> _triangles;
	BoundingAABB _boundingAABB;
public:
	IndexedMesh(void);
	virtual ~IndexedMesh(void);

	virtual Vertex *addVertex(glm::vec3 &pos, glm::vec3 &normal = glm::vec3(0,0,0));
	virtual Face* addFace(std::vector<glm::vec3> &positions);
	Face *addTriangle(const glm::vec3 &p0,const glm::vec3 &p1,const glm::vec3 &p2);

	std::vector<Triangle> getFaces(){return _triangles;}

	unsigned int getNumVertices(){return _vertices.size();}

	virtual float getArea()const;

	void clear(bool onlyFaces = false);

	virtual void calculateNormals();

	void removeDoubleTriangles(bool removeBoth = false);

	BoundingAABB getBoundingAABB()const{return _boundingAABB;}

#ifdef GL_VERSION_1_1
#include <OpenGLHelpers\AttribPusher.h>

	void drawWithNormals(bool faceNormals = true,float scale = 0.1){
		glColor3f(1,1,1);
		glColor3f(1,0,0);
		draw();
		
		AttribPusher __a(GL_LIGHTING_BIT);
		glDisable(GL_LIGHTING);
		glBegin(GL_LINES);
		for (auto tri = _triangles.begin(); tri != _triangles.end(); ++tri){
			if(faceNormals){
				glm::vec3 c = glm::vec3(tri->v0()->getPosition() + tri->v1()->getPosition() + tri->v2()->getPosition()) / glm::vec3(3.0,3.0,3.0);
				glm::vec3 offC = c + tri->getNormal() * scale;
				glColor3f(1,0,0);
				glVertex3fv(glm::value_ptr(c));
				glColor3f(0,0,1);
				glVertex3fv(glm::value_ptr(offC));
			}
		}
		glEnd();

	}


	void draw(){
		glBegin(GL_TRIANGLES);
		for (auto tri = _triangles.begin(); tri != _triangles.end(); ++tri){
			 //(*tri)->gl();
			 glNormal3fv(glm::value_ptr(tri->v0()->getNormal()));
			 glVertex3fv(glm::value_ptr(tri->v0()->getPosition()));

			 glNormal3fv(glm::value_ptr(tri->v1()->getNormal()));
			 glVertex3fv(glm::value_ptr(tri->v1()->getPosition()));

			 glNormal3fv(glm::value_ptr(tri->v2()->getNormal()));
			 glVertex3fv(glm::value_ptr(tri->v2()->getPosition()));
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
