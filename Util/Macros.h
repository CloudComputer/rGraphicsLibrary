#ifndef _MACROS_H_
#define _MACROS_H_

#ifndef _NO_DBG_
#define DBG() std::cout << __FUNCTION__ << " in " << __FILE__ << "@" << __LINE__ << std::endl;
#else
#define DBG()
#endif


#define IT_FOR(vec,i) for(auto i = vec.begin();i!=vec.end();++i)

#define FOR_XML_ELE(name,xml,child) for(auto child = xml->FirstChildElement(name); child ; child = child->NextSiblingElement(name))
#define FOR_XML_ALL_ELE(xml,child) for(auto child = xml->FirstChildElement(); child ; child = child->NextSiblingElement())

#endif