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




void RBFSystem::save(tinyxml2::XMLNode *parent){
	auto element = parent->GetDocument()->NewElement("RBFSystem");
	auto centers = parent->GetDocument()->NewElement("centers");
	auto trendFunc = parent->GetDocument()->NewElement("TrendFunction");
	auto minPosition = parent->GetDocument()->NewElement("minPosition");
	auto maxPosition = parent->GetDocument()->NewElement("maxPosition");
	parent->InsertEndChild(element);
	element->InsertEndChild(trendFunc);
	element->InsertEndChild(minPosition);
	element->InsertEndChild(maxPosition);
	element->InsertEndChild(centers);
	
	trendFunc->SetAttribute("c0",_trend._c[0]);
	trendFunc->SetAttribute("c1",_trend._c[1]);
	trendFunc->SetAttribute("c2",_trend._c[2]);
	trendFunc->SetAttribute("c3",_trend._c[3]);
	
	minPosition->SetAttribute("x",_min.x);
	minPosition->SetAttribute("y",_min.y);
	minPosition->SetAttribute("z",_min.z);
	maxPosition->SetAttribute("x",_max.x);
	maxPosition->SetAttribute("y",_max.y);
	maxPosition->SetAttribute("z",_max.z);

	for(auto k = _kernels.begin();k!=_kernels.end();++k){
		(*k)->save(centers);
	}

}






float H(RBFSystem *rbf,std::vector<glm::vec4> &points,int x,int y){
	float M = points.size();
	
	glm::vec3 X = glm::vec3(points[x]);
	glm::vec3 Y = glm::vec3(points[y]);
	float v = rbf->_kernels[x]->eval(Y.x,Y.y,Y.z);


	float tmp = 0;
	for(int i = 0;i<M;i++){
		//v -= 
	} 

	return v;
}


RBFFactory *RBFFactory::instance = 0;


RBFFactory* RBFFactory::GetFactory(){
	if(instance == 0)
		instance = new RBFFactory();
	return instance;
}

XMLObject* RBFFactory::create(tinyxml2::XMLElement *xml){
	auto rbf = new RBFSystem();
	
	int i = 10;
	
	for(auto c = xml->FirstChildElement() ; c != 0 ; c = c->NextSiblingElement() ) {
		auto n = c->Value();
		if(std::strcmp(n,"TrendFunction") == 0){
			rbf->_trend._c[0] = c->FloatAttribute("c0");
			rbf->_trend._c[1] = c->FloatAttribute("c1");
			rbf->_trend._c[2] = c->FloatAttribute("c2");
			rbf->_trend._c[3] = c->FloatAttribute("c3");
		}else if(std::strcmp(n,"minPosition") == 0){
			rbf->_min.x = c->FloatAttribute("x");
			rbf->_min.y = c->FloatAttribute("y");
			rbf->_min.z = c->FloatAttribute("z");
		}else if(std::strcmp(n,"maxPosition") == 0){
			rbf->_max.x = c->FloatAttribute("x");
			rbf->_max.y = c->FloatAttribute("y");
			rbf->_max.z = c->FloatAttribute("z");
		}else if(std::strcmp(n,"centers") == 0){
			float x,y,z,w,a;
			for(auto k = c->FirstChildElement() ; k != c->LastChildElement() ; k = k->NextSiblingElement() ) {
				auto type = k->Value();

				x = k->FloatAttribute("x");
				y = k->FloatAttribute("y");
				z = k->FloatAttribute("z");
				w = k->FloatAttribute("weight");

				if(std::strcmp(type,"ThinPlateSplineRBF") == 0){
					rbf->_kernels.push_back(new ThinPlateSplineRBF(x,y,z,w));
				}
				else if(std::strcmp(type,"Biharmonic") == 0){
					rbf->_kernels.push_back(new Biharmonic(x,y,z,w));
				}
				else if(std::strcmp(type,"Triharmonic") == 0){
					rbf->_kernels.push_back(new Triharmonic(x,y,z,w));
				}
				else if(std::strcmp(type,"GausianRBF") == 0){
					a = k->FloatAttribute("a");
					rbf->_kernels.push_back(new GausianRBF(x,y,z,w,a));
				}
				else if(std::strcmp(type,"MultiQuadricRBF") == 0){
					a = k->FloatAttribute("a");
					rbf->_kernels.push_back(new MultiQuadricRBF(x,y,z,w,a));
				}
				else if(std::strcmp(type,"InverseMultiQuadricRBF") == 0){
					a = k->FloatAttribute("a");
					rbf->_kernels.push_back(new InverseMultiQuadricRBF(x,y,z,w,a));
				}
			}
		}else{
			std::cerr << "Unkown xml child attribute for RBF System:" << n << std::endl;
		}
	}
	
	

	return rbf;

}