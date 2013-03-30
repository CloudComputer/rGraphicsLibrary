#include "Plane.h"

#include <Eigen\Dense>


Plane::Plane(){


}


Plane::Plane( const Plane &p ):a(p.a),b(p.b),c(p.c),d(p.d){
	
}

Plane::~Plane(){


}

glm::vec3 Plane::getNormal()const{
	return glm::normalize(glm::vec3(a,b,c));
}

#include <iostream>

Plane::Plane(const std::vector<glm::vec3> &points){
	Eigen::MatrixXf A = Eigen::MatrixXf::Zero(4,4);
	Eigen::VectorXf b = Eigen::VectorXf::Zero(4);

	int i;
	glm::vec3 center;
	for(i = 0;i<points.size();i++){
		center+=points[i];
	}
	center /= i;
	for(i = 0;i<points.size();i++){
		A(0,0) += std::pow(points[i].x,2);
		A(1,1) += std::pow(points[i].y,2);
		A(2,2) += std::pow(points[i].z,2);
		
		A(0,1) = A(1,0) += points[i].x*points[i].y;
		A(0,2) = A(2,0) += points[i].x*points[i].z;
		A(3,0) += points[i].x;
		A(3,1) += points[i].y;
		A(3,2) += points[i].z;
		
		b(0) += points[i].x;
		b(1) += points[i].y;
		b(2) += points[i].z;
		

		A(2,1) = A(1,2) += points[i].y*points[i].z;
		
	}
	A(3,3) = 1;
	b(3) = points.size();

	Eigen::VectorXf x = A.partialPivLu().solve(b);

	this->a = x(0);
	this->b = x(1);
	this->c = x(2);
	this->d = x(3);
}



glm::vec3 Plane::getPoint(float x,float y)const{
	glm::vec3 p;
	p.x = x;
	p.y = y;
	p.z = -(x*a + y*b + d) / c;
	return p;
}
float Plane::getDistanceToPlane(const glm::vec3 &p)const{
	return a*p.x + b*p.y + c*p.z + d;
}