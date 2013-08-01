#include "PointCloudInterpolation.h"

#include <Eigen\Dense>
#include <Eigen\Sparse>

#include <iostream>

#include <Util\Macros.h>

#include <Math\Plane.h>

float phi(const float &r){
	if(r>=1) return 0;
	if(r<=Eigen::NumTraits<float>::epsilon()) return 1;
	return std::powf(1-r,4)*(4*r+1);
}

float glob_phi(const glm::vec3 x0,const glm::vec3 x1,const float &supportSize){
	return phi(glm::distance(x0,x1)/supportSize);
}

bool sortOverlap (K3DTree<Point>::Node* a,K3DTree<Point>::Node* &b) { 
	return a->get().overlap < b->get().overlap;
}


Center::Center(const Point& p,PointCloudInterpolation *cloud,const PointCloudInterpolationHints &hints):lambda(0),w(0,0,0),P(p.P){
    w = -glm::normalize(p.N);
	findOptimalSupportSize(cloud,hints);
    //supportSize = hints.supportSize;
    cloud->maxSupportSize = std::max(cloud->maxSupportSize,supportSize);
    cloud->minSupportSize = std::min(cloud->minSupportSize,supportSize);
    cloud->avgSupportSize += supportSize;


    auto nodes = cloud->_points.findCloseTo(P,supportSize);
    IT_FOR(nodes,node){
        float ph = phi((*node)->get().P);
        (*node)->get().overlap += ph;
    }
}



void Center::findABCDEF(PointCloudInterpolation *pci){
    auto nodes = pci->_points.findCloseTo(P,supportSize);
    
    w = glm::vec3(0,0,0);
    IT_FOR(nodes,node){
        float ph = phi((*node)->get().P);
        w += (*node)->get().N * ph * (*node)->get().density;
    }
    w = glm::normalize(w);

    if(glm::dot(w,glm::vec3(0,0,1)) != 1){
        u = glm::cross(glm::vec3(0,0,1),w);
    }else{
        u =glm::cross(glm::vec3(0,1,0),w);
    }

    v = glm::cross(u,w);
    
    Eigen::MatrixXf M(6,6);
    Eigen::VectorXf b(6),x(6);
    for(int i = 0;i<6;i++){
        b(i) = 0;
        for(int j = 0;j<6;j++){
            M(i,j) = 0;
        }
    }

    IT_FOR(nodes,node){
        glm::vec3 p = (*node)->get().P;
        float ph,x,y,z,d = (*node)->get().density;
        x = glm::dot(u,p-P);
        y = glm::dot(v,p-P);
        z = glm::dot(w,p-P);
        ph = phi(p)*d;

        M(0,0) += x*x*x*x*ph;	M(0,1) += x*x*y*x*ph;	M(0,2) += x*x*y*y*ph;	M(0,3) += x*x*x*ph;	M(0,4) += x*x*y*ph;	M(0,5) += x*x*ph;
        M(1,0) += 2*x*y*x*x*ph;	M(1,1) += 2*x*y*y*x*ph;	M(1,2) += 2*x*y*y*y*ph;	M(1,3) += 2*x*y*x*ph;	M(1,4) += 2*x*y*y*ph;	M(1,5) += 2*x*y*ph;
        M(2,0) += y*y*x*x*ph;	M(2,1) += y*y*y*x*ph;	M(2,2) += y*y*y*y*ph;	M(2,3) += y*y*x*ph;	M(2,4) += y*y*y*ph;	M(2,5) += y*y*ph;
        M(3,0) += 1*x*x*x*ph;	M(3,1) += 1*x*y*x*ph;	M(3,2) += 1*x*y*y*ph;	M(3,3) += 1*x*x*ph;	M(3,4) += 1*x*y*ph;	M(3,5) += 1*x*ph;
        M(4,0) += 1*y*x*x*ph;	M(4,1) += 1*y*y*x*ph;	M(4,2) += 1*y*y*y*ph;	M(4,3) += 1*y*x*ph;	M(4,4) += 1*y*y*ph;	M(4,5) += 1*y*ph;
        M(5,0) += 1*1*x*x*ph;	M(5,1) += 1*1*y*x*ph;	M(5,2) += 1*1*y*y*ph;	M(5,3) += 1*1*x*ph;	M(5,4) += 1*1*y*ph;	M(5,5) += 1*1*ph;

        b(0) += ph*z*x*x;
        b(1) += 2*ph*z*x*y;
        b(2) += ph*z*y*y;
        b(3) += ph*z*x;
        b(4) += ph*z*y;
        b(5) += ph*z*1;
    }

    x = M.llt().solve(b);
    A = x(0);
    B = x(1);
    C = x(2);
    D = x(3);
    E = x(4);
    F = x(5);
}
void Center::findOptimalSupportSize(PointCloudInterpolation *pci,const PointCloudInterpolationHints &hints){
   float a = hints.minSupportSize;
   float b = hints.maxSupportSize;
   float step = (b-a)/10;
   float curMin = Eigen::NumTraits<float>::highest();
   float cur = a;

   while((b-a)>=(pci->L*10e-8)){
       for(int i = 0;i<10;i++){
           supportSize = cur;
           findABCDEF(pci);
           float err = eSa(pci);
           if(curMin>=err){
               curMin = err;
               a = cur;
               cur += step;
           }else{
               a = cur-step;
               b = cur;
               break;
           }
       }
       step /= 10.0;
   }

}

