#ifndef _LIGHTEXTRACTOR_H_
#define _LIGHTEXTRACTOR_H_

#include "OpenEXRIncludes.h"

#include <OpenGLHelpers\Light.h>

#include <glm/glm.hpp>


class LightExtractor{
protected:
	struct Region{
		double flux;
		unsigned int minX,maxX,minY,maxY;
	};

	static std::vector<Region> __splitRegions(std::vector<Region> regions,GreyBuffer buffer);
	static void __split(Region R,Region &r0,Region &r1,GreyBuffer buffer);
public:
	static std::vector<Light>     ExtractLights(RGBABuffer buffer,int numSplits = 4,bool directionalLights = true,float distance = 1.0);
	static std::vector<glm::vec2> ExtractLightLocations(RGBABuffer buffer,int numSplits = 4);
	static std::vector<glm::vec2> ExtractLightLocations(GreyBuffer buffer,int numSplits = 4);
	static std::vector<glm::vec2> ExtractLightLocations(GreyBuffer buffer,std::vector<Region> regions);
	static std::vector<Region>	  ExtractLightRegions(GreyBuffer buffer,int numSplits = 4);
};


#endif
