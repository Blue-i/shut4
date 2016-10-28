/*
 * EventManager.h
 *
 *  Created on: 25 Oct 2016
 *      Author: rag
 */

#ifndef EVENTMANAGER_H_
#define EVENTMANAGER_H_

#include "EventHandler.h"
#include "Events.h"
#include "os48.h"

class EventManager {

	static const uint8_t MAX_HANDLERS = 4;
	static const uint8_t MAX_EVENTS = 16;
	EventHandler * handlers[MAX_HANDLERS];
	Event events[MAX_EVENTS];
	Event * head;
	Event * tail;
	uint8_t numHandlers;

	os48::Mutex mutex;
	EventManager();
	EventManager(const EventManager &);
	EventManager& operator=(const EventManager &);
public:
	~EventManager();
	void queueEvent(Event);
	void addHandler(EventHandler*);
	void run();
	static EventManager * instance();
	volatile bool externalEventUpdate;
	volatile Event externalEvent;
};

#endif /* EVENTMANAGER_H_ */
