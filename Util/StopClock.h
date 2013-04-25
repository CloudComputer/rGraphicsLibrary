#ifndef _STOPCLOCK_H_
#define _STOPCLOCK_H_

#include <time.h>

#include <string>
#include <iostream>

class StopClock{
	clock_t _t;
	clock_t _stopedAt;
	bool _running;
public:
	StopClock(bool start = false);

	void start();
	void stop();
	void restart();
	void reset();

	int getElapsedMiliSeconds();
	int getElapsedSeconds();
	double getFractionElapsedSeconds();
	int getElapsedMinutes();
	double getFractionElapsedMinutes();
	int getElapsedHouse();
	double getFractionElapsedHouse();

};

class ScopeClock{
	StopClock sw;
	std::string pre,post;
public:
	ScopeClock(std::string preMsg,std::string postMsg = std::string("")){
		pre = preMsg;
		post = postMsg;
		sw.restart();
	}

	virtual ~ScopeClock(){
		sw.stop();
		std::cout << pre << " " << sw.getFractionElapsedSeconds() << " sec " << post << std::endl;
	}
};

#endif