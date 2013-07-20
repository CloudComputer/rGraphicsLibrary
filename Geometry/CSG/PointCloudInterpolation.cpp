#include "PointCloudInterpolation.h"

#include <Eigen\Dense>

#include <iostream>

#include <Util\Macros.h>

#define DBG() std::cout << __FUNCTION__ << ":" << __LINE__ << std::endl;

PointCloudPoint::PointCloudPoint(const glm::vec3 &P,const glm::vec3 &N):P(P),N(N),shift(0){
	w = N;
	if(glm::dot(w,glm::vec3(0,0,1)) != 1){
		u = glm::cross(glm::vec3(0,0,1),w);
	}else{
		u = glm::normalize(glm::cross(glm::vec3(0,1,0),w));
	}
	v = glm::normalize(glm::cross(u,w));
}

float PointCloudPoint::G(const glm::vec3 &p){
	glm::vec3 uvw;
	uvw.x = glm::dot(u,p-P);
	uvw.y = glm::dot(v,p-P);
	uvw.z = glm::dot(w,p-P);
	
	return uvw.z - (A*uvw.x*uvw.x + B*uvw.x*uvw.y + C*uvw.y*uvw.y);
}

void PointCloudPoint::init(SubPointCloud *owner){
	Eigen::Matrix3f M;
	Eigen::Vector3f C;
	M(0,0) = M(0,1) = M(0,2) = 0;
	M(1,0) = M(1,1) = M(1,2) = 0;
	M(2,0) = M(2,1) = M(2,2) = 0;
	C(0) = C(1) = C(2) = 0;

	IT_FOR(owner->_points,point){
		glm::vec3 p;
		float ph,x,y,z;
		p.x = point->getPosition()[0];
		p.y = point->getPosition()[1];
		p.z = point->getPosition()[2];
		x = glm::dot(u,p-P);
		y = glm::dot(v,p-P);
		z = glm::dot(w,p-P);
		ph = phi(glm::length(glm::vec3(x,y,z)),owner->supportSize);
		
		M(0,0) += x*x*x*x*ph; M(0,1) += x*x*x*y*ph; M(0,2) += x*x*y*y*ph;
		M(1,0) += x*x*x*y*ph; M(1,1) += x*x*y*y*ph; M(1,2) += x*y*y*y*ph;
		M(2,0) += x*x*y*y*ph; M(2,1) += x*y*y*y*ph; M(2,2) += y*y*y*y*ph;
		
		C(0) += (x*x*z)*ph;
		C(1) += (x*y*z)*ph;
		C(2) += (y*y*z)*ph;
	}
	Eigen::Vector3f ABC = Eigen::FullPivHouseholderQR<Eigen::Matrix3f>(M).solve(C);
	this->A = ABC(0);
	this->B = ABC(1);
	this->C = ABC(2);

}

float PointCloudPoint::phi(float _r,float supportSize){
	float r = _r / supportSize;
	if(r>=1) return 0;
	if(r<=Eigen::NumTraits<float>::epsilon()) return 1;
	return std::powf((1-r),4) * (4*r+1);
}

SubPointCloud::SubPointCloud(){
	_childs[0] = 0;
	_childs[1] = 0;
	_childs[2] = 0;
	_childs[3] = 0;
	_childs[4] = 0;
	_childs[5] = 0;
	_childs[6] = 0;
	_childs[7] = 0;
}


SubPointCloud::SubPointCloud(K3DTree<glm::vec3> &p){
	_childs[0] = 0;
	_childs[1] = 0;
	_childs[2] = 0;
	_childs[3] = 0;
	_childs[4] = 0;
	_childs[5] = 0;
	_childs[6] = 0;
	_childs[7] = 0;

	IT_FOR(p,node){
		glm::vec3 pos(node->getPosition()[0],node->getPosition()[1],node->getPosition()[2]);
		PointCloudPoint p(pos,node->get());
		_points.insert(pos,p);
	}
	
	_aabb.minPos().x = _points.findMin(0)->getPosition()[0];
	_aabb.minPos().y = _points.findMin(1)->getPosition()[1];
	_aabb.minPos().z = _points.findMin(2)->getPosition()[2];
	_aabb.maxPos().x = _points.findMax(0)->getPosition()[0];
	_aabb.maxPos().y = _points.findMax(1)->getPosition()[1];
	_aabb.maxPos().z = _points.findMax(2)->getPosition()[2];

	subdivide();
}


