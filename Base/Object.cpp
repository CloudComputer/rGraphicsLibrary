#include "Object.h"


std::ostream& operator<< (std::ostream& stream, const Object& obj){
	return stream << obj.toString();
}

std::ostream& operator<< (std::ostream& stream, const Object* obj){
	return stream << obj->toString();
}

