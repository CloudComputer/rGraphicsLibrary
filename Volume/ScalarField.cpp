#include "ScalarField.h"

#include "VectorField.h"
#include <Math\Interpolator.h>

#include <fstream>
#include <iostream>


ScalarField::ScalarField(glm::ivec3 dimensions,BoundingAABB boundingAABB):Field3D<float>(dimensions,boundingAABB){

}

glm::vec3 ScalarField::getGradient(glm::vec3 worldPos)const{
	glm::vec3 g;
	g.x = DiffXpm(worldPos);
	g.y = DiffYpm(worldPos);
	g.z = DiffZpm(worldPos);
	return g;
}

glm::vec3 ScalarField::getUpWindGradient(glm::vec3 worldPos,glm::vec3 V)const{
	glm::vec3 g;
	if(V.x > 0)
		g.x = DiffXp(worldPos);
	else
		g.x = DiffXm(worldPos);
	if(V.y > 0)
		g.y = DiffYp(worldPos);
	else
		g.y = DiffYm(worldPos);
	if(V.z > 0)
		g.z = DiffZp(worldPos);
	else
		g.z = DiffZm(worldPos);
	return g;
}


VectorField* ScalarField::getGradientField()const{
	VectorField *vf = new VectorField(_dimensions,_boundingAABB);
	FOR(_dimensions){
		glm::vec3 worldPos = _boundingAABB.getPosition(glm::vec3(x,y,z)/(glm::vec3)_dimensions);
		glm::vec3 g = getGradient(worldPos);
		vf->set(glm::ivec3(x,y,z),g);
	}
	return vf;
}

std::vector<glm::vec3> ScalarField::getSurfacePoints()const{
	std::vector<glm::vec3> points;
	FOR(_dimensions)if(!(z == 0||z==_dimensions.z - 1 || y == 0||y==_dimensions.y - 1 || x == 0||x==_dimensions.x - 1)){
		float s0,s1,s2;
		s0 = get(glm::ivec3(x,y,z-1));
		s1 = get(glm::ivec3(x,y,z));
		s2 = get(glm::ivec3(x,y,z+1));
		
		if(s1>0.6){
			points.push_back(_boundingAABB.getPosition(glm::vec3(x,y,z) / static_cast<glm::vec3>(_dimensions)));
			
		}
		continue;

		if(s1>s0 && s1>s2){
			points.push_back(_boundingAABB.getPosition(glm::vec3(x,y,z) / static_cast<glm::vec3>(_dimensions)));
			continue;
		}
		
		s0 = get(glm::ivec3(x,y-1,z));
		s2 = get(glm::ivec3(x,y+1,z));
		if(s1>s0 && s1>s2){
			points.push_back(_boundingAABB.getPosition(glm::vec3(x,y,z) / static_cast<glm::vec3>(_dimensions)));
			continue;
		}
		
		s0 = get(glm::ivec3(x-1,y,z));
		s2 = get(glm::ivec3(x+1,y,z));
		if(s1>s0 && s1>s2){
			points.push_back(_boundingAABB.getPosition(glm::vec3(x,y,z) / static_cast<glm::vec3>(_dimensions)));
			continue;
		}
	}
	return points;
}

float ScalarField::DiffXp(glm::vec3 worldPos)const{
	glm::ivec3 ip = (glm::ivec3)_boundingAABB.getDiscretePosition(worldPos,_dimensions);
	return (_data[_index(ip+glm::ivec3(1,0,0))] - _data[_index(ip)])/_delta.x;
}

float ScalarField::DiffXm(glm::vec3 worldPos)const{
	glm::ivec3 ip = (glm::ivec3)_boundingAABB.getDiscretePosition(worldPos,_dimensions);
	return (_data[_index(ip)] - _data[_index(ip-glm::ivec3(1,0,0))])/_delta.x;
}
float ScalarField::DiffXpm(glm::vec3 worldPos)const{
	glm::ivec3 ip = (glm::ivec3)_boundingAABB.getDiscretePosition(worldPos,_dimensions);
	return (_data[_index(ip+glm::ivec3(1,0,0))] - _data[_index(ip-glm::ivec3(1,0,0))])/(2*_delta.x);
}

