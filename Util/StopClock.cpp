#include "StopClock.h"

#include <iostream>

StopClock::StopClock(bool start){
	_running = false;
	if(start)
		this->start();
}

void StopClock::start(){
	if(!_running)
		_t = clock();
	_running = true;
}

void StopClock::stop(){
	_stopedAt = clock();
	_running = false;
}

void StopClock::restart(){
	_t = clock();
	_running = true;
}

void StopClock::reset(){
	_t = clock();
	_stopedAt = clock();
	_running = false;
}

int StopClock::getElapsedMiliSeconds(){
	if(_running){
		_stopedAt = clock();
	}
	return (_stopedAt - _t) / (CLOCKS_PER_SEC / 1000.0);
}

int StopClock::getElapsedSeconds(){
	if(_running){
		_stopedAt = clock();
	}
	return (_stopedAt - _t) / (float)CLOCKS_PER_SEC;
}

double StopClock::getFractionElapsedSeconds(){
	if(_running){
		_stopedAt = clock();
	}
	return (_stopedAt - _t) / (double)CLOCKS_PER_SEC;
}
int StopClock::getElapsedMinutes(){
	if(_running){
		_stopedAt = clock();
	}
	return getFractionElapsedSeconds() / 60;
}
double StopClock::getFractionElapsedMinutes(){
	if(_running){
		_stopedAt = clock();
	}
	return getFractionElapsedSeconds() / 60;
}
int StopClock::getElapsedHouse(){
	if(_running){
		_stopedAt = clock();
	}
	return getFractionElapsedSeconds() / 3600;
}
double StopClock::getFractionElapsedHouse(){
	if(_running){
		_stopedAt = clock();
	}
	return getFractionElapsedSeconds() / 3600;
}