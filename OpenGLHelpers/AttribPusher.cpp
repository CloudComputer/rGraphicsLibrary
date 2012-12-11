#include "AttribPusher.h"

AttribPusher::AttribPusher(GLbitfield attrib){
	_attrib = attrib;
	glPushAttrib(_attrib);
}

AttribPusher::~AttribPusher(){
	glPopAttrib();
}