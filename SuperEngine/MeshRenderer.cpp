#include "MeshRenderer.h"

#include <OpenGLHelpers\AttribPusher.h>
#include <OpenGLHelpers\OpenGLInfo.h>
#include <Geometry\Mesh\IndexedMesh.h>


#define BUFFER_OFFSET(i) ((void*)(i))

MeshRenderer::MeshRenderer():
_vertices(0),
_indices(0),
_smooth(true){
	_shader = ShaderProgram::CreateShaderProgramFromSources( GLSL_DIR "/MeshRenderer.vert",GLSL_DIR "/MeshRenderer.frag");
	_posId = _shader->getLocation("Pos");
	_normId = _shader->getLocation("Normal");
	glGenBuffers(1,&vbo);
	glGenBuffers(1,&ibo);
	
	_lights.push_back(Light(0));
	_lights.push_back(Light(1));
	
	_lights[0].setAmbientColor(glm::vec4(0,0,0,1));
	_lights[0].setDiffuseColor(glm::vec4(0.3,0.3,1,1));
	_lights[0].setAmbientColor(glm::vec4(1,1,1,1));
	_lights[0].setPosition(glm::vec3(-2,0,3));

	_lights[1].setAmbientColor(glm::vec4(0,0,0,1));
	_lights[1].setDiffuseColor(glm::vec4(1,0.3,0.3,1));
	_lights[1].setAmbientColor(glm::vec4(1,1,1,1));
	_lights[1].setPosition(glm::vec3(2,0,3));

	wireframe = !true;
 }


MeshRenderer::~MeshRenderer(){
	_deallocate();
	glDeleteBuffers(1,&vbo);
	glDeleteBuffers(1,&ibo);
}


void MeshRenderer::draw(){
	if(wireframe){
		_shader->unbind();
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		
		glVertexPointer(4,GL_FLOAT,7*sizeof(float),_vertices);
		glNormalPointer(  GL_FLOAT,7*sizeof(float),_vertices);

		
		glDrawElements(GL_TRIANGLES, _numIndices, GL_UNSIGNED_INT, _indices);

		glBindBuffer(GL_ARRAY_BUFFER,0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

		return;
	}


	{AttribPusher __a(GL_LIGHTING_BIT);
	glDisable(GL_LIGHTING);
	glPointSize(4);
	glBegin(GL_POINTS);
	for(int i = 0;i<_lights.size()&&i<64;i++){
		glColor3fv(glm::value_ptr(_lights[i].getDiffuseColor()));
		glVertex3fv(glm::value_ptr(_lights[i].getPosition()));
	}
	glEnd();
	}


	_shader->bind();
	_shader->setUniform("num_lights",glm::min(64,(int)_lights.size()));chkGLErr();
	_shader->setUniform("material.ambient",glm::vec4(0,0,0,1));chkGLErr();
	_shader->setUniform("material.diffuse",glm::vec4(1,1,1,1));chkGLErr();
	_shader->setUniform("material.specular",glm::vec4(1,1,1,1));chkGLErr();
	_shader->setUniform("material.shininess",20.0f);chkGLErr();
	chkGLErr();

	for(int i = 0;i<_lights.size()&&i<64;i++){
		char name[256];
		sprintf(name,"lights[%d].ambient",i);
		_shader->setUniform(name,_lights[i].getAmbientColor());
		sprintf(name,"lights[%d].diffuse",i);
		_shader->setUniform(name,_lights[i].getDiffuseColor());
		sprintf(name,"lights[%d].specular",i);
		_shader->setUniform(name,_lights[i].getSpecularColor());
		sprintf(name,"lights[%d].radius",i);
		_shader->setUniform(name,_lights[i].getRadius());
		/*sprintf(name,"lights[%d].constantAtt",i);
		_shader->setUniform(name,_lights[i].getConstantAttenuation());
		sprintf(name,"lights[%d].linearAtt",i);
		_shader->setUniform(name,_lights[i].getLinearAttenuation());
		sprintf(name,"lights[%d].quadraticAtt",i);
		_shader->setUniform(name,_lights[i].getQuadraticAttenuation());*/
		sprintf(name,"lights[%d].position",i);
		_shader->setUniform(name,_lights[i].getPosition());
		chkGLErr();
	}

	glBindBuffer(GL_ARRAY_BUFFER,vbo);
	
	glEnableVertexAttribArray(_posId);
	glVertexAttribPointer(_posId,4,GL_FLOAT,GL_FALSE,7*sizeof(float),BUFFER_OFFSET(0));

	glEnableVertexAttribArray(_normId);
	glVertexAttribPointer(_normId,3,GL_FLOAT,GL_FALSE,7*sizeof(float),BUFFER_OFFSET(4*sizeof(float)));

	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ibo);
	//glDrawArrays(GL_TRIANGLES,0,_numIndices);
	glDrawElements(GL_TRIANGLES, _numIndices, GL_UNSIGNED_INT, 0);

	glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
	
	glDisableVertexAttribArray(_posId);
	glDisableVertexAttribArray(_normId);

	_shader->unbind();
}


