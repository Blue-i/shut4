/*
 * Shutter.h
 *
 *  Created on: 22 Oct 2016
 *      Author: rag
 */

#ifndef SHUTTER_SHUTTER_H_
#define SHUTTER_SHUTTER_H_

#include "StateMachine.h"
#include "Component.h"
#include "Projector.h"
#include "PJLinkParser.h"
#include "EventHandler.h"
#include "Events.h"
#include "os48.h"

class Shutter : public Component<Projector>, public EventHandler {

	static const char * queryMessage;
	static const char * openMessage;
	static const char * closeMessage;

	const static unsigned long timeout = 125;
	const static unsigned long pollInterval = 1000;

	enum ShutterTargetState {
			OPEN,
			CLOSED,
			UNKNOWN
	} target;

	os48::Mutex targetMutex;
	unsigned long lastPollTime;

	StateMachine<Shutter> stateMachine;
	Projector * projector;

	PJLinkParser::PJLinkResponse getShutterState();

	friend class ShutterOpenState;
	friend class ShutterUnknownState;
	friend class ShutterClosedState;
	friend class ShutterOpeningState;
	friend class ShutterClosingState;

public:
	Shutter();
	virtual ~Shutter();
	virtual void bind(Projector*);
	virtual bool isBound();
	virtual void run();
	virtual void onEvent(Event);
};

#endif /* SHUTTER_SHUTTER_H_ */
