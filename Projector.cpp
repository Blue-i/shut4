/*
 * Projector.cpp
 *
 *  Created on: 11 Oct 2016
 *      Author: rag
 */

#include "Projector.h"
#include "os48.h"
#include <Logging.h>
//#include "EventManager.h"

Projector::Projector(IPAddress * ip, int port) :
	state(CONNECTING),
	lastPollTime(0),
	component(nullptr),
	ip(ip),
	port(port)

{
}

Projector::~Projector()
{
	client.stop();
}

void Projector::run()
{
//	Log.Debug("Projector Upd%s",CR);
	execute();
}

EthernetClient* Projector::getClient() {
	return &client;
}

void Projector::attachComponent(Component<Projector>* c) {
	c->bind(this);
	component = c;
}

size_t Projector::readFor(char buffer[], uint16_t timeout) {
	size_t size = sizeof(buffer);
	size_t bytesRead = 0;
	memset(buffer, 0, size);


	uint16_t startTime = millis();
	while(true){
		if(client.available()){
			char c = 0;
			OS48_ATOMIC_BLOCK{
				c = client.read();
			}
			Log.Debug("Got %d ", c);
			if(c != '\r') {
				buffer[bytesRead] = c;
				bytesRead++;
			} else {
				break;
			}
		} else {
			uint16_t time = millis();
			if(time - startTime > timeout){
				Log.Debug("TO%s",CR);
				bytesRead = 0;
				break;
			}
		}
	}


	buffer[bytesRead] = '\0';
	return bytesRead;
}

void Projector::enter() {
	switch(state){
	case CONNECTING:
		enterConnecting();
		break;
	case CONNECTED:
		enterConnected();
		break;
	case POLLING:
		enterPolling();
		break;
	}
}

void Projector::exit() {
	switch(state){
	case CONNECTING:
		exitConnecting();
		break;
	case CONNECTED:
		exitConnected();
		break;
	case POLLING:
		exitPolling();
		break;
	}
}

void Projector::execute() {
	switch(state){
	case CONNECTING:
		executeConnecting();
		break;
	case CONNECTED:
		executeConnected();
		break;
	case POLLING:
		executePolling();
		break;
	}
}

void Projector::changeState(STATE s) {
	exit();
	state = s;
	enter();
}

void Projector::enterConnecting() {
	Log.Debug("Proj > Conn %s", CR);
}

void Projector::executeConnecting() {
	bool status = false;
	status = client.connected();
	if(status) {
		Log.Debug("Proj Conn!%s",CR);
		changeState(CONNECTED);
		return;
	}

	Log.Debug("Proj Conn..%s", CR);

	status = client.connect(*ip, port);
	if(status){
		readFor(buffer, pollWait);
		Log.Debug("Proj %s%s", buffer, CR);
		return;
	}

//	OS48_ATOMIC_BLOCK{
		client.stop();
		client.flush();
//	}

}

void Projector::exitConnecting() {
	Log.Debug("Proj < Conn%s", CR);
}

void Projector::enterConnected() {
	Log.Debug("Proj > Contd%s",CR);
//	EventManager::instance()->queueEvent(PROJECTOR_CONNECTED);
}

void Projector::executeConnected() {
	uint16_t elapsedTime = 0;
	elapsedTime = millis() - lastPollTime;
	if(elapsedTime > pollInterval){
		Log.Debug("Proj Poll%s",CR);
		changeState(POLLING);
		return;
	}

	if(component != nullptr){
		component->run();
	}
}

void Projector::exitConnected() {
	Log.Debug("Proj < Contd%s",CR);
}

void Projector::enterPolling() {
	Log.Debug("Proj > PW%s", CR);
	lastPollTime = 0;
}

void Projector::executePolling() {
	unsigned long elapsedTime = millis() - lastPollTime;
	if(elapsedTime > Projector::pollInterval){
		OS48_ATOMIC_BLOCK{
			client.print("%1POWR ?\r");
			lastPollTime = millis();
		}
		size_t bytesRead = 0;
		bytesRead = readFor(buffer, pollWait);
		if(bytesRead > 1){
			Log.Debug("PW %s%s", buffer,CR);
			changeState(CONNECTED);
			return;
		}
//		OS48_ATOMIC_BLOCK{
			client.stop();
			client.flush();
//		}
		changeState(CONNECTING);
//		EventManager::instance()->queueEvent(PROJECTOR_DISCONNECTED);
	}
}

void Projector::exitPolling() {
	Log.Debug("Proj < PW%s", CR);

}

void Projector::reset() {
	client.stop();
	changeState(CONNECTING);
}
