/*
 * Component.h
 *
 *  Created on: 22 Oct 2016
 *      Author: rag
 */

#ifndef COMPONENT_COMPONENT_H_
#define COMPONENT_COMPONENT_H_

template<class T>
class Component {
public:
	virtual void bind(T*) = 0;
	virtual bool isBound() = 0;
	virtual void run() = 0;
	virtual ~ Component() {};
};

#endif /* COMPONENT_COMPONENT_H_ */
