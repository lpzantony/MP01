/*
 * EventListener.cpp
 *
 *  Created on: 1 sept. 2016
 *      Author: alopez
 */

#include "EventListener.h"

template<typename T>
EventListener<T>::EventListener() {
	// TODO Auto-generated constructor stub

}
template<typename T>
EventListener<T>::~EventListener() {
	// TODO Auto-generated destructor stub
}

template<typename T>
void EventListener<T>::pushBuf(T t){
	lockMutex();
	mBuf.push(t);
	unlockMutex();
}

template<typename T>
T EventListener<T>::frontBuf(){
	lockMutex();
	T t = mBuf.front();
	unlockMutex();
	return t;

}
template<typename T>
void EventListener<T>::popBuf(){
	lockMutex();
	mBuf.pop();
	unlockMutex();

}
template<typename T>
T EventListener<T>::popFrontBuf(){
	lockMutex();
	T t = mBuf.front();
	mBuf.pop();
	unlockMutex();
	return t;
}

template<typename T>
bool EventListener<T>::emptyBuf(){
	lockMutex();
	bool b = mBuf.empty();
	unlockMutex();
	return b;
}

