/*
 * idlethr.h
 *
 *  Created on: Aug 6, 2017
 *      Author: OS1
 */

#ifndef IDLETHR_H_
#define IDLETHR_H_

#include "system.h"

class IdleThread : public Thread{

public:
	IdleThread(StackSize stacksize = defaultStackSize,Time timeslice = 0);
	virtual void run();
	//void start();

};

#endif /* IDLETHR_H_ */
