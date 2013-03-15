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



template <typename KernelType>
class RBFSystem : public ImplicitFunction{
	struct TrendFunction{
		float _c[4];
		float eval(float x,float y,float z)const{return _c[0] + x * _c[1] + y * _c[2] + z* _c[3];}
		TrendFunction(){_c[0] = _c[1] = _c[2] = _c[3] = 0;}
		TrendFunction(float c0,float c1,float c2,float c3){_c[0] = c0;_c[1] = c1;_c[2] = c2;_c[3] = c3;}
	}_trend;

	std::vector<KernelType*> _kernels;
public:
	virtual float eval(glm::vec3 worldPos)const;
	virtual std::string toString()const{return "RBFSystem";}


	static RBFSystem *CreateFromPoints(std::vector<Vertex> &tree);
};

template <typename KernelType>
float RBFSystem<KernelType>::eval(glm::vec3 worldPos)const{
	float v = _trend.eval(worldPos.x,worldPos.y,worldPos.z);
	for(auto k = _kernels.begin();k!=_kernels.end();++k){
		v += (*k)->eval(worldPos.x,worldPos.y,worldPos.z);
	}
	return v;
}

template <typename KernelType>
RBFSystem<KernelType> *RBFSystem<KernelType>::CreateFromPoints(std::vector<Vertex> &points){
	RBFSystem<KernelType> *rbfs = new RBFSystem<KernelType>();

	unsigned long size = points.size(); 
	
	Eigen::MatrixXd A = Eigen::MatrixXd::Zero(size+4,size+4);
	Eigen::VectorXd p = Eigen::VectorXd::Zero(size+4);

	//Build a and p
	for(int i = 0;i<size;i++){
		KernelType *rbf= new KernelType(points[i].getPosition().x,points[i].getPosition().y,points[i].getPosition().z);
		rbfs->_kernels.push_back(rbf);
		for(int j = 0;j<size;j++){
			float v = rbf->eval(points[j].getPosition().x,points[j].getPosition().y,points[j].getPosition().z);
			A(i,j) = v;
		}
		A(i,size+0) = 1;
		A(i,size+1) = points[i].getPosition().x;
		A(i,size+2) = points[i].getPosition().y;
		A(i,size+3) = points[i].getPosition().z;
		A(size+0,i) = 1;
		A(size+1,i) = points[i].getPosition().x;
		A(size+2,i) = points[i].getPosition().y;
		A(size+3,i) = points[i].getPosition().z;

		p(i) = points[i].getPosition().w;
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


	Eigen::VectorXd x = A.colPivHouseholderQr().solve(p);
	double relative_error = (A*x - p).norm() / p.norm(); // norm() is L2 norm
	std::cout << A.norm()  << " " << A.determinant() << "" <<  std::endl;
	std::cout << "\tRelative error is:" << relative_error << " x.norm: " << x.norm() << ", p.norm " << p.norm() << std::endl;
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
