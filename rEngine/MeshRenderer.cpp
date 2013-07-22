#include "MeshRenderer.h"

#include "Texture.h"

#include <OpenGLHelpers\AttribPusher.h>
#include <OpenGLHelpers\Shader.h>
#include <Geometry\Mesh\IndexedMesh.h>

#include <Util\Macros.h>

#define BUFFER_OFFSET(i) ((void*)(i))

MeshRenderer::MeshRenderer():
_vertices(0),
_indices(0),
_smooth(true){
	glGenBuffers(1,&vbo);
	glGenBuffers(1,&ibo);
	
 }


MeshRenderer::~MeshRenderer(){
	_deallocate();
	glDeleteBuffers(1,&vbo);
	glDeleteBuffers(1,&ibo);
}

void MeshRenderer::readMaterial(tinyxml2::XMLElement *ele){
	std::vector<std::string> defs;
	
	_useTex = false;
	auto texture = ele->FirstChildElement("texture");
	if(texture){
		defs.push_back("USETEXTURE");
		_useTex = true;
		std::string textureSRC;
		textureName = texture->Attribute("name");
		auto c  = texture->Attribute("src");
		textureSRC =  (!c) ? "" : c;
		if(textureSRC.length()!=0)
			Texture::loadTexture(textureName,textureSRC);
	}
	
	auto material = ele->FirstChildElement("material");

	if(material){
		auto diffuse = material->FirstChildElement("diffuse");
		auto ambient = material->FirstChildElement("ambient");
		auto specular = material->FirstChildElement("specular");
		auto specularity = material->FirstChildElement("specularity");
		if(diffuse){
			auto c = diffuse->GetText();
			std::istringstream iss(c);
			glm::vec4 col = glm::vec4(0,0,0,255);
			iss >> col.r>>col.g>>col.b>>col.a;
			col /= 255;
			_mat.setDiffuse(col);
				
		}
		if(ambient){
			auto c = ambient->GetText();
			std::istringstream iss(c);	
			glm::vec3 col = glm::vec3(0,0,0);
			iss >> col.r >> col.g >> col.b;
			col /= 255;
			_mat.setAmbient(col);
		}
		if(specular){
			auto c = specular->GetText();	
			std::istringstream iss(c);	
			glm::vec3 col = glm::vec3(0,0,0);
			iss >> col.r >> col.g >> col.b;
			col /= 255;
			_mat.setSpecular(col);
		}
		if(specularity){
			auto c = specularity->GetText();	
			std::istringstream iss(c);
			float s = 0;
			iss >> s;
			_mat.setSpecularity(s);
		}
	}

	
	_shader = ShaderProgram::CreateShaderProgramFromSources( GLSL_DIR "/rMesh.vert",GLSL_DIR "/rMesh.frag",defs);
	_posId = _shader->getLocation("Pos");
	_normId = _shader->getLocation("Normal");
	_texCoordID = _shader->getLocation("TexCoord");
	_texID = _shader->getLocation("tex");
}

