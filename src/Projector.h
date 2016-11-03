/*
 * Projector.h
 *
 *  Created on: 31 Oct 2016
 *      Author: rag
 */
#pragma once

#ifndef PROJECTOR_H_
#define PROJECTOR_H_

#include "Timer.h"
#include "Component.h"
#include "Device.h"
#include "Ethernet2.h"
#include "EventBus.h"
#include "Shutter.h"

#define PROJECTOR_READ_TIMEOUT 1000
#define PROJECTOR_POLL_INTERVAL 10000

class Projector : public Device {
public:
	uint8_t id;
	static const size_t COMPONENTS_SIZE = 1;
	static const size_t BUFFER_SIZE = 32;

	enum STATE {
			CONNECTING,
			CONNECTED,
			POLLING
		};
private:

	static const char * POLL_MESSAGE;
	static const char * DEBUG_NAME;

	STATE state;

	Timer pollTimer;
	EthernetClient * client;
	IPAddress * ip;
	uint16_t port;
	EventBus * bus;
	char * buffer;
	Component<Device>* shutter;

	void enter();
	void exit();
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

	Projector(EthernetClient *client, IPAddress * ip, uint16_t port, EventBus *bus,  Component<Device>* sh);
	Projector(EthernetClient *client, IPAddress * ip, uint16_t port, EventBus *bus,  Component<Device>* sh, uint8_t id);
	Projector::STATE getState();
	virtual size_t readFor(char buffer[], size_t size, uint16_t timeout);
	virtual void write(const char buffer[]);
	virtual void reset();
	virtual void run();
	virtual ~Projector();

};

#endif /* PROJECTOR_H_ */
