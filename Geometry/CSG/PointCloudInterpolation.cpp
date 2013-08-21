#include "PointCloudInterpolation.h"

#include <Eigen\Dense>
#include <Eigen\Sparse>

#include <iostream>

#include <Util\Macros.h>
#include <Util\Logger.h>
#include <Math\Plane.h>

glm::vec3 vp(0,1,0);

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
    w = glm::normalize(p.N);
    findOptimalSupportSize(cloud,hints);
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
    
    //w = glm::vec3(0,0,0);
    //IT_FOR(nodes,node){
    //    float ph = phi((*node)->get().P);
    //    w += (*node)->get().N * ph * (*node)->get().density;
    //}
    //w = glm::normalize(w);
	/*glm::vec3 test = P -pci-> _avgCenter;
	if(glm::dot(w,test)<0)
		w = -w;*/

    if(glm::dot(w,glm::vec3(0,0,1)) != 1){
        u = glm::cross(glm::vec3(0,0,1),w);
    }else{
        u =glm::cross(glm::vec3(0,1,0),w);
    }

    v = glm::cross(u,w);
    
    Eigen::MatrixXf M(6,6);
    Eigen::VectorXf b(6),x(6),v1(6),v2(6);
    for(int i = 0;i<6;i++){
        b(i) = 0;
        v1(i) = 0;
        v2(i) = 0;
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

		v1(0) = x*x;
		v1(1) = 2*x*y;
		v1(2) = y*y;
		v1(3) = x;
		v1(4) = y;
		v1(5) = 1;

		v2(0) = ph * x*x;
		v2(1) = ph * 2*x*y;
		v2(2) = ph * y*y;
		v2(3) = ph * x;
		v2(4) = ph * y;
		v2(5) = ph;

		M += v1 * v2.transpose();
		b  += v2*z;
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
    return glob_phi(worldPos,P,supportSize);
    float a = phi(worldPos);
    float b = 0;
    auto nodes = pci->_centers.findCloseTo(worldPos,pci->maxSupportSize);
    IT_FOR(nodes,c){
        b += (*c)->get().phi(worldPos);
    }
    return a/b;//*/
}

