/*
 * ShutterLEDController.cpp
 *
 *  Created on: 29 Oct 2016
 *      Author: rag
 */

#include "ShutterLEDController.h"
#include "Logging.h"
#include "Arduino.h"

const char * ShutterLEDController::DEBUG_NAME = "LEDControl";

ShutterLEDController::ShutterLEDController(uint8_t green, uint8_t red) :
	state(UNKNOWN),
	target(UNKNOWN),
	connected(0),
	open(0),
	greenPin(green),
	redPin(red),
	greenState(LOW),
	redState(LOW),
	blinkTimer(SHUTTER_LED_BLINK_INTERVAL){
	OS48_ATOMIC_BLOCK{
		digitalWrite(greenPin, HIGH);
		greenState = HIGH;
		digitalWrite(redPin, HIGH);
		redState = HIGH;
	}
}

void ShutterLEDController::enter() {
	switch(state){
	case UNKNOWN:
		enterUnknown();
		break;
	case OPENING:
		enterOpening();
		break;
	case OPEN:
		enterOpen();
		break;
	case CLOSING:
		enterClosing();
		break;
	case CLOSED:
		enterClosed();
		break;
	}
}

void ShutterLEDController::execute() {
	switch(state){
	case UNKNOWN:
		executeUnknown();
		break;
	case OPENING:
		executeOpening();
		break;
	case OPEN:
		executeOpen();
		break;
	case CLOSING:
		executeClosing();
		break;
	case CLOSED:
		executeClosed();
		break;
	}
}

void ShutterLEDController::exit() {
	switch(state){
	case UNKNOWN:
		exitUnknown();
		break;
	case OPENING:
		exitOpening();
		break;
	case OPEN:
		exitOpen();
		break;
	case CLOSING:
		exitClosing();
		break;
	case CLOSED:
		exitClosed();
		break;
	}
}

void ShutterLEDController::changeState(STATE s) {
	exit();
	state = s;
	enter();
}

void ShutterLEDController::enterUnknown() {
	OS48_ATOMIC_BLOCK{
		digitalWrite(greenPin, HIGH);
		greenState = HIGH;
		digitalWrite(redPin, HIGH);
		redState = HIGH;
	}
}

void ShutterLEDController::executeUnknown() {

}

void ShutterLEDController::exitUnknown() {
}

void ShutterLEDController::enterOpening() {
	OS48_ATOMIC_BLOCK{
		digitalWrite(greenPin, HIGH);
		greenState = HIGH;
		digitalWrite(redPin, LOW);
		redState = LOW;
	}
	blinkTimer.reset();
}

void ShutterLEDController::executeOpening() {

	if(blinkTimer.completed()){
		if(greenState == HIGH){
			OS48_ATOMIC_BLOCK{
				digitalWrite(greenPin, LOW);
				greenState = LOW;
			}
		} else if(greenState == LOW){
			OS48_ATOMIC_BLOCK{
				digitalWrite(greenPin, HIGH);
				greenState = HIGH;
			}
		}
		blinkTimer.reset();
	}
}

void ShutterLEDController::exitOpening() {
}

void ShutterLEDController::enterOpen() {
	OS48_ATOMIC_BLOCK{
		digitalWrite(greenPin, HIGH);
		greenState = HIGH;
		digitalWrite(redPin, LOW);
		redState = LOW;
	}
}

void ShutterLEDController::executeOpen() {
}

void ShutterLEDController::exitOpen() {
	//Log.Debug("L < O%s",CR);
}

void ShutterLEDController::enterClosing() {
	OS48_ATOMIC_BLOCK{
		digitalWrite(greenPin, LOW);
		greenState = LOW;
		digitalWrite(redPin, HIGH);
		redState = HIGH;
	}
	blinkTimer.reset();
}

void ShutterLEDController::executeClosing() {
	if(blinkTimer.completed()){
		if(redState == HIGH){
			OS48_ATOMIC_BLOCK{
				digitalWrite(redPin, LOW);
				redState = LOW;
			}
		} else if(redState == LOW){
			OS48_ATOMIC_BLOCK{
				digitalWrite(redPin, HIGH);
				redState = HIGH;
			}
		}
		blinkTimer.reset();
	}
}

void ShutterLEDController::exitClosing() {
}

void ShutterLEDController::enterClosed() {
	OS48_ATOMIC_BLOCK{
		digitalWrite(greenPin, LOW);
		greenState = LOW;
		digitalWrite(redPin, HIGH);
		redState = HIGH;
	}
}

void ShutterLEDController::executeClosed() {
}

void ShutterLEDController::exitClosed() {
}

ShutterLEDController::~ShutterLEDController() {
	// TODO Auto-generated destructor stub
}

void ShutterLEDController::run() {
	execute();
	checkState();
}

void ShutterLEDController::checkState() {
	STATE targetState = UNKNOWN;
	STATE currentState = UNKNOWN;
	uint8_t openCount = 0;
	uint8_t connCount = 0;

	m.lock();
	targetState = target;
	currentState = state;
	openCount = open;
	connCount = connected;
	m.unlock();

	if(targetState == OPEN){
		if(connCount == openCount){
			if(currentState != OPEN) {
				changeState(OPEN);
			}
			return;
		} else if (connCount > 0 && currentState != OPENING) {
			changeState(OPENING);
			return;
		}
	}
	if(targetState == CLOSED){
		if(openCount == 0){
			if(currentState != CLOSED) {
				changeState(CLOSED);
			}
			return;
		} else {
			if(currentState != CLOSING){
				changeState(CLOSING);
				return;
			}
		}
	}
}

void ShutterLEDController::onEvent(Event e) {
	Log.Debug("%s ev %d%s",ShutterLEDController::DEBUG_NAME, e, CR);
	switch (e){
	case NO_EVENT:
		break;
	case SHUTTER_OPEN_BUTTON_PRESS:
		m.lock();
		target = OPEN;
		m.unlock();
		break;
	case SHUTTER_CLOSE_BUTTON_PRESS:
		m.lock();
		target = CLOSED;
		m.unlock();
		break;
	case PROJECTOR_CONNECTED:
		m.lock();
		if(connected < 4) connected++;
		m.unlock();
		break;
	case PROJECTOR_DISCONNECTED:
		m.lock();
		if(connected > 0) connected--;
		m.unlock();
		break;
	case SHUTTER_OPENED:
		m.lock();
		if(open < connected) open++;
		m.unlock();
		break;
	case SHUTTER_CLOSED:
		m.lock();
		if(open > 0) open--;
		m.unlock();
		break;
	case MAX_EVENT:
		break;
	}
	m.lock();
	Log.Debug("%s conn %d open %d%s",ShutterLEDController::DEBUG_NAME, connected, open, CR);
	m.unlock();

}

ShutterLEDController::STATE ShutterLEDController::getState() {
	return state;
}
