#include "ScalarField.h"

#include "VectorField.h"
#include <Math\Interpolator.h>

#include <Util\TmpPointer.h>

#include <fstream>
#include <iostream>


ScalarField::ScalarField(glm::ivec3 dimensions,BoundingAABB boundingAABB):Field3D<float>(dimensions,boundingAABB){

}

glm::vec3 ScalarField::getGetGradientCurl(const glm::ivec3 &pos)const{
	glm::vec3 Fx,Fy,Fz;
	
	Fx = (getGradient(pos+glm::ivec3(1,0,0))-getGradient(pos-glm::ivec3(1,0,0)))/(1.0f);
	Fy = (getGradient(pos+glm::ivec3(0,1,0))-getGradient(pos-glm::ivec3(0,1,0)))/(1.0f);
	Fz = (getGradient(pos+glm::ivec3(0,0,1))-getGradient(pos-glm::ivec3(0,0,1)))/(1.0f);

	glm::vec3 c;

	c.x = Fy.z/_delta.y - Fz.y/_delta.x; 
	c.y = Fz.x/_delta.z - Fx.z/_delta.z; 
	c.z = Fx.y/_delta.x - Fy.x/_delta.y; 

	return c;
}

glm::vec3 ScalarField::getGradient(const glm::vec3 &worldPos)const{
	glm::vec3 g;
	g.x = DiffXpm(worldPos);
	g.y = DiffYpm(worldPos);
	g.z = DiffZpm(worldPos);
	return g;
}

glm::vec3 ScalarField::getGradient(const glm::ivec3 &pos)const{
	glm::vec3 g;
	g.x = DiffXpm(pos);
	g.y = DiffYpm(pos);
	g.z = DiffZpm(pos);
	return g;
}

