#include "Arduino.h"
#include <Ethernet2.h>
#include <IPAddress.h>
#include <SPI.h>
#include "Projector.h"
#include <Shutter.h>
#include "os48.h"
#include "EventManager.h"
#include "Events.h"
#include <Logging.h>
#include "ShutterLEDController.h"
#include "Timer.h"
#include "Train.h"

#define LOGLEVEL LOG_LEVEL_DEBUG
#define OPEN_BUTTON 2
#define CLOSE_BUTTON 3
#define OPEN_LED 5
#define CLOSE_LED 6
#define LED1 7
#define LED2 8
#define PJLINK_PORT 4352

using namespace os48;
Scheduler* scheduler = Scheduler::get();

Task * tEvent;
Task * tled;
Task * tp1;
Task * tp2;

EventManager em;
ShutterLEDController lc(OPEN_LED,CLOSE_LED);
//
Train fastTrain;
Train slowTrain;

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
IPAddress ip2(10, 0, 0, 12);
//IPAddress ip3(10, 0, 0, 13);

EthernetClient c1;
EthernetClient c2;
Shutter s1(&em);
Shutter s2(&em);
Projector p1(&c1, &ip1, PJLINK_PORT, &em, &s1, LED1);
Projector p2(&c2, &ip2, PJLINK_PORT, &em, &s2, LED2);



void isr_open()
{
	OS48_ATOMIC_BLOCK{
		Log.Debug("OBT%s",CR);
		em.externalEvent = SHUTTER_OPEN_BUTTON_PRESS;
		em.externalEventUpdate = true;
		return;
	}

}

void isr_close()
{
	OS48_ATOMIC_BLOCK{
		Log.Debug("CBT%s",CR);
		em.externalEvent = SHUTTER_CLOSE_BUTTON_PRESS;
		em.externalEventUpdate = true;
		return;
	}
}
void flash(){
	Timer t(100);
	for(uint8_t i = 0; i < 5; ++i){
		digitalWrite(LED2, LOW);
		digitalWrite(CLOSE_LED, HIGH);
		while(!t.completed()){}
		t.reset();
		digitalWrite(CLOSE_LED, LOW);
		digitalWrite(OPEN_LED, HIGH);
		while(!t.completed()){}
		t.reset();
		digitalWrite(OPEN_LED, LOW);
		digitalWrite(LED1, HIGH);
		while(!t.completed()){}
		t.reset();
		digitalWrite(LED1, LOW);
		digitalWrite(LED2, HIGH);
		while(!t.completed()){}
		t.reset();
	}

	digitalWrite(CLOSE_LED, LOW);
	digitalWrite(OPEN_LED, LOW);
	digitalWrite(LED1, LOW);
	digitalWrite(LED2, LOW);

}
void setup()
{

	Log.Init(LOGLEVEL, 9600);


	pinMode(OPEN_BUTTON, INPUT_PULLUP);
	pinMode(CLOSE_BUTTON, INPUT_PULLUP);
	pinMode(CLOSE_LED, OUTPUT);
	pinMode(OPEN_LED, OUTPUT);
	pinMode(LED1, OUTPUT);
	pinMode(LED2, OUTPUT);

	digitalWrite(CLOSE_LED, LOW);
	digitalWrite(OPEN_LED, LOW);

	attachInterrupt(digitalPinToInterrupt(OPEN_BUTTON), isr_open, FALLING);
	attachInterrupt(digitalPinToInterrupt(CLOSE_BUTTON), isr_close, FALLING);

	Ethernet.begin(mac, ip0, dnsip, gateway, subnet);

	flash();

	em.addHandler(&s1);
	em.addHandler(&lc);
	em.addHandler(&s2);
	slowTrain.enqueue(&p1);
	slowTrain.enqueue(&p2);
	delay(3000);

	tp1 = scheduler->createTask(&loop1, 192);
	tp2 = scheduler->createTask(&loop2, 148);

	tEvent->clearStackFootprints();
	tp1->clearStackFootprints();
	tp2->clearStackFootprints();

	Log.Debug("S%s",CR);
	scheduler->start();
}

void loop() {
	;
}

//Timer timer1(3000);
void loop1()
{
	while(true){
		Projector * p = slowTrain.dequeue();
		if(p != nullptr){
//			Log.Debug("Sl !null %d\r\n", p->id);
			if(p->getState() == Projector::STATE::CONNECTING){
				p->run();
			}
			if(p->getState() != Projector::STATE::CONNECTING){
//				Log.Debug("Sl !Cn %d st %d\r\n", p->id, p->getState());
				fastTrain.enqueue(p);
//				Log.Debug("Ft eq %d\r\n",p->id);
			} else {
//				Log.Debug("Sl Cn %d\r\n", p->id);
				slowTrain.enqueue(p);
//				Log.Debug("Sl eq %d\r\n",p->id);
			}
		}
		task()->sleep(100);
//		}
	}
}

Timer timer2(3000);
void loop2()
{
	while(true){
//		Log.Debug("Fast -1\r\n");
		em.run();
		lc.run();
//		Log.Debug("Fast 0\r\n");
		Projector * p = fastTrain.dequeue();
		if(p != nullptr){
//			Log.Debug("FAST\r\n");
			p->run();
			if(p->getState() == Projector::STATE::CONNECTING){
//				Log.Debug("FAST 2\r\n");
				slowTrain.enqueue(p);
			} else {
//				Log.Debug("FAST 3\r\n");
				fastTrain.enqueue(p);
			}
		}
		if(timer2.completed()){
			Log.Debug("FastTrain ss: %d\r\n", task()->getLastStackFootprint());
			Log.Debug("Free mem: %d\r\n", scheduler->getFreeMemorySize());
			timer2.reset();
		}
	}
}


