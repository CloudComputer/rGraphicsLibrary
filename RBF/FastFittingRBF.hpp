#ifndef _FASTFITTINGRBF_HPP_
#define _FASTFITTINGRBF_HPP_

#ifndef _RBF_H_
#include "RBF.h"
#endif


#include <Geometry\BoundingGeometry\BoundingVolume.h>
#include <Util\TmpPointer.h>


class __rbf_SortXAxis{
	std::vector<glm::vec4> *_v;
public:
	__rbf_SortXAxis(std::vector<glm::vec4> *v):_v(v){}
	inline bool operator()(const unsigned int &id0,const unsigned int &id1){return (*_v)[id0].x < (*_v)[id1].x;}
};
struct __rbf_SortYAxis{
	std::vector<glm::vec4> *_v;
public:
	__rbf_SortYAxis(std::vector<glm::vec4> *v):_v(v){}
	inline bool operator()(const unsigned int &id0,const unsigned int &id1){return (*_v)[id0].y < (*_v)[id1].y;}
};
struct __rbf_SortZAxis{
	std::vector<glm::vec4> *_v;
public:
	__rbf_SortZAxis(std::vector<glm::vec4> *v):_v(v){}
	inline bool operator()(const unsigned int &id0,const unsigned int &id1){return (*_v)[id0].z < (*_v)[id1].z;}
};

class __rbf_Residual{
	float *_r;
	unsigned int _size;
public:
	__rbf_Residual(unsigned int size){
		_size = size;
		_r = new float[size];
	}
	virtual ~__rbf_Residual(){
		delete _r;
	}
	float &operator[](int id){return _r[id];}

	bool notDone(float accuracy){
		float v = 0;
		for(unsigned int i = 0;i<_size;i++){
			v += std::abs(_r[i]);
			if(v>accuracy)
				return true;
		}
		return false;
	}

	float* R(){return _r;}
};

struct __rbf_SubSpace{
	std::vector<unsigned int> Inner;
	std::vector<unsigned int> Outer;
	
	BoundingAABB innerAABB;
	BoundingAABB outerAABB;

	__rbf_SubSpace():innerAABB(glm::vec3(0,0,0),glm::vec3(0,0,0)),outerAABB(glm::vec3(0,0,0),glm::vec3(0,0,0)){}

	unsigned int sizeInner()const{return Inner.size();}
	unsigned int sizeOuter()const{return Outer.size();}
	unsigned int size()const{return sizeInner() + sizeOuter();}

	template<class T> T &sampleInner(std::vector<T> arr,unsigned int id){
		return arr[Inner[id]];
	}

	template<class T> T &sampleOuter(std::vector<T> arr,unsigned int id){
		return arr[Outer[id]];
	}
	
	template<class T> T &sampleInner(T* arr,unsigned int id){
		return arr[Inner[id]];
	}

	template<class T> T &sampleOuter(T* arr,unsigned int id){
		return arr[Outer[id]];
	}
};


void __rbf_Subdived(std::vector<glm::vec4> &points,unsigned int minSize,__rbf_SubSpace &s,__rbf_SubSpace &left,__rbf_SubSpace &right);

typedef TmpPointer<RBFSystem> tmpRBF;
template<typename KernelType>
RBFSystem *RBFSystem::FastFitting(std::vector<glm::vec4> &points,float accuracy){
	
	RBFSystem *rbf = new RBFSystem();
	if(points.size()==0)
		return rbf;

	std::vector<__rbf_SubSpace> subspace;
	std::vector<__rbf_SubSpace> tmpSpaces;
	tmpSpaces.push_back(__rbf_SubSpace());

	__rbf_Residual residual(points.size());
	BoundingAABB aabb(glm::vec3(points.at(0)),glm::vec3(points.at(0)));
	//K3DTree<int> tree; //
	int i=0;
	for(auto p = points.begin();p!=points.end();++p){
		//tree.insert(glm::vec3(*p),i);

		tmpSpaces[0].innerAABB.extend(glm::vec3(*p));
		tmpSpaces[0].Inner.push_back(i);

		rbf->_kernels.push_back(new KernelType(p->x,p->y,p->z));
		residual[i++] = p->w;
	}

	unsigned int minInnerSize = 500;

	//Subdivde space
	while(!tmpSpaces.empty()){
		__rbf_SubSpace s0,s1;
		__rbf_Subdived(points,minInnerSize,tmpSpaces[0],s0,s1);
		if(s0.sizeInner()>=2*minInnerSize){ //We can split again without invalidate minInnerSize
			tmpSpaces.push_back(s0);
		}else{
			subspace.push_back(s1);
		}

		if(s1.sizeInner()>=2*minInnerSize){ //We can split again without invalidate minInnerSize
			tmpSpaces.push_back(s1);
		}else{
			subspace.push_back(s1);
		}

		tmpSpaces.erase(tmpSpaces.begin()); //remove the splited subspace
	}
	//add points to outer space

	float outerSize = 0.3f; //TODO evaluate best value of outerSize
	for(auto s = subspace.begin();s!=subspace.end();++s){
		auto p0 = s->innerAABB.getPosition(glm::vec3(-outerSize,-outerSize,-outerSize));
		auto p1 = s->innerAABB.getPosition(glm::vec3(1+outerSize,1+outerSize,1+outerSize));
		s->outerAABB = BoundingAABB(p0,p1);
	}

	for(unsigned int i = 0;i<points.size();i++){
		glm::vec3 p = glm::vec3(points[i]);
		for(auto s = subspace.begin();s!=subspace.end();++s){
			if(s->outerAABB.inside(p) && !s->innerAABB.inside(p)){
				s->Outer.push_back(i);
			}
		}
	}

	
	//Choose a coarse grid
	unsigned int pointsFromEach = minInnerSize <= 100 ? 1 : minInnerSize/100;
	std::vector<unsigned int> Y;
	Y.reserve(pointsFromEach*subspace.size());
	for(auto s = subspace.begin();s!=subspace.end();++s){
		//TODO, check for best way of doing this
		for(unsigned int i = 0;i<pointsFromEach;i++){
			Y.push_back(s->Inner[i]);
		}
	}


	while(residual.notDone(accuracy)){
	
	}
	

	return rbf;
}


#endif