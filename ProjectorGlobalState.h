/*
 * ProjectorGlobalState.h
 *
 *  Created on: 22 Oct 2016
 *      Author: rag
 */

#ifndef PROJECTOR_PROJECTORGLOBALSTATE_H_
#define PROJECTOR_PROJECTORGLOBALSTATE_H_

#include "State.h"
#include "Projector.h"

class ProjectorGlobalState: public State<Projector> {
	ProjectorGlobalState();
	ProjectorGlobalState(const ProjectorGlobalState &);
	ProjectorGlobalState& operator=(const ProjectorGlobalState &);
public:

	virtual ~ProjectorGlobalState();
	virtual void enter(Projector* projector);
	virtual void execute(Projector* projector);
	virtual void exit(Projector* projector);
	static ProjectorGlobalState* instance();
};

#endif /* PROJECTOR_PROJECTORGLOBALSTATE_H_ */
