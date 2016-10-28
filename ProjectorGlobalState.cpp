/*
 * ProjectorGlobalState.cpp
 *
 *  Created on: 22 Oct 2016
 *      Author: rag
 */

#include "ProjectorGlobalState.h"

ProjectorGlobalState::ProjectorGlobalState() {
	// TODO Auto-generated constructor stub

}

ProjectorGlobalState::~ProjectorGlobalState() {
	// TODO Auto-generated destructor stub
}

void ProjectorGlobalState::enter(Projector* projector) {
}

void ProjectorGlobalState::execute(Projector* projector) {

}

void ProjectorGlobalState::exit(Projector* projector) {
}

ProjectorGlobalState* ProjectorGlobalState::instance() {
	static ProjectorGlobalState instance;
	return &instance;
}
