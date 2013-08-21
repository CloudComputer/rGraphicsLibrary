#ifndef _RENGINE_H_
#define _RENGINE_H_


#include <Base\Object.h>
#include <Base\Defines.h>

#include "rWindow.h"
#include "Scene.h"

#include <Util\StopClock.h>

class rEngine{
	std::vector<Scene*> _scenes;

public:
	rEngine();
	virtual ~rEngine();

	StopClock _sw;

    std::vector<Scene*> &getScenes(){return _scenes;}
	


	void addScene(Scene *s){_scenes.push_back(s);}

	virtual bool init(int argc,char **argv);
	virtual void loop();
};


#ifndef rENGINE_MAIN
#define rENGINE_MAIN(init) int main(int argc,char **argv){\
	rEngine *t = new rEngine();	\
	if(!t->init(argc,argv)){std::cerr << "Could not init engine" << std::endl;return RGL_EXIT_CODE_ENGINE_INIT_FAILURE;}\
	init(t,argc,argv);\
	t->loop();\
	return RGL_EXIT_CODE_OK;\
}
#endif

#endif