#ifndef _ULTRASOUNDVARIATIONALCLASSIFICATION_H_
#define _ULTRASOUNDVARIATIONALCLASSIFICATION_H_

//#define DBG(str) std::cout << str << " " << __FUNCTION__ << "@" << __FILE__ << ":" << __LINE__ << std::endl;
#define DBG(str)

#include "ScalarField.h"

class UltrasoundVariationalClassification : public ScalarField{
	std::vector<glm::vec3> gradients,blurred_gradints,curls;
	void init(ScalarField *vol,float alpha,float beta,float gamma,float w,float iso,float uind,float xi);
public:
	UltrasoundVariationalClassification(ScalarField *vol,float alpha,float beta,float gamma,float w,float iso,float uind,float xi):
	ScalarField(vol->getDimensions(),vol->getBoundingAABB())
	{
		init(vol,alpha,beta,gamma,w,iso,uind,xi);
	}
	
	UltrasoundVariationalClassification(ScalarField *vol,float alpha,float beta,float w,float iso,float uind,float xi):
	ScalarField(vol->getDimensions(),vol->getBoundingAABB()){
		 init(vol,alpha,beta,1-alpha-beta,w,iso,uind,xi);
	}

	virtual ~UltrasoundVariationalClassification();

};

#endif