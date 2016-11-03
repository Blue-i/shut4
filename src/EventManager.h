/*
 * EventManager.h
 *
 *  Created on: 25 Oct 2016
 *      Author: rag
 */
#pragma once

#ifndef EVENTMANAGER_H_
#define EVENTMANAGER_H_

#include "EventBus.h"
#include "EventHandler.h"
#include "Events.h"
#include "RingBufferFixed.h"
#include "os48.h"
#include <stdint.h>

#define EM_MAX_HANDLERS 6
#define EM_MAX_EVENTS 16

class EventManager : public EventBus {

	EventHandler * handlers[EM_MAX_HANDLERS];
	RingBufferF<Event,EM_MAX_EVENTS> events;
	uint8_t numHandlers;

	os48::Mutex mutex;
	EventManager(const EventManager &);
	EventManager& operator=(const EventManager &);
public:
	EventManager();
	~EventManager();
	void queueEvent(Event);
	void addHandler(EventHandler*);
	void run();
	static EventManager * instance();
	volatile bool externalEventUpdate;
	volatile Event externalEvent;
};

#endif /* EVENTMANAGER_H_ */