PointCloudInterpolation::PointCloudInterpolation(std::vector<glm::vec3> pointCloud, PointCloudInterpolationHints hints,std::vector<glm::vec3> normals):
	maxSupportSize(0),minSupportSize(100000),avgSupportSize(0)
{
	if(normals.size()==0) //shuffle the array of points to make kd-tree better balanced, but only if there are no normals specified.
		std::random_shuffle(pointCloud.begin(),pointCloud.end());
	std::vector<K3DTree<Point>::Node*> pointsLeft;
	int _i = 0;
	_avgCenter = glm::vec3(0,0,0);
	IT_FOR(pointCloud,p){
		Point point(*p);
		_avgCenter += *p;
		if(normals.size()!=0){
			point.N = normals[_i++];
		}
		pointsLeft.push_back(_points.insert(*p,point));
	}

	_avgCenter /= pointsLeft.size();

    LOG_DEBUG("Point Tree created");

	{
	float d = _points.depth();
	float s = pointsLeft.size();
	float f0 = s / (std::powf(2,d)-1);
	float f1 = (std::log(s)/std::log(2)) / d;

    LOG_DEBUG("Num points: "       << s);
	LOG_DEBUG("Depth of three: "   << d);
	LOG_DEBUG("Fill factor 0: "    << f0);
	LOG_DEBUG("Fill factor 1: "    << f1);
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
	LOG_DEBUG("Setting L: " << L);
	LOG_DEBUG("Setting H: " << C);
	LOG_DEBUG("Using hint.Tsa: " << hints.Tsa);

	_avgCenter = _aabb.getPosition(glm::vec3(0.5,0.5,0.5));
	
    LOG_DEBUG("Calculating densities");
	float densSum = 0;
	IT_FOR(_points,node){
		std::vector<glm::vec3> closePoints;
		auto nodes = _points.findNNearest(node->getPosition(),hints.K);
		node->get().density = 0;
		IT_FOR(nodes,p){
			closePoints.push_back((*p)->get().P); 
			auto o = (*p)->get().P - node->get().P;
			//node->get().density += phi(glm::length(o));
			node->get().density += glm::dot(o,o); //TEST WHAT HAPPENS IF WE USE phi(glm::length(o,o)) HERE
		}
		if(normals.size()==0){
			node->get().N = (Plane(closePoints)).getNormal();
			//glm::vec3 test = node->get().P - _avgCenter;
			//if(glm::dot(test,node->get().N)<0)
				//node->get().N = -node->get().N;
		}

		densSum += node->get().density;
    }
    float avgDens = densSum / _points.size(); 
	IT_FOR(_points,p){
		(p)->get().density /= avgDens;
	}
    LOG_DEBUG("Calculating densities completed, average density " << avgDens);

    LOG_DEBUG("Starting to create centers");
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
    int size = _centers.size();
	

	float d = _centers.depth();
	float s = size;
	float f0 = s / (std::powf(2,d)-1);
	float f1 = (std::log(s)/std::log(2)) / d;
    LOG_DEBUG("Centers created");

    LOG_DEBUG("Num centers: "       << size);
	LOG_DEBUG("Depth of three: " << d);
	LOG_DEBUG("Fill factor 0: " << f0);
	LOG_DEBUG("Fill factor 1: " << f1);
    LOG_DEBUG("Max support Size: " << maxSupportSize);
    LOG_DEBUG("Min support Size: " << minSupportSize);
    LOG_DEBUG("Avg support Size: " << avgSupportSize /  _centers.size());
	
	float minOverlap = 100000000;
	float maxOverlap = 0;
	IT_FOR(_points,p){
		minOverlap = std::min(minOverlap,(*p).get().overlap);
		maxOverlap = std::max(maxOverlap,(*p).get().overlap);
	}
	LOG_DEBUG("Min overlap " << minOverlap);
    LOG_DEBUG("Max overlap " << maxOverlap);



   // return;
	
	LOG_DEBUG("Global error before lambda fitting: " << eGlobal());

    LOG_DEBUG("Starting to fit lambdas");
	
    Eigen::SparseMatrix<float> A(size,size);
    LOG_DEBUG("Reserving memory for A, estimating " << size*100 << " entries");
	A.reserve(Eigen::VectorXi::Constant(size,100));
	Eigen::VectorXf b(size);
	Eigen::VectorXf l(size);
	int i,j;
	i = j = 0;

	float avgNonZero = 0;
	int numNonZero = 0;
	int maxNonZero = 0;
	int minNonZero = 10000;

	float denom = densSum * (L*L);
	IT_FOR(_centers,c0){
		i = -1;
		float B = 0;
		auto nodes = _points.findCloseTo(c0->get().P,c0->get().supportSize);
		auto centers2 = _centers.findCloseTo(c0->get().P,c0->get().supportSize);
		IT_FOR(_centers,c1){
			i++;
			float dist =  glm::distance(c0->get().P,(c1)->get().P);
			float supSize = c0->get().supportSize + (c1)->get().supportSize;
			if(dist>supSize){ //The centers does not share any points
				continue;
			}
			float v = 0;
			
			IT_FOR(nodes,point){
                float a = (*point)->get().density;
				if(c0->get().phi((*point)->get().P) == 0 || (c1)->get().phi((*point)->get().P) == 0)
					continue;
                a *= c0->get().PHI((*point)->get().P,this);
                a *= (c1)->get().PHI((*point)->get().P,this);
				v += a;
				if(i == 0){
					float c = (*point)->get().density;
					c *= c0->get().PHI((*point)->get().P,this);
					c *= -eval((*point)->get().P);
					B += c;
				}
			}
			v /= denom;
			if(i==0){
				float d = 1.0 / c0->get().supportSize;
				d *= d;
				d /= size;
				d *= hints.Treg;
				v += d;
				//TODO make sure you get here
			}
			if(v!=0){
				A.insert(i,j) = v;
				numNonZero++;
			}

		}
		b(j) = B / denom;
		j++;

		avgNonZero += numNonZero;
		minNonZero = std::min(numNonZero,minNonZero);
		maxNonZero = std::max(numNonZero,maxNonZero);
		numNonZero = 0;

	}
	avgNonZero /= j;
    LOG_DEBUG("Finished building A");
	LOG_DEBUG("Average number of nonzero/row: " << avgNonZero);
	LOG_DEBUG("Minimum number of nonzero/row: " << minNonZero);
	LOG_DEBUG("Maximum number of nonzero/row: " << maxNonZero);

	Eigen::ConjugateGradient<Eigen::SparseMatrix<float> > cg;
	cg.compute(A);
    switch(cg.info()){
    case Eigen::Success:
        LOG_DEBUG("Compute solver Success");
        break;
    case Eigen::NumericalIssue:
        LOG_ERROR("Compute solver failed: " << "NumericalIssue, The provided data did not satisfy the prerequisites.");
        break;
    case Eigen::NoConvergence:
        LOG_ERROR("Compute solver failed: " << "NoConvergence, Iterative procedure did not converge.");
        break;
    case Eigen::InvalidInput:
        LOG_ERROR("Compute solver failed: " << "InvalidInput, The inputs are invalid, or the algorithm has been improperly called. When assertions are enabled, such errors trigger an assert.");
        break;
    }

	LOG_DEBUG("Starting to solve linear system");
    l = cg.solve(b);
	LOG_INFO("Solver completed after " << cg.iterations() << " with an estimated error of " << cg.error());
	
	i = 0;
	IT_FOR(_centers,c0){
		c0->get().lambda = l(i++);
	}
}

float PointCloudInterpolation::guess(const glm::vec3 &worldPos){
	auto n = _points.findNearest(worldPos);
	return glm::distance(n->get().P,worldPos);
}
float PointCloudInterpolation::eval(const glm::vec3 &worldPos){
	float v = 0;
	float _ph = 0;
	auto nodes = _centers.findCloseTo(worldPos,maxSupportSize);
	IT_FOR(nodes,cc){
		Center *c = &(*cc)->get();
		//float ph = c->PHI(worldPos,this);
		float ph = c->phi(worldPos);
		if(ph!=ph)
			LOG_ERROR("ph!=ph - ph = " << ph << "    " << maxSupportSize);
		_ph += ph;
		float g  = c->g(worldPos);
		v += (g+c->lambda)*ph;
	}
	return v;
}

float PointCloudInterpolation::eGlobal(){
    float num = 0,denum = 0,f;
    IT_FOR(_points,p){
        f = eval((*p).get().P);
		if(f!=f){
			//LOG_ERROR("f!=f - f = " << f);
		}
        num   += (*p).get().density * (f*f);
        denum += (*p).get().density;
    }
    return std::sqrt(num/denum)/L;
}