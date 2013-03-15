#ifndef _MARCHINGTETRAHEDRA_H_
#define _MARCHINGTETRAHEDRA_H_

#include <Base/Object.h>
#include "../Mesh/Mesh.h"
#include "ImplicitFunction.h"
#include <Geometry/BoundingGeometry/BoundingVolume.h>


class MarchingTetrahedra : public Object{

	static void _evaluateTetra(Mesh *m,glm::vec3 p0,float v0,glm::vec3 p1,float v1,glm::vec3 p2,float v2,glm::vec3 p3,float v3);
public:
	template<typename MeshType>
	static Mesh* March(ImplicitFunction *function, BoundingAABB box,glm::ivec3 resultion);
};

template<typename MeshType>
static Mesh* MarchingTetrahedra::March(ImplicitFunction *function, BoundingAABB box,glm::ivec3 resultion){
	Mesh *m = new MeshType();
	float x,y,z,dx,dy,dz;
	auto minPos = box.getPosition(glm::vec3(0,0,0));
	auto maxPos = box.getPosition(glm::vec3(1,1,1));
	dx = (maxPos.x - minPos.x)/ (resultion.x);
	dy = (maxPos.y - minPos.y)/ (resultion.y);
	dz = (maxPos.z - minPos.z)/ (resultion.z);
	float v[8];
	glm::vec3 p[8];
	unsigned int tets[6][4] = {
		{0,1,3,5},
		{1,2,3,5},
		{2,3,5,6},
		{0,3,4,5},
		{7,4,3,5},
		{7,6,5,3}
	};
	for(int k = 0;k<resultion.z;k++){
		for(int j = 0;j<resultion.y;j++){
			for(int i = 0;i<resultion.x;i++){
				x = minPos.x + dx * i;
				y = minPos.y + dy * j;
				z = minPos.z + dz * k;

				p[0] = glm::vec3(x,y,z);
				p[1] = glm::vec3(x+dx,y,z);
				p[2] = glm::vec3(x+dx,y+dy,z);
				p[3] = glm::vec3(x,y+dy,z);
				p[4] = glm::vec3(x,y,z+dz);
				p[5] = glm::vec3(x+dx,y,z+dz);
				p[6] = glm::vec3(x+dx,y+dy,z+dz);
				p[7] = glm::vec3(x,y+dy,z+dz);
				
				for(int a = 0;a<8;a++){
					v[a] = function->eval(p[a]);
				}

				for(int a = 0;a<6;a++){
					_evaluateTetra(m,	p[tets[a][0]],v[tets[a][0]],
										p[tets[a][1]],v[tets[a][1]],
										p[tets[a][2]],v[tets[a][2]],
										p[tets[a][3]],v[tets[a][3]]);
				}
			}
		}
	}


	m->calculateNormals();

	return m;
}

#endif
