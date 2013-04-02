#include "RBF.h"

#include <Geometry\BoundingGeometry\BoundingVolume.h>

#include <Util\TmpPointer.h>

float RBFSystem::eval(glm::vec3 worldPos)const{
	glm::vec3 c = glm::vec3(worldPos - _min.x) / (_max.x - _min.x);

	float v = _trend.eval(c.x,c.y,c.z);
	for(auto k = _kernels.begin();k!=_kernels.end();++k){
		v += (*k)->eval(c.x,c.y,c.z);
	}
	return v;
}





class SortXAxis{
	std::vector<glm::vec4> *_v;
public:
	SortXAxis(std::vector<glm::vec4> *v):_v(v){}
	inline bool operator()(const unsigned int &id0,const unsigned int &id1){return (*_v)[id0].x < (*_v)[id1].x;}
};
struct SortYAxis{
	std::vector<glm::vec4> *_v;
public:
	SortYAxis(std::vector<glm::vec4> *v):_v(v){}
	inline bool operator()(const unsigned int &id0,const unsigned int &id1){return (*_v)[id0].y < (*_v)[id1].y;}
};
struct SortZAxis{
	std::vector<glm::vec4> *_v;
public:
	SortZAxis(std::vector<glm::vec4> *v):_v(v){}
	inline bool operator()(const unsigned int &id0,const unsigned int &id1){return (*_v)[id0].z < (*_v)[id1].z;}
};

class Residual{
	float *_r;
	unsigned int _size;
public:
	Residual(unsigned int size){
		_size = size;
		_r = new float[size];
	}
	virtual ~Residual(){
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

struct SubSpace{
	std::vector<unsigned int> Inner;
	std::vector<unsigned int> Outer;
	
	BoundingAABB innerAABB;
	BoundingAABB outerAABB;

	SubSpace():innerAABB(glm::vec3(0,0,0),glm::vec3(0,0,0)),outerAABB(glm::vec3(0,0,0),glm::vec3(0,0,0)){}

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

void Subdived(std::vector<glm::vec4> &points,unsigned int minSize,SubSpace &s,SubSpace &left,SubSpace &right){
	float dx,dy,dz;
	int splitAxis;
	dx = s.innerAABB.maxPos().x - s.innerAABB.minPos().x;
	dy = s.innerAABB.maxPos().y - s.innerAABB.minPos().y;
	dz = s.innerAABB.maxPos().z - s.innerAABB.minPos().z; 
	if(dx >= dy && dx >= dz)
		splitAxis = 0;
	else if(dy >= dx && dy >= dz)
		splitAxis = 1;
	else if(dz >= dx && dz >= dy)
		splitAxis = 2;

	glm::vec3 s0,s1;
	s0 = s.innerAABB.maxPos();
	s1 = s.innerAABB.minPos();
	
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
	
	left.innerAABB = BoundingAABB(s.innerAABB.minPos(),s0);
	right.innerAABB = BoundingAABB(s1,s.innerAABB.maxPos());

	for(auto i = s.Inner.begin() ; i != s.Inner.end() ; ++i){
		if(left.innerAABB.inside(glm::vec3(points[*i]))){
			left.Inner.push_back(*i);
		}else if(right.innerAABB.inside(glm::vec3(points[*i]))){
			right.Inner.push_back(*i);
		}else{
			std::cerr << "FAILED " << __FILE__ << " " << __LINE__ << std::endl;
		}
	}

	if(left.Inner.size()<minSize){
		switch(splitAxis){
		case 0:
			std::sort(right.Inner.begin(),right.Inner.end(),SortXAxis(&points));
			break;
		case 1:
			std::sort(right.Inner.begin(),right.Inner.end(),SortYAxis(&points));
			break;
		case 2:
			std::sort(right.Inner.begin(),right.Inner.end(),SortZAxis(&points));
			break;
		}
		
		while(left.Inner.size()<minSize){
			auto node = right.Inner[0];
			left.innerAABB.extend(glm::vec3(node));
			left.Inner.push_back(node);
			right.Inner.erase(right.Inner.begin());
		}

		switch(splitAxis){
		case 0:
			right.innerAABB = BoundingAABB(left.innerAABB.getPosition(glm::vec3(1,0,0)),s.innerAABB.maxPos());
			break;
		case 1:
			right.innerAABB = BoundingAABB(left.innerAABB.getPosition(glm::vec3(0,1,0)),s.innerAABB.maxPos());
			break;
		case 2:
			right.innerAABB = BoundingAABB(left.innerAABB.getPosition(glm::vec3(0,0,1)),s.innerAABB.maxPos());
			break;
		}
	}else if(right.Inner.size()<minSize){	
		switch(splitAxis){
		case 0:
			std::sort(left.Inner.begin(),left.Inner.end(),SortXAxis(&points));
			break;
		case 1:
			std::sort(left.Inner.begin(),left.Inner.end(),SortYAxis(&points));
			break;
		case 2:
			std::sort(left.Inner.begin(),left.Inner.end(),SortZAxis(&points));
			break;
		}
		
		while(right.Inner.size()<minSize){
			auto node = left.Inner[0];
			right.innerAABB.extend(glm::vec3(node));
			right.Inner.push_back(node);
			left.Inner.erase(left.Inner.begin());
		}

		switch(splitAxis){
		case 0:
			left.innerAABB = BoundingAABB(right.innerAABB.getPosition(glm::vec3(1,0,0)),s.innerAABB.maxPos());
			break;
		case 1:
			left.innerAABB = BoundingAABB(right.innerAABB.getPosition(glm::vec3(0,1,0)),s.innerAABB.maxPos());
			break;
		case 2:
			left.innerAABB = BoundingAABB(right.innerAABB.getPosition(glm::vec3(0,0,1)),s.innerAABB.maxPos());
			break;
		}
	}

}

typedef TmpPointer<RBFSystem> tmpRBF;
RBFSystem *RBFSystem::FastFitting(std::vector<glm::vec4> &points,float accuracy){
	
	RBFSystem *rbf = new RBFSystem();
	if(points.size()==0)
		return rbf;

	Residual residual(points.size());

	BoundingAABB aabb(glm::vec3(points.at(0)),glm::vec3(points.at(0)));
	K3DTree<int> tree; //
	int i=0;
	for(auto p = points.begin();p!=points.end();++p){
		tree.insert(glm::vec3(*p),i);
		aabb.extend(glm::vec3(*p));
		rbf->_kernels.push_back(new Biharmonic(p->x,p->y,p->z));
		residual[i++] = p->w;
	}

	unsigned int minInnerSize = 500;

	std::vector<SubSpace> subspace;
	std::vector<SubSpace> tmpSpaces;
	//Subdivde space
	while(!tmpSpaces.empty()){
		SubSpace s0,s1;
		Subdived(points,minInnerSize,tmpSpaces[0],s0,s1);
		if(s0.sizeInner()>=2*minInnerSize){ //We can split again without invalidate minInnerSize
			tmpSpaces.push_back(s0);
		}else{
			subspace.push_back(s0);
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