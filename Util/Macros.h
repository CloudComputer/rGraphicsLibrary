#ifndef _MACROS_H_
#define _MACROS_H_


#define IT_FOR(vec,i) for(auto i = vec.begin();i!=vec.end();++i)

#define FOR_XML_ELE(name,xml,child) for(auto child = xml->FirstChildElement(name); child ; child = child->NextSiblingElement(name))

#endif