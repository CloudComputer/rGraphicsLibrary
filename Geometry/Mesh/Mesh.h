#ifndef _MESH_H_
#define _MESH_H_

#include <Base/Object.h>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Geometry/BoundingGeometry/BoundingVolume.h>

#include "Vertex.h"
#include "Face.h"


class Mesh : public Object{
public:
	Mesh(){}
	virtual ~Mesh(){}
	virtual Vertex *addVertex(glm::vec3 &pos, glm::vec3 &normal) = 0;
	virtual Face* addFace(std::vector<glm::vec3> &positions) = 0;

	virtual void calculateNormals() = 0;

	virtual float distance (glm::vec3 &worldPos , bool signedDistance) = 0;
	virtual BoundingAABB getBoundingAABB()const = 0;



	template<typename MeshType>
	static Mesh* LoadWavefront(const char* filename);
	
};

template<typename MeshType> Mesh* Mesh::LoadWavefront(const char* filename){
	Mesh *mesh = new MeshType();

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
			std::vector<glm::vec3> pos;
			pos.push_back(positions[i0]);
			pos.push_back(positions[i1]);
			pos.push_back(positions[i2]);
			mesh->addFace(pos);
		}else {
			inFile.getline(line,256);
			std::cout << line << std::endl;
		}
	}
	mesh->calculateNormals();
	return mesh;
}


#endif