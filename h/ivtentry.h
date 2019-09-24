/*
 * ivtentry.h
 *
 *  Created on: Aug 10, 2017
 *      Author: OS1
 */

#ifndef IVTENTRY_H_
#define IVTENTRY_H_

#include "makro.h"

typedef unsigned char IVTNo;
typedef void interrupt (*InterruptRoutine)(...);

class IVTEntry{
public:
	IVTEntry(IVTNo ivtno, InterruptRoutine routine);
	~IVTEntry ();

	static IVTEntry* all_ivt[max_ivt_entries]; // lista svih IVTentry-a posle mozemo dohvatati pomocu ivt_num
	InterruptRoutine old_routine;
private:
	IVTNo ivt_num;

};


#endif /* IVTENTRY_H_ */
