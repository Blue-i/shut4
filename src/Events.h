/*
 * Events.h
 *
 *  Created on: 25 Oct 2016
 *      Author: rag
 */
#pragma once

#ifndef EVENTS_EVENTS_H_
#define EVENTS_EVENTS_H_

enum Event {
	NO_EVENT = 0,
	SHUTTER_OPEN_BUTTON_PRESS = 1,
	SHUTTER_CLOSE_BUTTON_PRESS = 2,
	PROJECTOR_CONNECTED = 3,
	PROJECTOR_DISCONNECTED = 4,
	SHUTTER_OPENED = 5,
	SHUTTER_CLOSED = 6,
	MAX_EVENT = 7
};


#endif /* EVENTS_EVENTS_H_ */
