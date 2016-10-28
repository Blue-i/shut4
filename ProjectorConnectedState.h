/*
 * ProjectorConnectedState.h
 *
 *  Created on: 22 Oct 2016
 *      Author: rag
 */

#ifndef PROJECTOR_PROJECTORCONNECTEDSTATE_H_
#define PROJECTOR_PROJECTORCONNECTEDSTATE_H_

#include "Projector.h"
#include "State.h"

class ProjectorConnectedState : public State<Projector> {
	ProjectorConnectedState();
	ProjectorConnectedState(const ProjectorConnectedState &);
	ProjectorConnectedState& operator=(const ProjectorConnectedState &);
public:
	virtual ~ProjectorConnectedState();
	virtual void enter(Projector* projector);
	virtual void execute(Projector* projector);
	virtual void exit(Projector* projector);
	static ProjectorConnectedState* instance();
};

#endif /* PROJECTOR_PROJECTORCONNECTEDSTATE_H_ */
