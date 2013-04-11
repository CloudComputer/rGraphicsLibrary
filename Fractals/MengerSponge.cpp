#include "MengerSponge.h"



MengerSponge::MengerSponge(bool inverted ,int subdivisions,bool variateInversion){
	_boxes.push_back(BoundingAABB(glm::vec3(-1,-1,-1),glm::vec3(1,1,1)));
	_inverted = inverted;
	_variateInversion = variateInversion;
	subdivide(subdivisions);
}


MengerSponge::~MengerSponge(){
	
}


void MengerSponge::toMesh(IndexedMesh *mesh){
	mesh->clear(true);
	glm::vec3 p[8];
	for(auto box = _boxes.begin();box != _boxes.end();++box){
		p[0] = box->getPosition(glm::vec3(0,0,0));
		p[1] = box->getPosition(glm::vec3(1,0,0));
		p[2] = box->getPosition(glm::vec3(0,1,0));
		p[3] = box->getPosition(glm::vec3(1,1,0));
		p[4] = box->getPosition(glm::vec3(0,0,1));
		p[5] = box->getPosition(glm::vec3(1,0,1));
		p[6] = box->getPosition(glm::vec3(0,1,1));
		p[7] = box->getPosition(glm::vec3(1,1,1));

	
	
		mesh->addTriangle(p[0],p[3],p[1]); //back
		mesh->addTriangle(p[0],p[2],p[3]);

		mesh->addTriangle(p[4],p[5],p[7]); //front
		mesh->addTriangle(p[4],p[7],p[6]);
	
		mesh->addTriangle(p[1],p[3],p[5]); //right
		mesh->addTriangle(p[5],p[3],p[7]);
	
		mesh->addTriangle(p[2],p[0],p[4]);//left
		mesh->addTriangle(p[2],p[4],p[6]);
	
		mesh->addTriangle(p[3],p[2],p[6]);//top
		mesh->addTriangle(p[3],p[6],p[7]);

		mesh->addTriangle(p[0],p[1],p[4]); //bottom
		mesh->addTriangle(p[1],p[5],p[4]);
	}

	
}

void MengerSponge::subdivide(int amount){
	if(amount<=0) return;
	int size = _boxes.size();
	glm::vec3 t0,t1;
	for(int i = 0;i<size;i++){
		for(int x = 0;x<3;x++)for(int y = 0;y<3;y++)for(int z = 0;z<3;z++){
			bool use = true;
			use = use && !(x == 1 && y == 1);
			use = use && !(z==1 && (x == 1 || y == 1));
			use = use != _inverted;
			if(!use) continue;

			t0.x = x / 3.0;
			t0.y = y / 3.0;
			t0.z = z / 3.0;
			t1 = t0 + glm::vec3(1.0/3.0,1.0/3.0,1.0/3.0);
			_boxes.push_back(BoundingAABB(_boxes[0].getPosition(t0),_boxes[0].getPosition(t1)));
		}
		_boxes.erase(_boxes.begin());
	}
	if(_variateInversion)
		_inverted = !_inverted;
	subdivide(amount-1);
}


