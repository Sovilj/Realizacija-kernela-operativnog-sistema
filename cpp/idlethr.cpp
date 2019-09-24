/*
 * idlethr.cpp
 *
 *  Created on: Aug 6, 2017
 *      Author: OS1
 */

#include "idlethr.h"
#include "system.h"
#include "SCHEDULE.h"
#include <stdlib.h>

IdleThread::IdleThread(StackSize stacksize,Time timeslice):Thread(stacksize,timeslice){}

void IdleThread::run(){

	while(PCB::all_pcb->size() > 2){			// ako imamo vise od 2 pcb-a(main i idle)
		PCB::running->set_status(BLOCKED);		// blokiramo idle i menjamo kontekst dok se neka nit

		dispatch();								// ne osposobi i preuzme

	}
	System::main_pcb->set_status(READY);		// ako su ostala samo 2 pcb-a
	Scheduler::put(System::main_pcb);			// postavljamo main u Scheduler
	PCB::running->set_status(BLOCKED);

	dispatch();
}

