#ifndef _RBFHELPER_H_
#define _RBFHELPER_H_

#include <ext\glm\glm.hpp>
#include <vector>

#define RBF_DEBUG

#include "RBF.h"

#include <Geometry\CSG\MarchingTetrahedra.h>

struct RBFHints{
	float offSurfPointDistnace;
	float smoothness;
	int minPointsForIterativeFitting;

	bool useCentersReduction;
	float centerReductionStartWith;
	float centerReductionFitingAccuracy;

	
	float fastFitAccuracy;
	int fastFitMinInnerSize;
	float fastFitOuterSize;
	int fastFitCoarseGridSize; 
	int fastFitMaxIterations;


	RBFHints(){
		offSurfPointDistnace = 0.02;
		smoothness = 0;
		minPointsForIterativeFitting = 4000;


		useCentersReduction = false;
		centerReductionStartWith = 0.2;
		centerReductionFitingAccuracy = 5*10e-2;


		fastFitAccuracy = 10e-7;
		fastFitMinInnerSize = 1000;
		fastFitOuterSize = 0.1f;
		fastFitCoarseGridSize = 60; 
		fastFitMaxIterations = 10;
	}
};


class RBFHelper{
	std::vector<glm::vec4> centers; //[x y z d]
	RBFSystem *rbf;

public:
	RBFHelper();
	virtual ~RBFHelper();

	RBFHints hints;

	void setPoints(std::vector<glm::vec3> points);

	template<class KernelType> void fit();
	template<class MeshType> MeshType* getMesh(int resolution = 20)const{
		return static_cast<MeshType*>(MarchingTetrahedra::March<MeshType>(rbf,resolution));
	}

};

typedef Eigen::FullPivHouseholderQR<Eigen::MatrixXf> RBFHelperSolver;

struct __rbfCenterReductionNode{
	static bool sortOnId;
	int id;
	float e;
};

bool operator< (const __rbfCenterReductionNode& a, const __rbfCenterReductionNode& b);


template<class KernelType> void RBFHelper::fit(){
	if(rbf != 0)
		delete rbf;
	
	if(!hints.useCentersReduction &&  centers.size() <= hints.minPointsForIterativeFitting){
		rbf = RBFSystem::CreateFromPoints<KernelType>( centers,hints.smoothness);
	}

	if(hints.useCentersReduction && hints.centerReductionStartWith < 1.0f  && hints.centerReductionStartWith > 0.0f){
		std::vector<glm::vec4> subCenters;
		std::vector<glm::vec4> leftCenters;
		float inc = 1.0f/hints.centerReductionStartWith;
		for(float i = 0;i<centers.size();i+=inc){ //create first set of points
			subCenters.push_back(centers[(int)(i)]);

			for(int j = i+1; j < (int)(i+inc) && j < centers.size() ; j++){
				leftCenters.push_back(centers[j]);
			}

		}

		std::cout << "This should be zero:" << (centers.size() - subCenters.size() - leftCenters.size()) << std::endl;

		bool notDone = true;
		while(notDone){
			std::cout << "Centers: " << subCenters.size() << std::endl;
			if(rbf != 0)
				delete rbf;
			if(subCenters.size() <= hints.minPointsForIterativeFitting)
				rbf = RBFSystem::CreateFromPoints<KernelType>( subCenters,hints.smoothness);
			else
				rbf = RBFSystem::FastFitting<KernelType,RBFHelperSolver>(subCenters,hints.smoothness,hints.fastFitAccuracy,hints.fastFitMinInnerSize,hints.fastFitOuterSize,hints.fastFitCoarseGridSize,hints.fastFitMaxIterations);

			notDone = false;
			std::vector<__rbfCenterReductionNode> outsides;

			//int j = 0;
			for(int j = 0; j<leftCenters.size(); j++){
			//IT_FOR(centers,c){
				__rbfCenterReductionNode r;
				r.id = j++;
				auto c = &leftCenters[j];
				r.e = std::abs(c->w  - rbf->eval(glm::vec3(*c)));
				
				if(r.e>hints.centerReductionFitingAccuracy){
					notDone = true;
					outsides.push_back(r);
				}
			}

			if(notDone){
				__rbfCenterReductionNode::sortOnId = false;
				std::sort(outsides.begin(),outsides.end());
				
				if(outsides.size() >= subCenters.size()){
					outsides.erase(outsides.begin() + subCenters.size() , outsides.end());
				}

				for(int j = 0;j<outsides.size();j++){
					subCenters.push_back(leftCenters[outsides[j].id]);
				}

				
				__rbfCenterReductionNode::sortOnId = true;
				std::sort(outsides.begin(),outsides.end());

				for(int j = 0;j<outsides.size();j++){
					leftCenters.erase(leftCenters.begin() + outsides[j].id);
				}

			}

		}
	}else{
		rbf = RBFSystem::FastFitting<KernelType,RBFHelperSolver>(centers,hints.smoothness,hints.fastFitAccuracy,hints.fastFitMinInnerSize,hints.fastFitOuterSize,hints.fastFitCoarseGridSize,hints.fastFitMaxIterations);
	}


}

#endif