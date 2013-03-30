#ifndef _DRAWABLEOBJECT_H_
#define _DRAWABLEOBJECT_H_

#include <Base/Object.h>


class DrawableObject : public Object{
public:
	DrawableObject(){}
	virtual ~DrawableObject(){}

	virtual void draw()=0;
};

#endif