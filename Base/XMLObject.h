#ifndef _XMLOBJECT_H_
#define _XMLOBJECT_H_

#include <tinyxml2\tinyxml2.h>

#include "Object.h"


//template <class T> 
class XMLObject : public Object{
public:
	XMLObject(){}
	virtual ~XMLObject(){}

	virtual void save(tinyxml2::XMLNode *parent) = 0;


};

#ifndef _XMLOBJECT_HPP_
#include "XMLObject.hpp"
#endif


#endif