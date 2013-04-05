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
			v += _r[i]*_r[i];
		}
		v = std::sqrt(v) / _size;
		std::cout << "Current ||rg||:" << v << "||rg|| - e = "  << v - accuracy << std::endl;
		return v>accuracy;
	}

	float* R(){return _r;}
};

struct __rbf_SubSpace{
	std::vector<unsigned int> Inner;
	std::vector<unsigned int> Outer;
	
	BoundingAABB innerAABB;
	BoundingAABB outerAABB;

	Eigen::HouseholderQR<Eigen::MatrixXf> S;

	//Eigen::MatrixXf A;
	Eigen::VectorXf b;

	__rbf_SubSpace():innerAABB(glm::vec3(0,0,0),glm::vec3(0,0,0)),outerAABB(glm::vec3(0,0,0),glm::vec3(0,0,0)){}

	unsigned int sizeInner()const{return Inner.size();}
	unsigned int sizeOuter()const{return Outer.size();}
	unsigned int size()const{return sizeInner() + sizeOuter();}

	template<class T> T &sampleInner(std::vector<T> &arr,unsigned int id){
		return arr[Inner[id]];
	}

	template<class T> T &sampleOuter(std::vector<T> &arr,unsigned int id){
		return arr[Outer[id]];
	}
	
	template<class T> T &sampleInner(T* arr,unsigned int id){
		return arr[Inner[id]];
	}

	template<class T> T &sampleOuter(T* arr,unsigned int id){
		return arr[Outer[id]];
	}

	
	
	float &sampleInner(Eigen::VectorXf &arr,unsigned int id){
		return arr(Inner[id]);
	}

	float &sampleOuter(Eigen::VectorXf &arr,unsigned int id){
		return arr(Outer[id]);
	}

	void buildMatrix(RBFSystem *s,std::vector<glm::vec4> &points,bool usePolynomial = false){
		int sizeI = sizeInner();
		int sizeO = sizeOuter();
		int extra = usePolynomial ? 4 : 0;
		Eigen::MatrixXf A = Eigen::MatrixXf::Zero(sizeI+sizeO+extra,sizeI+extra);
		for(int i = 0;i<sizeI;i++){
			for(int j = 0;j<sizeI;j++){
				auto p = sampleInner(points,j);
				auto k = sampleInner(s->_kernels,i);
				A(i,j) = k->eval(p.x,p.y,p.z);
			}
			if(usePolynomial){
				A(i,sizeI+0) = 1;
				A(i,sizeI+1) = sampleInner(points,i).x;
				A(i,sizeI+2) = sampleInner(points,i).y;
				A(i,sizeI+3) = sampleInner(points,i).z;
				A(sizeI+sizeO+0,i) = 1;
				A(sizeI+sizeO+1,i) = sampleInner(points,i).x;
				A(sizeI+sizeO+2,i) = sampleInner(points,i).y;
				A(sizeI+sizeO+3,i) = sampleInner(points,i).z;
			}
		}

		for(int i = 0;i<sizeO;i++){
			for(int j = 0;j<sizeI;j++){
				auto p = sampleOuter(points,i);
				A(sizeI+i,j) = sampleInner(s->_kernels,j)->eval(p.x,p.y,p.z);
			}
		}
		
		S = A.householderQr();

	}

	void buildVector(__rbf_Residual &r,bool usePolynomial = false){
		int size1 = sizeInner();
		int size2 = sizeOuter();
		int extra = usePolynomial ? 4 : 0;
		b = Eigen::VectorXf::Zero(size1+size2+extra);

		for(int i = 0;i<size1;i++){
			b(i) = sampleInner(r.R(),i);
		}
		for(int i = 0;i<size2;i++){
			b(size1 + i) = sampleOuter(r.R(),i);
		}
	}
};


void __rbf_Subdived(std::vector<glm::vec4> &points,unsigned int minSize,__rbf_SubSpace &s,__rbf_SubSpace &left,__rbf_SubSpace &right);

