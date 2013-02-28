#include "Image.h"

#include <IL\il.h>
#include <IL\ilu.h>
#include <IL\ilut.h>

bool Image::isInitzialied = false;

void Image::init(){
	if(isInitzialied)
		return;
	isInitzialied = true;

	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);
}

void Image::TakeScreenShot(const char *filename){
	init();

	ILuint img;
	ilGenImages(1, &img); 
	ilBindImage(img);
	ilutGLScreen();

	ilSaveImage(filename);
}