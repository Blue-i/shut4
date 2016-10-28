/*
 * Projector.h
 *
 *  Created on: 11 Oct 2016
 *      Author: rag
 */

#ifndef PROJECTOR_H_
#define PROJECTOR_H_

#include <Ethernet2.h>
#include "Component.h"

class Projector {
	enum STATE{
		CONNECTING,
		CONNECTED,
		POLLING
	} state;

	static const uint16_t pollInterval 	= 	3000;
	static const uint16_t pollWait 		= 	250;

	uint16_t 				lastPollTime;
	EthernetClient 			client;
	Component<Projector>* 	component;
	const IPAddress * 		ip;
	const int 				port;

	void enter();
	void exit();
	void execute();
	void changeState(STATE);

	void enterConnecting();
	void executeConnecting();
	void exitConnecting();

	void enterConnected();
	void executeConnected();
	void exitConnected();

	void enterPolling();
	void executePolling();
	void exitPolling();

public:
	char buffer[32];

	Projector(IPAddress * ip, int port);
	virtual ~Projector();
	void attachComponent(Component<Projector>*);
	void run();
	EthernetClient * getClient();
	size_t readFor(char buffer[], uint16_t timeout);
};

#endif /* PROJECTOR_H_ */

