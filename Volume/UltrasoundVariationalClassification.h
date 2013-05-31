#ifndef _ULTRASOUNDVARIATIONALCLASSIFICATION_H_
#define _ULTRASOUNDVARIATIONALCLASSIFICATION_H_

#define DBG(str) std::cout << str << " " << __FUNCTION__ << "@" << __FILE__ << ":" << __LINE__ << std::endl;
//#define DBG(str)

#include "ScalarField.h"

class UltrasoundVariationalClassification : public ScalarField{
public:
	UltrasoundVariationalClassification(ScalarField *vol,float alpha,float beta,float w,float iso,float uind,float xi);
	virtual ~UltrasoundVariationalClassification();

};

#endif