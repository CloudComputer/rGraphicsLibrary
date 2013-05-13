#ifndef _COLOR_H_
#define _COLOR_H_

#include <glm\glm.hpp>


class Color{
public:
	Color();
	virtual ~Color();

	
	static glm::vec3 HSV2RGB(float h,float s,float v);
	static glm::vec3 HSV2RGB(glm::vec3 hsv){return HSV2RGB(hsv.x,hsv.y,hsv.z);}
};

#endif