void MeshRenderer::_deallocate(){
	if(_vertices == 0)
		return;
	delete _vertices;
	delete _indices;
}

void MeshRenderer::_allocateData(int numVerts,int numIndices){
	_deallocate();
	_vertices = new float[numVerts*7];
	_indices = new unsigned int[numIndices];
	_numVerts = numVerts;
	_numIndices = numIndices;
}

void MeshRenderer::buildFromMesh(Mesh *m,bool smooth){
	auto imesh = dynamic_cast<IndexedMesh*>(m);
	if(imesh){
		auto faces = imesh->getFaces();
		K3DTree<unsigned int> tree;
		K3DTree<unsigned int>::Node *n[3];
		if(smooth){
			_allocateData(imesh->getNumVertices(),faces.size()*3);
			imesh->calculateNormals();
		}
		else{
			_allocateData(faces.size()*3,faces.size()*3);
		}
			
		int v = 0;
		int i = 0;
		for(auto t = faces.begin();t!=faces.end();++t){
			if(!smooth){
				_vertices[v++] = (*t)->v0()->getPosition().x;
				_vertices[v++] = (*t)->v0()->getPosition().y;
				_vertices[v++] = (*t)->v0()->getPosition().z;
				_vertices[v++] = (*t)->v0()->getPosition().w;
				_vertices[v++] = (*t)->getNormal().x;
				_vertices[v++] = (*t)->getNormal().y;
				_vertices[v++] = (*t)->getNormal().z;
				_vertices[v++] = (*t)->v1()->getPosition().x;
				_vertices[v++] = (*t)->v1()->getPosition().y;
				_vertices[v++] = (*t)->v1()->getPosition().z;
				_vertices[v++] = (*t)->v1()->getPosition().w;
				_vertices[v++] = (*t)->getNormal().x;
				_vertices[v++] = (*t)->getNormal().y;
				_vertices[v++] = (*t)->getNormal().z;
				_vertices[v++] = (*t)->v2()->getPosition().x;
				_vertices[v++] = (*t)->v2()->getPosition().y;
				_vertices[v++] = (*t)->v2()->getPosition().z;
				_vertices[v++] = (*t)->v2()->getPosition().w;
				_vertices[v++] = (*t)->getNormal().x;
				_vertices[v++] = (*t)->getNormal().y;
				_vertices[v++] = (*t)->getNormal().z;
				_indices[i] = i++;
				_indices[i] = i++;
				_indices[i] = i++;
			}else{
				n[0] = tree.find(glm::vec3((*t)->v0()->getPosition()));
				n[1] = tree.find(glm::vec3((*t)->v1()->getPosition()));
				n[2] = tree.find(glm::vec3((*t)->v2()->getPosition()));
				if(n[0] == 0){
					n[0] = tree.insert(glm::vec3((*t)->v0()->getPosition()),v/7);
					_vertices[v++] = (*t)->v0()->getPosition().x;
					_vertices[v++] = (*t)->v0()->getPosition().y;
					_vertices[v++] = (*t)->v0()->getPosition().z;
					_vertices[v++] = (*t)->v0()->getPosition().w;
					_vertices[v++] = (*t)->v0()->getNormal().x;
					_vertices[v++] = (*t)->v0()->getNormal().y;
					_vertices[v++] = (*t)->v0()->getNormal().z;
				
				}
				if(n[1] == 0){
					n[1] = tree.insert(glm::vec3((*t)->v1()->getPosition()),v/7);
					_vertices[v++] = (*t)->v1()->getPosition().x;
					_vertices[v++] = (*t)->v1()->getPosition().y;
					_vertices[v++] = (*t)->v1()->getPosition().z;
					_vertices[v++] = (*t)->v1()->getPosition().w;
					_vertices[v++] = (*t)->v1()->getNormal().x;
					_vertices[v++] = (*t)->v1()->getNormal().y;
					_vertices[v++] = (*t)->v1()->getNormal().z;
				}
				if(n[2] == 0){
					n[2] = tree.insert(glm::vec3((*t)->v2()->getPosition()),v/7);
					_vertices[v++] = (*t)->v2()->getPosition().x;
					_vertices[v++] = (*t)->v2()->getPosition().y;
					_vertices[v++] = (*t)->v2()->getPosition().z;
					_vertices[v++] = (*t)->v2()->getPosition().w;
					_vertices[v++] = (*t)->v2()->getNormal().x;
					_vertices[v++] = (*t)->v2()->getNormal().y;
					_vertices[v++] = (*t)->v2()->getNormal().z;
				}
				_indices[i++] = n[0]->get();
				_indices[i++] = n[1]->get();
				_indices[i++] = n[2]->get();
			}
		}
	}else{
		
	}
	
	
	glBindBuffer(GL_ARRAY_BUFFER,vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*7*_numVerts, _vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER,0);

	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*_numIndices, _indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
}



