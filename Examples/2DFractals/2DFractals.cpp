#include <sstream>
#include <complex>
#include <iostream>

#include <IMHelpers\IMHelpers.h>
#include <Util\Date.h>
#include <Math\Interpolator.h>

#include <Util\Color.h>


int width  = 1000;
int height = 1000;
int maxIt = 100;
double power = 2.0;

LinearInterpolator<double> realAxis(-2.2,1.2);
LinearInterpolator<double> imAxis(-1.5,1.5);

void assignColor(imImage *img,int id,glm::vec3 color){
	((unsigned char*)img->data[0])[id] = 255*color.r;
	((unsigned char*)img->data[1])[id] = 255*color.g;
	((unsigned char*)img->data[2])[id] = 255*color.b;
}

void createSimple(imImage *img){
	for(int x = 0;x<width;x++){
		//std::cout << x << std::endl;
		for(int y = 0;y<height;y++){
			int id = x + y*width;
			double i = x;i/=width;
			double j = y;j/=height;
			std::complex<double> z,zn,c(realAxis.interpolate(i),imAxis.interpolate(j));
			z = c;
			double abs = std::abs(z);
			double smooth = std::exp(-abs);
			int it = 0;
			while(abs <= 4 && it++<maxIt){
				z = std::pow(z,power) + c;
				abs = std::abs(z);
				smooth += std::exp(-abs);
			}
			if(it<=maxIt){
				double c = smooth/maxIt;
				double p = maxIt / (maxIt+1.0);
				p = 1;
				c = std::pow(c,1.0/(1+p));
				c = std::modf(smooth,&p);
				assignColor(img,id,Color::HSV2RGB(c,1,1));
				//assignColor(img,id,glm::vec3(c,c,c));
			}else{
				assignColor(img,id,glm::vec3(0,0,0));
			}
		}
	}
}

int main(int argc, char **argv){
	if(argc>=2) width = atoi(argv[1]);
	if(argc>=3) height = atoi(argv[2]);
	if(argc>=4) maxIt = atoi(argv[3]);
	if(argc>=5) power = atof(argv[4]);

	IM_Pointer img = imImageCreate(width,height,IM_RGB,IM_BYTE);
	createSimple(img.get());

	std::stringstream ss;
	ss << Date::nowForFilenames() << ".jpg";

	IM_SaveImage(img.get(),ss.str().c_str(),"JPEG");

	return 0;
}