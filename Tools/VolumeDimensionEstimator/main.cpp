
#include <string>
#include <sstream>
#include <iostream>

#include "Base\Object.h"
#include "Base\Assert.h"

struct VolSize : Object{
	int w,h,d,bps,size;

	VolSize():w(-1),h(-1),d(-1),bps(-1),size(-1){}

	std::string toString()const{
		return "VolSizeObject";
	}
};

int main(int argc, char ** argv){
	ASSERT_ONCE();
	VolSize o = VolSize();
	int data = 0;
	for(int i = 1;i<argc;i+=2){
		if(argv[i][1] == 'w'){
			o.w = atoi(argv[i+1]);
			data++;ASSERT_ONCE();
		}
		else if(argv[i][1] == 'h'){
			o.h = atoi(argv[i+1]);
			data++;ASSERT_ONCE();
		}
		else if(argv[i][1] == 'd'){
			o.d = atoi(argv[i+1]);
			data++;ASSERT_ONCE();
		}
		else if(argv[i][1] == 'b'){
			o.bps = atoi(argv[i+1]);
			data++;ASSERT_ONCE();
		}
		else if(argv[i][1] == 's'){
			o.size = atoi(argv[i+1]);
			data++;ASSERT_ONCE();
		}
	}

	if(o.size != -1){
		unsigned int minBPS = 1;
		unsigned int maxBPS = 4;
		unsigned int minW = 1;
		unsigned int maxW = o.size;
		unsigned int minH = 1;
		unsigned int maxH = o.size;
		unsigned int minD = 1;
		unsigned int maxD = o.size;
		if(o.bps != -1)
			minBPS = maxBPS = o.bps;
		if(o.w != -1)
			minW = maxW = o.w;
		if(o.h != -1)
			minH = maxH = o.h;
		if(o.d != -1)
			minD = maxD = o.d;
			
		for(unsigned int bps = minBPS ; bps <= maxBPS; bps++)
			for(unsigned int w = minW;w<=maxW;w++)
				for(unsigned int h = minH;h<=maxH;h++)
					if(o.size % (bps*w*h) == 0){
						int d = o.size / (bps*w*h);
						std::cout << w << "x" << h << "x" << d << " : BPS = " << bps << std::endl;
					}
							
	}
	


	return 0;
}