SubPointCloud::~SubPointCloud(){
	delete _childs[0];
	delete _childs[1];
	delete _childs[2];
	delete _childs[3];
	delete _childs[4];
	delete _childs[5];
	delete _childs[6];
	delete _childs[7];
	_childs[0] = 0;
	_childs[1] = 0;
	_childs[2] = 0;
	_childs[3] = 0;
	_childs[4] = 0;
	_childs[5] = 0;
	_childs[6] = 0;
	_childs[7] = 0;
}


bool SubPointCloud::hasChilds(){
	return _childs[0] != 0 ||
		   _childs[1] != 0 ||
		   _childs[2] != 0 ||
		   _childs[3] != 0 ||
		   _childs[4] != 0 ||
		   _childs[5] != 0 ||
		   _childs[6] != 0 ||
		   _childs[7] != 0;
}

float SubPointCloud::eval(const glm::vec3 &worldPos){
	float val = 0;
	if(hasChilds()){
		if(_childs[0]) val += _childs[0]->eval(worldPos);
		if(_childs[1]) val += _childs[1]->eval(worldPos);
		if(_childs[2]) val += _childs[2]->eval(worldPos);
		if(_childs[3]) val += _childs[3]->eval(worldPos);
		if(_childs[4]) val += _childs[4]->eval(worldPos);
		if(_childs[5]) val += _childs[5]->eval(worldPos);
		if(_childs[6]) val += _childs[6]->eval(worldPos);
		if(_childs[7]) val += _childs[7]->eval(worldPos);
	}else{
		IT_FOR(_points,p){
			glm::vec3 pos(p->getPosition()[0],p->getPosition()[1],p->getPosition()[2]);
			val += (p->get().shift + p->get().G(worldPos)) *  p->get().phi(glm::length(worldPos-pos),supportSize);
		}
	}

	return val;
}

void SubPointCloud::init(){
	if(hasChilds()){
		if(_childs[0])_childs[0]->init();
		if(_childs[1])_childs[1]->init();
		if(_childs[2])_childs[2]->init();
		if(_childs[3])_childs[3]->init();
		if(_childs[4])_childs[4]->init();
		if(_childs[5])_childs[5]->init();
		if(_childs[6])_childs[6]->init();
		if(_childs[7])_childs[7]->init();
	}else{
		supportSize = 0.75*glm::distance(_aabb.maxPos() , _aabb.minPos());
		IT_FOR(_points,p)
			p->get().init(this);
	}
}

void SubPointCloud::subdivide(){
	int a = -1;
	for(int i = 0;i<2;i++)for(int j = 0;j<2;j++)for(int k = 0;k<2;k++){
		glm::vec3 minT(i/2.0,j/2.0,k/2.0);
		glm::vec3 maxT = minT + glm::vec3(0.5,0.5,0.5);
		_childs[++a] = new SubPointCloud();
		_childs[a]->_aabb.minPos() = _aabb.getPosition(minT);
		_childs[a]->_aabb.maxPos() = _aabb.getPosition(maxT);
	}
	IT_FOR(_points,p){
		glm::vec3 pos;
		pos.x = p->getPosition()[0];
		pos.y = p->getPosition()[1];
		pos.z = p->getPosition()[2];
		for(int c = 0;c<8;c++){
			if(_childs[c]->_aabb.inside(pos)){
				_childs[c]->_points.insert(pos,p->get());
				break;
			}
		}
	}
	bool makeLeaf = false;
	for(int c = 0;c<8;c++){
		const unsigned int size = _childs[c]->_points.size();
		if(size>=8)
			_childs[c]->subdivide();
		else if(size==0){
			delete _childs[c];
			_childs[c] = 0;
		}else if(size == 1){
			makeLeaf = true;
			break;
		}
	}
	if(makeLeaf){
		delete _childs[0];
		delete _childs[1];
		delete _childs[2];
		delete _childs[3];
		delete _childs[4];
		delete _childs[5];
		delete _childs[6];
		delete _childs[7];
		_childs[0] = 0;
		_childs[1] = 0;
		_childs[2] = 0;
		_childs[3] = 0;
		_childs[4] = 0;
		_childs[5] = 0;
		_childs[6] = 0;
		_childs[7] = 0;
	}else{
		_points.clear();
	}
}

PointCloudInterpolation::PointCloudInterpolation(K3DTree<glm::vec3> &points):_points(points){
	_points.init();
}


PointCloudInterpolation::~PointCloudInterpolation(){

}


float PointCloudInterpolation::eval(const glm::vec3 &worldPos){
	return _points.eval(worldPos);
}