void MeshRenderer::CreateObject(rObject *&obj,tinyxml2::XMLElement *ele){
	obj = new MeshRenderer();
	auto renderer = static_cast<MeshRenderer*>(obj);
	renderer->readMaterial(ele);
	
	auto file = ele->FirstChildElement("file");
	if(file){
		auto c = file->Attribute("url");
		Mesh *m = Mesh::LoadWavefront<IndexedMesh>(c);
		renderer->buildFromMesh(m);
	}else{
		auto verts = ele->FirstChildElement("vertices");
		auto child = verts->FirstChild();
		auto text = child->ToText();
		while(!text){
			child = child->NextSibling();
			text = child->ToText();
		}
	
		std::istringstream issVert(text->Value());
		auto inds = ele->FirstChildElement("indices");
		child = inds->FirstChild();
		text = child->ToText();
		while(!text){
			child = child->NextSibling();
			text = child->ToText();
		}
		std::istringstream issInd(text->Value());
		MeshRenderer::vert vert;
		int i;
		std::vector<MeshRenderer::vert> vertices;
		std::vector<int> indices;
		while(issVert >> vert.x >> vert.y >> vert.z >> vert.nx >> vert.ny >> vert.nz >> vert.tx >> vert.ty){
			vertices.push_back(vert);
		}
		while(issInd >> i){
			indices.push_back(i);
		
		}
		renderer->_allocateData(vertices.size(),indices.size());
		i = 0;
		IT_FOR(vertices,v){
			renderer->_vertices[i++] = v->x;
			renderer->_vertices[i++] = v->y;
			renderer->_vertices[i++] = v->z;
			renderer->_vertices[i++] = v->nx;
			renderer->_vertices[i++] = v->ny;
			renderer->_vertices[i++] = v->nz;
			renderer->_vertices[i++] = v->tx;
			renderer->_vertices[i++] = v->ty;
		}

		i = 0;
		IT_FOR(indices,id){
			renderer->_indices[i++] = *id;
		}

		auto prim = ele->Attribute("primitive");
		if(prim){
			if(strcmp(prim,"quads") == 0)
				renderer->faceType = GL_QUADS;
			else
				renderer->faceType = GL_TRIANGLES;
		}else
			renderer->faceType = GL_TRIANGLES;

		glBindBuffer(GL_ARRAY_BUFFER,renderer->vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*8*renderer->_numVerts, renderer->_vertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER,0);

	
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,renderer->ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*renderer->_numIndices, renderer->_indices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
	}

}

void MeshRenderer::CreateFractal(rObject *&obj,tinyxml2::XMLElement *ele){
	obj = new MeshRenderer();
	auto renderer = static_cast<MeshRenderer*>(obj);
	obj = 0;
}


void MeshRenderer::onDraw(){                                  
	glEnable (GL_DEPTH_TEST);  
	
	chkGLErr();

	glBindBuffer(GL_ARRAY_BUFFER,vbo);
	
	glEnableVertexAttribArray(_posId);
	glVertexAttribPointer(_posId,3,GL_FLOAT,GL_FALSE,8*sizeof(float),BUFFER_OFFSET(0));
	
	chkGLErr();
	glEnableVertexAttribArray(_normId);
	glVertexAttribPointer(_normId,3,GL_FLOAT,GL_FALSE,8*sizeof(float),BUFFER_OFFSET(3*sizeof(float)));
	
	if(_useTex){
		chkGLErr();
		glEnableVertexAttribArray(_texCoordID);
		glVertexAttribPointer(_texCoordID,2,GL_FLOAT,GL_FALSE,8*sizeof(float),BUFFER_OFFSET(6*sizeof(float)));
	
	
		chkGLErr();
	
		glActiveTexture(GL_TEXTURE0);
		glUniform1i(_texID,0);
		glBindTexture(GL_TEXTURE_2D,Texture::getTextureID(textureName));
	}else{
		_shader->setUniform("matDiffuse",_mat.getDiffuse());
	}
	_shader->setUniform("matAmbient",_mat.getAmbient());
	_shader->setUniform("matSpecular",_mat.getSpecular());
	_shader->setUniform("matSpecularity",_mat.getSpecularity());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ibo);
	glDrawElements(faceType, _numIndices, GL_UNSIGNED_INT, 0);
	chkGLErr();

	glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
	chkGLErr();
	
	glDisableVertexAttribArray(_posId);
	glDisableVertexAttribArray(_normId);
	glDisableVertexAttribArray(_texID);

	
	if(_useTex)
		glBindTexture(GL_TEXTURE_2D,0);
	chkGLErr();
}


void MeshRenderer::_deallocate(){
	if(_vertices == 0)
		return;
	delete _vertices;
	delete _indices;
}

void MeshRenderer::_allocateData(int numVerts,int numIndices){
	_deallocate();
	_vertices = new float[numVerts*8];
	_indices = new unsigned int[numIndices];
	_numVerts = numVerts;
	_numIndices = numIndices;
}

