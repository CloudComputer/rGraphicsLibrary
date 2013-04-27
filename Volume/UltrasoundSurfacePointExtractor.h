#ifndef _ULTRASOUNDSURFACEPOINTEXTRACTOR_H_
#define _ULTRASOUNDSURFACEPOINTEXTRACTOR_H_

#include "ScalarField.h"

#include <IMHelpers\IMHelpers.h>

#ifndef NDEBUG 
#define US_DEBUG
#endif

#define X_AXIS 1
#define Y_AXIS 2
#define Z_AXIS 3

class UltrasoundSurfacePointExtractor{
public:
	static std::vector<glm::vec3> ExtractPoints(ScalarField *vol,int axis);
	static imImage* getSlice(ScalarField *vol,int axis,int slice);
	static void getPointsFromSlice(ScalarField *vol,int axis,int slice,imImage* img,HoughPoint &l0,HoughPoint &l1,std::vector<glm::vec3> &points);

};

#endif