#ifndef _HFITTINGRBF_HPP_
#define _HFITTINGRBF_HPP_

#ifndef _RBF_H_
#include "RBF.h"
#endif


template <typename KernelType> static RBFSystem *RBFSystem::HFromPoints(std::vector<glm::vec4> &points){
	auto rbf = new RBFSystem();
	rbf->_min = glm::vec3(0,0,0);
	rbf->_max = glm::vec3(1,1,1);

	//int M = points.size();
	//int l = 4;

	//for(int i = 0;i<M;i++){
	//	auto p = points[i];
	//	rbf->_kernels.push_back(new KernelType(p.x,p.y,p.z));
	//}

	//Eigen::MatrixXf A = Eigen::MatrixXf::Identity(M,M+l);
	//
	//for(int i = 0;i<M-l;i++){//for(int j = 0;j<l;j++){
	//	A(l+i,j+0) = 1;
	//	A(l+i,j+1) = points[l+i].x;
	//	A(l+i,j+2) = points[l+i].y;
	//	A(l+i,j+3) = points[l+i].z;
	//}

	//for(int i = 0;i<M-l;i++)for(int j = 0;j<M;j++){
	//	int row = l + i;
	//	int col = l + j;
	//	A(row,col) = H(rbf,points,i,j);
	//}


	return rbf;
}


#endif