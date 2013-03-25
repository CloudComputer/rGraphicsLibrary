#ifndef  _BMPWRITER_H_
#define  _BMPWRITER_H_

#include <fstream>
#include <stdint.h>
#include <glm\glm.hpp>

struct BMPMAGIC{
    unsigned char magic[2];
};

struct BMPHEADER{
    uint32_t size;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t offset;
};

struct BITMAPINFOHEADER40{
    uint32_t size;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bpp;
    uint32_t compress_type;
    uint32_t bmp_bytesz;
    int32_t hres;
    int32_t vres;
    uint32_t ncolors;
    uint32_t nimpcolors;
};

enum BMP_COMPRESS_TYPE{
  BMP_RGB = 0,
  BMP_RLE8,
  BMP_RLE4,
  BMP_BITFIELDS,
  BMP_JPEG,
  BMP_PNG,
};

struct PIXEL24BIT{
    uint8_t blue;
    uint8_t green;
    uint8_t red;
};




class BMPWriter
{
	int width,height;
	float *data;
public:
	BMPWriter(int w,int h);
	void setPixel(int x,int y,glm::vec3 color);
	void save(const char * filename);

	float getMaxPixelValue(){
		float max = 0;
		for(int i = 0;i<width*height*3;i++){
			if(data[i]>max)
				max = data[i];
		}
		return max;
	}

	void flat(){
		float max = getMaxPixelValue();
		for(int i = 0;i<width*height*3;i++){
			data[i] /= max;
		}
	}

	void scale(float scale = 0){
		float max = getMaxPixelValue();
		for(int i = 0;i<width*height*3;i++){
			data[i] = log(1+data[i]+scale)/log(1+max+scale);
		}
	}

	~BMPWriter(void);
};

#endif