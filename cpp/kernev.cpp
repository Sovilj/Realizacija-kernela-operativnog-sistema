/*
 * kernev.cpp
 *
 *  Created on: Aug 10, 2017
 *      Author: OS1
 */


#include "kernev.h"
#include "event.h"
#include "makro.h"
#include "pcb.h"
#include "SCHEDULE.H"
#include "ivtentry.h"

KernelEv* KernelEv::all_ev[max_ivt_entries]= {0};

KernelEv::KernelEv(IVTNo ivtno){
	lock;
	ivt_num = ivtno;
	ev_val = 1;						// sa binarni semafor
	thr_pcb = PCB::running;
	if ( all_ev[ivt_num] == 0){
		all_ev[ivt_num] = this;
	}
	unlock;
}
KernelEv::~KernelEv(){

	lock;
	if ( all_ev[ivt_num]!= 0) all_ev[ivt_num] = 0;
	if (thr_pcb->get_status() == BLOCKED){				// nit koja je bila blokirana i cekala je prekid
		thr_pcb->set_status(READY);						// se sada odblokira
		Scheduler::put(thr_pcb);
	}
	thr_pcb = 0;
	unlock;
}
void KernelEv::signal(){
	lock;
	if ( ev_val == 0){
		if ( thr_pcb != 0 ){
			thr_pcb->set_status(READY);
			Scheduler::put(thr_pcb);
			ev_val = 1;  						// sad je sem na 1 pa ce moci da se blokira opet
		}
	}
	unlock;
}
void KernelEv::wait(){
	lock;
	if ( thr_pcb == PCB::running){
		if ( ev_val == 1 && thr_pcb->get_status()!= BLOCKED){
			ev_val = 0;
			PCB::running->set_status(BLOCKED);
			unlock;
			dispatch();

		}else {unlock;return;}

	}else {unlock; return;}
}
