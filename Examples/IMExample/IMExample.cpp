#include <IMHelpers\IMHelpers.h>

#include <iostream>

#include <sstream>

//#include <Util\TmpPointer.h>




int main(int argc,char **argv){
	imImage* bmp = IM_LoadImage("test.bmp");
	if(!bmp)
		return 1;
	
	auto points = IM_GetLines(bmp,2);


	return 0;
}