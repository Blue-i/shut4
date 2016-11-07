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
#define LED3 9
#define LED4 11
#define PJLINK_PORT 4352

using namespace os48;
Scheduler* scheduler = Scheduler::get();

//Task * tEvent;
//Task * tled;
Task * tp1;
Task * tp2;
EventManager em;
ShutterLEDController lc(OPEN_LED,CLOSE_LED);

Train fastTrain;
Train slowTrain;

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
//EthernetClient c3;


Shutter s1(&em);
Shutter s2(&em);
//Shutter s3(&em);


Projector p1(&c1, &ip1, PJLINK_PORT, &em, &s1, LED1);
Projector p2(&c2, &ip2, PJLINK_PORT, &em, &s2, LED2);
//Projector p3(&c3, &ip3, PJLINK_PORT, &em, &s3, LED3);



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
		digitalWrite(LED4, LOW);
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
		digitalWrite(LED2, LOW);
		digitalWrite(LED3, HIGH);
		while(!t.completed()){}
		t.reset();
		digitalWrite(LED3, LOW);
		digitalWrite(LED4, HIGH);
		while(!t.completed()){}
		t.reset();
	}

	digitalWrite(CLOSE_LED, LOW);
	digitalWrite(OPEN_LED, LOW);
	digitalWrite(LED1, LOW);
	digitalWrite(LED2, LOW);
	digitalWrite(LED3, LOW);
	digitalWrite(LED4, LOW);

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
	pinMode(LED3, OUTPUT);
	pinMode(LED4, OUTPUT);

	digitalWrite(CLOSE_LED, LOW);
	digitalWrite(OPEN_LED, LOW);

	attachInterrupt(digitalPinToInterrupt(OPEN_BUTTON), isr_open, FALLING);
	attachInterrupt(digitalPinToInterrupt(CLOSE_BUTTON), isr_close, FALLING);

	Ethernet.begin(mac, ip0, dnsip, gateway, subnet);

	flash();

	em.addHandler(&lc);
	em.addHandler(&s1);
	em.addHandler(&s2);
//	em.addHandler(&s3);
//	em.addHandler(&s4);

	slowTrain.enqueue(&p1);
	slowTrain.enqueue(&p2);
//	slowTrain.enqueue(&p3);
//	slowTrain.enqueue(&p4);

	delay(3000);

	tp1 = scheduler->createTask(&loop1, 128);
	tp2 = scheduler->createTask(&loop2, 128);

//	tEvent->clearStackFootprints();
	tp1->clearStackFootprints();
	tp2->clearStackFootprints();

	Log.Debug("S%s",CR);
	scheduler->start();
}

void loop() {
//	em.run();
//	lc.run();
//	p1.run();
//	em.run();
//	lc.run();
//	p2.run();
//	em.run();
//	lc.run();
//	p3.run();
//	em.run();
//	lc.run();
//	p4.run();
}

Timer timer1(3000);
void loop1()
{
	while(true){
		Projector * p = slowTrain.dequeue();
		if(p != nullptr){
			Log.Debug("Sl !null %d\r\n", p->id);
			if(p->getState() == Projector::STATE::CONNECTING){
				p->run();
			}
			if(p->getState() != Projector::STATE::CONNECTING){
				fastTrain.enqueue(p);
			} else {
				slowTrain.enqueue(p);
			}
		}
		if(timer1.completed()){
			Log.Debug("SlowTrain ss: %d\r\n", task()->getLastStackFootprint());
			timer1.reset();
		}
		task()->sleep(1000);
	}
}

Timer timer2(3000);
//Timer resetTimer(30000);
void loop2()
{
	while(true){
		em.run();
		lc.run();
		Projector * p = fastTrain.dequeue();
		if(p != nullptr){
			p->run();
			if(p->getState() == Projector::STATE::CONNECTING){
				slowTrain.enqueue(p);
			} else {
				fastTrain.enqueue(p);
			}
		}
		if(timer2.completed()){
			Log.Debug("FastTrain ss: %d\r\n", task()->getLastStackFootprint());
			Log.Debug("Free mem: %d\r\n", scheduler->getFreeMemorySize());
			timer2.reset();
		}
//		if(resetTimer.completed()){
//			Log.Debug("RESET\r\n");
//			tp1->reset();
//			resetTimer.reset();
//		}
	}
}