typedef TmpPointer<RBFSystem> tmpRBF;
template<typename KernelType>
RBFSystem *RBFSystem::FastFitting(std::vector<glm::vec4> &points,float accuracy,int minInnerSize,float outerSize,int coarseGridSize, int maxIterations){
	if(points.size()==0)
		return new RBFSystem();
	RBFSystem *sg = new RBFSystem();
	TmpPointer<RBFSystem> s1 = new RBFSystem();
	TmpPointer<RBFSystem> s2 = new RBFSystem();

	std::vector<__rbf_SubSpace> subspace;
	std::vector<__rbf_SubSpace> tmpSpaces;
	__rbf_SubSpace coarseGrid;
	tmpSpaces.push_back(__rbf_SubSpace());
	
	__rbf_Residual residual(points.size());
	BoundingAABB aabb(glm::vec3(points.at(0)),glm::vec3(points.at(0)));
	//K3DTree<int> tree; //
	int i=0;
	for(auto p = points.begin();p!=points.end();++p){
		//tree.insert(glm::vec3(*p),i);

		tmpSpaces[0].innerAABB.extend(glm::vec3(*p));
		tmpSpaces[0].Inner.push_back(i);
		
		KernelType *k = new KernelType(p->x,p->y,p->z);
		KernelType *k2 = new KernelType(p->x,p->y,p->z);
		k->_weight = 0;
		k2->_weight = 0;
		sg->_kernels.push_back(k);
		s1->_kernels.push_back(new KernelType(p->x,p->y,p->z));
		s2->_kernels.push_back(k2);
		residual[i++] = p->w;
	}
	
	s2->_min = s1->_min = sg->_min = glm::vec3(0,0,0);
	s2->_max = s1->_max = sg->_max = glm::vec3(1,1,1);

	//Subdivde space
	while(!tmpSpaces.empty()){
		__rbf_SubSpace sub0,sub1;
		__rbf_Subdived(points,minInnerSize,tmpSpaces[0],sub0,sub1);
		if(sub0.sizeInner()>=2*minInnerSize){ //We can split again without invalidate minInnerSize
			tmpSpaces.push_back(sub0);
		}else{
			subspace.push_back(sub0);
		}

		if(sub1.sizeInner()>=2*minInnerSize){ //We can split again without invalidate minInnerSize
			tmpSpaces.push_back(sub1);
		}else{
			subspace.push_back(sub1);
		}

		tmpSpaces.erase(tmpSpaces.begin()); //remove the splited subspace
	}

	//add points to outer space
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
	for(auto s = subspace.begin();s!=subspace.end();++s){
		//TODO, check for best way of doing this
		unsigned int i;
		int left = coarseGridSize;
		int size = s->sizeInner();
		for(i = 0;i<s->sizeInner();i++){
			if(Random::getRandomGenerator()->getFloat() <= float(left) / size--){
				coarseGrid.innerAABB.extend(glm::vec3(s->sampleInner(points,i)));
				coarseGrid.Inner.push_back(s->Inner[i]);
				left--;
			}else{
			//*	
				coarseGrid.outerAABB.extend(glm::vec3(s->sampleInner(points,i)));
				coarseGrid.Outer.push_back(s->Inner[i]);//*/
			}
		}
	}
	coarseGrid.buildMatrix(&*s1,points,true);

	for(auto s = subspace.begin();s!=subspace.end();++s){
		s->buildMatrix(&*s1,points);
	}
	
	std::cout << sg->meanSqError(points) << std::endl;
	int _maxIterations = maxIterations;//TODO maybe remove maxIterations when stable
	while(--_maxIterations&&residual.notDone(accuracy)){
		for(auto s = subspace.begin();s!=subspace.end();++s){//(3)
			s->buildVector(residual); //(4)
			Eigen::VectorXf x = s->S.solve(s->b); //(4)
			
			int size = s->sizeInner(); //(4)
			for(int i = 0;i<size;i++){ //(4)
				s1->_kernels[s->Inner[i]]->setWeight(x(i)); //(4)
			} //(4)
		} //(5)

		//TODO missing (6)

		int pointssize = points.size();
		for(int i = 0;i<pointssize;i++){ //(8)
			residual[i] = residual[i]  - s1->eval(glm::vec3(points[i]));
		}


		//(9)
		coarseGrid.buildVector(residual,true);
		Eigen::VectorXf x1 = coarseGrid.S.solve(coarseGrid.b);
		for(int i = 0;i<coarseGrid.sizeInner();i++){ //
			s2->_kernels[coarseGrid.Inner[i]]->setWeight(x1(i)); //(4)
		}

		for(int i = 0;i<pointssize;i++){
			sg->_kernels[i]->_weight += s1->_kernels[i]->_weight + s2->_kernels[i]->_weight;
		}
		
		//*
		sg->_trend._c[0] += x1(coarseGrid.sizeInner()+0);
		sg->_trend._c[1] += x1(coarseGrid.sizeInner()+1);
		sg->_trend._c[2] += x1(coarseGrid.sizeInner()+2);
		sg->_trend._c[3] += x1(coarseGrid.sizeInner()+3);
		//*/

		for(int i = 0;i<pointssize;i++){
			residual[i] = points[i].w  - sg->eval(glm::vec3(points[i]));
			if(!(residual[i]==residual[i])){
				//std::cout << residual[i] << std::endl;
			}
		}

		std::cout << sg->meanSqError(points) << std::endl;
	}
	
	std::cout << "Iterations:" <<  maxIterations - _maxIterations << std::endl;
	return sg;
}


#endif