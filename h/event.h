/*
 * event.h
 *
 *  Created on: Aug 10, 2017
 *      Author: OS1
 */

#ifndef EVENT_H_
#define EVENT_H_
#include "kernev.h"


typedef unsigned char IVTNo;
class KernelEv;

class Event{
public:
	Event(IVTNo ivtno);
	~Event();

	void wait();
protected:
	friend class KernelEv;
	void signal();
private:
	KernelEv* myImpl;
};
#endif /* EVENT_H_ */
