#ifndef _RBF_H_
#define _RBF_H_

#include <Util\StopClock.h>

#include "Math\Polynomial.h"
#include "Geometry\KDTree\KDTree.h"
#include "Geometry\Mesh\Vertex.h"

#include "Geometry/CSG/CSG.h"


#include "Math\MatrixInversion.h"

#include <Base\XMLObjectHandler.h>

#include <vector>

#include <Eigen\Dense>



class RBF : public XMLObject{
	friend class RBFSystem;
	float _weight;
	float _center[3];
protected:
	virtual float _eval(float r2) const = 0;
	void addXMLTags(tinyxml2::XMLElement *node){
		node->SetAttribute("x",_center[0]);
		node->SetAttribute("y",_center[1]);
		node->SetAttribute("z",_center[2]);
		node->SetAttribute("weight",_weight);
	}
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

	void setWeight(float weight){
		/*if(!(weight==weight)){
			std::cout << weight << std::endl;
		}*/
		_weight = weight;
	}

	glm::vec3 getCenter()const{
		return glm::vec3(_center[0],_center[1],_center[2]);
	}
	
};
//
//template<unsigned int k>
//class TestRBF : public RBF{
//	virtual float _eval(float r2) const{
//		return (k%2==1)?-1:1 * std::powf(r2,k) * std::log(std::sqrt(r2));
//	}
//public:
//	TestRBF(float cx,float cy,float cz,float weight = 1):RBF(cx,cy,cz,weight){}
//	virtual void save(tinyxml2::XMLNode *parent){
//		auto element = parent->GetDocument()->NewElement("TestRBF");
//		parent->InsertEndChild(element);
//		addXMLTags(element);
//	}
//	virtual std::string toString()const{return "TestRBF";}
//};

class ThinPlateSplineRBF : public RBF{
	virtual float _eval(float r2) const{
		return r2*std::log(std::sqrt(r2));
	}
public:
	ThinPlateSplineRBF(float cx,float cy,float cz,float weight = 1):RBF(cx,cy,cz,weight){}
	virtual void save(tinyxml2::XMLNode *parent){
		auto element = parent->GetDocument()->NewElement("ThinPlateSplineRBF");
		parent->InsertEndChild(element);
		addXMLTags(element);
	}
	virtual std::string toString()const{return "ThinPlateSplineRBF";}
};


class Biharmonic : public RBF{
	virtual float _eval(float r2) const{
		return std::sqrt(r2);
	}
public:
	Biharmonic(float cx,float cy,float cz,float weight = 1):RBF(cx,cy,cz,weight){}
	virtual void save(tinyxml2::XMLNode *parent){
		auto element = parent->GetDocument()->NewElement("Biharmonic");
		parent->InsertEndChild(element);
		addXMLTags(element);
	}
	virtual std::string toString()const{return "Biharmonic";}
};

class Triharmonic : public RBF{
	virtual float _eval(float r2) const{
		return r2*std::sqrt(r2);
	}
public:
	Triharmonic(float cx,float cy,float cz,float weight = 1):RBF(cx,cy,cz,weight){}
	virtual void save(tinyxml2::XMLNode *parent){
		auto element = parent->GetDocument()->NewElement("Triharmonic");
		parent->InsertEndChild(element);
		addXMLTags(element);
	}
	virtual std::string toString()const{return "Triharmonic";}
};

class GausianRBF : public RBF{
	virtual float _eval(float r2) const{
		return std::exp(-_a*r2);
	}
	float _a;
public:
	GausianRBF(float cx,float cy,float cz,float weight = 1.0,float a = 50.0):RBF(cx,cy,cz,weight),_a(a*a){}
	virtual void save(tinyxml2::XMLNode *parent){
		auto element = parent->GetDocument()->NewElement("GausianRBF");
		parent->InsertEndChild(element);
		addXMLTags(element);
		element->SetAttribute("a",std::sqrtf(_a));
	}
	virtual std::string toString()const{return "GausianRBF";}
};

