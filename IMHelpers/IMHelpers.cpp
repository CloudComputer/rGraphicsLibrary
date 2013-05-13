#include "IMHelpers.h"


#include <stdio.h>

#include <algorithm>

#include <Util\TmpPointer.h>

#include <OpenGLHelpers\FBO.h>

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

imImage* IM_GetScreenshot(){
	int vp[4];
	glGetIntegerv(GL_VIEWPORT, vp);
	int w;int h;
	w = vp[2] - vp[0];
	h = vp[3] - vp[1];
	imImage *img = imImageCreate(w, h, IM_RGB, IM_BYTE);
	uint8_t* data = new uint8_t[w*h*3*sizeof(uint8_t)];
	
	glReadPixels(vp[0],vp[1],w,h,GL_RGB,GL_UNSIGNED_BYTE,data);
	for(int i = 0;i<w*h;i++){
		((uint8_t*)img->data[0])[i] = data[i*3+0];
		((uint8_t*)img->data[1])[i] = data[i*3+1];
		((uint8_t*)img->data[2])[i] = data[i*3+2];
	}

	delete [] data;
	return img;
}	

std::vector<HoughPoint> IM_GetLines(imImage* img,int maxLines,imImage* out,imImage* binOut,imImage* bin2Out){
	std::vector<HoughPoint> lines;

	float rhomax = std::sqrtf(img->width*img->width +img->height*img->height)/2;
	float hough_height=2*rhomax+1;
	
	IM_Pointer binary  = imImageCreate(img->width, img->height, IM_BINARY, IM_BYTE);
	IM_Pointer binary2 = imImageCreate(img->width, img->height, IM_BINARY, IM_BYTE);

	IM_Pointer hough        = imImageCreate(180, hough_height, IM_GRAY   , IM_INT);
	IM_Pointer hough_binary = imImageCreate(180, hough_height, IM_BINARY, IM_BYTE);

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
		IM_Pointer tmp = imImageCreate(img->width, img->height, IM_GRAY, IM_BYTE);
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