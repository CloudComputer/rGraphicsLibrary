#include "UltrasoundSurfacePointExtractor.h"


#include <math.h>

#include <algorithm>

#include <Math\Interpolator.h>

#include <Util\TmpPointer.h>


struct UVSample{
	float v;
	glm::vec2 p;
	bool operator<(const UVSample &s)const{
		return v < s.v;
	}
};

std::vector<glm::vec3> UltrasoundSurfacePointExtractor::ExtractPoints(ScalarField *vol,int axis){
	std::vector<glm::vec3> points;

	int slices = 0;
	glm::ivec2 res;
	if(axis==X_AXIS){
		res.x = vol->getDimensions().y;
		res.y = vol->getDimensions().z;
		
		slices = vol->getDimensions().x;
	}else if(axis==Y_AXIS){
		res.x = vol->getDimensions().x;
		res.y = vol->getDimensions().z;

		slices = vol->getDimensions().y;
	}else if(axis==Z_AXIS){
		res.x = vol->getDimensions().x;
		res.y = vol->getDimensions().y;

		slices = vol->getDimensions().z;
	}
	std::vector<imImage*> sliceImgs;

	std::vector<glm::vec2> candidates;

	for(int i = 0;i<slices;i++){
		sliceImgs.push_back(getSlice(vol,axis,i));

#ifndef US_DEBUG
		auto lines = IM_GetLines(sliceImgs[i],2);
#else
		IM_Pointer out = imImageCreate(res.x, res.y, IM_RGB, IM_BYTE);
		IM_Pointer binOut = imImageCreate(res.x, res.y, IM_BINARY, IM_BYTE);
		IM_Pointer bin2Out = imImageCreate(res.x, res.y, IM_BINARY, IM_BYTE);
		auto lines = IM_GetLines(sliceImgs[i],2,out.get(),binOut.get(),bin2Out.get());
		//*
		{
			std::stringstream ss;
			ss << "slice_"<<i<<".jpg";
			IM_SaveImage(out.get(),ss.str().c_str(),"JPEG");
		}
		{
			std::stringstream ss;
			ss << "binary_"<<i<<".jpg";
			IM_SaveImage(binOut.get(),ss.str().c_str(),"JPEG");
		}
		{
			std::stringstream ss;
			ss << "binary2_"<<i<<".jpg";
			IM_SaveImage(bin2Out.get(),ss.str().c_str(),"JPEG");
		}//*/
#endif	
		if(lines.size()!=2){
			std::cout << i  << std::endl;
		}

	

		float k[2],m[2];
		glm::vec2 focal;
		lines[0].ykxm(k[0],m[0],res.x,res.y);
		lines[1].ykxm(k[1],m[1],res.x,res.y);

		focal.x = (m[1]-m[0])/(k[0]-k[1]);
		focal.y = m[0] + k[0] * focal.x;

		if(focal.x >= 0 && focal.x <= res.x && focal.y <= 0 )
			candidates.push_back(focal);
		//std::cout << focal.x << " " << focal.y << std::endl;

		getPointsFromSlice(vol,axis,i,sliceImgs[i],lines[0],lines[1],points);
		std::cout << "ASDFe" << std::endl;

		#ifdef US_DEBUG //do this for every found point
		std::cout << "Slice done: " << i << std::endl;
		#endif
	}
	auto it = std::unique(candidates.begin(),candidates.end());
	candidates.erase(it,candidates.end());
	glm::vec2 _min = candidates[0],_max = candidates[0],_mean(0,0);
	for(auto p = candidates.begin(); p != candidates.end(); ++p){
		_min.x = std::min(_min.x , p->x);
		_min.y = std::min(_min.y , p->y);
		_max.x = std::max(_max.x , p->x);
		_max.y = std::max(_max.y , p->y);
		_mean.x += p->x;
		_mean.y += p->y;
		std::cout << p->x << " " << p->y << std::endl;
	}
	_mean /= candidates.size();
	auto aaaaa = _mean - _min;
	auto aaaaaa = _mean - _max;
	std::cout << candidates.size() << std::endl;
	/*for(int i = 0;i<slices;i++){
		getPointsFromSlice(vol,axis,i,sliceImgs[i],lines[0],lines[1],points);
	}*/

	return points;
}

