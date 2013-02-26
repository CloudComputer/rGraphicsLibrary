#include "Assert.h"

#include <map>
#include <sstream>

std::map<std::string,bool> __runned;

bool __hasBeenRuned(std::string file,int line){
	std::stringstream ss;
	ss << file << "@" << line;
	if(__runned.find(ss.str()) != __runned.end()){
		return true;
	}
	__runned[ss.str()] = true;
	return false;
}