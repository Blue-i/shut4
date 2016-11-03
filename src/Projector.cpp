#include "Projector.h"
#include "Events.h"
#include "Logging.h"
#include "os48.h"
#include "Arduino.h"

const char * Projector::POLL_MESSAGE = "%1POWR ?\r";
const char * Projector::DEBUG_NAME = "Projector";

Projector::Projector(EthernetClient *client, IPAddress * ip, uint16_t port, EventBus *bus, Component<Device>* sh) :
	id(0),
	state(CONNECTING),
	pollTimer(PROJECTOR_POLL_INTERVAL),
	client(client),
	ip(ip),
	port(port),
	bus(bus),
	buffer(new char[Projector::BUFFER_SIZE]()),
	shutter(sh)
{
	sh->bind(this,buffer,Projector::BUFFER_SIZE);
}

Projector::Projector(EthernetClient *client, IPAddress *ip, uint16_t port, EventBus *bus,  Component<Device>* sh, uint8_t id) :
	id(id),
	state(CONNECTING),
	pollTimer(PROJECTOR_POLL_INTERVAL),
	client(client),
	ip(ip),
	port(port),
	bus(bus),
	buffer(new char[Projector::BUFFER_SIZE]()),
	shutter(sh)
{
	sh->bind(this,buffer,Projector::BUFFER_SIZE);
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

void Projector::changeState(STATE s) {
	if (state == s) return;
	exit();
	state = s;
	enter();
}

void Projector::enterConnecting() {
	Log.Debug("%s %d > Cn\r\n", Projector::DEBUG_NAME, id);
	pollTimer.expire();
}

void Projector::executeConnecting() {
	bool status = false;
	status = client->connected();
	if(status) {
		changeState(CONNECTED);
		return;
	}

	if(pollTimer.completed()){
		pollTimer.reset();
		status = client->connect(*ip, port);
		if(status){
			size_t bytesRead = readFor(buffer, Projector::BUFFER_SIZE, PROJECTOR_READ_TIMEOUT);
			if(bytesRead > 0){
				Log.Debug("%s got %s\r\n", Projector::DEBUG_NAME, buffer);
				bus->queueEvent(PROJECTOR_CONNECTED);
				digitalWrite(id, HIGH);
				changeState(CONNECTED);
				return;
			}
		}
		client->stop();
		client->flush();
	}

}

void Projector::exitConnecting() {
	Log.Debug("%s %d < Cn\r\n", Projector::DEBUG_NAME, id);
}

void Projector::enterConnected() {
	Log.Debug("%s %d > Ct\r\n", Projector::DEBUG_NAME, id);
	pollTimer.reset();

}

void Projector::executeConnected() {
	if(pollTimer.completed()){
		changeState(POLLING);
		return;
	}

	shutter->run();
}

void Projector::exitConnected() {
	Log.Debug("%s %d < Ct\r\n", Projector::DEBUG_NAME, id);
}

void Projector::enterPolling() {
	Log.Debug("%s %d > P\r\n", Projector::DEBUG_NAME, id);
}

void Projector::executePolling() {
	if(pollTimer.completed()){
		size_t bytesRead = 0;
		OS48_NO_CS_BLOCK{
			client->print(Projector::POLL_MESSAGE);
			bytesRead = readFor(buffer, Projector::BUFFER_SIZE, PROJECTOR_READ_TIMEOUT);
		}
		pollTimer.reset();

		if(bytesRead > 0){
			Log.Debug("%s got %s\r\n", Projector::DEBUG_NAME, buffer);

			changeState(CONNECTED);
			return;
		}
		bus->queueEvent(PROJECTOR_DISCONNECTED);
		digitalWrite(id, LOW);
		shutter->reset();
		client->stop();
		changeState(CONNECTING);
	}
}

void Projector::exitPolling() {
	Log.Debug("%s %d < P\r\n", Projector::DEBUG_NAME, id);
}

Projector::STATE Projector::getState() {
	return state;
}

size_t Projector::readFor(char * buffer, size_t size, uint16_t timeout) {
	size_t bytesRead = 0;
	memset(buffer, 0, size);

	Timer t(timeout);
	while(true){
		if(bytesRead == size){
			bytesRead = 0;
			break;
		}
		if(client->available()){
			char c = 0;
			c = client->read();
			if(c != '\r') {
				buffer[bytesRead] = c;
				bytesRead++;
			} else {
				break;
			}
		} else {
			if(t.completed()){
				bytesRead = 0;
				break;
			}
		}
	}


	buffer[bytesRead] = '\0';
	return bytesRead;
}

void Projector::write(const char buffer[]) {
	if(state == CONNECTED){
		client->print(buffer);
	}
}

void Projector::reset() {
	pollTimer.expire();
	changeState(POLLING);
}

void Projector::run() {
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

Projector::~Projector() {

}