imImage* UltrasoundSurfacePointExtractor::getSlice(ScalarField *vol,int axis,int slice){
	glm::ivec2 res;
	if(axis==X_AXIS){
		res.x = vol->getDimensions().y;
		res.y = vol->getDimensions().z;
	}else if(axis==Y_AXIS){
		res.x = vol->getDimensions().x;
		res.y = vol->getDimensions().z;
	}else if(axis==Z_AXIS){
		res.x = vol->getDimensions().x;
		res.y = vol->getDimensions().y;
	}

	imImage* img = imImageCreate(res.x, res.y, IM_GRAY, IM_BYTE);
	auto data = (imbyte*)img->data[0];

	for(int x = 0; x < res.x ; x++)for(int y = 0; y < res.y ; y++) {
		float v;
		if(axis==X_AXIS){
			v = vol->get(glm::ivec3(slice,x,y));
		}else if(axis==Y_AXIS){
			v = vol->get(glm::ivec3(x,slice,y));
		}else if(axis==Z_AXIS){
			v = vol->get(glm::ivec3(x,y,slice));
		}
		int id = x + y*res.x;
		data[id] = v * 255;
	}

	return img;
}


void UltrasoundSurfacePointExtractor::getPointsFromSlice(ScalarField *vol,int axis,int slice,imImage* img,HoughPoint &l0,HoughPoint &l1,std::vector<glm::vec3> &points){
	//assert(!(l0==l1));
	if((l0==l1))
		return;
#ifdef US_DEBUG
	IM_Pointer dbgOut = imImageCreate(img->width,img->height, IM_RGB, IM_BYTE);
	imProcessConvertColorSpace(img,dbgOut.get());
#endif	
	IM_Pointer blured = imImageCreate(img->width,img->height, IM_GRAY, IM_BYTE);
	//imProcessMeanConvolve(img,blured.get(),3);
	imProcessGrayMorphOpen(img,blured.get(),3);

	//Extract points from slice using raytracing between l0 and l1, see matlab code
	float k[2],m[2];
	glm::vec2 focal;
	l0.ykxm(k[0],m[0],img->width,img->height);
	l1.ykxm(k[1],m[1],img->width,img->height);

	focal.x = (m[1]-m[0])/(k[0]-k[1]);
	focal.y = m[0] + k[0] * focal.x;

	std::cout << focal.x << " " << focal.y << std::endl;
	if(!(focal.x == focal.x))
		return;

	glm::vec2 entry[2],exit[2],tmp[2],dir[3];
	float dist[2],length[2];

	//find entry/exit
	for(int i = 0;i<2;i++){
		int found = 0;
		if(m[i] >= 0 && m[i] <= img->height){
			tmp[found].x = 0;
			tmp[found].y = m[i];
			found++;
		}
		float a =  m[i] + img->width * k[i];
		if(a >= 0 && a <= img->height){
			tmp[found].x = img->width;
			tmp[found].y = a;
			found++;
		}
		a = -m[i]/k[i];
		if(found<2 && a >= 0 && a <= img->width){
			tmp[found].x = a;
			tmp[found].y = 0;
			found++;
		}
		a = (img->height-m[i])/k[i];
		if(found<2 && a >= 0 && a <= img->width){
			tmp[found].x = a;
			tmp[found].y = img->height;
			found++;
		}
		assert(found==2);
		float d0,d1;
		d0 = glm::distance(focal,tmp[0]);
		d1 = glm::distance(focal,tmp[1]);
		if(d0<d1){
			entry[i] = tmp[0];
			exit[i] = tmp[1];
		}else{
			entry[i] = tmp[1];
			exit[i] = tmp[0];			
		}
		dir[i] = entry[i]-focal;
		dist[i] = glm::length(dir[i]);
		dir[i] /= dist[i]; //normalize
		length[i] = glm::distance(exit[i],entry[i]);
	}
	

	
	float prep = dir[0].x * dir[1].y - dir[0].y * dir[1].x;
	float angle = std::atan2f(prep,glm::dot(dir[0],dir[1]));
	
	
	glm::vec3 dim = glm::vec3(vol->getDimensions());
	int numSamples = 300;
	int res = 100; 


	for(int i = 0;i<numSamples;i++){
		float t = i/(numSamples-1.0);
		glm::mat2 m;
		m[0][0] = std::cos(angle*t);
		m[1][1] = std::cos(angle*t);
		m[1][0] = -std::sin(angle*t);
		m[0][1] = std::sin(angle*t);
		dir[2] = m*dir[0];
		float d = dist[1] * t  + dist[0] * (1-t);
		float l = length[1] * t  + length[0] * (1-t);
		
		float threshold = 0.2;

		std::vector<UVSample> samples;
		auto data = (imbyte*)blured->data[0];
		bool beenInsde = false;
		for(int j = 0;j<res;j++){
			float t2 = j/(res-1.0);
			UVSample s;
			s.p = focal +  dir[2] * (d+t2*l);
			if(s.p.x < 0 || s.p.y < 0 || s.p.x >= img->width-1 || s.p.y >= img->height-1) //we are outisde the volume/image. 
				if(beenInsde)
					break;  //we have existed the volume, break and start next ray
				else
					continue; //we have not yet entred the volume, continue on this ray
			beenInsde = true;
			glm::vec2 dd;
			glm::vec2 di;
			dd.x = std::modf(s.p.x,&di.x);
			dd.y = std::modf(s.p.y,&di.y);
			imbyte v[4];
			int id = di.x + di.y * img->width;
			v[0] = data[id];
			v[1] = data[id+1];
			v[2] = data[id+img->width];
			v[3] = data[id+img->width+1];
			s.v = std::sqrt(BiLinearInterpolator<imbyte>::interpolate(v[0],v[1],v[2],v[3],dd)/255.0);
			//if(s.v>10)
				samples.push_back(s);
		}
		if(samples.size() == 0)
			continue;
		float prev=10,diff;
		std::vector<UVSample> diffs;
		for(int j = 2;j<samples.size()-1;j++){
			diff = samples[j+1].v - samples[j-1].v;
			//diff = samples[j].v / prev;
			//diff = prev / samples[j].v;
			prev = samples[j].v;

			if(samples[j+1].v == 0 || samples[j-1].v == 0 || samples[j].v == 0)
				continue;

			if(diff >= 0.2){
				diffs.clear();
				UVSample s;
				s.p = samples[j].p;
				s.v = diff;
				diffs.push_back(s);
				break;
			}else{
				//continue;
			}

			if(j < 5)
				continue;//*/

			//diff /= samples[j].v;
			UVSample s;
			s.p = samples[j].p;
			s.v = diff;
			diffs.push_back(s);

		}

		/*float scale =  0.4;
		int kernel = 6;
		diffs.clear();
		for(int j = kernel;j<samples.size();j++){
				float d = 0;
				float s = samples[j].v;
				for(int k = j-kernel;k<j;k++){
					d += samples[k].v;
				}
				d /= kernel;
				UVSample ss;
				ss.p = samples[j].p;

				float ts = 1 - float(j)/samples.size();
				ts = std::powf(ts,scale);

				ss.v = (s / d) *ts;
				diffs.push_back(ss);
		}*/

		if(diffs.size()==0)
			continue;
		std::sort(diffs.rbegin(),diffs.rend());
		std::sort(samples.rbegin(),samples.rend());

		glm::vec3 p(diffs[0].p.x,diffs[0].p.y,slice);
		p = p / dim;
		auto v = vol->getBoundingAABB().getPosition(p);
		points.push_back(v);
		#ifdef US_DEBUG //do this for every found pointglm::vec2 dd;
		glm::vec2 di;
		glm::vec2 dd;
		dd.x = std::modf(samples[0].p.x,&di.x);
		dd.y = std::modf(samples[0].p.y,&di.y);
		
		if(di.x == 0 || di.y == 0 || di.x >= img->width-1 || di.y >= img->height-1)
			continue;
		int id = di.x + di.y * img->width;
		int a,b,c;
		a = 0;
		b = 1;
		c = 2;
		
		((imbyte*)dbgOut->data[a])[id] = 255;
		((imbyte*)dbgOut->data[a])[id+1] = 255;
		((imbyte*)dbgOut->data[a])[id-1] = 255;
		((imbyte*)dbgOut->data[a])[id+img->width] = 255;
		((imbyte*)dbgOut->data[a])[id-img->width] = 255;
		((imbyte*)dbgOut->data[a])[id+img->width+1] = 255;
		((imbyte*)dbgOut->data[a])[id-img->width+1] = 255;
		((imbyte*)dbgOut->data[a])[id+img->width-1] = 255;
		((imbyte*)dbgOut->data[a])[id-img->width-1] = 255;
		
		((imbyte*)dbgOut->data[b])[id] = 1;
		((imbyte*)dbgOut->data[b])[id+1] = 1;
		((imbyte*)dbgOut->data[b])[id-1] = 1;
		((imbyte*)dbgOut->data[b])[id+img->width] = 1;
		((imbyte*)dbgOut->data[b])[id-img->width] = 1;
		((imbyte*)dbgOut->data[b])[id+img->width+1] = 1;
		((imbyte*)dbgOut->data[b])[id-img->width+1] = 1;
		((imbyte*)dbgOut->data[b])[id+img->width-1] = 1;
		((imbyte*)dbgOut->data[b])[id-img->width-1] = 1;
		
		((imbyte*)dbgOut->data[c])[id] = 1;
		((imbyte*)dbgOut->data[c])[id+1] = 1;
		((imbyte*)dbgOut->data[c])[id-1] = 1;
		((imbyte*)dbgOut->data[c])[id+img->width] = 1;
		((imbyte*)dbgOut->data[c])[id-img->width] = 1;
		((imbyte*)dbgOut->data[c])[id+img->width+1] = 1;
		((imbyte*)dbgOut->data[c])[id-img->width+1] = 1;
		((imbyte*)dbgOut->data[c])[id+img->width-1] = 1;
		((imbyte*)dbgOut->data[c])[id-img->width-1] = 1;
		#endif
	}




	#ifdef US_DEBUG //do this for every found point
	//int id = samples[j].p.x + samples[j].p.y * img->width;
	//((imbyte*)dbgOut->data[1])[id] = 255;
	#endif

	//glm::vec3 dim = glm::vec3(vol->getDimensions());
	//*
	auto v = vol->getBoundingAABB().getPosition(glm::vec3(focal.x,focal.y,slice)/dim);
	points.push_back(v);
	
	 v = vol->getBoundingAABB().getPosition(glm::vec3(entry[0].x,entry[0].y,slice)/dim);
	points.push_back(v);
	 v = vol->getBoundingAABB().getPosition(glm::vec3(entry[1].x,entry[1].y,slice)/dim);
	points.push_back(v);
	
	 v = vol->getBoundingAABB().getPosition(glm::vec3(exit[0].x,exit[0].y,slice)/dim);
	points.push_back(v);
	 v = vol->getBoundingAABB().getPosition(glm::vec3(exit[1].x,exit[1].y,slice)/dim);
	points.push_back(v);//*/

#ifdef US_DEBUG
	std::stringstream ss;
	ss << "points_"<<slice<<".jpg";
	IM_SaveImage(dbgOut.get(),ss.str().c_str(),"JPEG");
#endif
}