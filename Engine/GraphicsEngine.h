#ifndef _GRAPHICSENGINE_H_
#define _GRAPHICSENGINE_H_

#include "EventHandler.h"


class GraphicsEngine : public Object{
public:
	GraphicsEngine(){}
	virtual ~GraphicsEngine(){}

	virtual void init() = 0;

	virtual void draw() = 0;

};

#endif