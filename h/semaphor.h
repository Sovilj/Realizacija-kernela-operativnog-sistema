/*
 * semaphor.h
 *
 *  Created on: Aug 8, 2017
 *      Author: OS1
 */

#ifndef SEMAPHOR_H_
#define SEMAPHOR_H_

typedef unsigned int Time;

class KernelSem;

class Semaphore{
public:
	Semaphore(int init = 1);
	virtual ~Semaphore();

	virtual int wait(Time maxTimeToWait); // wait vraca 0 ili 1 u zavisnosti da li je isteklo
	virtual void signal();				  // maxtimetowait(0) ili je nit deblokirana (1)

	int val()const ;
private:
	KernelSem* myImpl;
};

#endif /* SEMAPHOR_H_ */
