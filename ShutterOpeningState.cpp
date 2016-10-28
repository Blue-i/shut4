/*
 * ShutterOpeningState.cpp
 *
 *  Created on: 24 Oct 2016
 *      Author: rag
 */

#include "ShutterOpeningState.h"
#include "ShutterOpenState.h"
#include "ShutterClosedState.h"
#include "ShutterUnknownState.h"
#include <Logging.h>

ShutterOpeningState::ShutterOpeningState() {
	// TODO Auto-generated constructor stub

}

ShutterOpeningState::~ShutterOpeningState() {
	// TODO Auto-generated destructor stub
}

void ShutterOpeningState::enter(Shutter* shutter) {
//	Log.Debug("Shutter entering opening state");
	shutter->projector->getClient()->print(Shutter::openMessage);
	shutter->lastPollTime = 0;
}

void ShutterOpeningState::execute(Shutter* shutter) {
	unsigned long time = millis();
	if(abs(time - shutter->lastPollTime) < Shutter::pollInterval) return;

	PJLinkParser::PJLinkResponse response = shutter->getShutterState();

	switch(response){
	case PJLinkParser::OK:
		shutter->stateMachine.changeState(ShutterClosedState::instance());
		break;
	default:
		shutter->stateMachine.changeState(ShutterUnknownState::instance());
	}
}

void ShutterOpeningState::exit(Shutter*) {
//	Log.Debug("Shutter exiting opening state");
}

ShutterOpeningState* ShutterOpeningState::instance() {
	static ShutterOpeningState instance;
	return &instance;
}
