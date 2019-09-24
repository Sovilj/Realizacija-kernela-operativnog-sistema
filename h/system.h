/*
 * system.h
 *
 *  Created on: Aug 2, 2017
 *      Author: OS1
 */

#ifndef SYSTEM_H_
#define SYSTEM_H_

#include "makro.h"
#include "pcb.h"						// sadrzi  PCB* running i List* all_pcb
#include "thread.h"
#include "semaphor.h"					// da u testu samo include system.h
#include "ivtentry.h"
#include <iostream.h>

void tick();							// definicija metode...(i/o vreme)

class Thread;

class System{

public:
	System(){}

	static PCB* main_pcb;
	static PCB* idle_pcb;
	static Thread* idle_thread;

	static Time cur_time;				// trenutno vreme koje nit radi( za dispatch())

	static void start();				// pokrece sistem
	static void stop();					// zaustavlja sistem
	static void init();					// postavlja timer rutinu
	static void rest();					// restoruje timer rutinu

	static unsigned expl_flag ;  	    // flag za explicitno dispatch();
	static unsigned lock_flag;   	    // za zabranu preuzimanja bez zabrane prekida

	static void lockFlag();		        // f-je za lock_flag ( lock_flag=1 i lock_flag = 0)
	static void unlockFlag();

	static void interrupt timer();

};



#endif /* SYSTEM_H_ */
