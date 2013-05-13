#ifndef _DATE_H_
#define _DATE_H_


#include <string>

class Date{
public:
	Date();
	virtual ~Date();


	static std::string nowForFilenames();
};

#endif