/*
 * ShutterUnknownState.h
 *
 *  Created on: 23 Oct 2016
 *      Author: rag
 */

#ifndef SHUTTER_SHUTTERUNKNOWNSTATE_H_
#define SHUTTER_SHUTTERUNKNOWNSTATE_H_

#include "Shutter.h"
#include "State.h"

class ShutterUnknownState : public State<Shutter> {
	ShutterUnknownState();
	ShutterUnknownState(const ShutterUnknownState &);
	ShutterUnknownState& operator=(const ShutterUnknownState &);
public:
	virtual ~ShutterUnknownState();
	virtual void enter(Shutter*);
	virtual void execute(Shutter*);
	virtual void exit(Shutter*);
	static ShutterUnknownState* instance();
};

#endif /* SHUTTER_SHUTTERUNKNOWNSTATE_H_ */
