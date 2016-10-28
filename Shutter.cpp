/*
 * Shutter.cpp
 *
 *  Created on: 22 Oct 2016
 *      Author: rag
 */

#include <Logging.h>
#include "Shutter.h"
#include "ShutterUnknownState.h"
#include "ShutterClosedState.h"
#include "ShutterOpenState.h"
#include "ShutterUnknownState.h"

#include "PJLinkParser.h"

const char * Shutter::openMessage = "%1AVMT 30\r";
const char * Shutter::closeMessage = "%1AVMT 31\r";
const char * Shutter::queryMessage = "%1AVMT ?\r";

Shutter::Shutter() :
	target(UNKNOWN),
	lastPollTime(0),
	stateMachine(this),
	projector(NULL)
{
	stateMachine.setCurrentState(ShutterUnknownState::instance());
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
	OS48_NO_CS_BLOCK{
//		Log.Debug ("Shutter update");
	}
	stateMachine.update();
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
