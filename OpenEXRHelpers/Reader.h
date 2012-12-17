#ifndef __OPENEXR_READER__H_
#define __OPENEXR_READER__H_

#include "OpenEXRIncludes.h"




class OpenEXRReader{
public:
	static RGBABuffer readRGBA(std::string filename);
};

#endif