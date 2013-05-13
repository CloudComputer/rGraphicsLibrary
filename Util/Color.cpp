#include "Color.h"



Color::Color(){


}


Color::~Color(){


}
#include <iostream>
float fMod(float a,float b){
	float tmp;
	float d = a/b;
	float c = std::modff(a/b,&tmp);
	//std::cout << a << " " << b << " " << d << " " << c << " " << tmp << std::endl;
	return c;
}

//all values between 0...1
glm::vec3 Color::HSV2RGB(float h,float s,float v){
	/*float _h = h * 6.0f;
	
	float c = v*s;
	float m = v-c;
	float x = 1 - std::abs(fMod(_h,2.0f) - 1);
	x *= c;

	glm::vec3 rgb;
	if(h<0) return glm::vec3(0,0,0);
	else if(_h < 1) rgb = glm::vec3(c,x,0);
	else if(_h < 2) rgb = glm::vec3(x,c,0);
	else if(_h < 3) rgb = glm::vec3(0,c,x);
	else if(_h < 4) rgb = glm::vec3(0,x,c);
	else if(_h < 5) rgb = glm::vec3(x,0,c);
	else if(_h < 6) rgb = glm::vec3(c,0,x);
	return rgb+m;*/

	double r, g, b;

    int i = std::floor(h * 6);
    double f = h * 6 - i;
    double p = v * (1 - s);
    double q = v * (1 - f * s);
    double t = v * (1 - (1 - f) * s);

    switch(i % 6){
        case 0: r = v, g = t, b = p; break;
        case 1: r = q, g = v, b = p; break;
        case 2: r = p, g = v, b = t; break;
        case 3: r = p, g = q, b = v; break;
        case 4: r = t, g = p, b = v; break;
        case 5: r = v, g = p, b = q; break;
    }

	return glm::vec3(r,g,b);
}

