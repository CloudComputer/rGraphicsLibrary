#ifndef _MESHRENDERER_H_
#define _MESHRENDERER_H_

#include <Geometry\Mesh\Mesh.h>

#include <OpenGLHelpers\Light.h>

#include "rObject.h"
#include "Material.h"

class MeshRenderer : public rObject{
	struct vert{
		float x,y,z,nx,ny,nz,tx,ty;
	};
	float *_vertices;
	unsigned int *_indices;
	bool _smooth;

	Material _mat;
	bool _useTex;

	unsigned int _numVerts,_numIndices;
	
	GLuint vbo;
	GLuint ibo;
	std::string textureName;
	GLuint _posId,_normId,_texID,_texCoordID;

	void _deallocate();
	void _allocateData(int numVerts,int numIndices);

	void readMaterial(tinyxml2::XMLElement *ele);

protected:
	virtual void onDraw();
public:
	MeshRenderer();
	virtual ~MeshRenderer();
	
	void buildFromMesh(Mesh *m,bool smooth = true);

	virtual std::string toString()const{
		return "MeshRenderer";
	}
	static void CreateObject(rObject *&obj,tinyxml2::XMLElement *ele);
	static void CreateFractal(rObject *&obj,tinyxml2::XMLElement *ele);
};

#endif