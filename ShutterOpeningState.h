/*
 * ShutterOpeningState.h
 *
 *  Created on: 24 Oct 2016
 *      Author: rag
 */

#ifndef SHUTTER_SHUTTEROPENINGSTATE_H_
#define SHUTTER_SHUTTEROPENINGSTATE_H_

#include "Shutter.h"
#include "State.h"

class ShutterOpeningState : public State<Shutter> {
	ShutterOpeningState();
	ShutterOpeningState(const ShutterOpeningState &);
	ShutterOpeningState& operator=(const ShutterOpeningState &);
public:
	virtual ~ShutterOpeningState();
	virtual void enter(Shutter*);
	virtual void execute(Shutter*);
	virtual void exit(Shutter*);
	static ShutterOpeningState* instance();
};

#endif /* SHUTTER_SHUTTEROPENINGSTATE_H_ */
