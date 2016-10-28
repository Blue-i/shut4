/*
 * ShutterOpenState.h
 *
 *  Created on: 22 Oct 2016
 *      Author: rag
 */

#ifndef SHUTTER_SHUTTEROPENSTATE_H_
#define SHUTTER_SHUTTEROPENSTATE_H_

#include "Shutter.h"
#include "State.h"

class ShutterOpenState : public State<Shutter> {
	ShutterOpenState();
	ShutterOpenState(const ShutterOpenState &);
	ShutterOpenState& operator=(const ShutterOpenState &);
public:
	virtual ~ShutterOpenState();
	virtual void enter(Shutter* Shutter);
	virtual void execute(Shutter* Shutter);
	virtual void exit(Shutter* Shutter);
	static ShutterOpenState* instance();
};

#endif /* SHUTTER_SHUTTEROPENSTATE_H_ */
