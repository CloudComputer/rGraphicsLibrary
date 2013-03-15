#include "MarchingTetrahedra.h"

#include <iostream>

glm::vec3 __interpolate(glm::vec3 p0,float v0,glm::vec3 p1,float v1){
	float t = 0;
	if(v0 != v1)
		t = v0 / (v0 - v1);
	return t * p1 + (1-t) * p0;
}


void MarchingTetrahedra::_evaluateTetra(Mesh *m,glm::vec3 p0,float v0,glm::vec3 p1,float v1,glm::vec3 p2,float v2,glm::vec3 p3,float v3){
	int index = 0;
	if(v0 > 0) index+=1;
	if(v1 > 0) index+=2;
	if(v2 > 0) index+=4;
	if(v3 > 0) index+=8;
	std::vector<glm::vec3> pos;
	glm::vec3 a,b,c,d;
	if(index == 0 || index == 15)
		return;
	else if(index == 1 || index == 14){
		a = __interpolate(p0,v0,p2,v2);
		b = __interpolate(p0,v0,p1,v1);
		c = __interpolate(p0,v0,p3,v3);
		if(index==1){
			pos.push_back(a);
			pos.push_back(b);
			pos.push_back(c);
		}else{
			pos.push_back(a);
			pos.push_back(c);
			pos.push_back(b);
		}
		Face* f = m->addFace(pos);
	}
	else if(index == 2 || index == 13){
		a = __interpolate(p1,v1,p0,v0);
		b = __interpolate(p1,v1,p2,v2);
		c = __interpolate(p1,v1,p3,v3);
		if(index==2){
			pos.push_back(a);
			pos.push_back(b);
			pos.push_back(c);
		}else{
			pos.push_back(a);
			pos.push_back(c);
			pos.push_back(b);
		}
		Face* f = m->addFace(pos);
	
	}
	else if(index == 4 || index == 11){
		a = __interpolate(p2,v2,p0,v0);
		b = __interpolate(p2,v2,p1,v1);
		c = __interpolate(p2,v2,p3,v3);
		if(index==4){
			pos.push_back(a);
			pos.push_back(c);
			pos.push_back(b);
		}else{
			pos.push_back(a);
			pos.push_back(b);
			pos.push_back(c);
		}
		Face* f = m->addFace(pos);
	}
	else if(index == 7 || index == 8){
		a = __interpolate(p3,v3,p0,v0);
		b = __interpolate(p3,v3,p2,v2);
		c = __interpolate(p3,v3,p1,v1);
		if(index==7){
			pos.push_back(a);
			pos.push_back(b);
			pos.push_back(c);
		}else{
			pos.push_back(a);
			pos.push_back(c);
			pos.push_back(b);
		}
		Face* f = m->addFace(pos);
	}
	else if(index == 3 || index == 12){
		a = __interpolate(p0,v0,p2,v2);
		b = __interpolate(p1,v1,p3,v3);
		c = __interpolate(p0,v0,p3,v3);
		d = __interpolate(p1,v1,p2,v2);

		if(index == 3){
			pos.push_back(a);
			pos.push_back(b);
			pos.push_back(c);
		}else{
			pos.push_back(a);
			pos.push_back(c);
			pos.push_back(b);
		}
		Face* f = m->addFace(pos);
		if(index == 3){
			pos[1] = d;
			pos[2] = b;
		}else{
			pos[1] = b;
			pos[2] = d;
		}
		f = m->addFace(pos);
		
	}
	else if(index == 5 || index == 10){
		a = __interpolate(p2,v2,p3,v3);
		b = __interpolate(p0,v0,p1,v1);
		c = __interpolate(p0,v0,p3,v3);
		d = __interpolate(p1,v1,p2,v2);

		if(index == 5){
			pos.push_back(a);
			pos.push_back(b);
			pos.push_back(c);
		}else{
			pos.push_back(a);
			pos.push_back(c);
			pos.push_back(b);
		}
		
		Face* f = m->addFace(pos);
		if(index == 5){
			pos[1] = d;
			pos[2] = b;
		}else{
			pos[1] = b;
			pos[2] = d;
		}
		f = m->addFace(pos);
	
	}
	else if(index == 6 || index == 9){
		a = __interpolate(p1,v1,p3,v3);
		b = __interpolate(p0,v0,p2,v2);
		c = __interpolate(p0,v0,p1,v1);
		d = __interpolate(p2,v2,p3,v3);

		if(index == 6){
			pos.push_back(a);
			pos.push_back(c);
			pos.push_back(b);
		}else{
			pos.push_back(a);
			pos.push_back(b);
			pos.push_back(c);
		}
		
		Face* f = m->addFace(pos);
		if(index == 6){
			pos[1] = b;
			pos[2] = d;
		}else{
			pos[1] = d;
			pos[2] = b;
		}
		f = m->addFace(pos);
	}
}
