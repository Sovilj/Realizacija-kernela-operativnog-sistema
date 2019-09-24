/*
 * kernsem.cpp
 *
 *  Created on: Aug 8, 2017
 *      Author: OS1
 */
#include "kernsem.h"
#include "list.h"
#include "pcb.h"
#include "SCHEDULE.h"
#include <iostream.h>

int KernelSem::stat_sem_id = 0;
SemList* KernelSem::all_sem = new SemList();

KernelSem::KernelSem(int init){
	lock;
	id= ++stat_sem_id;					// auto id
	val = init;
	blocked_sem_list = new List();		// prazna lista svih blokiranih niti
	KernelSem::all_sem->put(this); 		// dodajem sem na listu svih
	unlock;
}
KernelSem::~KernelSem(){
	lock;
	while( val <0){
		signal();						// ako je ostalo nesto blokirano signalujemo mu
	}
	KernelSem::all_sem->remove(this);
	delete blocked_sem_list;
	unlock;
}

void KernelSem::signal(){
	lock;

	if ( val++ < 0){
		if(blocked_sem_list->is_empty() == 0){
			PCB* p = blocked_sem_list->get();
			p->wait_ret = 1;
			p->set_status(READY);
			Scheduler::put(p);
		}
	}
	unlock;
}
int KernelSem::wait(Time maxTimeToWait){
	lock;

	if( --val <  0){
		PCB::running->set_status(BLOCKED);
		PCB::running->set_sem_time(maxTimeToWait);
		blocked_sem_list->put(PCB::running);

		unlock;
		dispatch();
	}else
	{
		PCB::running->wait_ret=1;
		unlock;
	}
	return PCB::running->wait_ret;
}
int KernelSem::get_val()const {return val;}
int KernelSem::get_id()const {return id;}

void KernelSem::refresh(){
	lock;
	if ( blocked_sem_list->is_empty() == 1){
		unlock; return;
	}
	int i =0;
	PCB* p = blocked_sem_list->get_by_place(i);    		// prvi elem na sem

	while ( p != 0 ){
		if( p->get_sem_time() > 0){
			p->set_sem_time(p->get_sem_time()-1);		// smanjujemo vreme za 1

			if( p->get_sem_time() == 0 ){				// ako je doslo do 0 izbaciti iz liste blokiranih

				p->set_status(READY);
				p->wait_ret=0;             				// wait_ret na 0 posto je isteklo vreme
				Scheduler::put(p);
				val++;				      				// povecavam val posto izbacujemo jednu nit
				blocked_sem_list->remove(p);
			}
		}
		p = blocked_sem_list->get_by_place(++i);
	}
	unlock;
}







