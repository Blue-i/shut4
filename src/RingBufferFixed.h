/*
 * RingBuffer.h
 *
 *  Created on: 30 Oct 2016
 *      Author: rag
 */
#pragma once
#ifndef SRC_RINGBUFFERF_H_
#define SRC_RINGBUFFERF_H_
template<class T, int S>
class RingBufferF {
	T buffer[S+1];
	unsigned int size = S+1;
	unsigned int head;
	unsigned int tail;
public:
	RingBufferF();
	virtual ~RingBufferF();
	bool enqueue(T);
	T dequeue();
	bool isEmpty();
	bool isFull();
};

template<class T, int S>
inline RingBufferF<T,S>::RingBufferF() : head(0), tail(0) {
}

template<class T, int S>
inline RingBufferF<T,S>::~RingBufferF() {

}

template<class T, int S>
inline bool RingBufferF<T,S>::enqueue(T item) {
	if(!isFull()){
		buffer[tail] = item;
		tail = (tail + 1) % size;
		return true;
	}
	return false;
}

template<class T, int S>
inline T RingBufferF<T,S>::dequeue() {
	unsigned int item = head;
	head = (head + 1) % size;
	return buffer[item];
}

template<class T, int S>
inline bool RingBufferF<T,S>::isEmpty() {
	return head == tail;
}

template<class T, int S>
inline bool RingBufferF<T,S>::isFull() {
	unsigned int next = (tail + 1) % size;
	return next == head;
}

#endif /* SRC_RINGBUFFERF_H_ */
