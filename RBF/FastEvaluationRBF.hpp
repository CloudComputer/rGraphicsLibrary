#ifndef _FASTEVALUATIONRBF_HPP_
#define _FASTEVALUATIONRBF_HPP_

#ifndef _FASTEVALUATIONRBF_H_
#include "FastEvaluationRBF.h"
#endif


#include <boost/math/special_functions/factorials.hpp>

#include <stack>
#include <iostream>

#include <cmath>
#include <algorithm>


struct SortXAxis{
	inline bool operator()(const glm::vec4 &p0,const glm::vec4 &p1){
		return p0.x < p1.x;
	}
};
struct SortYAxis{
	inline bool operator()(const glm::vec4 &p0,const glm::vec4 &p1){
		return p0.y < p1.y;
	}
};
struct SortZAxis{
	inline bool operator()(const glm::vec4 &p0,const glm::vec4 &p1){
		return p0.z < p1.z;
	}
};


inline int fac(int a){
	return boost::math::factorial<float>(a);
}

int over(int n,int k){
	if(k < 0 || k > n) return 0;
	if(k > n - k)
		k = n - k;
	int c = 1;
	for(int i = 1;i<=k;i++){
		c = c * (n - (k - i));
		c = c / i;
	}
}

glm::vec3 sph2cart(const glm::vec3 &p){
	glm::vec3 a;
	a.x = p.x * std::sin(p.y) * std::cos(p.z);
	a.y = p.x * std::sin(p.y) * std::sin(p.z);
	a.z = p.x * std::cos(p.y);
	return a;
}

glm::vec3 cart2sph(const glm::vec3 &p){
	glm::vec3 a;
	a.x = glm::length(p);
	a.y = std::acos(p.z / a.x);
	a.z = std::atan2(p.y,p.z);
	return a;
}

FE_TMPL FastEvaluationRBFSystemNode<v>::FastEvaluationRBFSystemNode<v>(BoundingAABB aabb,FastEvaluationRBFSystemNode *parrent):
	_aabb(aabb),
	_parrent(parrent),
	_left(0),
	_right(0)
{
}

FE_TMPL FastEvaluationRBFSystemNode<v>::~FastEvaluationRBFSystemNode<v>(){
	if(_left!=0)
		delete _left;
	if(_right!=0)
		delete _right;
}

