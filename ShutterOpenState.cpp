/*
 * ShutterOpenState.cpp
 *
 *  Created on: 22 Oct 2016
 *      Author: rag
 */

#include "ShutterOpenState.h"
#include "ShutterClosedState.h"
#include "ShutterClosingState.h"
#include "ShutterUnknownState.h"
#include <Logging.h>

ShutterOpenState::ShutterOpenState() {
	// TODO Auto-generated constructor stub

}

ShutterOpenState::~ShutterOpenState() {
	// TODO Auto-generated destructor stub
}

void ShutterOpenState::enter(Shutter* shutter) {
	Log.Debug("Sh > O%s",CR);
}

void ShutterOpenState::execute(Shutter* shutter) {
	shutter->targetMutex.lock();
	Shutter::ShutterTargetState s = shutter->target;
	shutter->targetMutex.unlock();
	//check target is still valid
	switch(s){
		case Shutter::CLOSED:
			shutter->stateMachine.changeState(ShutterClosingState::instance());
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
		shutter->stateMachine.changeState(ShutterClosedState::instance());
		break;
	case PJLinkParser::AVMT_ON:
		break;
	default:
		shutter->stateMachine.changeState(ShutterUnknownState::instance());
	}
}

void ShutterOpenState::exit(Shutter* Shutter) {
	Log.Debug("Sh < O%s",CR);
}

ShutterOpenState* ShutterOpenState::instance() {
	static ShutterOpenState instance;
	return &instance;
}
