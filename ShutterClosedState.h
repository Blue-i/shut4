/*
 * ShutterClosedState.h
 *
 *  Created on: 23 Oct 2016
 *      Author: rag
 */

#ifndef SHUTTER_SHUTTERCLOSEDSTATE_H_
#define SHUTTER_SHUTTERCLOSEDSTATE_H_

#include "Shutter.h"
#include "State.h"
class Shutter;
class ShutterClosedState : public State<Shutter> {
	ShutterClosedState();
	ShutterClosedState(const ShutterClosedState &);
	ShutterClosedState& operator=(const ShutterClosedState &);
public:
	virtual ~ShutterClosedState();
	virtual void enter(Shutter*);
	virtual void execute(Shutter*);
	virtual void exit(Shutter*);
	static ShutterClosedState* instance();
};

#endif /* SHUTTER_SHUTTERCLOSEDSTATE_H_ */
