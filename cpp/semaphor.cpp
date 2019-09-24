/*
 * semaphor.cpp
 *
 *  Created on: Aug 8, 2017
 *      Author: OS1
 */


#include "semaphor.h"

#include "kernsem.h"

Semaphore::Semaphore(int init){
	lock;
	myImpl = new KernelSem(init);
	unlock;
}
Semaphore::~Semaphore(){
	lock;
	delete myImpl;
	unlock;
}
void Semaphore::signal(){
	lock;
	myImpl->signal();
	unlock;
}
int Semaphore::wait(Time maxTimeToWait){
	lock;
	int i = myImpl->wait(maxTimeToWait);
	unlock;
	return i;
}
int Semaphore::val()const {
	lock;
	int i =  myImpl->get_val();
	unlock;
	return i;
}

