/*
 * Shutter.h
 *
 *  Created on: 22 Oct 2016
 *      Author: rag
 */

#ifndef SHUTTER_SHUTTER_H_
#define SHUTTER_SHUTTER_H_

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

	const static unsigned long timeout 		= 125;
	const static unsigned long pollInterval = 1000;

	enum STATE {
		UNKNOWN,
		OPEN,
		OPENING,
		CLOSED,
		CLOSING
	} state, target;

	os48::Mutex 			targetMutex;
	unsigned long 			lastPollTime;
	Projector * 			projector;

	PJLinkParser::PJLinkResponse getShutterState();

	void enter();
	void execute();
	void exit();
	void changeState(STATE);

	void enterUnknown();
	void executeUnknown();
	void exitUnknown();

	void enterOpen();
	void executeOpen();
	void exitOpen();

	void enterOpening();
	void executeOpening();
	void exitOpening();

	void enterClosed();
	void executeClosed();
	void exitClosed();

	void enterClosing();
	void executeClosing();
	void exitClosing();


public:
	Shutter();
	virtual ~Shutter();
	virtual void bind(Projector*);
	virtual bool isBound();
	virtual void run();
	virtual void onEvent(Event);
};

#endif /* SHUTTER_SHUTTER_H_ */