FE_TMPL void FE_NODE::split(unsigned int threshold){
	float dx,dy,dz;
	int splitAxis;
	dx = _aabb.maxPos().x - _aabb.minPos().x;
	dy = _aabb.maxPos().y - _aabb.minPos().y;
	dz = _aabb.maxPos().z - _aabb.minPos().z; 
	if(dx >= dy && dx >= dz)
		splitAxis = 0;
	else if(dy >= dx && dy >= dz)
		splitAxis = 1;
	else if(dz >= dx && dz >= dy)
		splitAxis = 2;

	glm::vec3 s0,s1;
	s0 = _aabb.maxPos();
	s1 = _aabb.minPos();
	
	switch(splitAxis){
	case 0:
		s0.x -= dx/2;
		s1.x += dx/2;
		break;
	case 1:
		s0.y -= dy/2;
		s1.y += dy/2;
		break;
	case 2:
		s0.z -= dz/2;
		s1.z += dz/2;
		break;
	}

	_left = new FastEvaluationRBFSystemNode(BoundingAABB(_aabb.minPos(),s0),this);
	_right = new FastEvaluationRBFSystemNode(BoundingAABB(s1,_aabb.maxPos()),this);

	for(auto p = _points.begin();p!=_points.end();++p){
		if(_left->_aabb.inside(glm::vec3(*p))){
			_left->_points.push_back(glm::vec4(*p));
		}else if(_right->_aabb.inside(glm::vec3(*p))){
			_right->_points.push_back(glm::vec4(*p));
		}else{
			std::cerr << "FAILED " << __FILE__ << " " << __LINE__ << std::endl;
		}
	}

	_points.clear(); //points are no longer needed, use the points in the children, this save memory, lots of memory



	if(_left->_points.size()<threshold){
		switch(splitAxis){
		case 0:
			std::sort(_right->_points.begin(),_right->_points.end(),SortXAxis());
			break;
		case 1:
			std::sort(_right->_points.begin(),_right->_points.end(),SortYAxis());
			break;
		case 2:
			std::sort(_right->_points.begin(),_right->_points.end(),SortZAxis());
			break;
		}
		
		while(_left->_points.size()<threshold){
			auto node = _right->_points[0];
			_left->_aabb.extend(glm::vec3(node));
			_left->_points.push_back(node);
			_right->_points.erase(_right->_points.begin());
		}

		switch(splitAxis){
		case 0:
			_right->_aabb = BoundingAABB(_left->_aabb.getPosition(glm::vec3(1,0,0)),_aabb.maxPos());
			break;
		case 1:
			_right->_aabb = BoundingAABB(_left->_aabb.getPosition(glm::vec3(0,1,0)),_aabb.maxPos());
			break;
		case 2:
			_right->_aabb = BoundingAABB(_left->_aabb.getPosition(glm::vec3(0,0,1)),_aabb.maxPos());
			break;
		}
	}else if(_right->_points.size()<threshold){
		switch(splitAxis){
		case 0:
			std::sort(_left->_points.begin(),_left->_points.end(),SortXAxis());
			break;
		case 1:
			std::sort(_left->_points.begin(),_left->_points.end(),SortYAxis());
			break;
		case 2:
			std::sort(_left->_points.begin(),_left->_points.end(),SortZAxis());
			break;
		}


		
		while(_right->_points.size()<threshold){
			auto node = _left->_points[_left->_points.size()-1];
			_right->_aabb.extend(glm::vec3(node));
			_right->_points.push_back(node);
			_left->_points.erase(_left->_points.begin());
		}

		
		switch(splitAxis){
		case 0:
			_left->_aabb = BoundingAABB(_aabb.minPos(),_right->_aabb.getPosition(glm::vec3(0,1,1)));
			break;
		case 1:
			_left->_aabb = BoundingAABB(_aabb.minPos(),_right->_aabb.getPosition(glm::vec3(1,0,1)));
			break;
		case 2:
			_left->_aabb = BoundingAABB(_aabb.minPos(),_right->_aabb.getPosition(glm::vec3(1,1,0)));
			break;
		}
	}
	
	_left->_c = _left->_aabb.getPosition(glm::vec3(0.5f,0.5f,0.5f));
	_left->_h = _left->_aabb.maxPos() - _left->_c;
	_right->_c = _right->_aabb.getPosition(glm::vec3(0.5f,0.5f,0.5f));
	_right->_h = _right->_aabb.maxPos() - _right->_c;
}

FE_TMPL void FE_NODE::calcFarFieldSeries(unsigned int p){
	if(_left != 0){
		_left->calcFarFieldSeries(p);
		_right->calcFarFieldSeries(p);

		//TODO calc from children, see section 6
	}else{
		_farSeries.clear();
		for(int k = 0;k<=v;k++)for(int n = 0;n<=p-2*k;n++)for(int m = -n;m<=n;m++){
			std::complex<float> N = _N(n,m,k);
			/*if(N.real() == 0 && N.imag() == 0){
				continue;
			}*/
			_farSeries.push_back(N);
		}
		std::cout << _farSeries.size() << std::endl;
	}
}

