/*
 * EventManager.cpp
 *
 *  Created on: 25 Oct 2016
 *      Author: rag
 */

#include "EventManager.h"
#include <Logging.h>

EventManager::EventManager() :
	head(events),
	tail(events),
	numHandlers(0),
	externalEventUpdate(false),
	externalEvent(NO_EVENT) {
	// TODO Auto-generated constructor stub

}

EventManager::~EventManager() {
	// TODO Auto-generated destructor stub
}

void EventManager::queueEvent(Event e) {
		mutex.lock();
		if(head == tail){
			*head = e;
			tail = events + ((tail - events + 1) % MAX_EVENTS);
		} else {
			Event * next = events + ((tail - events + 1) % MAX_EVENTS);
			if(next != head) {
				*tail = e;
				tail = next;
			}
		}
		mutex.unlock();
}

void EventManager::addHandler(EventHandler* h) {
	mutex.lock();
	if(numHandlers < MAX_HANDLERS){
		handlers[numHandlers++] = h;
	}
	mutex.unlock();
}

void EventManager::run() {
	Event e = NO_EVENT;

	mutex.lock();
	if(head != tail){
		e = *head;
		head = events + ((head - events + 1) % MAX_EVENTS);
	}
	mutex.unlock();

	if(e != NO_EVENT){
		for(uint8_t i = 0; i < numHandlers; i++){
			handlers[i]->onEvent(e);
		}
	}

	e = NO_EVENT;
	OS48_ATOMIC_BLOCK{
		if(externalEventUpdate){
			e = externalEvent;
			externalEventUpdate = false;
		}
	}

	if(e != NO_EVENT){
		for(uint8_t i = 0; i < numHandlers; i++){
			handlers[i]->onEvent(e);
		}
	}

}

EventManager* EventManager::instance() {
	static EventManager instance;
	return &instance;
}
