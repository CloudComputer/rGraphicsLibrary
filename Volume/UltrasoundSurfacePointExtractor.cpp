#include "UltrasoundSurfacePointExtractor.h"


#include <Util\TmpPointer.h>

std::vector<glm::vec3> UltrasoundSurfacePointExtractor::ExtractPoints(ScalarField *vol,int axis){
	std::vector<glm::vec3> points;

	int slices = 0;
	glm::ivec2 res;
	if(axis==X_AXIS){
		res.x = vol->getDimensions().y;
		res.y = vol->getDimensions().z;
		
		slices = vol->getDimensions().x;
	}else if(axis==Y_AXIS){
		res.x = vol->getDimensions().x;
		res.y = vol->getDimensions().z;

		slices = vol->getDimensions().y;
	}else if(axis==Z_AXIS){
		res.x = vol->getDimensions().x;
		res.y = vol->getDimensions().y;

		slices = vol->getDimensions().z;
	}
	
	for(int i = 0;i<slices;i++){
		auto slice = getSlice(vol,axis,i);

#ifndef US_DEBUG
		auto lines = IM_GetLines(slice,2)
#else
		TmpPointer<imImage> out = imImageCreate(res.x, res.y, IM_RGB, IM_BYTE);
		auto lines = IM_GetLines(slice,2,out.get());
		std::stringstream ss;
		ss << "slice_"<<i<<".jpg";
		IM_SaveImage(out.get(),ss.str().c_str(),"JPEG");
#endif	
		if(lines.size()!=2){
			std::cout << i  << std::endl;
		}
	}


	return points;
}

imImage* UltrasoundSurfacePointExtractor::getSlice(ScalarField *vol,int axis,int slice){
	glm::ivec2 res;
	if(axis==X_AXIS){
		res.x = vol->getDimensions().y;
		res.y = vol->getDimensions().z;
	}else if(axis==Y_AXIS){
		res.x = vol->getDimensions().x;
		res.y = vol->getDimensions().z;
	}else if(axis==Z_AXIS){
		res.x = vol->getDimensions().x;
		res.y = vol->getDimensions().y;
	}

	imImage* img = imImageCreate(res.x, res.y, IM_GRAY, IM_BYTE);
	auto data = (imbyte*)img->data[0];

	for(int x = 0; x < res.x ; x++)for(int y = 0; y < res.y ; y++) {
		float v;
		if(axis==X_AXIS){
			v = vol->get(glm::ivec3(slice,x,y));
		}else if(axis==Y_AXIS){
			v = vol->get(glm::ivec3(x,slice,y));
		}else if(axis==Z_AXIS){
			v = vol->get(glm::ivec3(x,y,slice));
		}
		int id = x + y*res.x;
		data[id] = v * 255;
	}
	

	return img;
}

static void getPointsFromSlice(imImage* img,HoughPoint &l0,HoughPoint &l1,std::vector<glm::vec3> &points){
	
#ifdef US_DEBUG
	TmpPointer<imImage> dbgOut = imImageCreate(img->width,img->height, IM_RGB, IM_BYTE);
	imProcessConvertColorSpace(img,dbgOut.get());
#endif	

	//Extract points from slice using raytracing between l0 and l1, see matlab code

	#ifdef US_DEBUG //do this for every found point
	int id;
	((imbyte*)dbgOut->data[1])[id] = 255;
	#endif

	
#ifdef US_DEBUG
	static int slice = 0;
	std::stringstream ss;
	ss << "points_"<<slice++<<".jpg";
	IM_SaveImage(dbgOut.get(),ss.str().c_str(),"JPEG");
#endif
}