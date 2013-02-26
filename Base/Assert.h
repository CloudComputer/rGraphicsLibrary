#ifndef _ASSERT_H_
#define _ASSERT_H_

#include <iostream>
#include <string>

#define ASSERT_RETURN(msg,ret) std::cerr << "Assertion return message: "<< msg << "in function \"" << __FUNCTION__ << "\" at" << __FILE__ <<":"<<__LINE__ << std::endl; return ret;


bool __hasBeenRuned(std::string file,int line);

#ifdef _DEBUG
#define ASSERT_ONCE() if(__hasBeenRuned(__FILE__,__LINE__)) {  std::cerr << "Code at " << __FILE__ << ":" << __LINE__ << " has been runned more than one time, it is limmited to only once" << std::endl; exit(1);  }
#else
#define ASSERT_ONCE()
#endif



#endif
