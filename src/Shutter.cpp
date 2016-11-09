/*
 * Shutter.cpp
 *
 *  Created on: 1 Nov 2016
 *      Author: rag
 */

#include <Shutter.h>
#include "Logging.h"
//#include <iostream>

const char * Shutter::openMessage = "%1AVMT 30\r";
const char * Shutter::closeMessage = "%1AVMT 31\r";
const char * Shutter::queryMessage = "%1AVMT ?\r";
const char * Shutter::DEBUG_NAME = "Shutter";

Shutter::Shutter(EventBus* ev) : state(UNKNOWN),
	target(UNKNOWN),
	buffer(nullptr),
	bufSize(0),
	device(nullptr),
	bus(ev),
	pollTimer(SHUTTER_POLL_INTERVAL)
{
	pollTimer.expire();
}

Shutter::~Shutter() {
	// TODO Auto-generated destructor stub
}

PJLinkParser::PJLinkResponse Shutter::getShutterState() {
	if(!isBound()) return PJLinkParser::UNKNOWN;

	uint8_t bytesRead = device->readFor(buffer, bufSize, SHUTTER_READ_TIMEOUT);

	if(bytesRead == 0) {
		return PJLinkParser::UNKNOWN;		//timeout or incomplete
	}
	Log.Debug("%s got %s\r\n", Shutter::DEBUG_NAME, buffer);

	PJLinkParser::PJLinkResponse resp = PJLinkParser::instance()->parseMessage(buffer);

	return resp;
}

void Shutter::enter() {
	if(state == UNKNOWN){
		enterUnknown();
		return;
	} else if(state == OPENING){
		enterOpening();
		return;
	} else if(state == OPEN){
		enterOpen();
		return;
	} else if(state == CLOSING){
		enterClosing();
		return;
	} else if (state == CLOSED){
		enterClosed();
		return;
	}
}

void Shutter::exit() {
	if(state == UNKNOWN){
		exitUnknown();
		return;
	} else if(state == OPENING){
		exitOpening();
		return;
	} else if(state == OPEN){
		exitOpen();
		return;
	} else if(state == CLOSING){
		exitClosing();
		return;
	} else if (state == CLOSED){
		exitClosed();
		return;
	}
}

void Shutter::changeState(STATE s) {
	if (state == s) return;
	exit();
	state = s;
	enter();
}

void Shutter::enterUnknown() {
	Log.Debug("%s > UK\r\n", Shutter::DEBUG_NAME);
	device->reset();
}

void Shutter::executeUnknown() {
	if(!pollTimer.completed()) return;
	pollTimer.reset();
	uint8_t bytesRead = 0;
	OS48_NO_CS_BLOCK{
		device->write(Shutter::queryMessage);
		bytesRead = device->readFor(buffer, bufSize, SHUTTER_READ_TIMEOUT);
	}


	if(bytesRead == 0){
		device->reset();
		return;
	}
	Log.Debug("%s got %s\r\n", Shutter::DEBUG_NAME, buffer);

	PJLinkParser::PJLinkResponse resp = PJLinkParser::instance()->parseMessage(buffer);

	if(resp == PJLinkParser::AVMT_ON){
		bus->queueEvent(SHUTTER_CLOSED);
		onEvent(SHUTTER_CLOSE_BUTTON_PRESS);
		changeState(CLOSED);
		return;
	} else if(resp == PJLinkParser::AVMT_OFF){
		bus->queueEvent(SHUTTER_OPENED);
		onEvent(SHUTTER_OPEN_BUTTON_PRESS);
		changeState(OPEN);
		return;
	}
}

void Shutter::exitUnknown() {
	Log.Debug("%s < UK\r\n", Shutter::DEBUG_NAME);
}

void Shutter::enterOpen() {
	Log.Debug("%s > O\r\n", Shutter::DEBUG_NAME);
	pollTimer.reset();
}

void Shutter::executeOpen() {
	m.lock();
	STATE t = target;
	m.unlock();
	if(t == CLOSED){
		changeState(CLOSING);
		return;
	}

	if(pollTimer.completed()){
		pollTimer.reset();
		uint8_t bytesRead = 0;
		OS48_NO_CS_BLOCK{
			device->write(Shutter::queryMessage);
			bytesRead = device->readFor(buffer, bufSize, SHUTTER_READ_TIMEOUT);
		}

		if(bytesRead == 0){
			bus->queueEvent(SHUTTER_CLOSED);
			device->reset();
			return;
		}

		Log.Debug("%s got %s\r\n", Shutter::DEBUG_NAME, buffer);


		PJLinkParser::PJLinkResponse resp = PJLinkParser::instance()->parseMessage(buffer);

		if(resp == PJLinkParser::AVMT_ON){
			bus->queueEvent(SHUTTER_CLOSED);
			onEvent(SHUTTER_CLOSE_BUTTON_PRESS);
			changeState(CLOSED);
		} else if(resp != PJLinkParser::AVMT_OFF){
			bus->queueEvent(SHUTTER_CLOSED);
			device->reset();
			changeState(UNKNOWN);
		}
	}
}

