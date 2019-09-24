/*
 * list.h
 *
 *  Created on: Aug 2, 2017
 *      Author: OS1
 */

#ifndef LIST_H_
#define LIST_H_

#include "pcb.h"

class PCB;

class List {
	private :
		struct Elem{
			PCB* data;
			Elem* next;
			Elem(PCB* p){
				data = p;
				next = 0;
			};
		};
		Elem *head,*tail;
		int cnt;
	public:
		List();
		~List();

		void put (PCB* p);				// dodaje pcb na kraj liste
		PCB* get();						// brise pcb sa pocetka i vraca ga
		PCB* get_by_id(int i);			// vraca pcb bez brisanja
		PCB* get_by_place(int i);		// za iteriranje kroz listu (vraca u odnosu na poziciju)
		void remove(PCB* p);			// brise pcb sa liste

		int size();
		int is_empty();
};

#endif /* LIST_H_ */
