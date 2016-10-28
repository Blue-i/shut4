/*
 * ProjectorConnectedState.cpp
 *
 *  Created on: 22 Oct 2016
 *      Author: rag
 */

#include "ProjectorConnectedState.h"
#include "ProjectorPollWaitState.h"
#include <Logging.h>
#include "os48.h"
#define LOGLEVEL LOG_LEVEL_NOOUTPUT
ProjectorConnectedState::ProjectorConnectedState() {
	// TODO Auto-generated constructor stub

}

ProjectorConnectedState::~ProjectorConnectedState() {
	// TODO Auto-generated destructor stub
}

void ProjectorConnectedState::enter(Projector* projector) {
	Log.Debug("Proj > Contd%s",CR);
}

void ProjectorConnectedState::execute(Projector* projector) {
	uint16_t elapsedTime = 0;
	elapsedTime = millis() - projector->lastPollTime;
	Log.Debug("Contd time %d%s ", elapsedTime, CR);
	if(elapsedTime > projector->pollInterval){
		Log.Debug("Proj Polling");
		projector->stateMachine.changeState(ProjectorPollWaitState::instance());
		return;
	}


//	Log.Debug("Proj Contd%s",CR);
//	if(!projector->componentQueue.isEmpty()){
//		Component<Projector>* component = projector->componentQueue.dequeue();
//		component->run();
//		projector->componentQueue.enqueue(component);
//	}

}

void ProjectorConnectedState::exit(Projector* projector) {
	Log.Debug("Proj < Contd%s",CR);
}

ProjectorConnectedState* ProjectorConnectedState::instance() {
	static ProjectorConnectedState instance;
	return &instance;
}
