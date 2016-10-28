/*
 * State.h
 *
 *  Created on: 22 Oct 2016
 *      Author: rag
 */

#ifndef STATEMACHINE_STATE_H_
#define STATEMACHINE_STATE_H_

template <class T>
class State {
public:
	virtual void enter(T*) = 0;
	virtual void execute(T*) = 0;
	virtual void exit(T*) = 0;
	virtual ~State(){};
};

#endif /* STATEMACHINE_STATE_H_ */
