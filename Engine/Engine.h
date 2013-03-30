#ifndef _ENGINE_H_
#define _ENGINE_H_

#include <Base\Object.h>
#include <Base\Defines.h>

class Engine : public Object{
public:
	Engine();
	virtual ~Engine();

	virtual bool init() = 0;
	virtual void loop() = 0;
};


#ifndef ENGINE_MAIN
#define ENGINE_MAIN(ENGINE,init) int main(int argv,char &argc){\
	Engine *t = ENGINE;	\
	if(!t->init()){std::cerr << "Could not init engine" << std::endl;return RGL_EXIT_CODE_ENGINE_INIT_FAILURE;}\
	init(t);\
	t->loop();\
	return RGL_EXIT_CODE_OK;\
}
#endif

#endif