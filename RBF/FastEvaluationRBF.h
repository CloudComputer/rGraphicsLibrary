#ifndef _FASTEVALUATIONRBF_H_
#define _FASTEVALUATIONRBF_H_

#include <complex>

#include <glm\glm.hpp>
#include <vector>

#include <Geometry\BoundingGeometry\BoundingVolume.h>



#define FE_TMPL template <unsigned int v>
#define FE_SYS FastEvaluationRBFSystem<v>
#define FE_NODE FastEvaluationRBFSystemNode<v>

FE_TMPL class FastEvaluationRBFSystem;
FE_TMPL class FastEvaluationRBFSystemNode;

FE_TMPL class FastEvaluationRBFSystemNode{
	friend class FE_SYS;

	BoundingAABB _aabb;
	std::vector<glm::vec4> _points;
	std::vector<std::complex<float>> _farSeries;

	glm::vec3 _c,_h;
	
	FE_NODE *_parrent;
	FE_NODE *_left;
	FE_NODE *_right;

	FastEvaluationRBFSystemNode(BoundingAABB aabb,FE_NODE *parrent = 0);
	
	std::complex<float> _N(int n,int m,int k);

	void split(unsigned int threshold);
	void calcFarFieldSeries(unsigned int p);
public:
	virtual ~FastEvaluationRBFSystemNode();
};

FE_TMPL class FastEvaluationRBFSystem{
	friend class FE_NODE;
	FE_NODE *root;
	
	static float _a(int k,int n);
	static float _A(int n,int m);
	static float _P(int n,int m,float u);
	static std::complex<float> _Y(int n,int m,float theta,float phi);
	static std::complex<float> _I(int n,int m,glm::vec3 x);
	static std::complex<float> _O(int n,int m,glm::vec3 x);


public:
	FastEvaluationRBFSystem(const std::vector<glm::vec4> &points,int threshold = 100,unsigned int p = 15,float relativeAccuracy = 0.001);
	virtual ~FastEvaluationRBFSystem();

};

#ifndef _FASTEVALUATIONRBF_HPP_
#include "FastEvaluationRBF.hpp"
#endif


#endif