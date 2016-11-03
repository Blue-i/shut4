/*
 * PrjectorTrain.cpp
 *
 *  Created on: 2 Nov 2016
 *      Author: rag
 */

#include <Train.h>
//#include <iostream>

Train::Train() {
	// TODO Auto-generated constructor stub

}

Train::~Train() {
	// TODO Auto-generated destructor stub
}

void Train::enqueue(Projector* p) {
	m.lock();
	projectors.enqueue(p);
	m.unlock();
}

bool Train::isEmpty() {
	m.lock();
	bool e = projectors.isEmpty();
	m.unlock();
	return e;
}

Projector* Train::dequeue() {
	Projector * p = nullptr;
	m.lock();
	if(!projectors.isEmpty()){
//		std::cout << "!EMPTY" << std::endl;
		p = projectors.dequeue();
	}
	m.unlock();
	return p;
}
