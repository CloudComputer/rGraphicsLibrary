#ifndef _IMHELPERS_H_
#define _IMHELPERS_H_

#include "HoughPoint.h"

#include <vector>

#include <im.h>
#include <im_image.h>
#include <im_process.h>
#include <im_convert.h>
#include <im_complex.h>
#include <im_process_ana.h>





imImage* IM_LoadImage(const char* file_name);
void IM_SaveImage(imImage* image, const char* file_name, const char* format);

void IM_PrintError(int error);

std::vector<HoughPoint> IM_GetLines(imImage* img,int maxLines,imImage* out = 0,imImage* binOut=0,imImage* bin2Out=0);

#endif