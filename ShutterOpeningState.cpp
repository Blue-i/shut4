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
	Log.Debug("Sh > Op%s",CR);
	shutter->projector->getClient()->print(Shutter::openMessage);
	shutter->lastPollTime = 0;
}

void ShutterOpeningState::execute(Shutter* shutter) {
	unsigned long time = millis();
	if((time - shutter->lastPollTime) < Shutter::pollInterval) return;

	PJLinkParser::PJLinkResponse response = shutter->getShutterState();

	switch(response){
	case PJLinkParser::OK:
		Log.Debug("Sh OK%s",CR);
		shutter->stateMachine.changeState(ShutterClosedState::instance());
		break;
	default:
		shutter->stateMachine.changeState(ShutterUnknownState::instance());
	}
}

void ShutterOpeningState::exit(Shutter*) {
	Log.Debug("Sh < Op%s",CR);
}

ShutterOpeningState* ShutterOpeningState::instance() {
	static ShutterOpeningState instance;
	return &instance;
}