float ScalarField::DiffYp(glm::vec3 worldPos)const{
	glm::ivec3 ip = (glm::ivec3)_boundingAABB.getDiscretePosition(worldPos,_dimensions);
	return (_data[_index(ip+glm::ivec3(0,1,0))] - _data[_index(ip)])/_delta.y;
}
float ScalarField::DiffYm(glm::vec3 worldPos)const{
	glm::ivec3 ip = (glm::ivec3)_boundingAABB.getDiscretePosition(worldPos,_dimensions);
	return (_data[_index(ip)] - _data[_index(ip-glm::ivec3(0,1,0))])/_delta.y;
}
float ScalarField::DiffYpm(glm::vec3 worldPos)const{
	glm::ivec3 ip = (glm::ivec3)_boundingAABB.getDiscretePosition(worldPos,_dimensions);
	return (_data[_index(ip+glm::ivec3(0,1,0))] - _data[_index(ip-glm::ivec3(0,1,0))])/(2*_delta.y);
}

float ScalarField::DiffZp(glm::vec3 worldPos)const{
	glm::ivec3 ip = (glm::ivec3)_boundingAABB.getDiscretePosition(worldPos,_dimensions);
	return (_data[_index(ip+glm::ivec3(0,0,1))] - _data[_index(ip)])/_delta.z;
}
float ScalarField::DiffZm(glm::vec3 worldPos)const{
	glm::ivec3 ip = (glm::ivec3)_boundingAABB.getDiscretePosition(worldPos,_dimensions);
	return (_data[_index(ip)] - _data[_index(ip-glm::ivec3(0,0,1))])/_delta.z;
}
float ScalarField::DiffZpm(glm::vec3 worldPos)const{
	glm::ivec3 ip = (glm::ivec3)_boundingAABB.getDiscretePosition(worldPos,_dimensions);
	return (_data[_index(ip+glm::ivec3(0,0,1))] - _data[_index(ip-glm::ivec3(0,0,1))])/(2*_delta.z);
}

float ScalarField::DiffXXpm(glm::vec3 worldPos)const{
	assert(false && "Not yet implemented");
	return 0;
}

float ScalarField::DiffXYpm(glm::vec3 worldPos)const{
	assert(false && "Not yet implemented");
	return 0;
}
float ScalarField::DiffXZpm(glm::vec3 worldPos)const{
	assert(false && "Not yet implemented");
	return 0;
}
float ScalarField::DiffYYpm(glm::vec3 worldPos)const{
	assert(false && "Not yet implemented");
	return 0;
}
float ScalarField::DiffYZpm(glm::vec3 worldPos)const{
	assert(false && "Not yet implemented");
	return 0;
}
float ScalarField::DiffZZpm(glm::vec3 worldPos)const{
	assert(false && "Not yet implemented");
	return 0;
}

ScalarField* ScalarField::ReadFromRawfile(const char *filename,unsigned int w,unsigned int h,unsigned int d,unsigned int bps){
	FILE *file = fopen(filename,"r");
	if(!file){
		std::cerr << "Could not open file: " << filename << std::endl;
		return 0;
	}
	if(bps == 1){
		ScalarField *s = new ScalarField(glm::ivec3(w,h,d),BoundingAABB(glm::vec3(-1,-1,-1),glm::vec3(1,1,1)));
		unsigned char *data = new unsigned char[w*h*d];
		fread(data,1,w*h*d,file);
		for(int i = 0;i<w*h*d;i++) s->_data[i] = data[i]/float(0xFF);
		delete data;
		return s;
	}else if(bps == 2){
		ScalarField *s = new ScalarField(glm::ivec3(w,h,d));
		unsigned short *data = new unsigned short[w*h*d];
		fread(data,2,w*h*d,file);
		for(int i = 0;i<w*h*d;i++) s->_data[i] = data[i]/float(0xFFFF);
		delete data;
		return s;
	}else if(bps == 4){
		ScalarField *s = new ScalarField(glm::ivec3(w,h,d));
		unsigned int *data = new unsigned int[w*h*d];
		fread(data,4,w*h*d,file);
		for(int i = 0;i<w*h*d;i++) s->_data[i] = data[i]/float(0xFFFFFFFF);
		delete data;
		return s;
	}else{
		std::cerr << "Unsupported bit per sample, must be 1, 2 or 4 " << bps << " was given" << std::endl; 
	}



	return 0;
}


