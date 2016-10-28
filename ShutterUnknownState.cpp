/*
 * ShutterUnknownState.cpp
 *
 *  Created on: 23 Oct 2016
 *      Author: rag
 */

#include "ShutterUnknownState.h"
#include "ShutterClosingState.h"
#include "ShutterOpeningState.h"
#include "PJLinkParser.h"
#include "os48.h"
#include <Logging.h>

ShutterUnknownState::ShutterUnknownState() {
	// TODO Auto-generated constructor stub

}

ShutterUnknownState::~ShutterUnknownState() {
	// TODO Auto-generated destructor stub
}

void ShutterUnknownState::enter(Shutter* shutter) {
	OS48_NO_CS_BLOCK{
//		Log.Debug("Shutter entering unknown state");
	}
}

void ShutterUnknownState::execute(Shutter* shutter) {
	shutter->targetMutex.lock();
	Shutter::ShutterTargetState s = shutter->target;
	shutter->targetMutex.unlock();

	switch (s){
	case Shutter::CLOSED:
		shutter->stateMachine.changeState(ShutterClosingState::instance());
		break;
	case Shutter::OPEN:
		shutter->stateMachine.changeState(ShutterOpeningState::instance());
		break;
	default:
		//do nothing
		break;
	}
}

void ShutterUnknownState::exit(Shutter* shutter) {
	OS48_NO_CS_BLOCK{
//		Log.Debug("Shutter exiting unknown state");
	}
}

ShutterUnknownState* ShutterUnknownState::instance() {
	static ShutterUnknownState instance;
	return &instance;
}
