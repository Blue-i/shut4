/*
 * ShutterLEDController.h
 *
 *  Created on: 29 Oct 2016
 *      Author: rag
 */

#ifndef SHUTTERLEDCONTROLLER_H_
#define SHUTTERLEDCONTROLLER_H_

#include <Arduino.h>
#include "EventHandler.h"
#include "os48.h"

class ShutterLEDController : public EventHandler {

	static const uint16_t interval = 100;

	enum STATE {
		UNKNOWN,
		OPENING,
		OPEN,
		CLOSING,
		CLOSED
	} state, target;

	os48::Mutex m;
	uint8_t connected, open, greenPin, redPin, greenState, redState;
	unsigned long lastBlinkTime;
	void enter();
	void execute();
	void exit();
	void changeState(STATE);

	void enterUnknown();
	void executeUnknown();
	void exitUnknown();

	void enterOpening();
	void executeOpening();
	void exitOpening();

	void enterOpen();
	void executeOpen();
	void exitOpen();

	void enterClosing();
	void executeClosing();
	void exitClosing();

	void enterClosed();
	void executeClosed();
	void exitClosed();

	void checkState();

public:
	ShutterLEDController(uint8_t greenPin, uint8_t redPin);
	virtual ~ShutterLEDController();
	void run();
	virtual void onEvent(Event e);
};

#endif /* SHUTTERLEDCONTROLLER_H_ */
