#ifndef _RBF_H_
#define _RBF_H_


#include <Util\StopClock.h>

#include "Math\Polynomial.h"
#include "Geometry\KDTree\KDTree.h"
#include "Geometry\Mesh\Vertex.h"

#include "Geometry/CSG/ImplicitFunction.h"


#include "Math\MatrixInversion.h"


#include <vector>

#include <Eigen\Dense>


class RBF{
	float _weight;
	float _center[3];
	virtual float _eval(float r2) const = 0;
public:
	RBF(float cx,float cy,float cz,float weight = 1):_weight(weight){
		_center[0] = cx;
		_center[1] = cy;
		_center[2] = cz;
	}
	float eval(float x,float y,float z) const{
		float r = 0,d;

		d = _center[0] - x;
		r += d*d;

		d = _center[1] - y;
		r += d*d;

		d = _center[2] - z;
		r += d*d;

		return _eval(r) * _weight;
	}

	void setWeight(float weight){_weight = weight;}
	
};

class ThinPlateSplineRBF : public RBF{
	virtual float _eval(float r2) const{
		return r2*std::log(std::sqrt(r2));
		
	}
public:
	ThinPlateSplineRBF(float cx,float cy,float cz,float weight = 1):RBF(cx,cy,cz,weight){}
};


class Biharmonic : public RBF{
	virtual float _eval(float r2) const{
		return std::sqrt(r2);
	}
public:
	Biharmonic(float cx,float cy,float cz,float weight = 1):RBF(cx,cy,cz,weight){}
};

class Triharmonic : public RBF{
	virtual float _eval(float r2) const{
		return r2*std::sqrt(r2);
	}
public:
	Triharmonic(float cx,float cy,float cz,float weight = 1):RBF(cx,cy,cz,weight){}
};

class GausianRBF : public RBF{
	virtual float _eval(float r2) const{
		return std::exp(-_a*r2);
	}
	float _a;
public:
	GausianRBF(float cx,float cy,float cz,float weight = 1.0,float a = 1.0):RBF(cx,cy,cz,weight),_a(a*a){}
};

class MultiQuadricRBF : public RBF{
	virtual float _eval(float r2) const{
		return std::sqrt(1 + r2*_a);
	}
	float _a;
public:
	MultiQuadricRBF(float cx,float cy,float cz,float weight = 1.0,float a = 1.0):RBF(cx,cy,cz,weight),_a(a*a){}
};


class InverseMultiQuadricRBF : public RBF{
	virtual float _eval(float r2) const{
		return 1.0f / std::sqrt(1 + r2*_a);
	}
	float _a;
public:
	InverseMultiQuadricRBF(float cx,float cy,float cz,float weight = 1.0,float a = 1.0):RBF(cx,cy,cz,weight),_a(a*a){}
};




class RBFSystem : public ImplicitFunction{
	struct TrendFunction{
		float _c[4];
		float eval(float x,float y,float z)const{return _c[0] + x * _c[1] + y * _c[2] + z* _c[3];}
		TrendFunction(){_c[0] = _c[1] = _c[2] = _c[3] = 0;}
		TrendFunction(float c0,float c1,float c2,float c3){_c[0] = c0;_c[1] = c1;_c[2] = c2;_c[3] = c3;}
	}_trend;
	
	glm::vec3 _min,_max;
	std::vector<RBF*> _kernels;
public:
	virtual float eval(glm::vec3 worldPos)const;
	virtual std::string toString()const{return "RBFSystem";}

	template <typename KernelType> static RBFSystem *CreateFromPoints(std::vector<glm::vec4> &points,float w = 0);
	//template <typename KernelType> 
	static RBFSystem *FastFitting(std::vector<glm::vec4> &points,float accuracy);
};


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
	
	Eigen::MatrixXf A = Eigen::MatrixXf::Zero(size+4,size+4);
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
	double relative_error = (A*x - p).norm() / p.norm(); // norm() is L2 norm
	//std::cout << A.norm()  << " " << A.determinant() << "" <<  std::endl;
	std::cout << "\tRelative error is:" << relative_error <<  std::endl;
	//std::cout << x  << std::endl  << std::endl;
	for(int i = 0;i<size;i++){
		rbfs->_kernels[i]->setWeight(x(i));
	}
	rbfs->_trend._c[0] = x(size+0);
	rbfs->_trend._c[1] = x(size+1);
	rbfs->_trend._c[2] = x(size+2);
	rbfs->_trend._c[3] = x(size+3);
	

	return rbfs;
}

#endif
