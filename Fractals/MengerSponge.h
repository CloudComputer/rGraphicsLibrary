#ifndef _MENGERSPONGE_H_
#define _MENGERSPONGE_H_

#include "Geometry\BoundingGeometry\BoundingVolume.h"

#include <Geometry\Mesh\IndexedMesh.h>

#include <vector>

class MengerSponge{
	std::vector<BoundingAABB> _boxes;
	bool _inverted;
	bool _variateInversion;
public:
	MengerSponge(bool inverted = false,int subdivisions = 0,bool variateInversion=false);
	virtual ~MengerSponge();
	
	void subdivide(int amount = 1);
	
	void toMesh(IndexedMesh *mesh);

};

#endif