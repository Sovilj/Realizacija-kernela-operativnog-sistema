/*
 * event.cpp
 *
 *  Created on: Aug 10, 2017
 *      Author: OS1
 */


#include "event.h"
#include "makro.h"
#include "kernev.h"
#include "ivtentry.h"

Event::Event(IVTNo ivtno){
	lock;
	myImpl = new KernelEv(ivtno);
	unlock;
}
Event::~Event(){
	lock;
	delete myImpl;
	unlock;
}
void Event::wait(){
	lock;
	myImpl->wait();
	unlock;
}
void Event::signal(){
	lock;
	myImpl->signal();
	unlock;
}
