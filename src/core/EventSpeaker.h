/*
 * EventSpeaker.h
 *
 *  Created on: 20 juin 2016
 *      Author: edwardvergara
 */

#ifndef EVENTSPEAKER_H_
#define EVENTSPEAKER_H_

//-----------------------------------------------------
// EventSpeaker Class
//-----------------------------------------------------

#include "EventListener.h"
#include <vector>


template <typename T> class EventListener;


template <typename T>
class EventSpeaker {
public:
	virtual ~EventSpeaker();		//destructor

	bool addObserver( EventListener<T>* eventListener);
	bool removeObserver(const EventListener<T>* eventListener);
	bool notifyObservers(std::vector<std::string> body, const T event);
	uint32_t ObserversCout();

protected:
	//constructor is protected because this class is abstract, it’s only meant to be inherited!
	EventSpeaker();
private:
	std::vector<EventListener<T>*> m_ObserverVec;
	// -------------------------
	// Disabling default copy constructor and default
	// assignment operator.
	// -------------------------
	EventSpeaker(const EventSpeaker& yRef);
	EventSpeaker& operator=(const EventSpeaker& yRef);

};

using UDPSpeaker = EventSpeaker<std::string>;
using StringSpeaker = EventSpeaker<std::string>;


#include "EventSpeaker.inl"
#endif /* EVENTSPEAKER_H_ */





