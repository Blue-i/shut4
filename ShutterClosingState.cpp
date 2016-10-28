/*
 * ShutterClosingState.cpp
 *
 *  Created on: 24 Oct 2016
 *      Author: rag
 */

#include "ShutterClosingState.h"
#include "ShutterOpenState.h"
#include "ShutterClosedState.h"
#include "ShutterUnknownState.h"
#include <Logging.h>

ShutterClosingState::ShutterClosingState() {
	// TODO Auto-generated constructor stub

}

ShutterClosingState::~ShutterClosingState() {
	// TODO Auto-generated destructor stub
}

void ShutterClosingState::enter(Shutter* shutter) {
	Log.Debug("Sh > Clg%s",CR);
	OS48_ATOMIC_BLOCK{
		shutter->projector->getClient()->print(Shutter::closeMessage);
	}
	shutter->lastPollTime = 0;
}

void ShutterClosingState::execute(Shutter* shutter) {
	unsigned long time = millis();
	if((time - shutter->lastPollTime) < Shutter::pollInterval) return;

	PJLinkParser::PJLinkResponse response = shutter->getShutterState();

	switch(response){
	case PJLinkParser::OK:
		Log.Debug("Sh OK");
		shutter->stateMachine.changeState(ShutterClosedState::instance());
		break;
	default:
		shutter->stateMachine.changeState(ShutterUnknownState::instance());
	}
}

void ShutterClosingState::exit(Shutter*) {
	Log.Debug("Sh < Clg%s",CR);

}

ShutterClosingState* ShutterClosingState::instance() {
	static ShutterClosingState instance;
	return &instance;
}
