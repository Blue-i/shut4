/*
 * Shutter.h
 *
 *  Created on: 1 Nov 2016
 *      Author: rag
 */
#pragma once

#ifndef SHUTTER_H_
#define SHUTTER_H_

#include <Component.h>
#include <EventHandler.h>
#include "PJLinkParser.h"
#include "Device.h"
#include "Timer.h"
#include "EventBus.h"
#include "os48.h"

#define SHUTTER_READ_TIMEOUT 1000
#define SHUTTER_POLL_INTERVAL 10000

class Shutter: public Component<Device>, public EventHandler {
public:
	enum STATE {
		UNKNOWN,
		OPEN,
		OPENING,
		CLOSED,
		CLOSING
	};
private:

	static const char * queryMessage;
	static const char * openMessage;
	static const char * closeMessage;
	static const char * DEBUG_NAME;

	STATE state, target;

	char * buffer;
	size_t bufSize;
	Device * device;
	EventBus * bus;
	Timer pollTimer;
	os48::Mutex m;

	PJLinkParser::PJLinkResponse getShutterState();

	void enter();
	void exit();
	void changeState(STATE);

	void enterUnknown();
	void executeUnknown();
	void exitUnknown();

	void enterOpen();
	void executeOpen();
	void exitOpen();

	void enterOpening();
	void executeOpening();
	void exitOpening();

	void enterClosed();
	void executeClosed();
	void exitClosed();

	void enterClosing();
	void executeClosing();
	void exitClosing();

public:
	Shutter(EventBus*);
	virtual ~Shutter();
	Shutter::STATE getState();

	//component
	virtual void bind(Device*, char*, size_t);
	virtual bool isBound();
	virtual void run();
	virtual void reset();

	//eventHandler
	virtual void onEvent(Event);
};

#endif /* SHUTTER_H_ */
