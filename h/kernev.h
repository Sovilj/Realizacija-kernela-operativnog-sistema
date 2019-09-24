/*
 * kernev.h
 *
 *  Created on: Aug 10, 2017
 *      Author: OS1
 */

#ifndef KERNEV_H_
#define KERNEV_H_

#include "ivtentry.h"
#include "makro.h"
#include "pcb.h"

#define PREPAREENTRY(ivtno,oldorno)\
void interrupt interruptRoutine##ivtno (...); \
IVTEntry entry##ivtno (ivtno, interruptRoutine##ivtno); \
void interrupt interruptRoutine##ivtno (...){\
lock;\
if ( oldorno == 1)\
IVTEntry::all_ivt[ivtno]->old_routine();\
if (KernelEv::all_ev[ivtno] != 0)\
KernelEv::all_ev[ivtno]->signal();\
unlock;\
}\

// posto imam 2 static liste i eventova i ivtentry-a samo uz pomoc ivtno pronalazim i pozivam iz njih sta mi treba


typedef unsigned char IVTNo;

class KernelEv {

public:
	KernelEv(IVTNo ivtno);
	~KernelEv();

	void wait();
	void signal();

	static KernelEv* all_ev[max_ivt_entries];	// lista svih eventova
												// posle mozemo da nadjem event po ivt_num-u
private:
	int ev_val;				// za binarni semafor
	PCB* thr_pcb;			// nit na kojoj je pokrenut event
	IVTNo ivt_num;

};

#endif /* KERNEV_H_ */
