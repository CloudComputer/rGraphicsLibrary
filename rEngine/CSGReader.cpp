#include "CSGReader.h"

#include "MeshRenderer.h"

#include <Geometry\Mesh\IndexedMesh.h>

#include <Geometry\CSG\CSG.h>
#include <Geometry\CSG\PointCloudInterpolation.h>
#include <Geometry\CSG\MarchingTetrahedra.h>

#include <Volume\UltrasoundVariationalClassification.h>

#include <Geometry\Clustering\Clusterer.h>

#include <Util\Macros.h>
#include <Util\Logger.h>

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
	LOG_INFO("Starting to extract mesh with MarchingTetrahedra");
	auto pci = dynamic_cast<PointCloudInterpolation*>(csg);
	if(pci){
		m = MarchingTetrahedra::March<IndexedMesh>(pci,res);
	}else{
		m = MarchingTetrahedra::March<IndexedMesh>(&cahced,aabb,dimm);
	}
	LOG_INFO("Mesh extracted");
	

	obj = new MeshRenderer();
	auto renderer = static_cast<MeshRenderer*>(obj);
	renderer->readMaterial(ele);
	renderer->buildFromMesh(m);
	LOG_INFO("Mesh renderer initilized");

	auto save = ele->FirstChildElement("save");
	if(save){
		auto file= save->Attribute("file");
		if(file){
			LOG_INFO("Saving mesh to " << file);
			m->save(file);
		}
	}

}

