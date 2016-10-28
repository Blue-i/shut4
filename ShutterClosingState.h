/*
 * ShutterClosingState.h
 *
 *  Created on: 24 Oct 2016
 *      Author: rag
 */

#ifndef SHUTTER_SHUTTERCLOSINGSTATE_H_
#define SHUTTER_SHUTTERCLOSINGSTATE_H_

#include "Shutter.h"
#include "State.h"

class ShutterClosingState : public State<Shutter> {
	ShutterClosingState();
	ShutterClosingState(const ShutterClosingState &);
	ShutterClosingState& operator=(const ShutterClosingState &);
public:
	virtual ~ShutterClosingState();
	virtual void enter(Shutter*);
	virtual void execute(Shutter*);
	virtual void exit(Shutter*);
	static ShutterClosingState* instance();
};

#endif /* SHUTTER_SHUTTERCLOSINGSTATE_H_ */
