/*
 * ShutterClosedState.cpp
 *
 *  Created on: 23 Oct 2016
 *      Author: rag
 */

#include "ShutterClosedState.h"
#include "ShutterOpenState.h"
#include "ShutterOpeningState.h"
#include "ShutterUnknownState.h"
#include <Logging.h>

ShutterClosedState::ShutterClosedState() {
	// TODO Auto-generated constructor stub

}

ShutterClosedState::~ShutterClosedState() {
	// TODO Auto-generated destructor stub
}

void ShutterClosedState::enter(Shutter* shutter) {
	Log.Debug("Sh > C%s",CR);
}

void ShutterClosedState::execute(Shutter* shutter) {
	shutter->targetMutex.lock();
	Shutter::ShutterTargetState s = shutter->target;
	shutter->targetMutex.unlock();
	//check target is still valid
	switch(s){
	case Shutter::OPEN:
		shutter->stateMachine.changeState(ShutterOpeningState::instance());
		return;
	case Shutter::UNKNOWN:
		shutter->stateMachine.changeState(ShutterUnknownState::instance());
		return;
	default:
		//continue to poll
		break;
	}

	unsigned long time = millis();
	if((time - shutter->lastPollTime) < Shutter::pollInterval) return;

	PJLinkParser::PJLinkResponse response = shutter->getShutterState();

	switch(response){
	case PJLinkParser::AVMT_OFF:
		break;
	case PJLinkParser::AVMT_ON:
		shutter->stateMachine.changeState(ShutterOpenState::instance());
		break;
	default:
		shutter->stateMachine.changeState(ShutterUnknownState::instance());
	}
}

void ShutterClosedState::exit(Shutter* shutter) {
	Log.Debug("Sh < C%s",CR);
}

ShutterClosedState* ShutterClosedState::instance() {
	static ShutterClosedState instance;
	return &instance;
}
