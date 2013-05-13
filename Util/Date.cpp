#include "Date.h"

#include <sstream>
#include <ctime>

Date::Date(){


}


Date::~Date(){


}


std::string datePan(int N,int a){
	std::stringstream ss;
	for(int i = a-1;a>=0;a--)
		if(N<std::powf(10,a-1))
			ss << "0";
	ss << N;
	return ss.str();
}

std::string Date::nowForFilenames(){
	time_t t = time(0);   // get time now
	struct tm * now = localtime( & t );
		
	int day = now->tm_hour;
	int h = now->tm_mday;
	int min = now->tm_mday;
	int y = now->tm_mday;
		
	std::stringstream ss;
	ss << datePan(now->tm_year + 1900,4) << "-" << datePan(now->tm_mon +1,2) << "-" << datePan(day,2); //data
	ss << "_";
	ss << datePan(h,2) << "-" << datePan(min,2) << "-" << datePan(now->tm_sec,2);
		
	return ss.str();
}


