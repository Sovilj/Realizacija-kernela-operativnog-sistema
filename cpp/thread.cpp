/*
 * thread.cpp
 *
 *  Created on: Aug 2, 2017
 *      Author: OS1
 */

#include "makro.h"
#include "thread.h"
#include "SCHEDULE.h"
#include "pcb.h"
#include "system.h"


Thread::Thread(StackSize stacksize , Time timeslice){
	lock;
	myPCB = new PCB(this,stacksize,timeslice); // pozivamo konstruktor PCB-a koji proverava
	unlock;									   // stacksize i dodaje nit u Scheduler i na listu svih pcb-ova
}

Thread::~Thread(){
	lock;
	waitToComplete();				          // cekamo da nit prvo zavrsi
	if ( myPCB != 0) delete myPCB;            // pa pozivamo destruktor PCB-a
	unlock;
}
void Thread::start(){
	lock;
	if (myPCB->get_status() == NEW){
		myPCB->create_stack();				  // ako je status NEW kreiramo stek
		myPCB->set_status(READY);			  // niti i stavljamo je u scheduler
		Scheduler::put(myPCB);
	}

	unlock;
}

void Thread::waitToComplete(){
	lock;

	if ( myPCB->get_status() == NEW){unlock; return;}       //ne cekamo novu nit( koja nije startovana )
	if ( myPCB->get_status() == FINISHED){unlock; return;}	//ne cekamo gotovu nit
	if ( myPCB->get_status() == ACTIVE){unlock; return;}	//ne cekamo sami sebe
	if ( myPCB == System::main_pcb) {unlock; return;}		//ne cekamo main
	if ( this == System::idle_thread) {unlock; return;}     //ne cekamo na idle da zavrsi(ne radi nista)

	PCB::running->set_status(BLOCKED);						//blokiramo running
	myPCB->wait_list->put(PCB::running);					//stavljamo ga na listu cekanja
    unlock;

	dispatch();

}
ID Thread::getId(){
	lock;
	int i = 0;
	if (myPCB!=0){
		i = myPCB->get_id();
	}
	unlock;
	return i;
}
ID Thread::getRunningId(){
	lock;
	int i = PCB::running->get_id();
	unlock;
	return i;

}
Thread* Thread::getThreadById(ID id){
	lock;
	PCB* temp = PCB::all_pcb->get_by_id(id);				// iz liste svih PCB-ova izvlacimo PCB
	unlock;													// get_thread vraca nit ili 0
	return temp->get_thread();
}

void dispatch(){
	lock;
	System::expl_flag = 1;
	System::timer();
	unlock;
}