float Center::eLocal(PointCloudInterpolation *pci){
    if(A!=A || B!=B || C!=C || D!=D || E!=E || F!=F)
        return 999999999;

    float num = 0,denum = 0;
    auto points = pci->_points.findCloseTo(P,supportSize);
	if(points.size()<=2)
		return 999999999;
    IT_FOR(points,p){
        float v = (*p)->get().density;
        v *= phi((*p)->get().P);
        denum += v;
        float g = this->g((*p)->get().P);
        g /= glm::length(dg((*p)->get().P));
        v *= g*g;
        g = this->g((*p)->get().P);
        num += v;
    }
    return std::min(std::sqrt(num/(denum))/pci->L,999999998.0f);
}

float Center::eSa(PointCloudInterpolation *pci){
    float e = eLocal(pci);
    e = e*e;
    e += pci->C / (supportSize*supportSize);
    return e;
}


float Center::g(const glm::vec3 &p){
    float x = glm::dot(u,p-P);
    float y = glm::dot(v,p-P);
    float z = glm::dot(w,p-P);

    return z - (A*x*x + 2*B*x*y + C*y*y + D*x + E*y + F); 
}


glm::vec3 Center::dg(const glm::vec3 &p){
    float x = glm::dot(u,p-P);
    float y = glm::dot(v,p-P);
    float z = glm::dot(w,p-P);

    glm::vec3 grad;
    grad.x = -(2*A*x + B*y + D);
    grad.y = -(B*x + 2*C*y + E);
    grad.z = 1;


    return (u*grad.x) + (v*grad.y) + (w*grad.z); 
}

float Center::phi(const glm::vec3 &worldPos){
    return glob_phi(worldPos,P,supportSize);
}

float Center::PHI(const glm::vec3 &worldPos,PointCloudInterpolation *pci){
    float a = phi(worldPos);
    float b = 0;
    auto nodes = pci->_centers.findCloseTo(worldPos,pci->maxSupportSize);
    IT_FOR(nodes,c){
        b += (*c)->get().phi(worldPos);
    }
    return a/b;
}

