#ifndef __TIMER_CPP_
#define __TIMER_CPP_

#include "Timer.h"

Timer::Timer() {
	timerWasStarted = false;
}

void Timer::start() throw (std::runtime_error) {
	if (timerWasStarted)
		//throw std::runtime_error("start() while timer has already started");
		std::cout << "start() while timer has already started" << std::endl;
	else {
		timerWasStarted = true;
		gettimeofday(&beginTime, NULL);
	}
}

void Timer::stop() throw (std::logic_error) {
	if (!timerWasStarted)
		//throw std::logic_error("stop() before timer was started");
		std::cout << "stop() before timer was started" << std::endl;
	else {
		timerWasStarted = false;
		gettimeofday(&duration, nullptr);
	}
}

double Timer::getElapsedTime() const throw (std::logic_error) {
	if (timerWasStarted)
		throw std::logic_error("getElapsedTime() while timer wasn't started");
	else if (duration.tv_usec <= 0)
		throw std::logic_error("getElapsedTime() while timer not started");
	else {
		double t1, t2;
		t1 = beginTime.tv_sec + (beginTime.tv_usec/1000000.0);
		t2 = duration.tv_sec + (duration.tv_usec/1000000.0);
		return t2-t1;
	}
}

#endif
