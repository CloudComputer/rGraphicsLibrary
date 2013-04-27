#include "IMHelpers.h"


#include <stdio.h>

#include <algorithm>

#include <Util\TmpPointer.h>

imImage* IM_LoadImage(const char* file_name){
	int error;
	imFile* ifile = imFileOpen(file_name, &error);
	if (!ifile) {
		IM_PrintError(error);
		return 0;
	}
  
	imImage* image = imFileLoadImage(ifile, 0, &error);  // load the first image in the file.
	if (!image)
		IM_PrintError(error);
    
	imFileClose(ifile);  

	return image;
}


void IM_SaveImage(imImage* image, const char* file_name, const char* format){
	int error;
	imFile* ifile = imFileNew(file_name, format, &error);
	if (!ifile){
		IM_PrintError(error);
		return;
	}
  
	error = imFileSaveImage(ifile, image);
	if (error != IM_ERR_NONE)
		IM_PrintError(error);

	imFileClose(ifile);  
}

void IM_PrintError(int error){
	switch (error){
	case IM_ERR_OPEN:
		printf("Error Opening File.\n");
		break;
	case IM_ERR_MEM:
		printf("Insuficient memory.\n");
		break;
	case IM_ERR_ACCESS:
		printf("Error Accessing File.\n");
		break;
	case IM_ERR_DATA:
		printf("Image type not Suported.\n");
		break;
	case IM_ERR_FORMAT:
		printf("Invalid Format.\n");
		break;
	case IM_ERR_COMPRESS:
		printf("Invalid or unsupported compression.\n");
		break;
	case IM_ERR_NONE:
		break;
	default:
		printf("Unknown Error.\n");
	}
}

#include <iostream>
std::vector<HoughPoint> IM_GetLines(imImage* img,int maxLines,imImage* out,imImage* binOut,imImage* bin2Out){
	std::vector<HoughPoint> lines;

	float rhomax = std::sqrtf(img->width*img->width +img->height*img->height)/2;
	float hough_height=2*rhomax+1;
	
	TmpPointer<imImage> binary  = imImageCreate(img->width, img->height, IM_BINARY, IM_BYTE);
	TmpPointer<imImage> binary2 = imImageCreate(img->width, img->height, IM_BINARY, IM_BYTE);

	TmpPointer<imImage> hough        = imImageCreate(180, hough_height, IM_GRAY   , IM_INT);
	TmpPointer<imImage> hough_binary = imImageCreate(180, hough_height, IM_BINARY, IM_BYTE);

	auto data = (imbyte*)img->data[0];
	for(int x = 0;x!=img->width;x++)for(int y = 0;y!=img->height;y++){
		int id = x + y*img->width;
		if(data[id] != 0){
			((bool*)binary->data[0])[id] = true;	
		}else{
			((bool*)binary->data[0])[id] = false;
		}
		if(binOut)
			((bool*)binOut->data[0])[id] = ((bool*)binary->data[0])[id];
		/*if(bin2Out)
			((bool*)bin2Out->data[0])[id] = ((bool*)binary->data[0])[id];*/
	}
	imProcessBinMorphClose(binary.get(),binary2.get(),1,1);
	imProcessFillHoles(binary2.get(),binary.get(),8);


	if(binOut){
		imProcessFillHoles(binary2.get(),binOut,8);
	}

	imProcessBinMorphOutline(binary.get(),binary2.get(),3,1);
	
	
	if(bin2Out)
		imProcessBinMorphOutline(binary.get(),bin2Out,3,1);
	
	imProcessHoughLines(binary2.get(), hough.get());
	imProcessLocalMaxThreshold(hough.get(), hough_binary.get(), 7, 100);

	for(int x = 0;x!=hough->width;x++)for(int y = 0;y!=hough->height;y++){
		int id = x + y*hough->width;

		/*if(!((bool*)hough_binary->data[0])[id]){
			continue;
		}*/

		int tmp = ((int*)hough->data[0])[id];
		HoughPoint hp(tmp,x,y,rhomax);
		lines.push_back(hp);
	}
	auto it = std::unique(lines.begin(),lines.end());
	lines.erase(it,lines.end());
	std::sort(lines.begin(),lines.end());

	
	if(lines.size()>maxLines){
		lines.erase(lines.begin()+maxLines,lines.end());
	}

	if(out!=0){
		TmpPointer<imImage> tmp = imImageCreate(img->width, img->height, IM_GRAY, IM_BYTE);
		imProcessHoughLinesDraw(img,hough.get(),hough_binary.get(),tmp.get());
		imProcessConvertColorSpace(tmp.get(), out);
	}

	/*for(auto l = lines.begin();l!=lines.end();++l){
		float k,m;
		l->ykxm(k,m,img->width,img->height);
		std::cout << "y="<<k<<"x+"<<m<<std::endl;
	}*/

	return lines;
}