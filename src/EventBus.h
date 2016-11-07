#pragma once

#ifndef EVENTBUS_H_
#define EVENTBUS_H_

#include "Events.h"
#include "EventHandler.h"

class EventBus {
public:
	virtual void queueEvent(Event) = 0;
	virtual void addHandler(EventHandler*) = 0;
	virtual ~EventBus() {};
};

#endif /* EVENTBUS_H_ */
