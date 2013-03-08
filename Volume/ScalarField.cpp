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

typedef KDTree<glm::vec3,3,float> Tree;
typedef Tree::Node			 Node;
KDTree<glm::vec3,3,float>* ScalarField::getSurfacePoints(float iso)const{
	Tree* tree = new KDTree<glm::vec3,3,float>();
	float s,sx,sy,sz;

	glm::ivec3 dx(1,0,0),dy(0,1,0),dz(0,0,1);

	FOR(_dimensions){//if(!(z == 0||z==_dimensions.z - 1 || y == 0||y==_dimensions.y - 1 || x == 0||x==_dimensions.x - 1)){
		if(z < 4 || z >= _dimensions.z-5){
			continue;
		}
		glm::ivec3 pos(x,y,z);
		s = get(pos);
		if(s<0.4) continue;
		glm::vec3 p = _getWorldPos(pos);
		bool surface = true;
		for(int Z = z-4;surface && Z<z+4;Z++)if(Z!=z){
			surface = surface && s >= get(glm::ivec3(x,y,Z));
		}
		if(surface){
			tree->insert(glm::value_ptr(p),glm::normalize(glm::vec3(DiffXpm(p),DiffXpm(p),DiffXpm(p))));
		}
	}
	return tree;
}

//
//typedef KDTree<glm::vec3,3,float> Tree;
//typedef Tree::Node			 Node;
//KDTree<glm::vec3,3,float>* ScalarField::getSurfacePoints(float iso)const{
//	Tree* tree = new KDTree<glm::vec3,3,float>();
//	float s,sx,sy,sz;
//
//	glm::ivec3 dx(1,0,0),dy(0,1,0),dz(0,0,1);
//
//	FOR(_dimensions){//if(!(z == 0||z==_dimensions.z - 1 || y == 0||y==_dimensions.y - 1 || x == 0||x==_dimensions.x - 1)){
//		if(x == _dimensions.x-1|| y == _dimensions.y-1|| z == _dimensions.z-1){
//			continue;
//		}
//		glm::ivec3 pos(x,y,z);
//		s = get(pos);
//		sx = get(pos+dx);
//		sy = get(pos+dy);
//		sz = get(pos+dz);
//		glm::vec3 p =  _getWorldPos(pos);
//		glm::vec3 px = _getWorldPos(pos+dx);
//		glm::vec3 py = _getWorldPos(pos+dy);
//		glm::vec3 pz = _getWorldPos(pos+dz);
//		if((s>= iso && sx < iso) || (s < iso && sx >= iso)){
//			float t = (iso - s) / (s - sx);
//			glm::vec3 newPoint = px * t + (1-t) * p;
//			tree->insert(glm::value_ptr(newPoint),glm::normalize(glm::vec3(DiffXpm(newPoint),DiffXpm(newPoint),DiffXpm(newPoint))));
//		}
//
//		if((s>= iso && sy < iso) || (s < iso && sy >= iso)){
//			float t = (iso - s) / (s - sy);
//			glm::vec3 newPoint = py * t + (1-t) * p;
//			tree->insert(glm::value_ptr(newPoint),glm::normalize(glm::vec3(DiffXpm(newPoint),DiffXpm(newPoint),DiffXpm(newPoint))));
//		}
//
//		if((s>= iso && sz < iso) || (s < iso && sz >= iso)){
//			float t = (iso - s) / (s - sz);
//			glm::vec3 newPoint = pz * t + (1-t) * p;
//			tree->insert(glm::value_ptr(newPoint),glm::normalize(glm::vec3(DiffXpm(newPoint),DiffXpm(newPoint),DiffXpm(newPoint))));
//		}
//	}
//	return tree;
//}

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

float ScalarField::DiffXp(glm::ivec3 pos)const{return DiffXp(_getWorldPos(pos));}
float ScalarField::DiffXm(glm::ivec3 pos)const{return DiffXm(_getWorldPos(pos));}
float ScalarField::DiffXpm(glm::ivec3 pos)const{return DiffXpm(_getWorldPos(pos));}
float ScalarField::DiffYp(glm::ivec3 pos)const{return DiffYp(_getWorldPos(pos));}
float ScalarField::DiffYm(glm::ivec3 pos)const{return DiffYm(_getWorldPos(pos));}
float ScalarField::DiffYpm(glm::ivec3 pos)const{return DiffYpm(_getWorldPos(pos));}
float ScalarField::DiffZp(glm::ivec3 pos)const{return DiffZp(_getWorldPos(pos));}
float ScalarField::DiffZm(glm::ivec3 pos)const{return DiffZm(_getWorldPos(pos));}
float ScalarField::DiffZpm(glm::ivec3 pos)const{return DiffZpm(_getWorldPos(pos));}

float ScalarField::DiffXXpm(glm::ivec3 pos)const{return DiffXXpm(_getWorldPos(pos));}
float ScalarField::DiffXYpm(glm::ivec3 pos)const{return DiffXYpm(_getWorldPos(pos));}
float ScalarField::DiffXZpm(glm::ivec3 pos)const{return DiffXZpm(_getWorldPos(pos));}
float ScalarField::DiffYYpm(glm::ivec3 pos)const{return DiffYYpm(_getWorldPos(pos));}
float ScalarField::DiffYZpm(glm::ivec3 pos)const{return DiffYZpm(_getWorldPos(pos));}
float ScalarField::DiffZZpm(glm::ivec3 pos)const{return DiffZZpm(_getWorldPos(pos));}

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