void MeshRenderer::buildFromMesh(Mesh *m,bool smooth){
	auto imesh = dynamic_cast<IndexedMesh*>(m);
	if(imesh){
		std::vector<Triangle> faces = imesh->getFaces();
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
				_vertices[v++] = t->v0()->getPosition().x;
				_vertices[v++] = t->v0()->getPosition().y;
				_vertices[v++] = t->v0()->getPosition().z;
				_vertices[v++] = t->getNormal().x;
				_vertices[v++] = t->getNormal().y;
				_vertices[v++] = t->getNormal().z;
				_vertices[v++] = 0;
				_vertices[v++] = 0;

				_vertices[v++] = t->v1()->getPosition().x;
				_vertices[v++] = t->v1()->getPosition().y;
				_vertices[v++] = t->v1()->getPosition().z;
				_vertices[v++] = t->getNormal().x;
				_vertices[v++] = t->getNormal().y;
				_vertices[v++] = t->getNormal().z;
				_vertices[v++] = 0;
				_vertices[v++] = 0;

				_vertices[v++] = t->v2()->getPosition().x;
				_vertices[v++] = t->v2()->getPosition().y;
				_vertices[v++] = t->v2()->getPosition().z;
				_vertices[v++] = t->getNormal().x;
				_vertices[v++] = t->getNormal().y;
				_vertices[v++] = t->getNormal().z;
				_vertices[v++] = 0;
				_vertices[v++] = 0;

				_indices[i] = i++;
				_indices[i] = i++;
				_indices[i] = i++;
			}else{
				n[0] = tree.find(glm::vec3(t->v0()->getPosition()));
				n[1] = tree.find(glm::vec3(t->v1()->getPosition()));
				n[2] = tree.find(glm::vec3(t->v2()->getPosition()));
				if(n[0] == 0){
					n[0] = tree.insert(glm::vec3(t->v0()->getPosition()),v/8);
					_vertices[v++] = t->v0()->getPosition().x;
					_vertices[v++] = t->v0()->getPosition().y;
					_vertices[v++] = t->v0()->getPosition().z;
					_vertices[v++] = t->v0()->getNormal().x;
					_vertices[v++] = t->v0()->getNormal().y;
					_vertices[v++] = t->v0()->getNormal().z;
					_vertices[v++] = 0;
					_vertices[v++] = 0;
				}
				if(n[1] == 0){
					n[1] = tree.insert(glm::vec3(t->v1()->getPosition()),v/8);
					_vertices[v++] = t->v1()->getPosition().x;
					_vertices[v++] = t->v1()->getPosition().y;
					_vertices[v++] = t->v1()->getPosition().z;
					_vertices[v++] = t->v1()->getNormal().x;
					_vertices[v++] = t->v1()->getNormal().y;
					_vertices[v++] = t->v1()->getNormal().z;
					_vertices[v++] = 0;
					_vertices[v++] = 0;
				}
				if(n[2] == 0){
					n[2] = tree.insert(glm::vec3(t->v2()->getPosition()),v/8);
					_vertices[v++] = t->v2()->getPosition().x;
					_vertices[v++] = t->v2()->getPosition().y;
					_vertices[v++] = t->v2()->getPosition().z;
					_vertices[v++] = t->v2()->getNormal().x;
					_vertices[v++] = t->v2()->getNormal().y;
					_vertices[v++] = t->v2()->getNormal().z;
					_vertices[v++] = 0;
					_vertices[v++] = 0;
				}
				_indices[i++] = n[0]->get();
				_indices[i++] = n[1]->get();
				_indices[i++] = n[2]->get();
			}
		}
	}else{
		
	}
	
	
	glBindBuffer(GL_ARRAY_BUFFER,vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*8*_numVerts, _vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER,0);

	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*_numIndices, _indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);	

	faceType = GL_TRIANGLES;
	

	
}



