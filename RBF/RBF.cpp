#include "RBF.h"

float RBFSystem::meanSqError(const std::vector<glm::vec4> &points){
	float d = 0,dx;
	for(auto p = points.begin();p!=points.end();p++){
		dx =  p->w - eval(glm::vec3(*p));
		d += dx*dx;
	}
	return d/points.size();
}


float RBFSystem::eval(glm::vec3 worldPos){
	return eval(worldPos,true);
}


float RBFSystem::eval(glm::vec3 worldPos,bool useTrendFunc){
	glm::vec3 c = glm::vec3(worldPos - _min.x) / (_max.x - _min.x);

	float v = 0;
	if(useTrendFunc)
		v = _trend.eval(c.x,c.y,c.z);
	for(auto k = _kernels.begin();k!=_kernels.end();++k){
		v += (*k)->eval(c.x,c.y,c.z);
	}
	return v;
}




void __rbf_Subdived(std::vector<glm::vec4> &points,unsigned int minSize,__rbf_SubSpace &s,__rbf_SubSpace &left,__rbf_SubSpace &right){
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
			auto p  = glm::vec3(points[*i]);
			std::cout << p.x << " " << p.y << " " << p.z << std::endl;
			std::cout << s.innerAABB << std::endl;
			std::cout << left.innerAABB << std::endl;
			std::cout << right.innerAABB << std::endl;
			std::cout << splitAxis << std::endl;

			std::cerr << "FAILED " << __FILE__ << " " << __LINE__ << std::endl;
		}
	}
	bool fitAABB = false;
	if(left.Inner.size()<minSize){
		fitAABB = true;
		switch(splitAxis){
		case 0:
			std::sort(right.Inner.begin(),right.Inner.end(),__rbf_SortXAxis(&points));
			break;
		case 1:
			std::sort(right.Inner.begin(),right.Inner.end(),__rbf_SortYAxis(&points));
			break;
		case 2:
			std::sort(right.Inner.begin(),right.Inner.end(),__rbf_SortZAxis(&points));
			break;
		}
		
		while(left.Inner.size()<minSize){
			auto node = right.Inner[0];
			left.innerAABB.extend(glm::vec3(points[node]));
			left.Inner.push_back(node);
			right.Inner.erase(right.Inner.begin());
		}
	}else if(right.Inner.size()<minSize){	
		fitAABB = true;
		switch(splitAxis){
		case 0:
			std::sort(left.Inner.begin(),left.Inner.end(),__rbf_SortXAxis(&points));
			break;
		case 1:
			std::sort(left.Inner.begin(),left.Inner.end(),__rbf_SortYAxis(&points));
			break;
		case 2:
			std::sort(left.Inner.begin(),left.Inner.end(),__rbf_SortZAxis(&points));
			break;
		}
		
		while(right.Inner.size()<minSize){
			auto node = left.Inner[0];
			right.innerAABB.extend(glm::vec3(points[node]));
			right.Inner.push_back(node);
			left.Inner.erase(left.Inner.begin());
		}
	}

	if(fitAABB){
		left.innerAABB.minPos() = left.innerAABB.maxPos() = glm::vec3(points[left.Inner[0]]);
		right.innerAABB.minPos() = right.innerAABB.maxPos() = glm::vec3(points[right.Inner[0]]);

		for(int i = 1;i<right.sizeInner();i++){
			right.innerAABB.extend(glm::vec3(points[right.Inner[i]]));
		}
		for(int i = 1;i<left.sizeInner();i++){
			left.innerAABB.extend(glm::vec3(points[left.Inner[i]]));
		}
	}

}