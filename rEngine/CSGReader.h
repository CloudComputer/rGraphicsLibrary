#ifndef _CSGRRADER_H_
#define _CSGRRADER_H_


#include <tinyxml2\tinyxml2.h>

class CSG;
class rObject;

class CSGReader {
protected:
	static CSG* read(tinyxml2::XMLElement *ele);
public:
	static void ReadXML(rObject *&obj,tinyxml2::XMLElement *ele);
};

#endif