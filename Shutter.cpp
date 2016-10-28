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

	projector->getClient()->print(Shutter::queryMessage);

	unsigned long startTime = millis();
	size_t bytesRead = 0;

	while(abs(millis() - startTime) < Shutter::timeout && 	//not timed out
			bytesRead < Shutter::bufferSize-1 &&			//not overflowing
			buffer[bytesRead] != '\r'){						//not end of message

		buffer[bytesRead] = projector->getClient()->read();
		if(buffer[bytesRead] != -1) bytesRead++;
	}

	if(bytesRead == 0 || buffer[bytesRead] != '\r') return PJLinkParser::UNKNOWN;		//timeout or incomplete
	buffer[++bytesRead] = '\0';

	PJLinkParser::PJLinkResponse resp = PJLinkParser::instance()->parseMessage((char *) buffer);
	buffer[0] = '\0';

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