float tryReadFloat(tinyxml2::XMLElement *ele,const char *attr,float elseValue){
	auto attribute = ele->Attribute(attr);
	if(!attribute)
		return elseValue;
	return  ele->FloatAttribute(attr);
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
		std::vector<glm::vec3> normals;
		PointCloudInterpolationHints hints;
		auto ePoints = ele->FirstChildElement("points");
		if(!ePoints){
			std::cerr << "No points specified to interpolate in point cloud" << std::endl;
			exit(-1);
		}
		auto aMesh = ePoints->Attribute("mesh");
		auto aUltrasound = ePoints->Attribute("ultrasound");
		auto aDimensions = ePoints->Attribute("dimensions");
		auto N = ePoints->IntAttribute("amount");
		if(aUltrasound && aDimensions){
			glm::ivec3 d;
			std::istringstream iss(aDimensions);
			if(iss  >> d.x >> d.y >> d.z){
				float alpha, beta,gamma, w, iso, uind, xi, threshold;
				alpha = tryReadFloat(ePoints,"alpha",0.5);
				beta = tryReadFloat(ePoints,"beta",0.5);
				gamma = tryReadFloat(ePoints,"gamma",0.5);
				iso = tryReadFloat(ePoints,"iso",0.6);
				w = tryReadFloat(ePoints,"w",0.6);
				uind = tryReadFloat(ePoints,"uind",0.95);
				xi = tryReadFloat(ePoints,"xi",0.8);	
                
                LOG_INFO("Loading ultrasound dataset");
				TmpPointer<ScalarField> vol = ScalarField::ReadFromRawfile(aUltrasound,d.x,d.y,d.z);LOG_INFO("Loaded dataset, begin classification");
				ScalarField* clippedVol = 0;
				auto clip = ele->FirstChildElement("clip");
				if(clip){
					glm::ivec3 clipMin(0,0,0),clipMax(d);
					std::istringstream issMin(clip->Attribute("min"));
					std::istringstream issMax(clip->Attribute("max"));
					issMin >> clipMin.x >> clipMin.y >> clipMin.z;
					issMax >> clipMax.x >> clipMax.y >> clipMax.z;
					clippedVol = vol->clip(clipMin,clipMax); //Warning, this is mem leak
					LOG_INFO("Using clipped volume " << clipMin << clipMax);
				}else{
					clippedVol = vol.get(); 
				}

				TmpPointer<UltrasoundVariationalClassification> usClassified = new UltrasoundVariationalClassification(clippedVol,alpha,beta,gamma,w,iso,uind,xi);LOG_INFO("classification done, starting lowpass filtering");
				TmpPointer<ScalarField> blured = usClassified->blur();LOG_INFO("lowpass done, starting canny operator");
				TmpPointer<ScalarField> bluredOrig = clippedVol->blur();LOG_INFO("lowpass done, starting canny operator");
				TmpPointer<ScalarField> surfVol = blured->Canny();LOG_INFO("canny done");
				KDTree<Vertex,3,float> pointTree;
				
				LOG_DEBUG(aUltrasound);
				LOG_DEBUG(d);
				LOG_DEBUG(vol->getDimensions());
				LOG_DEBUG(clippedVol->getDimensions());
				LOG_DEBUG(usClassified->getDimensions());
				LOG_DEBUG(blured->getDimensions());
				LOG_DEBUG(surfVol->getDimensions())

				std::stringstream ss;
				ss << aUltrasound << ".classified.raw";

			//	usClassified->saveAsRaw(ss.str().c_str());

				glm::vec3 minP = clippedVol->getBoundingAABB().minPos();
				glm::vec3 maxP = clippedVol->getBoundingAABB().maxPos();
				minP.x = std::min(std::min(minP.x,minP.y),minP.z);
				maxP.x = std::max(std::max(maxP.x,maxP.y),maxP.z);
				minP.z = minP.y = minP.x;
				maxP.z = maxP.y = maxP.x;

				//TmpPointer<ScalarField> test = surfVol->doubleSize();

                LOG_INFO("Extracting points");
				auto d2 = d;
				FOR(surfVol->getDimensions()){
					if(surfVol->get(glm::ivec3(x,y,z)) >= 0.1){
						auto pos = surfVol->getBoundingAABB().getPosition(glm::vec3(x,y,z) / glm::vec3(d2));
						auto g = bluredOrig->getGradient(pos);
						Vertex v;
						pos -= minP;
						pos /= (maxP-minP);
						v.setPosition(glm::vec4(pos,1));;
						float l = glm::length(g);
						if(l!=0){
							v.setNormal(g/l);
						}
						else
							v.setNormal(glm::vec3(0,0,0));
						if(l!=l || g.x != g.x){
							std::cout << "sometin wong" << std::endl;
						}
						pointTree.insert(glm::value_ptr(pos),v);
					}
				}
                LOG_INFO("Cluserting extracted points");
				float dd = 1*glm::length(glm::vec3(1.0/d2.x,1.0/d2.y,1.0/d2.z));
				std::cout << dd << std::endl;
				glm::vec3 vp(0,1,0);
				KDTree<Vertex,3,float> *cluster = Clusterer::ClusterPoints(&pointTree,dd,10,true)[0].getPoints();
                IT_FOR((*cluster),clust){
					points.push_back(glm::vec3(clust->get().getPosition()));
					normals.push_back(clust->get().getNormal());
				}
                LOG_INFO( std::string("Clusering removed ") << (pointTree.size() - points.size()));
			}
		}
		else if(aMesh){
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
					normals.push_back(tri->getNormal());
				}
			}
		}else{
			std::cerr << "No source for points" << std::endl;
			exit(-1);
		}

		LOG_INFO("Number of points " << points.size());
		LOG_INFO("Number of normals " << normals.size());

		auto eHints = ele->FirstChildElement("hints");
		if(eHints){
			auto eOverlap = eHints->FirstChildElement("overlap");
			auto eK = eHints->FirstChildElement("K");
			auto eMinSupportSize = eHints->FirstChildElement("minSupportSize");
			auto eMaxSupportSize = eHints->FirstChildElement("maxSupportSize");
			auto eReg = eHints->FirstChildElement("reg");
			auto eSa = eHints->FirstChildElement("Tsa");
			if(eOverlap)		hints.TOverlap = atof(eOverlap->GetText());
			if(eK)				hints.K = atoi(eK->GetText());
			if(eMinSupportSize)	hints.minSupportSize = atof(eMinSupportSize->GetText());
			if(eMaxSupportSize)	hints.maxSupportSize = atof(eMaxSupportSize->GetText());
			if(eReg)			hints.Treg = atof(eReg->GetText());
			if(eSa)				hints.Tsa = atof(eSa->GetText());
		}
        LOG_INFO("Starting initialization of PointCloudInterpolation");
        auto csg = new PointCloudInterpolation(points,hints,normals);
        LOG_INFO("PointCloudInterpolator created with global error: " << csg->eGlobal());
		//exit(0);
		return csg;
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