/*
 * Component.h
 *
 *  Created on: 22 Oct 2016
 *      Author: rag
 */
#pragma once

#ifndef COMPONENT_COMPONENT_H_
#define COMPONENT_COMPONENT_H_
#include "Arduino.h"
template<class T>
class Component {
public:
	virtual void bind(T*, char*, size_t) = 0;
	virtual bool isBound() = 0;
	virtual void run() = 0;
	virtual void reset() = 0;
	virtual ~ Component() {};
};

#endif /* COMPONENT_COMPONENT_H_ */
