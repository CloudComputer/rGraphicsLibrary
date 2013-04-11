#ifndef _MESHRENDERER_H_
#define _MESHRENDERER_H_

#include <Geometry\Mesh\Mesh.h>

#include <Engine/DrawableObject.h>

#include <OpenGLHelpers\Shader.h>
#include <OpenGLHelpers\Light.h>

class MeshRenderer : public DrawableObject{
	float *_vertices;
	unsigned int *_indices;
	bool _smooth;

	unsigned int _numVerts,_numIndices;
	

	std::vector<Light> _lights;

	bool wireframe;

	ShaderProgram *_shader;

	GLuint vbo;
	GLuint ibo;
	GLuint _posId,_normId;

	void _deallocate();
	void _allocateData(int numVerts,int numIndices);
public:
	MeshRenderer();
	virtual ~MeshRenderer();
	
	virtual void draw();


	void buildFromMesh(Mesh *m,bool smooth = true);

	virtual std::string toString()const{
		return "MeshRenderer";
	}
};

#endif