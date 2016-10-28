/*
 * Projector.h
 *
 *  Created on: 11 Oct 2016
 *      Author: rag
 */

#ifndef PROJECTOR_H_
#define PROJECTOR_H_

#include <Ethernet2.h>
#include "QueueArray.h"
#include "StateMachine.h"
#include "Component.h"

class Projector {

	static const uint16_t pollInterval = 3000;
	static const uint16_t pollWait = 250;

	uint16_t lastPollTime;
	EthernetClient client;
	StateMachine<Projector> stateMachine;
	QueueArray<Component<Projector>*> componentQueue;
	const IPAddress * ip;
	const int port;
	char buffer[32];

	friend class ProjectorConnectedState;
	friend class ProjectorConnectingState;
	friend class ProjectorPollWaitState;

public:
	Projector(IPAddress * ip, int port);
	virtual ~Projector();
	void attachComponent(Component<Projector>*);
	void run();
	EthernetClient * getClient();
	size_t readFor(char buffer[], uint16_t timeout);
};

#endif /* PROJECTOR_H_ */

