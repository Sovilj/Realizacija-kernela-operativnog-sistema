/*
 * semlist.h
 *
 *  Created on: Aug 9, 2017
 *      Author: OS1
 */
#ifndef SEMLIST_H_
#define SEMLIST_H_

#include "makro.h"
#include "kernsem.h"

class SemList {
private :
	struct Elem{
		KernelSem * sem;
		Elem * next;
		Elem(KernelSem* s){sem = s; next = 0;}
	};
	Elem *head, *tail;
	int cnt;
public:
	SemList();
	~SemList();

	void put(KernelSem* s);
	void remove(KernelSem* s);
	KernelSem* get_by_id(int i);
	KernelSem* get_by_place(int i);		// za iteriranje kroz listu

	int is_empty();
	int size();

	void update_all();


};


#endif /* SEMLIST_H_ */