PointCloudInterpolation::PointCloudInterpolation(std::vector<glm::vec3> pointCloud, PointCloudInterpolationHints hints,std::vector<glm::vec3> normals):
	maxSupportSize(0),minSupportSize(100000),avgSupportSize(0)
{
	std::vector<K3DTree<Point>::Node*> pointsLeft;
	int _i = 0;
	IT_FOR(pointCloud,p){
		Point point(*p);
		if(normals.size()!=0){
			point.N = normals[_i++];
		}
		pointsLeft.push_back(_points.insert(*p,point));
	}
	
	auto minX = _points.findMin(0)->getPosition()[0];
	auto minY = _points.findMin(1)->getPosition()[1];
	auto minZ = _points.findMin(2)->getPosition()[2];
	auto maxX = _points.findMax(0)->getPosition()[0];
	auto maxY = _points.findMax(1)->getPosition()[1];
	auto maxZ = _points.findMax(2)->getPosition()[2];
	float dx = maxX - minX;
	float dy = maxY - minY;
	float dz = maxZ - minZ;
	_aabb.minPos() = glm::vec3(minX,minY,minZ);
	_aabb.maxPos() = glm::vec3(maxX,maxY,maxZ);

	L = std::sqrt(dx*dx + dy*dy + dz*dz);
    C = L*hints.Tsa;
    C = C*C;
	
	float densSum = 0;
	IT_FOR(_points,node){
		std::vector<glm::vec3> closePoints;
		auto nodes = _points.findNNearest(node->getPosition(),hints.K);
		node->get().density = 0;
		IT_FOR(nodes,p){
			closePoints.push_back((*p)->get().P);
			auto o = (*p)->get().P - node->get().P;
			node->get().density += glm::dot(o,o);
		}
		if(normals.size()==0)
			node->get().N = (Plane(closePoints)).getNormal();
		/*if(glm::dot(node->get().N,node->get().P-glm::vec3(0.5,0.5,0.5))<0)
			node->get().N = -node->get().N;*/
		densSum += node->get().density;
	}
	float avgDens = densSum / _points.size(); 
	std::cout << avgDens << std::endl;
	/*
	IT_FOR(_points,node){
		auto nodes = _points.findNNearest(node->getPosition(),hints.K);
		glm::vec3 n(0,0,0);
		node->get().density /= avgDens;
		IT_FOR(nodes,p){
			n += (*p)->get().N;
		}
		n = glm::normalize(n);
		if(glm::dot(n,node->get().N)<0){
			std::cout << n.x << " " << n.y << " " << n.z << std::endl;
			std::cout << node->get().N.x << " " << node->get().N.y << " " << node->get().N.z << std::endl << std::endl;
			node->get().N = -node->get().N;
		}
	}*/

	while(!pointsLeft.empty()){
		Center c(pointsLeft[0]->get(),this,hints);
		_centers.insert(c.P,c);
		pointsLeft[0]->get().overlap = hints.TOverlap;
		pointsLeft.erase(pointsLeft.begin());
	
	
		std::sort(pointsLeft.begin(),pointsLeft.end(),sortOverlap);
		if(pointsLeft[0]->get().overlap >= hints.TOverlap) //all points left are done
			pointsLeft.clear();
		else if(pointsLeft[pointsLeft.size()-1]->get().overlap < hints.TOverlap){ //non of the points left are done
			continue;
		}
		else{
			int a,b;
			a = 0;
			b = pointsLeft.size()-1;
			while(true){
				int i =  (a+b)/2;
				if(pointsLeft[i]->get().overlap>=hints.TOverlap){
					b = i;
				}else{
					a = i;
				}
				if(b-a==1){
					pointsLeft.erase(pointsLeft.begin()+b+1,pointsLeft.end());
					break;
				}
			}
		}
	}
    std::cout << "Num centers: "       << _centers.size() << std::endl;
    std::cout << "Max support Size: " << maxSupportSize << std::endl;
    std::cout << "Min support Size: " << minSupportSize << std::endl;
    std::cout << "Avg support Size: " << avgSupportSize /  _centers.size()  << std::endl;
	
	float minOverlap = 100000000;
	float maxOverlap = 0;
	IT_FOR(_points,p){
		minOverlap = std::min(minOverlap,(*p).get().overlap);
		maxOverlap = std::max(maxOverlap,(*p).get().overlap);
	}
	std::cout << "Min overlap " << minOverlap << std::endl;
	std::cout << "Max overlap " << maxOverlap << std::endl;

    return;

	int size = _centers.size();

	Eigen::SparseMatrix<float> A(size,size);
	A.reserve(Eigen::VectorXi::Constant(size,100));
	Eigen::VectorXf b(size);
	Eigen::VectorXf l(size);
	int i,j;
	i = j = 0;

	float avgNonZero = 0;
	int numNonZero = 0;
	int maxNonZero = 0;
	int minNonZero = 10000;

	float denom = densSum / (L*L);
	IT_FOR(_centers,c0){
		i = 0;
		float B = 0;
		IT_FOR(_centers,c1){
			auto nodes = _points.findCloseTo(c0->get().P,c0->get().supportSize);
			float v = 0;
			IT_FOR(nodes,point){
                float a = (*point)->get().density;
				if(c0->get().phi((*point)->get().P) == 0 || c1->get().phi((*point)->get().P) == 0)
					continue;
                a *= c0->get().PHI((*point)->get().P,this);
                a *= c1->get().PHI((*point)->get().P,this);
				//a *= phi((*point)->get().P,c0->get().P,c0->get().supportSize); // change this part
				//a *= phi((*point)->get().P,c1->get().P,c1->get().supportSize); // and this part to use PHI instead of phi
				v += a;
				if(i == 0){
					float c = (*point)->get().density;
					c *= c0->get().PHI((*point)->get().P,this);
					c *= -eval((*point)->get().P);
					B += c;
				}
			}
			v /= denom;
			if(c0 == c1){
				float d = 1.0 / c0->get().supportSize;
				d *= d;
				d /= size;
				d *= hints.Treg;
				v += d;
			}
			if(v!=0){
				A.insert(i,j) = v;
				numNonZero++;
			}
			i++;

		}
		b(j) = B / denom;
		j++;
		
		std::cout << j << " ";
	
		avgNonZero += numNonZero;
		minNonZero = std::min(numNonZero,minNonZero);
		maxNonZero = std::max(numNonZero,maxNonZero);
		numNonZero = 0;

	}
	avgNonZero /= j;
	std::cout << std::endl << "Avg: " << avgNonZero << std::endl;
	std::cout << "Min: " << minNonZero << std::endl;
	std::cout << "Max: " << maxNonZero << std::endl;

	Eigen::ConjugateGradient<Eigen::SparseMatrix<float> > cg;
	cg.compute(A);
	switch(cg.info()){
	case Eigen::Success:
		break;
	case Eigen::NumericalIssue:
		std::cerr << "NumericalIssue, The provided data did not satisfy the prerequisites." << std::endl;
		break;
	case Eigen::NoConvergence:
		std::cerr << "NoConvergence, Iterative procedure did not converge." << std::endl;
		break;
	case Eigen::InvalidInput:
		std::cerr << "InvalidInput, The inputs are invalid, or the algorithm has been improperly called. When assertions are enabled, such errors trigger an assert." << std::endl;
		break;
	}

	//cg.setMaxIterations(15);
	//cg.setTolerance(Eigen::NumTraits<float>::epsilon()*10);
	l = cg.solve(b);
	std::cout << "#iterations: " << cg.iterations() << std::endl;
	std::cout << "estimated error: " << cg.error() << std::endl;
	
	i = 0;
	IT_FOR(_centers,c0){
		c0->get().lambda = l(i++);
	}
}

float PointCloudInterpolation::eval(const glm::vec3 &worldPos){
	float v = 0;
	float _ph = 0;
	auto nodes = _centers.findCloseTo(worldPos,maxSupportSize);
	IT_FOR(nodes,cc){
		Center *c = &(*cc)->get();
		float ph = c->phi(worldPos);
        //ph = c->PHI(worldPos,this);
		_ph += ph;
		float g  = c->g(worldPos);
		//if(c->lambda!=0)
		//	g = 0;
		v += (g+c->lambda)*ph;
	}
	return v;
}

float PointCloudInterpolation::eGlobal(){
    float num = 0,denum = 0,f;
    IT_FOR(_points,p){
        f = eval((*p).get().P);
        num   += (*p).get().density * (f*f);
        denum += (*p).get().density;
    }
    return std::sqrt(num/denum)/L;
}