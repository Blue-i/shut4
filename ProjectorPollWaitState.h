/*
 * ProjectorPollWaitState.h
 *
 *  Created on: 22 Oct 2016
 *      Author: rag
 */

#ifndef PROJECTOR_PROJECTORPOLLWAITSTATE_H_
#define PROJECTOR_PROJECTORPOLLWAITSTATE_H_

#include "Projector.h"
#include "State.h"

class ProjectorPollWaitState : public State<Projector> {
	ProjectorPollWaitState();
	ProjectorPollWaitState(const ProjectorPollWaitState &);
	ProjectorPollWaitState& operator=(const ProjectorPollWaitState &);
public:
	virtual ~ProjectorPollWaitState();
	virtual void enter(Projector* projector);
	virtual void execute(Projector* projector);
	virtual void exit(Projector* projector);
	static ProjectorPollWaitState* instance();
};

#endif /* PROJECTOR_PROJECTORPOLLWAITSTATE_H_ */
