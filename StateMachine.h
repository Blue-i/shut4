/*
 * StateMachine.h
 *
 *  Created on: 22 Oct 2016
 *      Author: rag
 */

#ifndef STATEMACHINE_STATEMACHINE_H_
#define STATEMACHINE_STATEMACHINE_H_
#include "State.h"

template<typename T>
class StateMachine {
	T* pOwner;
	State<T>* pCurrentState;
	State<T>* pPreviousState;
	State<T>* pGlobalState;
public:
	StateMachine(T* owner): pOwner(owner),
							pCurrentState(nullptr),
							pPreviousState(nullptr),
							pGlobalState(nullptr){}

	void setCurrentState(State<T>* s) {pCurrentState = s;}
	void setGlobalState(State<T>* s) {pGlobalState = s;}
	void setPreviousState(State<T>* s) {pPreviousState = s;}

	void update() const
	{
		if(pGlobalState) pGlobalState->execute(pOwner);
		if(pCurrentState) pCurrentState->execute(pOwner);
	}

	void changeState(State<T>* pNewState)
	{
		pPreviousState = pCurrentState;
		pCurrentState->exit(pOwner);
		pCurrentState = pNewState;
		pCurrentState->enter(pOwner);
	}

	void revertToPreviousState()
	{
		changeState(pPreviousState);
	}

	State<T>* getCurrentState() const {return pCurrentState;}
	State<T>* getGlobalState() const {return pGlobalState;}
	State<T>* getPreviousState() const {return pPreviousState;}

	bool isInState(const State<T>& state) const
	{
		return &state == pCurrentState;
	}
};


#endif /* STATEMACHINE_STATEMACHINE_H_ */
