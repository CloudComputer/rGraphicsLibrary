#include "CSGReader.h"

#include "MeshRenderer.h"

#include <Geometry\Mesh\IndexedMesh.h>

#include <Geometry\CSG\CSG.h>
#include <Geometry\CSG\PointCloudInterpolation.h>
#include <Geometry\CSG\MarchingTetrahedra.h>

#include <Util\Macros.h>

void CSGReader::ReadXML(rObject *&obj,tinyxml2::XMLElement *ele){
	CSG *csg = read(ele->FirstChildElement());
	if(!csg){
		std::cerr << "Failed to create CSG" << std::endl;
		obj = 0;
		return;
	}
	glm::vec3 minB(0,0,0);
	glm::vec3 maxB(0,0,0);
	auto eBounds = ele->FirstChildElement("bounds");
	if(eBounds){
		auto eMin = eBounds->FirstChildElement("min");
		auto eMax = eBounds->FirstChildElement("max");
		if(eMin){
			std::istringstream iss(eMin->GetText());
			iss >> minB.x >> minB.y >> minB.z;
		}
		if(eMax){
			std::istringstream iss(eMax->GetText());
			iss >> maxB.x >> maxB.y >> maxB.z;
		}
	}BoundingAABB aabb(minB,maxB);

	int res = 10;
	auto eRes = ele->FirstChildElement("resolution");
	if(eRes){
		res = std::atoi(eRes->GetText());
	}

	//Calculate 3D res and do march
	glm::vec3 offsets = maxB - minB;
	float maxOff = std::max(std::max(offsets.x,offsets.y),offsets.z);
	glm::ivec3 dimm;
	dimm.x = res * (offsets.x / maxOff);
	dimm.y = res * (offsets.y / maxOff);
	dimm.z = res * (offsets.z / maxOff);

	CSGCache cahced(csg);
	Mesh *m;
	auto pci = dynamic_cast<PointCloudInterpolation*>(csg);
	if(pci){
		m = MarchingTetrahedra::March<IndexedMesh>(pci,res);
	}else{
		m = MarchingTetrahedra::March<IndexedMesh>(&cahced,aabb,dimm);
	}
	

	obj = new MeshRenderer();
	auto renderer = static_cast<MeshRenderer*>(obj);
	renderer->readMaterial(ele);
	renderer->buildFromMesh(m);
}

CSG* CSGReader::read(tinyxml2::XMLElement *ele){
	auto name = ele->Name();
	if(strcmp(name,"union")==0){
		auto csg = new CSGCombine(Union);
		FOR_XML_ALL_ELE(ele,c){
			csg->addCSG(read(c));
		}
		return csg;
	}
	else if(strcmp(name,"difference")==0){
		auto csg = new CSGCombine(Difference);
		FOR_XML_ALL_ELE(ele,c){
			csg->addCSG(read(c));
		}
		return csg;
	}
	else if(strcmp(name,"intersection")==0){
		auto csg = new CSGCombine(Intersection);
		FOR_XML_ALL_ELE(ele,c){
			csg->addCSG(read(c));
		}
		return csg;
	}
	else if(strcmp(name,"pointCloud")==0){
		std::vector<glm::vec3> points;
		PointCloudInterpolationHints hints;
		auto ePoints = ele->FirstChildElement("points");
		if(!ePoints){
			std::cerr << "No points specified to interpolate in point cloud" << std::endl;
			exit(-1);
		}
		auto aMesh = ePoints->Attribute("mesh");
		auto N = ePoints->IntAttribute("amount");
		if(aMesh){
			auto *m = static_cast<IndexedMesh*>(Mesh::LoadWavefront<IndexedMesh>(aMesh));
			glm::vec3 minP = m->getBoundingAABB().minPos();
			glm::vec3 maxP = m->getBoundingAABB().maxPos();
			minP.x = std::min(std::min(minP.x,minP.y),minP.z);
			maxP.x = std::max(std::max(maxP.x,maxP.y),maxP.z);
			minP.z = minP.y = minP.x;
			maxP.z = maxP.y = maxP.x;
			float meshArea = m->getArea();
			auto triangles = m->getFaces();
			for(auto tri = triangles.begin();tri!=triangles.end();++tri){
				float p = N * (tri->area() / meshArea);
				int numPoints = std::floorf(p + 0.5);
				for(int i = 0;i<numPoints;i++){
					auto p = Random::getRandomGenerator()->inTriangle(*tri);
					p -= minP;
					p /= (maxP-minP);
					points.push_back(p);
				}
			}
		}else{
			std::cerr << "No source for points" << std::endl;
			exit(-1);
		}


		return new PointCloudInterpolation(points,hints);
	}
	else if(strcmp(name,"sphere")==0){
		glm::vec3 pos;
		std::istringstream iss(ele->Attribute("center"));
		iss >> pos.x >> pos.y >> pos.z;
		float r = ele->FloatAttribute("radius");
		return new CSGSphere(r,pos);
	}
	else{
		std::cerr << "Unkown CSG type" << std::endl;
		return 0;
	}
}