/*
 * Projector.cpp
 *
 *  Created on: 11 Oct 2016
 *      Author: rag
 */

#include "Projector.h"
#include "ProjectorGlobalState.h"
#include "ProjectorConnectingState.h"
#include "os48.h"
#include <Logging.h>

Projector::Projector(IPAddress * ip, int port) :
	lastPollTime(0),
	stateMachine(this),
	component(nullptr),
	ip(ip),
	port(port)

{
	stateMachine.setGlobalState(ProjectorGlobalState::instance());
	stateMachine.setCurrentState(ProjectorConnectingState::instance());
}

Projector::~Projector()
{
	client.stop();
}

void Projector::run()
{
//	Log.Debug("Projector Upd%s",CR);
	stateMachine.update();
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