void Shutter::exitOpen() {
	Log.Debug("%s < O\r\n", Shutter::DEBUG_NAME);
}

void Shutter::enterOpening() {
	Log.Debug("%s > Opp\r\n", Shutter::DEBUG_NAME);
	uint8_t bytesRead = 0;
	OS48_NO_CS_BLOCK{
		device->write(Shutter::openMessage);
		bytesRead = device->readFor(buffer, bufSize, SHUTTER_READ_TIMEOUT);
	}

	if(bytesRead == 0){
		device->reset();
		return;
	}
	Log.Debug("%s got %s\r\n", Shutter::DEBUG_NAME, buffer);

	PJLinkParser::PJLinkResponse resp = PJLinkParser::instance()->parseMessage(buffer);
	if(resp == PJLinkParser::OK){
		bus->queueEvent(SHUTTER_OPENED);
		changeState(OPEN);
		return;
	} else {
		device->reset();
	}
	changeState(UNKNOWN);
}

void Shutter::executeOpening() {
	enterOpening();
}

void Shutter::exitOpening() {
	Log.Debug("%s < Opp\r\n", Shutter::DEBUG_NAME);
}

void Shutter::enterClosed() {
	Log.Debug("%s > C\r\n", Shutter::DEBUG_NAME);
//	bus->queueEvent(SHUTTER_CLOSED);
	pollTimer.reset();
}

void Shutter::executeClosed() {
	m.lock();
	STATE t = target;
	m.unlock();
	if(t == OPEN){
		changeState(OPENING);
		return;
	}
	if(pollTimer.completed()){
		pollTimer.reset();
		uint8_t bytesRead = 0;
		OS48_NO_CS_BLOCK{
			device->write(Shutter::queryMessage);
			bytesRead = device->readFor(buffer, bufSize, SHUTTER_READ_TIMEOUT);
		}
		if(bytesRead == 0){
			device->reset();
			return;
		}
		Log.Debug("%s got %s\r\n", Shutter::DEBUG_NAME, buffer);

		PJLinkParser::PJLinkResponse resp = PJLinkParser::instance()->parseMessage(buffer);

		if(resp == PJLinkParser::AVMT_OFF){
			bus->queueEvent(SHUTTER_OPENED);
			onEvent(SHUTTER_OPEN_BUTTON_PRESS);
			changeState(OPEN);
		} else if(resp != PJLinkParser::AVMT_ON){
			device->reset();
			changeState(UNKNOWN);
		}
	}
}

void Shutter::exitClosed() {
	Log.Debug("%s < C\r\n", Shutter::DEBUG_NAME);
}

void Shutter::enterClosing() {
	Log.Debug("%s > Cls\r\n", Shutter::DEBUG_NAME);
	uint8_t bytesRead = 0;
	OS48_NO_CS_BLOCK{
		device->write(Shutter::closeMessage);
		bytesRead = device->readFor(buffer, bufSize, SHUTTER_READ_TIMEOUT);
	}

	if(bytesRead == 0){
		device->reset();
		return;
	}
	Log.Debug("%s got %s\r\n", Shutter::DEBUG_NAME, buffer);

	PJLinkParser::PJLinkResponse resp = PJLinkParser::instance()->parseMessage(buffer);
	if(resp == PJLinkParser::OK){
		bus->queueEvent(SHUTTER_CLOSED);
		changeState(CLOSED);
		return;
	} else {
		device->reset();
	}
	changeState(UNKNOWN);

}

void Shutter::executeClosing() {
	enterClosing();
}

void Shutter::exitClosing() {
	Log.Debug("%s < Cls\r\n", Shutter::DEBUG_NAME);
}

void Shutter::bind(Device* d, char* b, size_t size) {
	device = d;
	buffer = b;
	bufSize = size;
}

bool Shutter::isBound() {
	return device != nullptr;
}

void Shutter::run() {
	if(state == UNKNOWN){
		executeUnknown();
		return;
	} else if(state == OPENING){
		executeOpening();
		return;
	} else if(state == OPEN){
		executeOpen();
		return;
	} else if(state == CLOSING){
		executeClosing();
		return;
	} else if (state == CLOSED){
		executeClosed();
		return;
	}
}

void Shutter::reset() {
	if(state == OPEN){
		bus->queueEvent(SHUTTER_CLOSED);
	}
	m.lock();
	target = UNKNOWN;
	m.unlock();
	changeState(UNKNOWN);
}

void Shutter::onEvent(Event e) {
	if(e == SHUTTER_OPEN_BUTTON_PRESS){
		m.lock();
		target = OPEN;
		m.unlock();
		return;
	} else if(e == SHUTTER_CLOSE_BUTTON_PRESS){
		m.lock();
		target = CLOSED;
		m.unlock();
		return;
	}
}

Shutter::STATE Shutter::getState() {
	return state;
}
