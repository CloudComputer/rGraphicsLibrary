#ifndef _MARCHINGTETRAHEDRA_H_
#define _MARCHINGTETRAHEDRA_H_

#include <Base/Object.h>
#include "../Mesh/Mesh.h"
#include "ImplicitFunction.h"

#include "PointCloudInterpolation.h"

#include <Geometry/BoundingGeometry/BoundingVolume.h>

#include <RBF\RBF.h>

#include <Geometry\KDTree\KDTree.h>

#include <Geometry\BoundingGeometry\BoundingVolume.h>

class MarchingTetrahedra : public Object{

	static void _evaluateTetra(Mesh *m,const glm::vec3 &p0,const float &v0,const glm::vec3 &p1,const float &v1,const glm::vec3 &p2,const float &v2,const glm::vec3 &p3,const float &v3);
public:
	template<typename MeshType> static Mesh* March(ImplicitFunction *function,const BoundingAABB &box,const glm::ivec3 &resultion);
	template<typename MeshType> static Mesh* March(RBFSystem *rbf,const int &resultion);
	template<typename MeshType> static Mesh* March(PointCloudInterpolation *pci,const int &resultion);
};

template<typename MeshType>
Mesh* MarchingTetrahedra::March(RBFSystem *rbf, const int &resultion){
	Mesh *mesh = new MeshType();
	CSGCache rbfEval(rbf);

	BoundingAABB box(rbf->_kernels[0]->getCenter(),rbf->_kernels[0]->getCenter());
	K3DTree<int> tree;
	int i = 0;
	for(auto k = rbf->_kernels.begin();k != rbf->_kernels.end();++k){
		tree.insert((*k)->getCenter(),i++);
		box.extend((*k)->getCenter());
	}
	

	float x,y,z,w,h,d,dm;
	auto minPos = box.getPosition(glm::vec3(0,0,0));
	auto maxPos = box.getPosition(glm::vec3(1,1,1));
	w = (maxPos.x - minPos.x);
	h = (maxPos.y - minPos.y);
	d = (maxPos.z - minPos.z);

	float max = std::max(std::max(w,h),d);
	dm = max/resultion;
	glm::ivec3 res;
	res.x = std::ceil(w/dm);
	res.y = std::ceil(h/dm);
	res.z = std::ceil(d/dm);

	std::cout << res.x << " " << res.y << " " << res.z << std::endl;
	
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

	float d3 = 10*std::sqrt(3*dm*dm);
	glm::vec3 center;
	for(int k = -1;k<res.z+1;k++){ //plus/minus one since our bounding box fits the centers of the rbf prefectly
		for(int j = -1;j<res.y+1;j++){
			for(int i = -1;i<res.x+1;i++){
				x = minPos.x + dm * i;
				y = minPos.y + dm * j;
				z = minPos.z + dm * k;
				
				center.x = minPos.x + dm * (i+0.5);
				center.y = minPos.y + dm * (j+0.5);
				center.z = minPos.z + dm * (k+0.5);

								
				/*auto n = tree.findNearest(center);
				glm::vec3 closest(n->getPosition()[0],n->getPosition()[1],n->getPosition()[2]);
				if(glm::distance(center,closest) > d3)
					continue;*/
				
				p[0] = glm::vec3(x,y,z);
				p[1] = glm::vec3(x+dm,y,z);
				p[2] = glm::vec3(x+dm,y+dm,z);
				p[3] = glm::vec3(x,y+dm,z);
				p[4] = glm::vec3(x,y,z+dm);
				p[5] = glm::vec3(x+dm,y,z+dm);
				p[6] = glm::vec3(x+dm,y+dm,z+dm);
				p[7] = glm::vec3(x,y+dm,z+dm);
				
				for(int a = 0;a<8;a++){
					v[a] = rbfEval.eval(p[a]);
				}

				for(int a = 0;a<6;a++){
					_evaluateTetra(mesh,p[tets[a][0]],v[tets[a][0]],
										p[tets[a][1]],v[tets[a][1]],
										p[tets[a][2]],v[tets[a][2]],
										p[tets[a][3]],v[tets[a][3]]);
				}
			}
		}
	}


	mesh->calculateNormals();

	return mesh;
}



template<typename MeshType>
Mesh* MarchingTetrahedra::March(PointCloudInterpolation *pci, const int &resultion){
	Mesh *mesh = new MeshType();
	auto tree = pci->getCenters();
	CSGCache pciEval(pci);

	BoundingAABB box = pci->getAABB();
	float x,y,z,w,h,d,dm;
	auto minPos = box.getPosition(glm::vec3(0,0,0));
	auto maxPos = box.getPosition(glm::vec3(1,1,1));
	w = (maxPos.x - minPos.x);
	h = (maxPos.y - minPos.y);
	d = (maxPos.z - minPos.z);

	float max = std::max(std::max(w,h),d);
	dm = max/resultion;
	glm::ivec3 res;
	res.x = std::ceil(w/dm);
	res.y = std::ceil(h/dm);
	res.z = std::ceil(d/dm);
	
	std::cout << res.x << " " << res.y << " " << res.z << std::endl;

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

	float d3 = 1.5*std::sqrt(3*dm*dm);
	glm::vec3 center;
	for(int k = -1;k<res.z+1;k++){ //plus/minus one since our bounding box fits the centers of the rbf prefectly
		for(int j = -1;j<res.y+1;j++){
			for(int i = -1;i<res.x+1;i++){
				x = minPos.x + dm * i;
				y = minPos.y + dm * j;
				z = minPos.z + dm * k;
				
				center.x = minPos.x + dm * (i+0.5);
				center.y = minPos.y + dm * (j+0.5);
				center.z = minPos.z + dm * (k+0.5);

								
				auto n = tree->findNearest(center);
				if(glm::distance(center,n->get().P) > n->get().supportSize/1.5)
					continue;
				
				p[0] = glm::vec3(x,y,z);
				p[1] = glm::vec3(x+dm,y,z);
				p[2] = glm::vec3(x+dm,y+dm,z);
				p[3] = glm::vec3(x,y+dm,z);
				p[4] = glm::vec3(x,y,z+dm);
				p[5] = glm::vec3(x+dm,y,z+dm);
				p[6] = glm::vec3(x+dm,y+dm,z+dm);
				p[7] = glm::vec3(x,y+dm,z+dm);
				
				for(int a = 0;a<8;a++){
					v[a] = pciEval.eval(p[a]);
				}

				for(int a = 0;a<6;a++){
					_evaluateTetra(mesh,p[tets[a][0]],v[tets[a][0]],
										p[tets[a][1]],v[tets[a][1]],
										p[tets[a][2]],v[tets[a][2]],
										p[tets[a][3]],v[tets[a][3]]);
				}
			}
		}
	}


	mesh->calculateNormals();

	return mesh;
}
	

template<typename MeshType>
Mesh* MarchingTetrahedra::March(ImplicitFunction *function,const BoundingAABB &box,const glm::ivec3 &resultion){
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
