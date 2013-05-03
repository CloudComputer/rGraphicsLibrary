#ifndef _HTML_H_
#define _HTML_H_

#include <string>
#include <sstream>

class HTML{
	std::stringstream head;
	std::stringstream body; 
public:
	HTML();
	virtual ~HTML();
	
	void appendToHead(std::string code);
	void appendToBody(std::string code);


	void save(const char *filename)const;

	
};

#endif