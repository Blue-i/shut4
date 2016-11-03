/*
 * Timer.h
 *
 *  Created on: 31 Oct 2016
 *      Author: rag
 */
#pragma once

#ifndef TIMER_H_
#define TIMER_H_

#include "Arduino.h"

class Timer {
	volatile unsigned long timeCreated;
	unsigned long length;
public:
	Timer(unsigned long length);
	Timer(unsigned long length, unsigned long timeCreated);
	bool completed();
	void reset();
	void expire();
	virtual ~Timer();
};

#endif /* TIMER_H_ */
