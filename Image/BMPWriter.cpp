#include "BMPWriter.h"

template<typename T>  T clamp(T a,T b,T c){
	if(a <= b) return b;
	if(a >= c) return c;
	return a;
}

BMPWriter::BMPWriter(int w,int h){
	width = w;
	height = h;
	data = (float *)malloc(sizeof(float)*3*w*h);
}

void BMPWriter::setPixel(int x,int y,glm::vec3 color){
	data[(x+y*width)*3+0] = color[0];
	data[(x+y*width)*3+1] = color[1];
	data[(x+y*width)*3+2] = color[2];
}

float ScaleInput(float t){
	return t;
}

void BMPWriter::save(const char * filename){
	BMPMAGIC mp;
    BMPHEADER bmpheader;
    BITMAPINFOHEADER40 infoheader;
    mp.magic[0] = 'B';
    mp.magic[1] = 'M';
    bmpheader.size = sizeof(BMPMAGIC) * sizeof(BMPHEADER) * sizeof(BITMAPINFOHEADER40) * width*height*3;
    bmpheader.reserved1 = 0;
    bmpheader.reserved1 = 0;
    bmpheader.offset = 54;


    infoheader.size = 40;
    infoheader.width = width;
    infoheader.height = height;
    infoheader.planes = 1;
    infoheader.bpp = 24;
    infoheader.compress_type = 0;
    infoheader.bmp_bytesz = width*height*3;
    infoheader.hres = 0x130B0000;
    infoheader.vres = 0x130B0000;
    infoheader.ncolors = 0;
    infoheader.nimpcolors = 0;

    FILE *f = fopen(filename,"wb");
    assert(f != NULL);
    fwrite(&mp,sizeof(BMPMAGIC),1,f);
    fwrite(&bmpheader,sizeof(BMPHEADER),1,f);
    fwrite(&infoheader,sizeof(BITMAPINFOHEADER40),1,f);

    int i=0;
    for(int y = 0; y<infoheader.height;y++){
        unsigned int pad_bytes = 0;
        for(int x = 0; x<infoheader.width;x++){
            pad_bytes++;
            PIXEL24BIT pixel;
			int index = (x+y*width)*3;
			pixel.red   = 255*clamp(ScaleInput(data[index+0]),0.f,1.f);
            pixel.green = 255*clamp(ScaleInput(data[index+1]),0.f,1.f);
            pixel.blue  = 255*clamp(ScaleInput(data[index+2]),0.f,1.f);
			/*if(data[index+0] != 0 ||data[index+1] != 0 ||data[index+2] != 0)
				pixel.blue  = 255;*/
            i++;
            fwrite(&pixel,sizeof(PIXEL24BIT),1,f);
        }
        pad_bytes = pad_bytes % 4;
        uint8_t pad = 0;
        for(unsigned int p = 0;p<pad_bytes;p++)
            fwrite(&pad,1,1,f);
    }
    fclose(f);
}

BMPWriter::~BMPWriter(void){
	free (data);
}