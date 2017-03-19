/*
 * EventListener.h
 *
 *  Created on: 1 sept. 2016
 *      Author: alopez
 */

#ifndef EVENTLISTENER_H_
#define EVENTLISTENER_H_

#include <queue>          // std::queue
#include <string>
#include <mutex>
#include <vector>


//class GPIOAgent;



template<typename T>
class EventListener {
public:
	virtual ~EventListener();		// Destructor
	virtual void notify(std::vector<std::string> body, const T event)=0;
protected:
//constructor is protected because this class is abstract, it’s only meant to be inherited!
	EventListener();
	void pushBuf(T);
	T frontBuf();
	void popBuf();
	T popFrontBuf();
	bool emptyBuf();
private:
	// -------------------------
	// Disabling default copy constructor and default
	// assignment operator.
	// -------------------------
	EventListener(const EventListener& yRef);
	EventListener& operator=(const EventListener& yRef);

	std::queue<T> mBuf;
	std::mutex bufMutex;
	void lockMutex(){ bufMutex.lock();}
	void unlockMutex(){bufMutex.unlock();}

};

using StringListener = EventListener<std::string> ;


#include "EventListener.inl"
#endif /* EVENTLISTENER_H_ */
