#ifndef _FACTORY_H_
#define _FACTORY_H_

#include "XMLObject.h"


class Factory : public Object{
protected:
	Factory(std::string name);
public:
	virtual ~Factory();
	
	virtual XMLObject *create(tinyxml2::XMLElement *xmlelement) = 0;

};

#endif