#ifndef _STOPCLOCK_H_
#define _STOPCLOCK_H_

#include <time.h>

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

#endif