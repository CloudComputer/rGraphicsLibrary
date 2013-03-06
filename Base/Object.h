#ifndef _OBJECT_H_
#define _OBJECT_H_

#include <string>
#include <sstream>

class Object{

	
public:
	virtual std::string toString() const = 0;

	virtual ~Object(){}


	friend std::ostream& operator<< (std::ostream& stream, const Object& obj);
	friend std::ostream& operator<< (std::ostream& stream, const Object* obj);
};



#endif
