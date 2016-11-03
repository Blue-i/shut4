/*
 * Device.h
 *
 *  Created on: 31 Oct 2016
 *      Author: rag
 */
#pragma once

#ifndef DEVICE_H_
#define DEVICE_H_

#include "Arduino.h"
#include "Component.h"
#include <stdint.h>

class Device {
public:
	virtual ~Device() {};
	virtual size_t readFor(char * buffer, size_t size, uint16_t timeout) = 0;
	virtual void write(const char buffer[]) = 0;
	virtual void reset() = 0;
};

#endif /* DEVICE_H_ */
