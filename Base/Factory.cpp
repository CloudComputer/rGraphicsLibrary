#include "Factory.h"

#include "XMLObjectHandler.h"


Factory::Factory(std::string name){
	XMLObjectHandler::Handler()->registerFactory(name,this);
}


Factory::~Factory(){


}


