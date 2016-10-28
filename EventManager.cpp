/*
 * EventManager.cpp
 *
 *  Created on: 25 Oct 2016
 *      Author: rag
 */

#include "EventManager.h"
#include <Logging.h>

EventManager::EventManager() : numHandlers(0), externalEventUpdate(false), externalEvent(NO_EVENT) {
	// TODO Auto-generated constructor stub

}

EventManager::~EventManager() {
	// TODO Auto-generated destructor stub
}

void EventManager::queueEvent(Event e) {
		mutex.lock();
//		Log.Debug("EventManager got event %d", e);
		events.enqueue(e);
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
	if(!events.isEmpty()){
		e = events.dequeue();
	}
	mutex.unlock();

	if(e != NO_EVENT){
		for(size_t i = 0; i < numHandlers; i++){
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
		for(size_t i = 0; i < numHandlers; i++){
			handlers[i]->onEvent(e);
		}
	}

}

EventManager* EventManager::instance() {
	static EventManager instance;
	return &instance;
}
