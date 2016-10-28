/*
 * ProjectorConnectingState.cpp
 *
 *  Created on: 22 Oct 2016
 *      Author: rag
 */

#include "ProjectorConnectingState.h"
#include "ProjectorConnectedState.h"
#include <Logging.h>
#include "os48.h"


ProjectorConnectingState::ProjectorConnectingState() {
	// TODO Auto-generated constructor stub

}

ProjectorConnectingState::~ProjectorConnectingState() {
	// TODO Auto-generated destructor stub
}

void ProjectorConnectingState::enter(Projector* projector) {
//	Serial.println("Entering Connecting State");
	Log.Debug("Proj > Conn %s", CR);
}

void ProjectorConnectingState::execute(Projector* projector) {
//	while(!projector->client){}
	if(projector->client.connected()) {
		Log.Debug("Proj Conn!%s",CR);
		projector->stateMachine.changeState(ProjectorConnectedState::instance());
		return;
	}

	Log.Debug("Proj Conn..%s", CR);
	OS48_ATOMIC_BLOCK{
		projector->client.stop();
		projector->client.flush();
	}
	if(projector->client.connect(*projector->ip, projector->port)){
		projector->readFor(projector->buffer, projector->pollWait);
		Log.Debug("Proj %s%s", projector->buffer, CR);
	}

}

void ProjectorConnectingState::exit(Projector* projector) {
//	Serial.println("Exiting Connecting State");
	Log.Debug("Proj < Conn%s", CR);
}

ProjectorConnectingState* ProjectorConnectingState::instance() {
	static ProjectorConnectingState instance;
	return &instance;
}
