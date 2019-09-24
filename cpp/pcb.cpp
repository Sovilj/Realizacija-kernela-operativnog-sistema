/*
 * pcb.cpp
 *
 *  Created on: Aug 2, 2017
 *      Author: OS1
 */


#include <dos.h>
#include <stdlib.h>
#include <iostream.h>
#include "pcb.h"
#include "makro.h"
#include "SCHEDULE.H"
#include "thread.h"


List* PCB::all_pcb = new List();		// inicijalizacija static elemenata
PCB* PCB::running = 0;
int PCB::stat_id = 0;

PCB::PCB(Thread* thr , StackSize stacksize , Time timeslice){
	lock;
	id = ++stat_id;						// dodeljen auto id
	my_thread = thr;

	if ( stacksize > max_stack_size )	// stack_size ogranicavamo na
		stack_size = max_stack_size ;	// max_stack_size
	else
		stack_size = stacksize;
	stack = 0;

	time_slice = timeslice;
	status = NEW;
	sp = ss = bp = 0;
	sem_time = 0;
	wait_ret = 0;						// za wait f-ju semafora

	wait_list = new List();				// lista PCB koji cekaju na ovaj
	all_pcb->put(this);					// lista svih pcb-ova

	unlock;

}
PCB::~PCB(){
	lock;
	if ( wait_list != 0)
		delete wait_list;
	if ( stack != 0)
		delete stack;					// brisemo stek i listu
	my_thread = 0;
	unlock;
}
ID PCB::get_id(){
	lock;
	ID i = id;
	unlock;
	return i;
}
int PCB::get_status(){
	lock;
	int s = status;
	unlock;
	return s;
}
void PCB::set_status(int s){
	lock;
	status = s;
	unlock;
}
Time PCB::get_time_slice(){
	lock;
	Time t = time_slice;
	unlock;
	return t;
}
Time PCB::get_sem_time(){
	lock;
	Time t = sem_time;
	unlock;
	return t;
}
void PCB::set_sem_time(Time t){
	lock;
	sem_time = t;
	unlock;
}
Thread* PCB::get_thread(){
	lock;
	Thread* thr = my_thread;
	unlock;
	if ( thr != 0) return thr;
	else return 0;
}

void PCB::create_stack(){
#ifndef BCC_BLOCK_IGNORE
	lock;

	stack = new unsigned[stack_size];

	stack[stack_size -1] = int_mask;   				// postavljamo PSW I = 1 omogucava prekide nakon povratka iz rutine
	stack[stack_size -2] = FP_SEG(PCB::wrapper);    // wrapper obuhvata run metodu nase niti
	stack[stack_size -3] = FP_OFF(PCB::wrapper);    // povratna adresa se manipulise da bude
												    // f-ja wrapper koja ce da pokrene nit tj run()
	sp = bp = FP_OFF(stack+stack_size-12);          // sp se posle dodavanja PSW,PC i ax..(9) nalazi
	ss = FP_SEG(stack+stack_size -12);		        // na -12-tom mestu pa to pamtimo

	unlock;
#endif
}

void PCB::wrapper(){

	PCB::running->my_thread->run();  			 // od running PCB-a poziva run() niti
										         // kada run() zavrsi treba ugasiti nit
	lock;
	PCB::running->set_status(FINISHED);
	PCB* temp;
	while (!running->wait_list->is_empty()){    // sve niti koje cekaju na ovu koju gasimo
											    // treba da stavimo u stanje ready
		temp = PCB::running->wait_list->get();
		temp->set_status(READY);
		Scheduler::put(temp);					// sve koje su cekale idu kao READY u Scheduler
	}
	PCB::all_pcb->remove(running);				// uklanjamo PCB iz liste svih
	unlock;

	dispatch();									// konacno kada smo ugasili nit zelimo
}												// da druga preuzme



