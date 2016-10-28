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
//	Log.Debug("Shutter entering closing state");
	shutter->projector->getClient()->print(Shutter::closeMessage);
	shutter->lastPollTime = 0;
}

void ShutterClosingState::execute(Shutter* shutter) {
	unsigned long time = millis();
	if(abs(time - shutter->lastPollTime) < Shutter::pollInterval) return;

	PJLinkParser::PJLinkResponse response = shutter->getShutterState();

	switch(response){
	case PJLinkParser::OK:
//		Log.Debug("Shutter got OK");
		shutter->stateMachine.changeState(ShutterClosedState::instance());
		break;
	default:
		shutter->stateMachine.changeState(ShutterUnknownState::instance());
	}
}

void ShutterClosingState::exit(Shutter*) {
//	Log.Debug("Shutter exiting closing state");

}

ShutterClosingState* ShutterClosingState::instance() {
	static ShutterClosingState instance;
	return &instance;
}
