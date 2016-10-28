/*
 * ProjectorConnectingState.h
 *
 *  Created on: 22 Oct 2016
 *      Author: rag
 */

#ifndef PROJECTOR_PROJECTORCONNECTINGSTATE_H_
#define PROJECTOR_PROJECTORCONNECTINGSTATE_H_

#include "Projector.h"
#include "State.h"

class ProjectorConnectingState : public State<Projector> {
	ProjectorConnectingState();
	ProjectorConnectingState(const ProjectorConnectingState &);
	ProjectorConnectingState& operator=(const ProjectorConnectingState &);
public:
	virtual ~ProjectorConnectingState();
	virtual void enter(Projector* projector);
	virtual void execute(Projector* projector);
	virtual void exit(Projector* projector);
	static ProjectorConnectingState* instance();
};

#endif /* PROJECTOR_PROJECTORCONNECTINGSTATE_H_ */
