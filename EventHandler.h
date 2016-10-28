/*
 * EventHandler.h
 *
 *  Created on: 25 Oct 2016
 *      Author: rag
 */

#ifndef EVENTS_EVENTHANDLER_H_
#define EVENTS_EVENTHANDLER_H_

#include "Events.h"

class EventHandler {
public:
	virtual void onEvent(Event) = 0;
	virtual ~EventHandler() {};
};



#endif /* EVENTS_EVENTHANDLER_H_ */
