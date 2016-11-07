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
	pollTimer.expire();
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
	pollTimer.expire();
}

void Projector::enter() {
	if(state == CONNECTING){
		enterConnecting();
		return;
	} else if(state == CONNECTED){
		enterConnected();
		return;
	} else if(state == POLLING){
		enterPolling();
		return;
	}
}

void Projector::exit() {
	if(state == CONNECTING){
		exitConnecting();
		return;
	} else if(state == CONNECTED){
		exitConnected();
		return;
	} else if(state == POLLING){
		exitPolling();
		return;
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
		Log.Debug("%s %d conn0\r\n", Projector::DEBUG_NAME, id);
		changeState(CONNECTED);
		return;
	}

	if(pollTimer.completed()){
		Log.Debug("%s %d conn1\r\n", Projector::DEBUG_NAME, id);
		pollTimer.reset();
		status = client->connect(*ip, port);

		if(status){
			Log.Debug("%s %d conn2\r\n", Projector::DEBUG_NAME, id);
			size_t bytesRead = readFor(buffer, Projector::BUFFER_SIZE, PROJECTOR_READ_TIMEOUT);

			if(bytesRead > 0){
				Log.Debug("%s got %s\r\n", Projector::DEBUG_NAME, buffer);
				bus->queueEvent(PROJECTOR_CONNECTED);
				digitalWrite(id, HIGH);
				changeState(CONNECTED);
				return;
			}
		}
		Log.Debug("%s %d conn3\r\n", Projector::DEBUG_NAME, id);
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
//	if(pollTimer.completed()){
		size_t bytesRead = 0;
		OS48_NO_CS_BLOCK{
			client->print(Projector::POLL_MESSAGE);
			bytesRead = readFor(buffer, Projector::BUFFER_SIZE, PROJECTOR_READ_TIMEOUT);
		}
//		pollTimer.reset();

		if(bytesRead > 0){
			Log.Debug("%s got %s\r\n", Projector::DEBUG_NAME, buffer);

			changeState(CONNECTED);
			return;
		}
		bus->queueEvent(PROJECTOR_DISCONNECTED);
		digitalWrite(id, LOW);
		shutter->reset();
		client->stop();
		Log.Debug("%s stopped\r\n", Projector::DEBUG_NAME);
		changeState(CONNECTING);
//	}
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
	if(state == CONNECTING){
		executeConnecting();
		return;
	} else if(state == CONNECTED){
		executeConnected();
		return;
	} else if(state == POLLING){
		executePolling();
		return;
	}
}

Projector::~Projector() {

}
