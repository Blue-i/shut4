/*
 * PrjectorTrain.h
 *
 *  Created on: 2 Nov 2016
 *      Author: rag
 */
#pragma once

#ifndef TRAIN_H_
#define TRAIN_H_

#include "RingBufferFixed.h"
#include "os48.h"
#include "Projector.h"

class Train {
	RingBufferF<Projector*, 4> projectors;
	os48::Mutex m;
public:
	Train();
	virtual ~Train();
	void enqueue(Projector*);
	bool isEmpty();
	Projector * dequeue();
};

#endif /* TRAIN_H_ */
