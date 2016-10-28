/*
 * EventManager.h
 *
 *  Created on: 25 Oct 2016
 *      Author: rag
 */

#ifndef EVENTMANAGER_H_
#define EVENTMANAGER_H_

#include "QueueArray.h"
#include "EventHandler.h"
#include "Events.h"
#include "os48.h"

class EventManager {

	static const int MAX_HANDLERS = 64;
	QueueArray<Event> events;
	EventHandler * handlers[MAX_HANDLERS];
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
