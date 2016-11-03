/*
 * Timer.cpp
 *
 *  Created on: 31 Oct 2016
 *      Author: rag
 */

#include <Timer.h>

Timer::Timer(unsigned long length) : timeCreated(0), length(length){
	timeCreated = millis();
}

Timer::Timer(unsigned long length, unsigned long timeCreated)  : timeCreated(0), length(length) {
}

bool Timer::completed() {
	return millis() - timeCreated > length;
}

void Timer::reset() {
	timeCreated = millis();
}

Timer::~Timer() {
	// TODO Auto-generated destructor stub
}

void Timer::expire() {
	timeCreated = millis() - length;
}
