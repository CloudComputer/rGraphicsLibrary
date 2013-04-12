#ifndef _XMLOBJECTHANDLER_H_
#define _XMLOBJECTHANDLER_H_

#include "XMLObject.h"

#include "Factory.h"

#include <vector>
#include <map>

class XMLObjectHandler{
	XMLObjectHandler();
	virtual ~XMLObjectHandler();

	static XMLObjectHandler *instance;

	std::map<std::string,Factory*> _factories;

	void init();
	

public:
	static XMLObjectHandler* Handler();
	void registerFactory(std::string name,Factory *f);

	
	void save(XMLObject* obj,std::string filename);
	void save(std::vector<XMLObject*> &objects,std::string filename);

	void read(std::vector<XMLObject*> &objects,std::string filename);

};

#endif