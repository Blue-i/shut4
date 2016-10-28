/*
 * Shutter.cpp
 *
 *  Created on: 22 Oct 2016
 *      Author: rag
 */

#include <Logging.h>
#include "Shutter.h"

#include "PJLinkParser.h"

const char * Shutter::openMessage = "%1AVMT 30\r";
const char * Shutter::closeMessage = "%1AVMT 31\r";
const char * Shutter::queryMessage = "%1AVMT ?\r";

Shutter::Shutter() :
	state(UNKNOWN),
	target(UNKNOWN),
	lastPollTime(0),
	projector(NULL)
{
}

Shutter::~Shutter() {
}

void Shutter::bind(Projector* p) {
	projector = p;
}

bool Shutter::isBound() {
	return projector != NULL;
}

PJLinkParser::PJLinkResponse Shutter::getShutterState() {
	if(!isBound()) return PJLinkParser::UNKNOWN;

	uint8_t bytesRead = projector->readFor(projector->buffer,Shutter::timeout);

	if(bytesRead == 0) return PJLinkParser::UNKNOWN;		//timeout or incomplete

	PJLinkParser::PJLinkResponse resp = PJLinkParser::instance()->parseMessage((char *) projector->buffer);

	return resp;
}

void Shutter::run() {
	execute();
}

void Shutter::enter() {
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

void Shutter::execute() {
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

void Shutter::exit() {
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

void Shutter::changeState(STATE s) {
	exit();
	state = s;
	enter();
}

void Shutter::enterUnknown() {
	Log.Debug("Sh > Uk%s",CR);
}

void Shutter::executeUnknown() {
	targetMutex.lock();
	STATE targetState = target;
	targetMutex.unlock();

	switch (targetState){
	case CLOSED:
		changeState(CLOSING);
		break;
	case OPEN:
		changeState(OPENING);
		break;
	default:
		//do nothing
		break;
	}
}

void Shutter::exitUnknown() {
	Log.Debug("Sh < Uk");
}

void Shutter::enterOpen() {
	Log.Debug("Sh > O%s",CR);
}

void Shutter::executeOpen() {
	targetMutex.lock();
	STATE targetState = target;
	targetMutex.unlock();
	//check target is still valid
	switch(targetState){
		case CLOSED:
			changeState(CLOSING);
			return;
		case UNKNOWN:
			changeState(UNKNOWN);
			return;
		default:
			//continue to poll
			break;
		}

	unsigned long time = millis();
	if((time - lastPollTime) < Shutter::pollInterval) return;

	PJLinkParser::PJLinkResponse response = getShutterState();

	switch(response){
	case PJLinkParser::AVMT_OFF:
		changeState(CLOSED);
		break;
	case PJLinkParser::AVMT_ON:
		break;
	default:
		changeState(UNKNOWN);
	}
}

void Shutter::exitOpen() {
	Log.Debug("Sh < O%s",CR);
}

void Shutter::enterOpening() {
	Log.Debug("Sh > Op%s",CR);
	projector->getClient()->print(Shutter::openMessage);
	lastPollTime = 0;
}

void Shutter::executeOpening() {
	unsigned long time = millis();
	if((time - lastPollTime) < Shutter::pollInterval) return;

	PJLinkParser::PJLinkResponse response = getShutterState();

	switch(response){
	case PJLinkParser::OK:
		Log.Debug("Sh OK%s",CR);
		changeState(OPEN);
		break;
	default:
		changeState(UNKNOWN);
	}
}

void Shutter::exitOpening() {
	Log.Debug("Sh < Op%s",CR);
}

void Shutter::enterClosed() {
	Log.Debug("Sh > C%s",CR);
}

void Shutter::executeClosed() {
	targetMutex.lock();
	STATE targetState = target;
	targetMutex.unlock();
	//check target is still valid
	switch(targetState){
	case OPEN:
		changeState(OPENING);
		return;
	case UNKNOWN:
		changeState(UNKNOWN);
		return;
	default:
		//continue to poll
		break;
	}

	unsigned long time = millis();
	if((time - lastPollTime) < Shutter::pollInterval) return;

	PJLinkParser::PJLinkResponse response = getShutterState();

	switch(response){
	case PJLinkParser::AVMT_OFF:
		break;
	case PJLinkParser::AVMT_ON:
		changeState(OPEN);
		break;
	default:
		changeState(UNKNOWN);
	}
}

void Shutter::exitClosed() {
	Log.Debug("Sh < C%s",CR);
}

void Shutter::enterClosing() {
	Log.Debug("Sh > Clg%s",CR);
	OS48_ATOMIC_BLOCK{
		projector->getClient()->print(Shutter::closeMessage);
	}
	lastPollTime = 0;
}

void Shutter::executeClosing() {
	unsigned long time = millis();
	if((time - lastPollTime) < Shutter::pollInterval) return;

	PJLinkParser::PJLinkResponse response = getShutterState();

	switch(response){
	case PJLinkParser::OK:
		Log.Debug("Sh OK");
		changeState(CLOSED);
		break;
	default:
		changeState(UNKNOWN);
	}
}

void Shutter::exitClosing() {
	Log.Debug("Sh < Clg%s",CR);
}

void Shutter::onEvent(Event e) {
	switch(e){
	case SHUTTER_OPEN_BUTTON_PRESS:
		targetMutex.lock();
		OS48_NO_CS_BLOCK{
			Log.Debug("Shutter got open event");
		}
		target = OPEN;
		targetMutex.unlock();
		break;
	case SHUTTER_CLOSE_BUTTON_PRESS:
		targetMutex.lock();
		OS48_NO_CS_BLOCK{
			Log.Debug("Shutter got close event");
		}
		target = CLOSED;
		targetMutex.unlock();
		break;
	default:
		//do nothing
		break;
	}
}
