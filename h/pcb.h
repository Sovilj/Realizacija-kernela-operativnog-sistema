/*
 * pcb.h
 *
 *  Created on: Aug 2, 2017
 *      Author: OS1
 */

#ifndef PCB_H_
#define PCB_H_

#include <dos.h>
#include "list.h"
#include "thread.h"
#include "makro.h"

class List;
class Thread;

class PCB{
	private :
		ID id;
		static int stat_id;				// static id koji sluzi za auto-ID
		Thread* my_thread;

		StackSize stack_size;
		unsigned int * stack;

		Time time_slice;
		Time sem_time;       			 // vreme cekanja na semaforu
		volatile int status;
	public:
		PCB(Thread* thr = 0,StackSize stacksize, Time timeslice);
		~PCB();

		int get_status() ;				// get i set
		void set_status(int stat);
		Time get_time_slice();
		Time get_sem_time();
		void set_sem_time(Time t);
		ID get_id();
		Thread* get_thread();

		unsigned int sp,ss,bp;			// promenljive za stek
		static PCB* running;			// globalno running
		static List* all_pcb; 			// globalna lista svih pcb-ova

		List * wait_list;				// lista svih pcb-ova koji cekaju (waitToComplete)

		int wait_ret;			  		// return wait rutine iz semafora( da li je 0 ili 1)

		void create_stack();			// kreiranje steka pri inicijalizaciji
		static void wrapper();			// f-ja koja obuhvata run() i zavrsava nit

};

#endif /* PCB_H_ */
