/*
 * ShutterLEDController.h
 *
 *  Created on: 29 Oct 2016
 *      Author: rag
 */
#pragma once

#ifndef SHUTTERLEDCONTROLLER_H_
#define SHUTTERLEDCONTROLLER_H_

#include <Arduino.h>
#include "EventHandler.h"
#include "Timer.h"
#include "os48.h"

#define SHUTTER_LED_BLINK_INTERVAL 50

class ShutterLEDController : public EventHandler {
public:

	enum STATE {
		UNKNOWN,
		OPENING,
		OPEN,
		CLOSING,
		CLOSED
	};
private:
	static const char * DEBUG_NAME;
	STATE state, target;
	os48::Mutex m;
	uint8_t connected, open, greenPin, redPin, greenState, redState;
	Timer blinkTimer;
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
	ShutterLEDController::STATE getState();
};

#endif /* SHUTTERLEDCONTROLLER_H_ */