FE_TMPL FE_SYS::FastEvaluationRBFSystem(const std::vector<glm::vec4> &points,int threshold,unsigned int p,float relativeAccuracy ){
	if(points.empty()){
		root = 0;
		return;
	}
	
	glm::vec3 _min(points[0]),_max(points[0]);
	root = new FE_NODE(BoundingAABB(glm::vec3(),glm::vec3()));
	for(int i = 1;i<points.size();i++){
		_min.x = std::min(_min.x,points[i].x);
		_min.y = std::min(_min.y,points[i].y);
		_min.z = std::min(_min.z,points[i].z);

		_max.x = std::max(_max.x,points[i].x);
		_max.y = std::max(_max.y,points[i].y);
		_max.z = std::max(_max.z,points[i].z);

		root->_points.push_back(glm::vec4(points[i]));
	}
	root->_aabb = BoundingAABB(_min,_max);
	root->_c = root->_aabb.getPosition(glm::vec3(0.5f,0.5f,0.5f));
	root->_h = root->_aabb.maxPos() - root->_c;

	std::vector<FE_NODE*> nodes;
	nodes.push_back(root);
	while(!nodes.empty()){
		auto node = nodes[0];
		nodes.erase(nodes.begin());
		if(node->_points.size()<threshold*2){ //only split nodes with points more than threshold*2
			continue;
		}
		node->split(threshold);
		nodes.push_back(node->_left);
		nodes.push_back(node->_right);
	}


	root->calcFarFieldSeries(p);


}

FE_TMPL
FE_SYS::~FastEvaluationRBFSystem(){
	if(root!=0)
		delete root;
}


FE_TMPL std::complex<float> FE_NODE::_N(int n,int m,int k){
	std::complex<float> a = 0;
	float b = FE_SYS::_a(k,n+2*k);
	if(b == 0)
		return 0;
	for(auto p = _points.begin() ; p != _points.end() ; ++p){
		//BUG
		float d = p->w; //TODO check if this is correct 
		glm::vec3 xc = glm::vec3(*p)-_c;
		a += d * std::pow(glm::dot(xc,xc),2.0f*k) * FE_SYS::_I(n,-m,xc);
	}
	a *= (n%2==1) ? - b: b;
	return a;
}


FE_TMPL float FE_SYS::_a(int k,int n){
	if(v==1 && k == 0) return -1.0f / (2*n-1);
	if(v==1 && k == 1) return 1.0f / (2*n-1);

	float a = std::powf(-1.0f,v+k);
	for(int j = 1;j<=v;j++) a*= 2*j-1;
	a *= over(v,k);
	for(int l = 0;l<=v;l++)if(l!=k) a /= 2*n - 2*k - 2*l + 1;
	return a;
}

FE_TMPL std::complex<float> FE_SYS::_I(int n,int m,glm::vec3 x){
	glm::vec3 s = cart2sph(x);
	return std::pow(std::complex<float>(0,-1),-std::abs(m)) * _A(n,m) * std::pow(s.x,n) * _Y(n,m,s.y,s.z);
}


FE_TMPL float FE_SYS::_P(int n,int m,float u){
	if(n<0) return 0;
	if(n < -m || n > m) return 0;
	if(m<0){
		return ((-m)%2==1?-1.f:1.f) * fac(n-m) / fac(n+m) * _P(n,-m,u);
	}
	float a = m%2==1 ? -1 : 1;
	for(int j = 1;j<=m;j++) a *= 2*j - 1;
	a *= std::pow(1-u*u,m/2.0f);
	return a;
}

FE_TMPL float FE_SYS::_A(int n,int m){
	//if(m<0) return _A(n,-m); //not needed
	float a = (n%2==1)?-1:1;
	float b = fac(n-m)*fac(n+m);
	return a / std::sqrt(b);
}

FE_TMPL std::complex<float> FE_SYS::_Y(int n,int m,float theta,float phi){
	std::complex<float> a = 1;
	if(m>0 && m%2==1) a = -1;
	a *= std::sqrt(fac(n-m)/float(fac(n+m)));
	a *= _P(n,m,std::cos(theta));
	a *= std::exp(std::complex<float>(0,m*phi));
	return a;
}


#endif