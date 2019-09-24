/*
 * kernsem.h
 *
 *  Created on: Aug 8, 2017
 *      Author: OS1
 */
#ifndef KERNSEM_H_
#define KERNSEM_H_

#include "semaphor.h"
#include "list.h"
#include "pcb.h"
#include "semlist.h"

class Semaphore;
class SemList;

class KernelSem {
public:
	KernelSem(int init);
	~KernelSem();

	void signal();
	int wait(Time maxTimeToWait);
	void refresh();

	int get_val() const ;
	int get_id() const ;

	static SemList* all_sem;  // lista svih semafora

	static int stat_sem_id;   //  za auto id semafora
private:
	int id;
	int val;
	List* blocked_sem_list;	  //lista blokiranih niti na pojedinacnom sem


};

#endif /* KERNSEM_H_ */
