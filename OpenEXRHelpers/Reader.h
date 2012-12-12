#ifndef __OPENEXR_READER__H_
#define __OPENEXR_READER__H_

#include "OpenEXRIncludes.h"

struct RGBABuffer{
	Imf::Rgba *buffer;
	Imath::V2i dim;
};


class OpenEXRReader{
public:
	static RGBABuffer readRGBA(std::string filename);
};

#endif