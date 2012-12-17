#include "OpenEXRIncludes.h"

std::vector<Light> LightExtractor::ExtractLights(RGBABuffer buffer,int numSplits,bool directionalLights,float distance){
	std::vector<Light> lights;
	GreyBuffer greyBuffer = GreyBuffer::Convert(buffer,weight);
	
	std::vector<Region> regions = ExtractLightRegions(greyBuffer,numSplits);
	std::vector<glm::vec2> uvs =  ExtractLightLocations(greyBuffer,regions);
	std::vector<glm::vec2>::iterator uv = uvs.begin();

	int i = -1;
	for(;uv!=uvs.end();++uv){
		float phi = 2*M_PI*(uv->x - 0.5);
		float theta = M_PI * uv->y;
		Light light(++i);
		if(directionalLights){
			light.setDirection(glm::vec3(sin(phi)*sin(theta), cos(theta), cos(phi)*sin(theta)));
		}else{
			light.setPosition(-glm::vec3(distance) * glm::vec3(sin(phi)*sin(theta), -cos(theta), -cos(phi)*sin(theta)));
		}
		float r,g,b;
		r = g = b = 0;
		for(int u = regions[i].minX;u<=regions[i].maxX;u++)for(int v = regions[i].minY;v<=regions[i].maxY;v++){
			r += buffer.sample(u,v,LatLong).r;
			g += buffer.sample(u,v,LatLong).g;
			b += buffer.sample(u,v,LatLong).b;		
		}
		//size *= uvs.size();
		light.setDiffuseColor(glm::vec4(r,g,b,1));
		light.setSpecularColor(glm::vec4(r,g,b,1));
		light.setAmbientColor(glm::vec4(0,0,0,1));


		lights.push_back(light);
	}



	return lights;
}

std::vector<glm::vec2> LightExtractor::ExtractLightLocations(RGBABuffer buffer,int numSplits){
	return ExtractLightLocations(GreyBuffer::Convert(buffer,weight),numSplits);
}

std::vector<glm::vec2> LightExtractor::ExtractLightLocations(GreyBuffer buffer,int numSplits){
	std::vector<Region> regions = ExtractLightRegions(buffer,numSplits);
	return ExtractLightLocations(buffer,regions);
}

std::vector<glm::vec2> LightExtractor::ExtractLightLocations(GreyBuffer buffer,std::vector<Region> regions){	
	std::vector<glm::vec2> lights;
	std::vector<Region>::iterator region;
	for(region = regions.begin();region != regions.end();++region){
		glm::vec2 uv;
		uv.x = 0;
		uv.y = 0;
		float val = 0;
		for(int u = region->minX;u<=region->maxX;u++)for(int v = region->minY;v<=region->maxY;v++){
			float s = buffer.sample(u,v,LatLong);
			if(s>val){
				uv.x = float(u)/buffer.dim.x;
				uv.y = float(v)/buffer.dim.y;
				val = s;
			}
		}

		lights.push_back(uv);
	}


	return lights;
}

std::vector<LightExtractor::Region> LightExtractor::ExtractLightRegions(GreyBuffer buffer,int numSplits){
	//Build first region
	Region r;
	r.minX = 0;
	r.minY = 0;
	r.maxX = buffer.dim.x-1;
	r.maxY = buffer.dim.y-1;
	r.flux = 0;
	int adam = 0;
	for(int v = 0;v<buffer.dim.y;v++)for(int u = 0;u<buffer.dim.x;u++){
		r.flux += buffer.sample(u,v,LatLong);
	}

	std::vector<Region> regions;
	regions.push_back(r);
	for(int i = 0;i<numSplits;i++){
		regions = __splitRegions(regions,buffer);
	}

	return regions;
}

std::vector<LightExtractor::Region> LightExtractor::__splitRegions(std::vector<LightExtractor::Region> oldRegions,GreyBuffer buffer){
	std::vector<Region> newRegions;
	std::vector<Region>::iterator region;
	for(region = oldRegions.begin();region != oldRegions.end();++region){
		Region r0,r1;
		if((region->maxX - region->minX)<=4 || (region->maxY - region->minY)<=4){
			newRegions.push_back(*region);
			continue;
		}
		__split((*region),r0,r1,buffer);
		newRegions.push_back(r0);
		newRegions.push_back(r1);
	}


	return newRegions;
}

void LightExtractor::__split(Region R,Region &r0,Region &r1,GreyBuffer buffer){
	bool vert = (R.maxX - R.minX) >= (R.maxY - R.minY);
	r1.flux = R.flux;
	r0.flux = 0;
	
	unsigned int size;
	if(vert){
		size = R.maxY - R.minY;
	}else{	
		size = R.maxX - R.minX;
	}

	int iteration = -1;
	while(true){
		iteration++;

		double flux = 0;
		for(unsigned int i = 0;i<=size;i++){
			if(vert){
				flux += buffer.sample(R.minX + iteration,R.minY + i,LatLong);
			}else{
				flux += buffer.sample(R.minX+i,R.minY + iteration,LatLong);
			}
		}
		r1.flux -= flux;
		if(abs(r0.flux - r1.flux) <= flux){
			if(vert){
					r0.minX = R.minX;
					r0.maxX = R.minX + iteration - 1;
					r1.minX = R.minX + iteration + 1;
					r1.maxX = R.maxX;
					
					r0.minY = r1.minY = R.minY;
					r0.maxY = r1.maxY = R.maxY;

				}else{
					r0.minY = R.minY;
					r0.maxY = R.minY + iteration - 1;
					r1.minY = R.minY + iteration + 1;
					r1.maxY = R.maxY;
					
					r0.minX = r1.minX = R.minX;
					r0.maxX = r1.maxX = R.maxX;
				}
			
			
			if(r0.flux < r1.flux){
				r0.flux += flux;
				if(vert){
					r0.maxX++;	
				}else{
					r0.maxY++;
				}
			}else{
				r1.flux += flux;
				if(vert){
					r1.minX--;	
				}else{
					r1.minY--;
				}
			}
			break;
		}
		r0.flux += flux;
	}

}