class MultiQuadricRBF : public RBF{
	virtual float _eval(float r2) const{
		return std::sqrt(1 + r2*_a);
	}
	float _a;
public:
	MultiQuadricRBF(float cx,float cy,float cz,float weight = 1.0,float a = 1.0):RBF(cx,cy,cz,weight),_a(a*a){}
	virtual void save(tinyxml2::XMLNode *parent){
		auto element = parent->GetDocument()->NewElement("MultiQuadricRBF");
		parent->InsertEndChild(element);
		addXMLTags(element);
		element->SetAttribute("a",std::sqrtf(_a));
	}
	virtual std::string toString()const{return "MultiQuadricRBF";}
};


class InverseMultiQuadricRBF : public RBF{
	virtual float _eval(float r2) const{
		return 1.0f / std::sqrt(1 + r2*_a);
	}
	float _a;
public:
	InverseMultiQuadricRBF(float cx,float cy,float cz,float weight = 1.0,float a = 1.0):RBF(cx,cy,cz,weight),_a(a*a){}
	virtual void save(tinyxml2::XMLNode *parent){
		auto element = parent->GetDocument()->NewElement("InverseMultiQuadricRBF");
		parent->InsertEndChild(element);
		addXMLTags(element);
		element->SetAttribute("a",std::sqrtf(_a));
	}
	virtual std::string toString()const{return "InverseMultiQuadricRBF";}
};


class RBFSystem : public CSG , public XMLObject{
	friend class RBFFactory;
	friend class MarchingTetrahedra;
	template<class S> friend class __rbf_SubSpace;
	friend float H(RBFSystem *rbf,std::vector<glm::vec4> &points,int x,int y);
	struct TrendFunction{
		float _c[4];
		float eval(float x,float y,float z)const{return _c[0] + x * _c[1] + y * _c[2] + z* _c[3];}
		TrendFunction(){_c[0] = _c[1] = _c[2] = _c[3] = 0;}
		TrendFunction(float c0,float c1,float c2,float c3){_c[0] = c0;_c[1] = c1;_c[2] = c2;_c[3] = c3;}
	}_trend;
	
	glm::vec3 _min,_max;
	std::vector<RBF*> _kernels;
	virtual float eval(glm::vec3 worldPos,bool useTrendFunc);
public:
	virtual float eval(const glm::vec3 &worldPos);
	virtual std::string toString()const{return "RBFSystem";}

	float meanSqError(const std::vector<glm::vec4> &points);
	
	virtual void save(tinyxml2::XMLNode *parent);
	
	glm::vec3 getMin()const{return _min;}
	glm::vec3 getMax()const{return _max;}

	template <typename KernelType> static RBFSystem *HFromPoints(std::vector<glm::vec4> &points);
	template <typename KernelType> static RBFSystem *CreateFromPoints(std::vector<glm::vec4> &points,float w = 0);
	template <typename KernelType,class Solver> static RBFSystem *FastFitting(std::vector<glm::vec4> &points,float smoothNess,float accuracy,int minInnerSize = 500,float outerSize = 2.0f,int coarseGridSize = 4, int maxIterations = 1000);
};


class RBFFactory : public Factory{
protected:
	RBFFactory():Factory("RBFSystem"){}
	static RBFFactory *instance;
public:
	static RBFFactory *GetFactory();

	virtual ~RBFFactory(){}

	virtual std::string toString()const{return "RBFFactory";}
	virtual XMLObject* create(tinyxml2::XMLElement *xmlelement);
};

#ifndef _STANDARDFITTINGRBF_HPP_
#include "StandardFittingRBF.hpp"
#endif
#ifndef _HFITTINGRBF_HPP_
#include "HFittingRBF.hpp"
#endif
#ifndef _FASTFITTINGRBF_HPP_
#include "FastFittingRBF.hpp"
#endif


#endif
