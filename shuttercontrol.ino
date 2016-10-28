#include <QueueArray.h>

#define LOGLEVEL LOG_LEVEL_DEBUG
#include "Arduino.h"
#include <Ethernet2.h>
#include <IPAddress.h>
#include <SPI.h>
#include "Projector.h"
#include "os48.h"
//#include "EventManager.h"
#include "Events.h"
#include <Logging.h>

using namespace os48;
Scheduler* scheduler = Scheduler::get();
//Task * te;
Task * t1;
//Task * t2;
//Task * t3;

//byte mac[] = { 0x90, 0xA2, 0xDA, 0x10, 0x87, 0x72 };
//byte mac[] = { 0x90, 0xA2, 0xDA, 0x10, 0xB9, 0x7D };
//byte mac[] = { 0x90, 0xA2, 0xDA, 0x10, 0xB9, 0x87 };
//byte mac[] = { 0x90, 0xA2, 0xDA, 0x10, 0xBA, 0x2B };
//byte mac[] = { 0x90, 0xA2, 0xDA, 0x10, 0xB9, 0x3D };
//byte mac[] = { 0x90, 0xA2, 0xDA, 0x10, 0x87, 0x46 };
//byte mac[] = { 0x90, 0xA2, 0xDA, 0x10, 0x84, 0xDD };
byte mac[] = { 0x90, 0xA2, 0xDA, 0x10, 0x85, 0x38 };
IPAddress dnsip(10, 0, 0, 254);
IPAddress gateway(10, 0, 0, 254);
IPAddress subnet(255, 255, 255, 0);
IPAddress ip0(10, 0, 0, 10);
IPAddress ip1(10, 0, 0, 11);
//IPAddress ip2(10, 0, 0, 12);
//IPAddress ip3(10, 0, 0, 13);

int PJLINK_PORT = 4352;
Projector p1(&ip1, PJLINK_PORT);
//Projector p2(&ip2, PJLINK_PORT);
//Projector p3(&ip3, PJLINK_PORT);

//Shutter s1;
//Shutter s2;
//Shutter s3;

const int OPEN_BUTTON = 2;
const int CLOSE_BUTTON = 3;
const int OPEN_LED = 11;
const int CLOSE_LED = 12;

void isr_open()
{
//	Log.Debug("Open button");
//	EventManager::instance()->externalEvent = SHUTTER_OPEN_BUTTON_PRESS;
//	EventManager::instance()->externalEventUpdate = true;
	return;
}

void isr_close()
{
//	Log.Debug("Close button");
//	EventManager::instance()->externalEvent = SHUTTER_CLOSE_BUTTON_PRESS;
//	EventManager::instance()->externalEventUpdate = true;

	return;
}

void setup()
{

	Log.Init(LOGLEVEL, 9600);


	pinMode(OPEN_BUTTON, INPUT_PULLUP);
	pinMode(CLOSE_BUTTON, INPUT_PULLUP);
//	pinMode(CLOSE_LED, OUTPUT);
//	pinMode(OPEN_LED, OUTPUT);

//	digitalWrite(CLOSE_LED, LOW);
//	digitalWrite(OPEN_LED, LOW);

	attachInterrupt(digitalPinToInterrupt(OPEN_BUTTON), isr_open, FALLING);
	attachInterrupt(digitalPinToInterrupt(CLOSE_BUTTON), isr_close, FALLING);

	Ethernet.begin(mac, ip0, dnsip, gateway, subnet);

//	p1.attachComponent(&s1);
//	p2.attachComponent(&s2);
//	p3.attachComponent(&s3);

//	EventManager::instance()->addHandler(&s1);
//	EventManager::instance()->addHandler(&s2);
//	EventManager::instance()->addHandler(&s3);

	delay(3000);
	Serial.print("Starting!\r\n");

//	te = scheduler->createTask(&eventLoop, 60);
	t1 = scheduler->createTask(&loop1, 120);
//	t2 = scheduler->createTask(&loop2, 60);
//	t3 = scheduler->createTask(&loop3, 60);

	scheduler->start();
}

void loop()
{
//	p1.run();
}

void eventLoop(){
	while(true){
//		EventManager::instance()->run();
	}
}

void loop1()
{
	while(true){
		p1.run();
	}
}

void loop2()
{
	while(true){
//		p2.run();
	}
}

void loop3()
{
	while(true){
//		p3.run();
	}
}

