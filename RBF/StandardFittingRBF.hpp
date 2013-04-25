#ifndef _STANDARDFITTINGRBF_HPP_
#define _STANDARDFITTINGRBF_HPP_

#ifndef _RBF_H_
#include "RBF.h"
#endif




template <typename KernelType>
RBFSystem *RBFSystem::CreateFromPoints(std::vector<glm::vec4> &points,float w){
	RBFSystem *rbfs = new RBFSystem();
	
	rbfs->_min = glm::vec3(points[0]);
	rbfs->_max = glm::vec3(points[0]);
	for(auto _p = points.begin()+1;_p != points.end() ; _p++){
		rbfs->_min.x = std::min(rbfs->_min.x,_p->x);
		rbfs->_min.y = std::min(rbfs->_min.y,_p->y);
		rbfs->_min.z = std::min(rbfs->_min.z,_p->z);
		rbfs->_max.x = std::max(rbfs->_max.x,_p->x);
		rbfs->_max.y = std::max(rbfs->_max.y,_p->y);
		rbfs->_max.z = std::max(rbfs->_max.z,_p->z);
	}

	unsigned long size = points.size(); 
	Eigen::MatrixXf A;
	try{
		A = Eigen::MatrixXf::Zero(size+4,size+4);
	}catch(...){
		delete rbfs;
		return 0;
	}
	Eigen::VectorXf p = Eigen::VectorXf::Zero(size+4);

	//Build a and p
	for(int i = 0;i<size;i++){
		glm::vec3 c = glm::vec3(points[i] - rbfs->_min.x) / (rbfs->_max.x - rbfs->_min.x);
		KernelType *rbf= new KernelType(c.x,c.y,c.z);
		rbfs->_kernels.push_back(rbf);
		for(int j = 0;j<size;j++){
			glm::vec3 b = glm::vec3(points[j] - rbfs->_min.x) / (rbfs->_max.x - rbfs->_min.x);
			float v = rbf->eval(b.x,b.y,b.z);
			A(i,j) = v;
		}
		A(i,i) -= w;
		A(i,size+0) = 1;
		A(i,size+1) = c.x;
		A(i,size+2) = c.y;
		A(i,size+3) = c.z;
		A(size+0,i) = 1;
		A(size+1,i) = c.x;
		A(size+2,i) = c.y;
		A(size+3,i) = c.z;

		p(i) = points[i].w;
	}

	// Calc Ax = p
	/*
	std::cout << A  << std::endl  << std::endl;
	std::cout << p  << std::endl  << std::endl;*/
	
	/*
	{
		StopClock s;
		s.start();
		Eigen::VectorXd x = A.partialPivLu().solve(p);
		s.stop();
		std::cout << "partialPivLu finshged after: " << s.getFractionElapsedSeconds() << " relative error: " << ((A*x - p).norm() / p.norm()) << std::endl; 
	}
	
	{
		StopClock s;
		s.start();
		Eigen::VectorXd x = A.fullPivLu().solve(p);
		s.stop();
		std::cout << "fullPivLu finshged after: " << s.getFractionElapsedSeconds() << " relative error: " << ((A*x - p).norm() / p.norm()) << std::endl; 
	}
	
	{
		StopClock s;
		s.start();
		Eigen::VectorXd x = A.householderQr().solve(p);
		s.stop();
		std::cout << "householderQr finshged after: " << s.getFractionElapsedSeconds() << " relative error: " << ((A*x - p).norm() / p.norm()) << std::endl; 
	}
	
	{
		StopClock s;
		s.start();
		Eigen::VectorXd x = A.colPivHouseholderQr().solve(p);
		s.stop();
		std::cout << "colPivHouseholderQr finshged after: " << s.getFractionElapsedSeconds() << " relative error: " << ((A*x - p).norm() / p.norm()) << std::endl; 
	}
	
	{
		StopClock s;
		s.start();
		Eigen::VectorXd x = A.fullPivHouseholderQr().solve(p);
		s.stop();
		std::cout << "fullPivHouseholderQr finshged after: " << s.getFractionElapsedSeconds() << " relative error: " << ((A*x - p).norm() / p.norm()) << std::endl; 
	}
	
	{
		StopClock s;
		s.start();
		Eigen::VectorXd x = A.llt().solve(p);
		s.stop();
		std::cout << "llt finshged after: " << s.getFractionElapsedSeconds() << " relative error: " << ((A*x - p).norm() / p.norm()) << std::endl; 
	}

	{
		StopClock s;
		s.start();
		Eigen::VectorXd x = A.ldlt().solve(p);
		s.stop();
		std::cout << "ldlt finshged after: " << s.getFractionElapsedSeconds() << " relative error: " << ((A*x - p).norm() / p.norm()) << std::endl; 
	}//*/


	//Eigen::VectorXf x = A.colPivHouseholderQr().solve(p);
	Eigen::VectorXf x = A.partialPivLu().solve(p);
	//double relative_error = (A*x - p).norm() / p.norm(); // norm() is L2 norm
	//std::cout << A.norm()  << " " << A.determinant() << "" <<  std::endl;
	//std::cout << "\tRelative error is:" << relative_error <<  std::endl;
	//std::cout << x  << std::endl  << std::endl;
	for(int i = 0;i<size;i++){
		rbfs->_kernels[i]->setWeight(x(i));
	}
	rbfs->_trend._c[0] = x(size+0);
	rbfs->_trend._c[1] = x(size+1);
	rbfs->_trend._c[2] = x(size+2);
	rbfs->_trend._c[3] = x(size+3);
	
	//std::cout << "meanSqError is: " <<  rbfs->meanSqError(points);

	return rbfs;
}

#endif
