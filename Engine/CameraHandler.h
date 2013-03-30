#ifndef _CAMERAHANDLER_H_
#define _CAMERAHANDLER_H_

#include <Base\Object.h>


class CameraHandler : public Object{
public:
	CameraHandler(){}
	virtual ~CameraHandler(){}

	virtual void setCamera()const = 0;

};

#endif