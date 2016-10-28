/*
 * ProjectorPollWaitState.cpp
 *
 *  Created on: 22 Oct 2016
 *      Author: rag
 */

#include "ProjectorPollWaitState.h"
#include "ProjectorConnectedState.h"
#include "ProjectorConnectingState.h"
#include <Logging.h>
#include "os48.h"

ProjectorPollWaitState::ProjectorPollWaitState() {
	// TODO Auto-generated constructor stub

}

ProjectorPollWaitState::~ProjectorPollWaitState() {
	// TODO Auto-generated destructor stub
}

void ProjectorPollWaitState::enter(Projector* projector) {
	Log.Debug("Proj > PW%s", CR);
	OS48_ATOMIC_BLOCK{
		projector->client.print("%1POWR ?\r");
	}
	projector->lastPollTime = millis();
}

void ProjectorPollWaitState::execute(Projector* projector) {

	size_t bytesRead = 0;
	bytesRead = projector->readFor(projector->buffer, projector->pollWait);
	if(bytesRead > 1){
		Log.Debug("PW %s%s", projector->buffer,CR);
		projector->stateMachine.changeState(ProjectorConnectedState::instance());
		return;
	}
	OS48_ATOMIC_BLOCK{
		projector->client.stop();
		projector->client.flush();
	}
	projector->stateMachine.changeState(ProjectorConnectingState::instance());
}

void ProjectorPollWaitState::exit(Projector* projector) {
	Log.Debug("Proj < PW%s", CR);
}

ProjectorPollWaitState* ProjectorPollWaitState::instance() {
	static ProjectorPollWaitState instance;
	return &instance;
}