glm::vec3 ScalarField::getUpWindGradient(const glm::vec3 &worldPos,const glm::vec3 &V)const{
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

std::vector<glm::ivec3> ScalarField::_cannyCases;
std::vector<glm::vec3> ScalarField::_cannyNormCases;

void ScalarField::createCannyCases(){
	if(_cannyCases.size()) //if we have nonzero entries in the vector, this has already been runned.
		return;
	
	_cannyCases.push_back(glm::ivec3(0,0,1));//0
	_cannyCases.push_back(glm::ivec3(0,1,0));//1
	_cannyCases.push_back(glm::ivec3(1,0,0));//2
	_cannyCases.push_back(glm::ivec3(1,1,0));//3
	_cannyCases.push_back(glm::ivec3(1,-1,0));//4
	_cannyCases.push_back(glm::ivec3(1,-1,-1));//5
	_cannyCases.push_back(glm::ivec3(1,1,-1));//6
	_cannyCases.push_back(glm::ivec3(1,-1,1));//7
	_cannyCases.push_back(glm::ivec3(1,1,1));//8
	_cannyCases.push_back(glm::ivec3(0,1,1));//9
	_cannyCases.push_back(glm::ivec3(0,1,-1));//10
	_cannyCases.push_back(glm::ivec3(1,0,-1));//11
	_cannyCases.push_back(glm::ivec3(1,0,1));//12
	
	for(int i= 0;i<_cannyCases.size();i++){
		_cannyNormCases.push_back( glm::normalize(glm::vec3(_cannyCases[i])));
	}

}


//TODO make this const
ScalarField* ScalarField::Canny(bool blurFirst){
	createCannyCases();
	ScalarField* vOut = new ScalarField(_dimensions,_boundingAABB);
	ScalarField* vIn = this;
	if(blurFirst)
		vIn = blur();
	TmpPointer<VectorField> vVec = vIn->getGradientField();

	FOR(_dimensions)if(!(x==0||y==0||z==0||x==_dimensions.x-1||y==_dimensions.y-1||z==_dimensions.z-1)){
		glm::ivec3 pos(x,y,z),s0,s1;
		float v,v0,v1;
		auto grad = vVec->get(pos);
		//if(glm::length(grad) <= 1e-7) //test with this if something is not right
		//	continue;
		auto norm = glm::normalize(grad);

		int case_ = -1;
		float bestCase = 0;
		for(int i= 0;i<_cannyCases.size();i++){
			float d = std::abs(glm::dot(_cannyNormCases[i],norm));
			if(d>bestCase){
				bestCase = d;
				case_ = i;
			}
		}
		s0 = pos + _cannyCases[case_];
		s1 = pos - _cannyCases[case_];
		
		v  = get(pos);
		v0 = get(s0);
		v1 = get(s1);
		
		if(v>v0 && v>v1 && v >= 0.15){
			vOut->set(pos,1);
		}else{
			vOut->set(pos,0.0);
		}
	}




	if(blurFirst)
		delete vIn;
	return vOut;

}

ScalarField *ScalarField::blur()const{
	ScalarField *s = new ScalarField(_dimensions,_boundingAABB);
	FOR(_dimensions){
		float v = 0;
		for(int i=x-1;i<=x+1;i++)
			for(int j=y-1;j<=y+1;j++)
				for(int k=z-1;k<=z+1;k++){
					v += _data[_index(glm::ivec3(i,j,k))];
		}
		s->set(glm::ivec3(x,y,z),v/27.0f);
	}
	return s;
}

typedef KDTree<Vertex,3,float> Tree;
typedef Tree::Node			 Node;
KDTree<Vertex,3,float>* ScalarField::getSurfacePoints(float iso)const{
	Tree* tree = new KDTree<Vertex,3,float>();
	float s,sx,sy,sz;

	glm::ivec3 dx(1,0,0),dy(0,1,0),dz(0,0,1);

	for(int x = 0;x<_dimensions.x;x++)for(int z = 0;z<_dimensions.z;z++){
		int surface = false;
		for(int y = 4;y<_dimensions.y-5 && !surface;y++){
	//FOR(_dimensions){//if(!(z == 0||z==_dimensions.z - 1 || y == 0||y==_dimensions.y - 1 || x == 0||x==_dimensions.x - 1)){
			if(y < 4 || y >= _dimensions.y-5){
				continue;
			}
			glm::ivec3 pos(x,y,z);
			s = get(pos);
			if(s<iso) continue;
			glm::vec3 p = _getWorldPos(pos);
			surface = true;
			for(int Y = y-4;surface && Y<y+4;Y++)if(Y!=y){
				surface = surface && s >= get(glm::ivec3(x,Y,z));
			}
			if(surface){
				Vertex v;
				v.getPosition() = glm::vec4(p,0);
				v.setNormal(glm::normalize(getGradient(p)));
				tree->insert(glm::value_ptr(p),v);
			}
		}
		if(!surface){
		/*	glm::ivec3 ip(x,maxY,z);
			glm::vec3 p = _getWorldPos(pos);
			Vertex v;
			v.getPosition() = glm::vec4(p,0);
			v.setNormal(glm::vec3(0,-1,0));
			tree->insert(glm::value_ptr(p),v);*/
		}
	}
	return tree;
}

float ScalarField::DiffXp(const glm::vec3 &worldPos)const{
	glm::ivec3 ip = (glm::ivec3)_boundingAABB.getDiscretePosition(worldPos,_dimensions);
	return DiffXp(ip);
}

float ScalarField::DiffXm(const glm::vec3 &worldPos)const{
	glm::ivec3 ip = (glm::ivec3)_boundingAABB.getDiscretePosition(worldPos,_dimensions);
	return DiffXm(ip);
}
float ScalarField::DiffXpm(const glm::vec3 &worldPos)const{
	glm::ivec3 ip = (glm::ivec3)_boundingAABB.getDiscretePosition(worldPos,_dimensions);
	return DiffXpm(ip);
}




float ScalarField::DiffYp(const glm::vec3 &worldPos)const{
	glm::ivec3 ip = (glm::ivec3)_boundingAABB.getDiscretePosition(worldPos,_dimensions);
	return DiffYp(ip);
}
float ScalarField::DiffYm(const glm::vec3 &worldPos)const{
	glm::ivec3 ip = (glm::ivec3)_boundingAABB.getDiscretePosition(worldPos,_dimensions);
	return DiffYm(ip);
}
float ScalarField::DiffYpm(const glm::vec3 &worldPos)const{
	glm::ivec3 ip = (glm::ivec3)_boundingAABB.getDiscretePosition(worldPos,_dimensions);
	return DiffYpm(ip);
}





float ScalarField::DiffZp(const glm::vec3 &worldPos)const{
	glm::ivec3 ip = (glm::ivec3)_boundingAABB.getDiscretePosition(worldPos,_dimensions);
	return DiffZp(ip);
}
float ScalarField::DiffZm(const glm::vec3 &worldPos)const{
	glm::ivec3 ip = (glm::ivec3)_boundingAABB.getDiscretePosition(worldPos,_dimensions);
	return DiffZm(ip);
}
float ScalarField::DiffZpm(const glm::vec3 &worldPos)const{
	glm::ivec3 ip = (glm::ivec3)_boundingAABB.getDiscretePosition(worldPos,_dimensions);
	return DiffZpm(ip);
}




float ScalarField::DiffXXpm(const glm::vec3 &worldPos)const{
	glm::ivec3 ip = (glm::ivec3)_boundingAABB.getDiscretePosition(worldPos,_dimensions);
	return DiffXXpm(ip);
}

float ScalarField::DiffXYpm(const glm::vec3 &worldPos)const{
	glm::ivec3 ip = (glm::ivec3)_boundingAABB.getDiscretePosition(worldPos,_dimensions);
	return DiffXYpm(ip);
}
float ScalarField::DiffXZpm(const glm::vec3 &worldPos)const{
	glm::ivec3 ip = (glm::ivec3)_boundingAABB.getDiscretePosition(worldPos,_dimensions);
	return DiffXZpm(ip);
}
float ScalarField::DiffYYpm(const glm::vec3 &worldPos)const{
	glm::ivec3 ip = (glm::ivec3)_boundingAABB.getDiscretePosition(worldPos,_dimensions);
	return DiffYYpm(ip);
}
float ScalarField::DiffYZpm(const glm::vec3 &worldPos)const{
	glm::ivec3 ip = (glm::ivec3)_boundingAABB.getDiscretePosition(worldPos,_dimensions);
	return DiffYZpm(ip);
}
float ScalarField::DiffZZpm(const glm::vec3 &worldPos)const{
	glm::ivec3 ip = (glm::ivec3)_boundingAABB.getDiscretePosition(worldPos,_dimensions);
	return DiffZZpm(ip);
}

float ScalarField::DiffXp(glm::ivec3 ip)const{
	return (_data[_index(ip+glm::ivec3(1,0,0))] - _data[_index(ip)])/_delta.x;
}
float ScalarField::DiffXm(glm::ivec3 ip)const{
	return (_data[_index(ip)] - _data[_index(ip-glm::ivec3(1,0,0))])/_delta.x;
}
float ScalarField::DiffXpm(glm::ivec3 ip)const{
	return (_data[_index(ip+glm::ivec3(1,0,0))] - _data[_index(ip-glm::ivec3(1,0,0))])/(2*_delta.x);
}


float ScalarField::DiffYp(glm::ivec3 ip)const{
	return (_data[_index(ip+glm::ivec3(0,1,0))] - _data[_index(ip)])/_delta.y;
}
float ScalarField::DiffYm(glm::ivec3 ip)const{
	return (_data[_index(ip)] - _data[_index(ip-glm::ivec3(0,1,0))])/_delta.y;
}
float ScalarField::DiffYpm(glm::ivec3 ip)const{
	return (_data[_index(ip+glm::ivec3(0,1,0))] - _data[_index(ip-glm::ivec3(0,1,0))])/(2*_delta.y);
}


float ScalarField::DiffZp(glm::ivec3 ip)const{
	return (_data[_index(ip+glm::ivec3(0,0,1))] - _data[_index(ip)])/_delta.z;
}
float ScalarField::DiffZm(glm::ivec3 ip)const{
	return (_data[_index(ip)] - _data[_index(ip-glm::ivec3(0,0,1))])/_delta.z;
}
float ScalarField::DiffZpm(glm::ivec3 ip)const{
	return (_data[_index(ip+glm::ivec3(0,0,1))] - _data[_index(ip-glm::ivec3(0,0,1))])/(2*_delta.z);
}

float ScalarField::DiffXXpm(glm::ivec3 ip)const{
	assert(false && "Not yet implemented");
	return 0;
}
float ScalarField::DiffXYpm(glm::ivec3 ip)const{
	assert(false && "Not yet implemented");
	return 0;
}
float ScalarField::DiffXZpm(glm::ivec3 ip)const{
	assert(false && "Not yet implemented");
	return 0;
}
float ScalarField::DiffYYpm(glm::ivec3 ip)const{
	assert(false && "Not yet implemented");
	return 0;
}
float ScalarField::DiffYZpm(glm::ivec3 ip)const{
	assert(false && "Not yet implemented");
	return 0;
}
float ScalarField::DiffZZpm(glm::ivec3 ip)const{
	assert(false && "Not yet implemented");
	return 0;
}

void ScalarField::saveAsRaw(const char *filename){
	FILE *file = fopen(filename,"wb");

	FOR(_dimensions){
		int id = _index(glm::ivec3(x,y,z));
		unsigned char c = _data[id]*255;
		fwrite(&c,1,1,file);
	}
}

ScalarField* ScalarField::ReadFromRawfile(const char *filename,unsigned int w,unsigned int h,unsigned int d,unsigned int bps){
	FILE *file = fopen(filename,"r");
	if(!file){
		std::cerr << "Could not open file: " << filename << std::endl;
		return 0;
	}

	float x,y,z,m;
	m = std::max(std::max(w,h),d);
	x = w / m;
	y = h / m;
	z = d / m;

	z *= 3.0;

	if(bps == 1){
		ScalarField *s = new ScalarField(glm::ivec3(w,h,d),BoundingAABB(glm::vec3(-x,-y,-z),glm::vec3(x,y,z)));
		unsigned char *data = new unsigned char[w*h*d];
		fread(data,1,w*h*d,file);
		for(int i = 0;i<w*h*d;i++) s->_data[i] = data[i]/float(0xFF);
		delete data;
		return s;
	}else if(bps == 2){
		ScalarField *s = new ScalarField(glm::ivec3(w,h,d),BoundingAABB(glm::vec3(-x,-y,-z),glm::vec3(x,y,z)));
		unsigned short *data = new unsigned short[w*h*d];
		fread(data,2,w*h*d,file);
		for(int i = 0;i<w*h*d;i++) s->_data[i] = data[i]/float(0xFFFF);
		delete data;
		return s;
	}else if(bps == 4){
		ScalarField *s = new ScalarField(glm::ivec3(w,h,d),BoundingAABB(glm::vec3(-x,-y,-z),glm::vec3(x,y,z)));
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


